#pragma once
namespace GUI {
	void drawMenu();
}

class MenuOptions
{
public:
	bool speedhack = false;
	bool autorepair = false;
	bool autorefuel = false;
	int moneyValue = 100000;
	int XPValue = 100000;

	bool updateMoney = false;
	bool updateXP = false;

	bool doRepair = false;
	bool doRefuel = false;
};


inline MenuOptions g_Options;