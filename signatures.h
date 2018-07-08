#ifndef SIGNATURES_H
#define SIGNATURES_H

#include "libnames.h"

struct Signature
{
	uintptr_t& result;
	const char* module;
	const char* pattern;

#ifdef STACK_STRING
	~Signature()
	{
		delete pattern;
	}
#endif
};

#ifdef STACK_STRING
#define SIGNATURE(out, lib, sig) {(uintptr_t&)out, lib, (new StackString(sig))->val()}
#else
#define SIGNATURE(out, lib, sig) {(uintptr_t&)out, lib, sig}
#endif

#if defined(__linux__)
const Signature signatures[] = {
	SIGNATURE(clientMode, clientLib, "[48 8D 05 *? ? ? ?] 48 89 E5 48 89 05 ? ? ? ? E8 ? ? ? ? 5D 48"),
	SIGNATURE(CL_RunPrediction, engineLib, "55 48 89 E5 53 48 83 EC 08 E8 ? ? ? ? 83 B8 A0 01 00 00 06 48 89 C3"),
	SIGNATURE(RunSimulationFunc, clientLib, "55 48 89 E5 41 57 49 89 CF 41 56 49 89 FE 41 55 41 54 41 89 F4 53"),
	SIGNATURE(GetWeaponInfo, clientLib, "55 48 89 E5 41 57 41 56 41 55 41 89 F5 41 54 45 31 E4 53"),
	SIGNATURE(weaponDatabase, clientLib, "74 32 [48 8B 05 **-24? ? ? ?] 48 8B 00"),
	SIGNATURE(SetAbsOrigin, clientLib, "55 48 89 E5 48 89 5D E8 48 89 FB 4C 89 65 F0 49 89 F4 4C 89 6D F8 48 83 EC 20 E8 B1 ? ? ? F3 41 0F"),
	SIGNATURE(SetAbsAngles, clientLib, "55 48 89 E5 48 89 5D E0 48 89 FB 4C 89 6D F0 49 89 F5 4C 89 65 E8 4C 89 75 F8 48 ? ? ? E8 ? ? ? ? F3"),
	SIGNATURE(SetAbsVelocity, clientLib, "55 48 89 E5 48 89 5D E8 48 89 FB 4C 89 65 F0 49 89 F4 4C 89 6D F8 48 83 EC 30 F3 0F 10 06 0F 2F 87 CC 00 00 00 75 12"),
	SIGNATURE(SetupBones, clientLib, "55 48 8D 05 ? ? ? ? 48 89 E5 41 57 41 56 41 55 4C 8D 2D"),
	SIGNATURE(effectsHead, clientLib, "[4C 8B 2D *? ? ? ?] 4D 85 ED 0F 85 94 00 00 00 0F 1F 44 00 00"),
	SIGNATURE(Weapon_ShootPosition, clientLib, "55 48 89 E5 53 48 89 FB 48 83 EC 38 E8 ? ? ? ? 80 BB")
};
#elif defined(__APPLE__)
const Signature signatures[] = {
	SIGNATURE(clientMode, clientLib, "[48 8B 3D **? ? ? ?] 48 8B 07 5D FF A0 D8"),
	SIGNATURE(CL_RunPrediction, engineLib, "55 48 89 E5 53 50 E8 ? ? ? ? 48 89 C3 83 BB ? ? ? 00 06 75 1D"),
	SIGNATURE(RunSimulationFunc, clientLib, "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 CE 48 89 D3 F3 0F 11 45 D0"),
	SIGNATURE(GetWeaponInfo, clientLib, "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 41 89 F6 49 89 FC 41 81 FE FE FF 00 00"),
	SIGNATURE(weaponDatabase, clientLib, "4C 89 F7 E8 ? ? ? ? 48 85 C0 75 12 [48 8D 05 **-24? ? ? ?] 48 8B 38"),
	SIGNATURE(SetAbsOrigin, clientLib, "55 48 89 E5 41 57 41 56 53 50 49 89 F6 48 89 FB E8 ? ? ? ? F3 41 0F 10 06"),
	SIGNATURE(SetAbsAngles, clientLib, "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 49 89 F6 48 89 FB E8 ? ? ? ? F3 41 0F 10 06"),
	SIGNATURE(SetAbsVelocity, clientLib, "55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F6 48 89 FB F3 41 0F 10 06"),
	SIGNATURE(SetupBones, clientLib, "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC ? ? ? ? F3 ? ? ? ? ? ? ? 89 CB 89 95"),
	SIGNATURE(effectsHead, clientLib, "[4C 8B 3D *? ? ? ?] 4D 85 FF 0F 84 ? ? ? ? 48 8D 1D"),
	SIGNATURE(viewRender, clientLib, "[48 89 05 *? ? ? ?] 48 C7 05 ? ? ? ? 00 00 00 00 48 8D ? ? ? ? ? 48 8D ? ? ? ? ? 48 89 DE 48 83 C4 08"),
	SIGNATURE(Weapon_ShootPosition, clientLib, "55 48 89 E5 53 48 83 EC 18 48 89 FB E8 ? ? ? ? 0F 13 45 E8 F3 0F 11 4D F0 80 BB")
};
#elif defined(_WIN32)
const Signature signatures[] = {
	SIGNATURE(clientMode, clientLib, "A1 *? ? ? ? 8B 80 ? ? ? ? 5D"),
	SIGNATURE(clientState, engineLib, "A1 *? ? ? ? 8B 80 ? ? ? ? C3"),
	SIGNATURE(CL_RunPrediction, engineLib, "57 8B 3D ? ? ? ? 83 BF 08 01 00 00 06 75 4A"),
	SIGNATURE(RunSimulationFunc, clientLib, "55 8B EC 83 EC 08 53 8B 5D 10 56"),
	SIGNATURE(GetWeaponInfo, clientLib, "55 8B EC 83 EC 0C 53 8B 5D 08 89 4D F4 81 FB FF FF 00 00"),
	SIGNATURE(weaponDatabase, clientLib, "*? ? ? ? FF 10 0F B7 C0 B9 ? ? ? ? 50"),
	SIGNATURE(SetAbsOrigin, clientLib, "55 8B EC 83 E4 F8 51 53 56 57 8B F1"),
	SIGNATURE(SetAbsAngles, clientLib, "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"),
	SIGNATURE(SetAbsVelocity, clientLib, "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1"),
	SIGNATURE(SetupBones, clientLib, "55 8B EC 83 E4 F0 B8 D8 1C 00 00 E8 ? ? ? ? 56"),
	SIGNATURE(effectsHead, clientLib, "8B 35 *? ? ? ? 85 F6 0F 84 ? ? ? ? BB FF FF 00 00 8B 0E"),
	SIGNATURE(Weapon_ShootPosition, clientLib, "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF ? ? ? ? ? 80")
};
#endif

#endif
