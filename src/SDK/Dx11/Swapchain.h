#pragma once
HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
HWND GetWindowMeme();

bool GetD3D11SwapchainDeviceContext(void** pSwapchainTable, size_t Size_Swapchain, LPCWSTR appWindow);