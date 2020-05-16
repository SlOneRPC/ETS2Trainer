#include "../pch.h"
#include "GUI.h"


void GUI::drawMenu() {
	static bool visible = true;

	ImGui::SetNextWindowSize(ImVec2{ 270,330 }, ImGuiCond_Once);
	if (ImGui::Begin("StonedCheats", &visible,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		static bool test = false;
		ImGui::Checkbox("Enable Speedhack", &g_Options.speedhack);
		ImGui::Checkbox("Enable Auto Repair",&g_Options.autorepair);
		ImGui::Checkbox("Enable Auto Refuel", &g_Options.autorefuel);
		if (ImGui::Button("Repair")) {

		}
		if (ImGui::Button("Refuel")) {

		}
		ImGui::SliderInt("Money value", &g_Options.moneyValue, 1, 2000000);
		if (ImGui::Button("Set Money")) {
			g_Options.updateMoney = true;
		}
		ImGui::SliderInt("XP value", &g_Options.XPValue, 1, 2000000);
		if (ImGui::Button("Set XP")) {
			g_Options.updateXP = true;
		}
	}
	ImGui::End();
}