#include "Menu.h"
#include "Hooking.h"
#include "Features.h"

Menu::Menu()
{
	g_Menu = this;
	g_Options.moneyValue = g_Features->GetMoney();
	g_Options.XPValue = g_Features->GetXp();
}

void Menu::OnRender()
{
	ImGui::SetNextWindowSize(ImVec2{ 270,330 }, ImGuiCond_Once);
	if (ImGui::Begin("ETS2 Trainer", &g_ShowMenu,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		if (ImGui::Checkbox("Disable Damage", g_Options.disableDamage)) {
			g_Features->DisableDamage(g_Options.disableDamage);
		}

		{
			if (ImGui::Button("Repair")) {
				g_Features->Repair();
			}
			if (ImGui::Button("Refuel")) {
				g_Features->Refuel();
			}
		}

		{
			if (ImGui::SliderInt("Money value", g_Options.moneyValue, 1, 2000000))
			{
				g_Features->SetMoney(g_Options.moneyValue);
			}
			if (ImGui::SliderInt("XP value", g_Options.XPValue, 1, 2000000))
			{
				g_Features->SetXP(g_Options.XPValue);
			}
		}

		{
			if (ImGui::Button("Teleport To Camera")) 
			{
				g_Features->TeleportToCameraCoords();
			}
		}
	}
	ImGui::End();
}