#include "Features.h"
#include "SDK/Memory/Memory.h"

Features::Features()
{
	g_Features = this;
	moduleBase = GetModuleHandle(NULL);
	std::uint8_t* profileSig = Memory::SigScan(moduleBase, "48 8b 05 ? ? ? ? 48 8b d9 8b 90 ? ? ? ? 48 8b 80 ? ? ? ? 48 8b 88 ? ? ? ? e8 ? ? ? ? 48 8b 4b ? 3b 81 ? ? ? ? 0f 92 c0"); // mov     rax, cs:qword_1B4D768

	ULONG32 profileRelativeOffset = *(ULONG32*)(profileSig + 3); // Offset to the Qword
	profileBase = (uintptr_t)(profileSig + profileRelativeOffset + 7); // Offset by the number of instructions

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