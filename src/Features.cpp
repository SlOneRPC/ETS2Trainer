#include "Features.h"
#include "SDK/Memory/Memory.h"

template <typename T>
static constexpr auto relativeToAbsolute(uintptr_t address, int addressOffset, int instructionCount) noexcept
{
	return (T)(address + instructionCount + *reinterpret_cast<std::int32_t*>(address + addressOffset));
}

Features::Features()
{
	g_Features = this;
	moduleBase = GetModuleHandle(NULL);
	
	profileBase = relativeToAbsolute<uintptr_t>((uintptr_t)Memory::SigScan(moduleBase, "48 8b 05 ? ? ? ? 48 8b d9 8b 90 ? ? ? ? 48 8b 80 ? ? ? ? 48 8b 88 ? ? ? ? e8 ? ? ? ? 48 8b 4b ? 3b 81 ? ? ? ? 0f 92 c0"), 3, 7);

	currentTruck = *(Truck**)Memory::FindDMAAddy(profileBase, { 0x18, 0x80 });
	moneyAddress = (int64_t*)Memory::FindDMAAddy(profileBase, { 0x10, 0x10 });
	xpAddress = (int64_t*)Memory::FindDMAAddy(profileBase, { 0x195C });
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
	static auto damageFuncAddress = Memory::SigScan(moduleBase, "0f 28 cf ? ? ? ? ? 41 c7 84 24") + 3;
	static auto displayDamageFuncAddress = Memory::SigScan(moduleBase, "? ? ? ? ? f3 0f 10 15 ? ? ? ? 48 8d 54 24 ? 48 8b 4b");
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
	Vector3* cameraCoords = (Vector3*)Memory::FindDMAAddy((uintptr_t)moduleBase + 0x1B3D760, { 0x38, 0x0, 0x40 });
	TeleportToCoords(*cameraCoords);
}

void Features::TeleportToCoords(Vector3& coords)
{
	static auto teleportFuncAddress = Memory::SigScan(moduleBase, "48 81 ec ? ? ? ? 48 83 79 ? ? 41 0f b6 d9") - 20;
	Memory::Patch(teleportFuncAddress, (BYTE*)"\x44\x88\x4C\x24\x20\x55\x53\x56\x41\x54\x41\x56\x48\x8D\xAC\x24\x00\xFE\xFF\xFF", 20);

	__int64* teleportObj = (__int64*)(Memory::FindDMAAddy((uintptr_t)moduleBase + 0x1B3D728, { 0x10D0 }));
	typedef char(__fastcall* teleport_t) (__int64 teleportPtr, Vector3* cameraCoords, int64_t teleportFlag, int64_t extraCalculations);

	teleport_t TeleportTruck = (teleport_t)((uintptr_t)moduleBase + 0x32DF10);

	TeleportTruck(*teleportObj, &coords, 0, 0);
}

void Features::BunnyHop()
{
	Vector3* truckCoords = (Vector3*)Memory::FindDMAAddy((uintptr_t)moduleBase + 0x1B0BF00, { 0x0, 0x68, 0x140, 0x0, 0x14 });
	//Vector3* truckRotation = (Vector3*)Memory::FindDMAAddy((uintptr_t)moduleBase + 0x1B0BF00, { 0, 68, 140, 0, 24 });
	
	Vector3 newCoords = Vector3(truckCoords->x, truckCoords->y + 50, truckCoords->z);
	*truckCoords = newCoords;
}