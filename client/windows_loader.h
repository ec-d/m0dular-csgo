#ifndef WINDOWS_LOADER_H
#define WINDOWS_LOADER_H

#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "windows_headers.h"
#include "loader.h"

//TODO: Don't rely on LTO removing all WinLoader code not meant to be accessible to the clients

//Windows CSGO is 32 bit and we are currently targetting 32 bit systems
using nptr_t = uint32_t;

#ifndef _MSC_VER
#define __stdcall
#define __thiscall
#define STRCASECMP strcasecmp
#else
#define STRCASECMP _stricmp
#endif

using GetProcAddressFn = int(__stdcall*)(void*, const char*);
using LoadLibraryAFn = void*(__stdcall*)(const char*);
using LdrpHandleTlsDataSTDFn = int(__stdcall*)(void*);
using LdrpHandleTlsDataThisFn = int(__thiscall*)(void*);

struct WinSection
{
    uint32_t bufOffset;
	uint32_t virtOffset;
	uint32_t bufSize;
	uint32_t virtSize;
	uint32_t prot;
};

struct WinRelocation
{
	uint32_t bufOffset;
};

//Name offset will be inverted if the value has to be imported by ordinal instead of name
struct WinImport
{
	uint32_t nameOffset;
    uint32_t virtOffset;
};

struct WinImportH
{
	uint64_t module;
	WinImport imp;
};

struct WinImportThunk
{
    uint32_t moduleNameOffset;
    uint32_t importCount;
    uint32_t importOffset;
};

struct WinModule;

struct PackedWinModule
{
	uint32_t modBufSize;
	uint32_t bufSize;
	uint32_t allocSize;
	uint32_t entryPointOffset;
	uint32_t sectionOffset;
	uint32_t nameOffset;
	uint32_t importsWHOffset;
	uint32_t importsOffset;
	uint32_t importThunksOffset;
	uint32_t relocationOffset;

	uint64_t state;
	//Pointers are at the end for simple communication with the server
	const char* moduleBuffer;
	const char* buffer;

	static constexpr size_t serSizeSub = 2 * sizeof(char*) - sizeof(uint64_t);

	PackedWinModule(const WinModule& mod);
	PackedWinModule(const char*);
	char* ToBuffer(uint32_t* outSize);

	PackedWinModule()
		: moduleBuffer(nullptr), buffer(nullptr) {}


	PackedWinModule(const PackedWinModule& o)
	{
		memcpy(this, &o, sizeof(*this));
		moduleBuffer = nullptr;
		buffer = nullptr;
	}

	PackedWinModule& operator=(const PackedWinModule& o)
	{
		memcpy(this, &o, sizeof(*this));
		moduleBuffer = nullptr;
		buffer = nullptr;
		return *this;
	}

	~PackedWinModule()
	{
		if (state > 1)
			return;

		if (buffer)
			free((void*)buffer);
		if (moduleBuffer)
			free((void*)moduleBuffer);
	}

	void PerformRelocations(nptr_t base);

	//This is only meant to be available to the client side
#ifdef _WIN32
	void SetupInPlace(HANDLE processHandle, char* targetModuleAddress, char* targetDataAddress);
#endif
};

struct WinModule
{
	char* moduleBuffer;

	std::vector<WinSection> sections;

	uint32_t entryPointOffset;
	uint32_t moduleSize;

	std::vector<char> names;
	std::vector<WinImportH> importsWH;
	std::vector<WinImport> thunkedImports;
	std::vector<WinImportThunk> importThunk;
	std::vector<WinRelocation> relocations;

	WinModule()
		: moduleBuffer()
	{
	}

	WinModule(const char* buf, size_t size, ModuleList* moduleList = nullptr, bool is64 = false);

	uint32_t VirtToFile(uint32_t virtOffset, WinSection*& hint);
	uint32_t FileToVirt(uint32_t virtOffset, WinSection*& hint);

	~WinModule()
	{
		if (moduleBuffer)
			free(moduleBuffer);
	}

};

struct WinLoadData
{
	PackedWinModule* packedModule;
	char* outBuf;
	GetProcAddressFn pGetProcAddress;
	LoadLibraryAFn pLoadLibraryA;
	LdrpHandleTlsDataSTDFn pHandleTlsDataSTD;
	LdrpHandleTlsDataThisFn pHandleTlsDataThis;
};

#ifdef _WIN32
unsigned long __stdcall LoadPackedModule(void* loadData);
#endif

#endif