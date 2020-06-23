#include "pch.h"
#include "Features.h"
#include "GUI/GUI.h"
#include "Memory.h"
void Features::runLoop() {

	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	static uintptr_t trucksPartAddress = Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x18, 0x78,0x18 });
	
	static float* chassisDMGAdress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x8,0x10 });
	static float* cabinDMGAddress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x10,0x10 });
	static float* TransmissionDMGAddress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x28,0x10 });
	static float* EngineDMGAddress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x20,0x10 });
	static float* WheelsDMGAddress = (float*)Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x18,0x78, 0x120 });
	
	static int* moneyAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x10, 0x10 });
	static int* xpAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x192C });

	if (g_Options.autorepair || g_Options.doRepair) {
		
		if (chassisDMGAdress) {
			*chassisDMGAdress = 0;
		}

		if (cabinDMGAddress) {
			*cabinDMGAddress = 0;
		}

		if (TransmissionDMGAddress) {
			*TransmissionDMGAddress = 0;
		}

		if (EngineDMGAddress) {
			*EngineDMGAddress = 0;
		}

		if (WheelsDMGAddress) {
			*WheelsDMGAddress = 0;
		}
		g_Options.doRepair = false;
	}

	if(g_Options.updateMoney){
		if(moneyAddress)
			*moneyAddress = g_Options.moneyValue;
		g_Options.updateMoney = false;
	}

	if (g_Options.updateXP) {
		if(xpAddress)
			*xpAddress = g_Options.XPValue;
		g_Options.updateXP = false;
	}
}