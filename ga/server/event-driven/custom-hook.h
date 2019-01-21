#pragma once


#include "hook-common.h"

// D3D11.1
typedef HRESULT (STDMETHODCALLTYPE *TD3D11CreateDevice)(
	IDXGIAdapter			*pAdapter,
	D3D_DRIVER_TYPE			DriverType,
	HMODULE					Software,
	UINT					Flags,
	const D3D_FEATURE_LEVEL	*pFeatureLevels,
	UINT					FeatureLevels,
	UINT					SDKVersion,
	ID3D11Device			**ppDevice,
	D3D_FEATURE_LEVEL		*pFeatureLevel,
	ID3D11DeviceContext		**ppImmediateContext
	);

typedef HRESULT(STDMETHODCALLTYPE *TCreateSwapChainForHwnd)(
	IDXGIFactory2							*This,
	IUnknown                              *pDevice,
	HWND                                  hWnd,
	const DXGI_SWAP_CHAIN_DESC1           *pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
	IDXGIOutput                           *pRestrictToOutput,
	IDXGISwapChain1                       **ppSwapChain
	);

typedef  HRESULT(STDMETHODCALLTYPE *TDXGISwapChain1Present1)(
	IDXGISwapChain1 * This,
	UINT SyncInterval,
	UINT Flags,
	const DXGI_PRESENT_PARAMETERS *pPresentParameters
	);

typedef  HRESULT(STDMETHODCALLTYPE *TDXGISwapChain1Present)(
	IDXGISwapChain1 * This,
	UINT SyncInterval,
	UINT Flags
	);

typedef HRESULT(STDMETHODCALLTYPE *TCreateDXGIFactory1)(
	REFIID riid,
	void **ppFactory
	);



extern TD3D11CreateDevice pD3D11CreateDevice;
extern TCreateDXGIFactory1 pCreateDXGIFactory1;
extern TCreateSwapChainForHwnd pCreateSwapChainForHwnd;


DllExport HRESULT __stdcall hook_D3D11CreateDevice(
	IDXGIAdapter			*pAdapter,
	D3D_DRIVER_TYPE			DriverType,
	HMODULE					Software,
	UINT					Flags,
	const D3D_FEATURE_LEVEL	*pFeatureLevels,
	UINT					FeatureLevels,
	UINT					SDKVersion,
	ID3D11Device			**ppDevice,
	D3D_FEATURE_LEVEL		*pFeatureLevel,
	ID3D11DeviceContext		**ppImmediateContext
);
DllExport HRESULT __stdcall hook_CreateSwapChainForHwnd(
	IDXGIFactory2							*This,
	IUnknown                              *pDevice,
	HWND                                  hWnd,
	const DXGI_SWAP_CHAIN_DESC1           *pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
	IDXGIOutput                           *pRestrictToOutput,
	IDXGISwapChain1                       **ppSwapChain
);

DllExport HRESULT __stdcall hook_DXGISwapChain1Present(
	IDXGISwapChain1 * This,
	UINT SyncInterval,
	UINT Flags
);
DllExport HRESULT __stdcall hook_DXGISwapChain1Present1(
	IDXGISwapChain1 * This,
	UINT SyncInterval,
	UINT Flags,
	const DXGI_PRESENT_PARAMETERS *pPresentParameters
	);

// D3D12
typedef HRESULT(STDMETHODCALLTYPE *TD3D12CreateDevice) (
	IUnknown				*pAdapter,
	D3D_FEATURE_LEVEL		MinimumFeatureLevel,
	REFIID					riid,
	void					**ppDevice
	);
extern TD3D12CreateDevice pD3D12CreateDevice;
DllExport HRESULT __stdcall hook_D3D12CreateDevice(
	IUnknown				*pAdapter,
	D3D_FEATURE_LEVEL		MinimumFeatureLevel,
	REFIID					riid,
	void					**ppDevice
);




DllExport HRESULT __stdcall hook_DXGISwapChainPresentD11(
	IDXGISwapChain * This,
	LARGE_INTEGER captureTv,
	LARGE_INTEGER initialTv,
	LARGE_INTEGER freq,
	int frame_interval
);

extern int hook_d11_1(const char *hook_type, const char *hook_method);
extern int hook_d12(const char *hook_type, const char *hook_method);
extern void proc_hook_IDXGISwapChain1_Present(IDXGISwapChain1 *ppSwapchain1);
extern int DXGI_get_resolution1(IDXGISwapChain1 *pSwapChain);
extern bool check_dx_device_version1(IDXGISwapChain1 * This, const GUID IID_target);