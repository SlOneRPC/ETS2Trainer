#include "pch.h"
#include "Features.h"
#include "GUI/GUI.h"
#include "Memory.h"
void Features::runLoop() {

	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

	if (g_Options.autorepair) {
		static float* repairAddress = (float*)Memory::FindDMAAddy(moduleBase + 0x014DD770, { 0x18, 0x80,0x18,0x8,0x10 });
		if(repairAddress)
			*repairAddress = 0.15;//0 doesnt seem to work might need to nop the function, 15% damage is fine for now
	}

	if (g_Options.speedhack) {

	}

	if(g_Options.updateMoney){
		static int* moneyAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x014DD770, { 0x10, 0x10 });
		*moneyAddress = g_Options.moneyValue;
		g_Options.updateMoney = false;
	}

	if (g_Options.updateXP) {
		static int* xpAddress = (int*)Memory::FindDMAAddy(moduleBase + 0x14DD770, { 0x1924 });
		*xpAddress = g_Options.XPValue;
		g_Options.updateXP = false;
	}
}