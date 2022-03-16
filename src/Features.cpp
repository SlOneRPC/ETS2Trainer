#include "pch.h"
#include "Features.h"
#include "GUI/GUI.h"
#include "Memory/Memory.h"
#include "ETS2.h"
uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

Truck* currentTruck = nullptr;
uintptr_t profileBase;

void Features::setup() {
	std::uint8_t* profileSig = Memory::SigScan(GetModuleHandle(NULL), "48 8b 05 ? ? ? ? 33 ff 4c 8b 40"); // mov  rbx, cs:qword_1A1E348

	ULONG32 profileRelativeOffset = *(ULONG32*)(profileSig + 3); // Offset to the Qword
	profileBase = (uintptr_t)(profileSig + profileRelativeOffset + 7); // Offset by the number of instructions

	currentTruck = *(Truck**)Memory::FindDMAAddy(profileBase, { 0x18, 0x80 });
}


void Features::runLoop() {

	if (currentTruck == nullptr)
		return;

	if (g_Options.autorepair || g_Options.doRepair) {
		currentTruck->ChassiDmg = 0;
		currentTruck->EngineDmg = 0;
		currentTruck->TransmissionDmg = 0;
		currentTruck->CabinDmg = 0;
		currentTruck->WheelDmg = 0;
		g_Options.doRepair = false;
	}

	if (g_Options.autorefuel || g_Options.doRefuel) {
		currentTruck->Fuel = 1;
		g_Options.doRefuel = false;
	}

	if(g_Options.updateMoney){
		static int* moneyAddress = (int*)Memory::FindDMAAddy(profileBase, { 0x10, 0x10 });
		if(moneyAddress)
			*moneyAddress = g_Options.moneyValue;
		g_Options.updateMoney = false;
	}

	if (g_Options.updateXP) {
		static int* xpAddress = (int*)Memory::FindDMAAddy(profileBase, { 0x195C });
		if(xpAddress)
			*xpAddress = g_Options.XPValue;
		g_Options.updateXP = false;
	}
}