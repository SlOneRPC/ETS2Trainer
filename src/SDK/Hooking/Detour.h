#pragma once
#include "../../pch.h"

class detour_hook
{
public:
	detour_hook();

	void start(std::string name, void* target, void* detour);
	void enable();
	void disable();

	template <typename T>
	T get_original();

	void fix_hook_address();
private:
	std::string m_name;
	void* m_target;
	void* m_detour;
	void* m_original{};
};

template <typename T>
inline T detour_hook::get_original()
{
	return static_cast<T>(m_original);
}