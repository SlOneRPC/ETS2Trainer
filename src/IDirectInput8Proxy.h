/**************************************************************
 *
 * Solution    : Euro Truck Simulator 2 Multiplayer
 * Project     : Client Core
 * Licence     : See LICENSE in the top level directory
 * File		   : IDirectInput8Proxy.h
 * Developers  : RootKiller <rootkiller.programmer@gmail.com>
 *
 ***************************************************************/
#pragma once
#include "pch.h"

class IDirectInput8Proxy : public IDirectInput8
{
private:
	IDirectInput8W *    m_pDevice;

public:
	IDirectInput8Proxy(IDirectInput8 * dinput);

	/*** IUnknown methods ***/
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID * ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	/*** IDirectInput8A methods ***/
	HRESULT STDMETHODCALLTYPE CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W * lplpDirectInputDevice, LPUNKNOWN pUknOuter);
	HRESULT STDMETHODCALLTYPE EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE GetDeviceStatus(REFGUID rguidInstance);
	HRESULT STDMETHODCALLTYPE RunControlPanel(HWND hwndOwner, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE hinst, DWORD dwVersion);
	HRESULT STDMETHODCALLTYPE FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance);
	HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics(LPCWSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData);
};