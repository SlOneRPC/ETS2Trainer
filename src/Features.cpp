#include "pch.h"
#include "Features.h"
#include "GUI/GUI.h"
#include "Memory.h"

uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

uintptr_t trucksPartAddress;
float* chassisDMGAdress;
float* cabinDMGAddress;
float* TransmissionDMGAddress;
float* EngineDMGAddress;
float* WheelsDMGAddress;

void Features::setup() {
	trucksPartAddress = Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x18, 0x78,0x18 });
	chassisDMGAdress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x8,0x10 });
	cabinDMGAddress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x10,0x10 });
	EngineDMGAddress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x20,0x10 });
	TransmissionDMGAddress = (float*)Memory::FindDMAAddy(trucksPartAddress, { 0x28,0x10 });
	WheelsDMGAddress = (float*)Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x18, 0x78,0x120 });
}


void Features::runLoop() {
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
		static int* moneyAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x10, 0x10 });
		if(moneyAddress)
			*moneyAddress = g_Options.moneyValue;
		g_Options.updateMoney = false;
	}

	if (g_Options.updateXP) {
		static int* xpAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x015171D0, { 0x192C });
		if(xpAddress)
			*xpAddress = g_Options.XPValue;
		g_Options.updateXP = false;
	}
}