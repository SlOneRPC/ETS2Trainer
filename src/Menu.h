#pragma once
#include "pch.h"

#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}

template <typename T = bool>
class Var {
public:
	std::string name;
	std::shared_ptr<T> value;
	int32_t size;
	Var(std::string name, T v) : name(name) {
		value = std::make_shared<T>(v);
		size = sizeof(T);
	}
	operator T() { return *value; }
	operator T* () { return &*value; }
	operator T() const { return *value; }
	Var<T>& operator=(const T& other)
	{
		*value = other;
		return *this;
	}
	bool operator==(const T& other) { return *value == other; }
	bool operator==(const Var<T>& other) { return *value == *other.value && name == other.name && size == other.size; }

	//operator T*() const { return value; }
};


class Options
{
public:
	OPTION(bool, speedhack, false);
	OPTION(bool, disableDamage, false);
	OPTION(int, moneyValue, 100000);
	OPTION(int, XPValue, 100000);
};


class Menu {
public:
	Menu();

	void OnRender();

	bool visible = true;
private:

	void HandleInput();
};

inline Options g_Options;
inline Menu* g_Menu;