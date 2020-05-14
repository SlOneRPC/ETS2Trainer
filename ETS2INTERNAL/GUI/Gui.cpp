#include "../pch.h"
#include "GUI.h"


void GUI::drawMenu() {
	static bool visible = true;

	static bool speedhack = false;
	static bool autorepair = false;
	static bool autorefuel = false;
	static int moneyValue = 100000;

	ImGui::SetNextWindowSize(ImVec2{ 270,330 }, ImGuiCond_Once);
	if (ImGui::Begin("StonedCheats", &visible,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		static bool test = false;
		ImGui::Checkbox("Enable Speedhack", &speedhack);
		ImGui::Checkbox("Enable Auto Repair",&autorepair);
		ImGui::Checkbox("Enable Auto Refuel", &autorefuel);
		if (ImGui::Button("Repair")) {

		}
		if (ImGui::Button("Refuel")) {

		}
		ImGui::SliderInt("Money value", &moneyValue, 1, 1000000000);
		if (ImGui::Button("Set Money")) {

		}
		ImGui::SliderInt("XP value", &moneyValue, 1, 1000000000);
		if (ImGui::Button("Set XP")) {

		}
	}
	ImGui::End();
}