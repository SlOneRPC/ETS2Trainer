#include "pch.h"
#include "detour.h"
#include "MinHook.h"

detour_hook::detour_hook() {}

void detour_hook::start(std::string name, void* target, void* detour) 
{
	m_name = std::move(name);
	m_target = target;
	m_detour = detour;

	if (auto status = MH_CreateHook(m_target, m_detour, &m_original); status == MH_OK)
	{
		writeToConsole("Created Hook [" + m_name + "]");
	}
	else {
		writeToConsole("Hooking " + m_name + " failed! Error - " + MH_StatusToString(status),true);
	}
}
//
//detour_hook::~detour_hook() noexcept
//{
//	/*if (m_target)
//	{
//		MH_RemoveHook(m_target);
//	}
//	writeToConsole("Removed hook" + m_name);*/
//}

void detour_hook::enable()
{
	if (auto status = MH_EnableHook(m_target); status == MH_OK)
		writeToConsole("Started Hook [" + m_name + "]");
	else
		writeToConsole("Failed to start hook [" + m_name + "], Error - " + MH_StatusToString(status),true);
}

void detour_hook::disable()
{
	if (auto status = MH_DisableHook(m_target); status == MH_OK)
		writeToConsole("Disabled Hook [" + m_name + "]");
	else
		writeToConsole("Failed to disable hook [" + m_name + "], Error - " + MH_StatusToString(status),true);
}

DWORD exp_handler(PEXCEPTION_POINTERS exp, std::string const& name)
{
	return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION
		? EXCEPTION_EXECUTE_HANDLER
		: EXCEPTION_CONTINUE_SEARCH;
}

