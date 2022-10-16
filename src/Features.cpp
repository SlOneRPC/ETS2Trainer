#include "Features.h"
#include "SDK/Memory/Memory.h"

template <typename T>
static constexpr auto relativeToAbsolute(uintptr_t address, int addressOffset, int instructionCount) noexcept
{
	return (T)(address + instructionCount + *reinterpret_cast<std::int32_t*>(address + addressOffset));
}

static std::uint8_t* teleportFuncAddress = nullptr;
static std::uint8_t* damageFuncAddress = nullptr;
static std::uint8_t* displayDamageFuncAddress = nullptr;

static Vector3* cameraCoords = nullptr;
static Vector3* truckCoords = nullptr;
static int64_t* teleportObj = nullptr;

Features::Features()
{
	g_Features = this;
	moduleBase = GetModuleHandle(NULL);
	
	profileBase = relativeToAbsolute<uintptr_t>(
		(uintptr_t)Memory::SigScan(moduleBase, "48 8b 05 ? ? ? ? 48 8b d9 8b 90 ? ? ? ? 48 8b 80 ? ? ? ? 48 8b 88 ? ? ? ? e8 ? ? ? ? 48 8b 4b ? 3b 81 ? ? ? ? 0f 92 c0"), 3, 7);

	currentTruck = *(Truck**)Memory::FindDMAAddy(profileBase, { 0x18, 0x80 });
	moneyAddress = (int64_t*)Memory::FindDMAAddy(profileBase, { 0x10, 0x10 });
	xpAddress = (int64_t*)Memory::FindDMAAddy(profileBase, { 0x195C });

	cameraCoords = (Vector3*)Memory::FindDMAAddy(relativeToAbsolute<uintptr_t>(
		(uintptr_t)Memory::SigScan(moduleBase, "48 8b 05 ? ? ? ? 33 ff 4d 8b f8"), 3, 7), 
		{ 0x38, 0x0, 0x40 });
	
	//truckCoords = (Vector3*)Memory::FindDMAAddy((uintptr_t)moduleBase + 0x1B0BF00, { 0x0, 0x68, 0x140, 0x0, 0x14 });

	auto teleportOffset = *(int32_t*)(Memory::SigScan(moduleBase, "48 8b 89 ? ? ? ? e8 ? ? ? ? 48 8b 5c 24 ? 48 8b 74 24 ? 48 83 c4 ? 5f c3 cc cc cc cc cc cc cc cc cc cc 40 55") + 3);
	teleportObj = (int64_t*)(Memory::FindDMAAddy(relativeToAbsolute<uintptr_t>(
		(uintptr_t)Memory::SigScan(moduleBase, "48 8b 05 ? ? ? ? f3 44 0f 10 25"), 3, 7), 
		{ (unsigned int)teleportOffset }));

	teleportFuncAddress = Memory::SigScan(moduleBase, "48 81 ec ? ? ? ? 48 83 79 ? ? 41 0f b6 d9") - 20;
	damageFuncAddress = Memory::SigScan(moduleBase, "0f 28 cf ? ? ? ? ? 41 c7 84 24") + 3;
	displayDamageFuncAddress = Memory::SigScan(moduleBase, "? ? ? ? ? f3 0f 10 15 ? ? ? ? 48 8d 54 24 ? 48 8b 4b");
}

void Features::Repair()
{
	currentTruck->ChassiDmg = 0;
	currentTruck->EngineDmg = 0;
	currentTruck->TransmissionDmg = 0;
	currentTruck->CabinDmg = 0;
	currentTruck->WheelDmg = 0;
}

void Features::Refuel()
{
	currentTruck->Fuel = 1;
}
void Features::SetMoney(int64_t money)
{
	*moneyAddress = money;
}

void Features::SetXP(int64_t xp)
{
	*xpAddress = xp;
}

int64_t& Features::GetMoney()
{
	return *moneyAddress;
}

int64_t& Features::GetXp()
{
	return *xpAddress;
}

void Features::DisableDamage(bool enable)
{
	if (enable) 
	{
		Memory::Nop(damageFuncAddress, 5);
		Memory::Nop(displayDamageFuncAddress, 5);
	}
	else 
	{
		Memory::Patch(damageFuncAddress, (BYTE*)"\xE8\xC7\x26\xDA\xFF", 5);
		Memory::Patch(displayDamageFuncAddress, (BYTE*)"\xE8\xB0\x23\xB9\xFF", 5);
	}
}

void Features::TeleportToCameraCoords()
{
	TeleportToCoords(*cameraCoords);
}

void Features::TeleportToCoords(Vector3& coords)
{
	typedef char(__fastcall* teleport_t) (int64_t teleportPtr, Vector3* cameraCoords, int64_t teleportFlag, int64_t extraCalculations);
	teleport_t TeleportTruck = (teleport_t)(teleportFuncAddress);

	TeleportTruck(*teleportObj, &coords, 0, 0);
}