#pragma once
#include "pch.h"
#include "ETS2.h"

class Vector3 { float x, y, z; };

class Features {
public:
	Features();

	void Repair();
	void Refuel();
	void SetMoney(int64_t money);
	void SetXP(int64_t xp);
	int64_t& GetMoney();
	int64_t& GetXp();
	void DisableDamage(bool enable);
	void TeleportToCameraCoords();
	void TeleportToCoords(Vector3& coords);

private:
	Truck* currentTruck = nullptr;
	uintptr_t profileBase;
	HMODULE moduleBase;
	int64_t* moneyAddress = nullptr;
	int64_t* xpAddress = nullptr;
};

inline Features* g_Features = nullptr;