#include "../../pch.h"
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
		LogInfo("Created Hook [" + m_name + "]");
	}
	else {
		LogError("Hooking " + m_name + " failed! Error - " + MH_StatusToString(status));
	}
}

void detour_hook::enable()
{
	if (auto status = MH_EnableHook(m_target); status == MH_OK)
		LogInfo("Started Hook [" + m_name + "]");
	else
		LogError("Failed to start hook [" + m_name + "], Error - " + MH_StatusToString(status));
}

void detour_hook::disable()
{
	if (auto status = MH_DisableHook(m_target); status == MH_OK)
		LogInfo("Disabled Hook [" + m_name + "]");
	else
		LogError("Failed to disable hook [" + m_name + "], Error - " + MH_StatusToString(status));
}

DWORD exp_handler(PEXCEPTION_POINTERS exp, std::string const& name)
{
	return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION
		? EXCEPTION_EXECUTE_HANDLER
		: EXCEPTION_CONTINUE_SEARCH;
}