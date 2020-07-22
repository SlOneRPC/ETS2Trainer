#include "pch.h"
#include "Features.h"
#include "GUI/GUI.h"
#include "Memory.h"
#include "ETS2.h"
uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

Truck* currentTruck = nullptr;
TruckParts* currentParts = nullptr;
void Features::setup() {
	currentTruck = *(Truck**)Memory::FindDMAAddy(moduleBase + 0x0160ABA0, { 0x18, 0x78 });
	currentParts = currentTruck->partsPTR;
}


void Features::runLoop() {

	if (!currentTruck)
		return;

	if (g_Options.autorepair || g_Options.doRepair) {
		currentParts->chassisPTR->damage = 0;
		currentParts->cabinPTR->damage = 0;
		currentParts->enginePTR->damage = 0;
		currentParts->transPTR->damage = 0;
		currentTruck->wheelDmg = 0;
		g_Options.doRepair = false;
	}

	if (g_Options.autorefuel || g_Options.doRefuel) {
		currentTruck->Fuel = 1;
		g_Options.doRefuel = false;
	}

	if(g_Options.updateMoney){
		static int* moneyAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x0160ABA0, { 0x10, 0x10 });
		if(moneyAddress)
			*moneyAddress = g_Options.moneyValue;
		g_Options.updateMoney = false;
	}

	if (g_Options.updateXP) {
		static int* xpAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x0160ABA0, { 0x1914 });
		if(xpAddress)
			*xpAddress = g_Options.XPValue;
		g_Options.updateXP = false;
	}
}