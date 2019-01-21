#include <stdio.h>
#include <assert.h>

#include "ga-common.h"
#include "vsource.h"
#include "dpipe.h"
#include "rtspconf.h"

#include "ga-hook-common.h"
#include "hook-function.h"
#include "custom-hook.h"

#include <GL/glu.h>
#include "custom-hook.h"

TD3D11CreateDevice pD3D11CreateDevice = NULL;
TD3D12CreateDevice pD3D12CreateDevice = NULL;
TCreateDXGIFactory1 pCreateDXGIFactory1 = NULL;
TCreateSwapChainForHwnd pCreateSwapChainForHwnd = NULL;
TDXGISwapChain1Present pDXGISwapChain1Present = NULL;
TDXGISwapChain1Present1 pDXGISwapChain1Present1 = NULL;
DXGI_FORMAT pDXGI_FORMAT1 = DXGI_FORMAT_UNKNOWN;

enum DX_VERSION {
	dx_none = 0,
	dx_9,
	dx_10,
	dx_10_1,
	dx_11
};
static enum DX_VERSION dx_version1 = dx_none;

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
) {
	ga_error("hook_D3D11CreateDevice");
	ga_error("Info:{IDXGIAdapter:%d,D3D_DRIVER_TYPE:%d,HMODULE:%d,UINT:%d,D3D_FEATURE_LEVEL:%d,UINT:%d,UINT:%d,ID3D11Device:%d,D3D_FEATURE_LEVEL:%d,ID3D11DeviceContext:%d}",
		(pAdapter == NULL ? 0 : pAdapter),
		DriverType,
		(Software == NULL ? 0 : &Software),
		&Flags,
		(pFeatureLevels == NULL ? 0 : &pFeatureLevels),
		&FeatureLevels,
		&SDKVersion,
		(ppDevice == NULL ? 0 : *ppDevice),
		(pFeatureLevel == NULL ? 0 : pFeatureLevel),
		(ppImmediateContext == NULL ? 0 : *ppImmediateContext));
	HRESULT hr = pD3D11CreateDevice(pAdapter, DriverType, Software, 
		Flags, pFeatureLevels, FeatureLevels, 
		SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

	HRESULT subHr = S_OK;
	if (ppDevice != NULL && ppImmediateContext != NULL) {
		ID3D11Device *pDevice = (*ppDevice);

		IDXGIDevice *pDXGIDevice = NULL;
		subHr = pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
		if (pDXGIDevice == NULL) {
			ga_error("Get DXGIDevice failed.");
			return subHr;
		}
		assert(pDXGIDevice);

		IDXGIAdapter *pDXGIAdapter = NULL;
		subHr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
		if (pDXGIAdapter == NULL) {
			ga_error("Get DXGIAdater failed.");
			return subHr;
		}
		assert(pDXGIAdapter);
		ga_error("ad:%d", pDXGIAdapter);

		IDXGIFactory2 *pDXGIFactory2 = NULL;
		subHr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pDXGIFactory2));
		if (pDXGIFactory2 == NULL) {
			ga_error("Get DXGIFactory2 failed.");
			return subHr;
		}
		assert(pDXGIFactory2);
		ga_error("fc:%d", pDXGIFactory2);

		{
			uintptr_t* pInterfaceVTable = (uintptr_t*)*(uintptr_t*)pDXGIFactory2;
			pCreateSwapChainForHwnd = (TCreateSwapChainForHwnd)pInterfaceVTable[15];//12   // 10: IDXGIFactory::CreateSwapChain  
			ga_hook_function("IDXGIFactory2::CreateSwapChainForHwnd", pCreateSwapChainForHwnd, hook_CreateSwapChainForHwnd);
		}
	}
	return hr;
}

DllExport HRESULT __stdcall hook_CreateSwapChainForHwnd(
	IDXGIFactory2						  *This,
	IUnknown                              *pDevice,
	HWND                                  hWnd,
	const DXGI_SWAP_CHAIN_DESC1           *pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
	IDXGIOutput                           *pRestrictToOutput,
	IDXGISwapChain1                       **ppSwapChain
) {
	ga_error("hook_CreateSwapChainForHwnd");
	ga_error("Info:{IUnknown:%d,HWND:%d,DXGI_SWAP_CHAIN_DESC1:%d,DXGI_SWAP_CHAIN_FULLSCREEN_DESC:%d,IDXGIOutput:%d,IDXGISwapChain1:%d}",
		(pDevice == NULL ? 0 : 1),
		(hWnd == NULL ? 0 : 1),
		(pDesc == NULL ? 0 : 1),
		(pFullscreenDesc == NULL ? 0 : 1),
		(pRestrictToOutput == NULL ? 0 : 1),
		(ppSwapChain == NULL ? 0 : 1));
	ga_error("th:%d", This);
	HRESULT hr = S_OK;
	if (pDevice != NULL)
	hr = pCreateSwapChainForHwnd(This, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);

	if (pDevice != NULL && ppSwapChain != NULL)
		proc_hook_IDXGISwapChain1_Present(*ppSwapChain);

	return hr;
}

void proc_hook_IDXGISwapChain1_Present(IDXGISwapChain1 *ppSwapchain1) {
	ga_error("proc_hook_IDXGISwapChain1_Present");
	uintptr_t *pInterfaceVTable = (uintptr_t *)*(uintptr_t *)ppSwapchain1;   // IDXGISwapChain

	pDXGISwapChain1Present = (TDXGISwapChain1Present)pInterfaceVTable[8];   // IDXGISwapChain::Present()

	ga_hook_function("IDXGISwapChain1::Present", pDXGISwapChain1Present, hook_DXGISwapChain1Present);
}

DllExport HRESULT __stdcall hook_DXGISwapChain1Present1(
	IDXGISwapChain1 * This,
	UINT SyncInterval,
	UINT Flags,
	const DXGI_PRESENT_PARAMETERS *pPresentParameters
) {
	ga_error("hook_DXGISwapChain1Present");
	static int frame_interval;
	static LARGE_INTEGER initialTv, captureTv, freq;
	static int capture_initialized = 0;
	//
	int i;
	dpipe_buffer_t *data;
	vsource_frame_t *frame;
	//
	DXGI_SWAP_CHAIN_DESC pDESC;
	HRESULT hr = pDXGISwapChain1Present(This, SyncInterval, Flags);

	if (resolution_retrieved == 0) {
		if (DXGI_get_resolution1(This) >= 0) {
			resolution_retrieved = 1;
		}
		return hr;
	}

	if (vsource_initialized == 0) {
		ga_error("video source not initialized.\n");
		return hr;
	}


	This->GetDesc(&pDESC);
	pDXGI_FORMAT1 = pDESC.BufferDesc.Format;   // extract screen format for sws_scale

	if (pDESC.BufferDesc.Width != game_width
		|| pDESC.BufferDesc.Height != game_height) {
		ga_error("game width/height mismatched (%dx%d) != (%dx%d)\n",
			pDESC.BufferDesc.Width, pDESC.BufferDesc.Height,
			game_width, game_height);
		return hr;
	}

	//
	if (enable_server_rate_control && ga_hook_video_rate_control() < 0)
		return hr;

	if (dx_version1 == dx_none) {
		//bool check_result = FALSE;
		if (check_dx_device_version1(This, IID_ID3D10Device)) {
			dx_version1 = dx_10;
			ga_error("[DXGISwapChain] DirectX 10\n");
		}
		else if (check_dx_device_version1(This, IID_ID3D10Device1)) {
			dx_version1 = dx_10_1;
			ga_error("[DXGISwapChain] DirectX 10.1\n");
		}
		else if (check_dx_device_version1(This, IID_ID3D11Device)) {
			dx_version1 = dx_11;
			ga_error("[DXGISwapChain] DirectX 11\n");
		}
	}

	if (capture_initialized == 0) {
		frame_interval = 1000000 / video_fps; // in the unif of us
		frame_interval++;
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&initialTv);
		capture_initialized = 1;
	}
	else {
		QueryPerformanceCounter(&captureTv);
	}

	hr = 0;

	// d3d10 / d3d10.1
	if (dx_version1 == dx_11) {
		return hook_DXGISwapChainPresentD11(This, captureTv, initialTv, freq, frame_interval);
	}

	return hr;
}

DllExport HRESULT __stdcall hook_DXGISwapChain1Present(
	IDXGISwapChain1 * This,
	UINT SyncInterval,
	UINT Flags
) {
	ga_error("hook_DXGISwapChain1Present");
	static int frame_interval;
	static LARGE_INTEGER initialTv, captureTv, freq;
	static int capture_initialized = 0;
	//
	int i;
	dpipe_buffer_t *data;
	vsource_frame_t *frame;
	//
	DXGI_SWAP_CHAIN_DESC pDESC;
	HRESULT hr = pDXGISwapChain1Present(This, SyncInterval, Flags);

	if (resolution_retrieved == 0) {
		if (DXGI_get_resolution1(This) >= 0) {
			resolution_retrieved = 1;
		}
		return hr;
	}

	if (vsource_initialized == 0) {
		ga_error("video source not initialized.\n");
		return hr;
	}


	This->GetDesc(&pDESC);
	pDXGI_FORMAT1 = pDESC.BufferDesc.Format;   // extract screen format for sws_scale

	if (pDESC.BufferDesc.Width != game_width
		|| pDESC.BufferDesc.Height != game_height) {
		ga_error("game width/height mismatched (%dx%d) != (%dx%d)\n",
			pDESC.BufferDesc.Width, pDESC.BufferDesc.Height,
			game_width, game_height);
		return hr;
	}

	//
	if (enable_server_rate_control && ga_hook_video_rate_control() < 0)
		return hr;

	if (dx_version1 == dx_none) {
		//bool check_result = FALSE;
		if (check_dx_device_version1(This, IID_ID3D10Device)) {
			dx_version1 = dx_10;
			ga_error("[DXGISwapChain] DirectX 10\n");
		}
		else if (check_dx_device_version1(This, IID_ID3D10Device1)) {
			dx_version1 = dx_10_1;
			ga_error("[DXGISwapChain] DirectX 10.1\n");
		}
		else if (check_dx_device_version1(This, IID_ID3D11Device)) {
			dx_version1 = dx_11;
			ga_error("[DXGISwapChain] DirectX 11\n");
		}
	}

	if (capture_initialized == 0) {
		frame_interval = 1000000 / video_fps; // in the unif of us
		frame_interval++;
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&initialTv);
		capture_initialized = 1;
	}
	else {
		QueryPerformanceCounter(&captureTv);
	}

	hr = 0;

	// d3d10 / d3d10.1
	if (dx_version1 == dx_11) {
		return hook_DXGISwapChainPresentD11(This, captureTv, initialTv, freq, frame_interval);
	}

	return hr;
}

int DXGI_get_resolution1(IDXGISwapChain1 *pSwapChain) {
	DXGI_SWAP_CHAIN_DESC pDESC;
	static int initialized = 0;
	//
	if (initialized > 0) {
		return 0;
	}
	// get current resolution
	pSwapChain->GetDesc(&pDESC);
	//
	if (ga_hook_get_resolution(pDESC.BufferDesc.Width, pDESC.BufferDesc.Height) < 0)
		return -1;
	initialized = 1;
	return 0;
}

bool check_dx_device_version1(IDXGISwapChain1 * This, const GUID IID_target) {
	IUnknown *pDevice = NULL;
	HRESULT hr;
	ga_error("check_dx_device_version1");
	if (dx_version1 != dx_none)
		return FALSE;  // this device has been checked

	hr = This->GetDevice(IID_target, (void**)&pDevice);
	ga_error("check_dx_device_version1: 0x%x", hr);
	if (FAILED(hr) || pDevice == NULL) {  // failed to get this device 
		if (pDevice != NULL)
			pDevice->Release();
		return FALSE;
	}

	pDevice->Release();
	return TRUE;
}


DllExport HRESULT __stdcall hook_D3D12CreateDevice(
	IUnknown				*pAdapter,
	D3D_FEATURE_LEVEL		MinimumFeatureLevel,
	REFIID					riid,
	void					**ppDevice
) {
	GA_DBG("hook_D3D12CreateDevice");
	return S_OK;
}

int hook_d11_1(const char *hook_type, const char *hook_method) {
	GA_DBG("hook d11_1");
	HMODULE hMod;
	//
	if ((hMod = GetModuleHandle("d3d11.dll")) == NULL) {
		if ((hMod = LoadLibrary("d3d11.dll")) == NULL) {
			ga_error("Load d3d11.dll failed.\n");
			return -1;
		}
	}
	pD3D11CreateDevice = (TD3D11CreateDevice)GetProcAddress(hMod, "D3D11CreateDevice");
	if (pD3D11CreateDevice == NULL) {
		ga_error("GetProcAddress(D3D11CreateDevice) failed.\n");
		return -1;
	}
	//
	ga_hook_function("D3D11CreateDevice",
		pD3D11CreateDevice,
		hook_D3D11CreateDevice);
	return 0;
}

int hook_d12(const char *hook_type, const char *hook_method) {
	GA_DBG("hook d12");
	HMODULE hMod;
	//
	if ((hMod = GetModuleHandle("d3d12.dll")) == NULL) {
		if ((hMod = LoadLibrary("d3d12.dll")) == NULL) {
			ga_error("Load d3d11.dll failed.\n");
			return -1;
		}
	}
}

DllExport HRESULT __stdcall hook_DXGISwapChainPresentD11(
	IDXGISwapChain * This,
	LARGE_INTEGER captureTv,
	LARGE_INTEGER initialTv,
	LARGE_INTEGER freq,
	int frame_interval
) {
	ga_error("hook_DXGISwapChainPresentD11");
	int i;
	dpipe_buffer_t *data;
	vsource_frame_t *frame;

	HRESULT hr = S_OK;
	void *ppDevice;
	This->GetDevice(IID_ID3D11Device, &ppDevice);
	ID3D11Device *pDevice = (ID3D11Device*)ppDevice;

	ID3D11DeviceContext *pDeviceContext = NULL;
	pDevice->GetImmediateContext(&pDeviceContext);

	ID3D11RenderTargetView *pRTV = NULL;
	ID3D11Resource *pSrcResource = NULL;
	pDeviceContext->OMGetRenderTargets(1, &pRTV, NULL);
	pRTV->GetResource(&pSrcResource);

	// TODO: try to check resource type
	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	pSrcResource->GetType(&resType);
	if (resType != D3D11_RESOURCE_DIMENSION_TEXTURE2D) {
		GA_DBG("Resource type isn't supported.");
		return hr;
	}

	ID3D11Texture2D *pSrcBuffer = NULL;
	ID3D11Texture2D *pDstBuffer = NULL;

	hr = pSrcResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pSrcBuffer));
	if (FAILED(hr)) {
		GA_DBG("Get Texture2D source from resource failed.");
		return hr;
	}
	assert(pSrcBuffer);

	D3D11_TEXTURE2D_DESC desc;
	pSrcBuffer->GetDesc(&desc);
	if (desc.SampleDesc.Count > 1) {
		//GA_DBG("desc.SampleDesc.Count > 1");
		// MSAA content must be resolved before being copied to a staging texture
		//GA_DBG("desc:{desc.SampleDesc.Count:%d,ArraySize:%d,MipLevels:%d}", desc.SampleDesc.Count, desc.ArraySize, desc.MipLevels);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		ID3D11Texture2D *pTemp = NULL;
		hr = pDevice->CreateTexture2D(&desc, 0, &pTemp);
		if (FAILED(hr)) {
			GA_DBG("Create temp Texture2D failed: 0x%x", hr);
			return hr;
		}
		assert(pTemp);

		DXGI_FORMAT fmt = desc.Format;
		UINT support = 0;
		hr = pDevice->CheckFormatSupport(fmt, &support);
		if (FAILED(hr)) {
			GA_DBG("Check support format failed: 0x%x", hr);
			return hr;
		}

		if (!(support & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE)) {
			GA_DBG("!(support & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE)");
			return E_FAIL;
		}

		for (UINT item = 0; item < desc.ArraySize; ++item) {
			for (UINT level = 0; level < desc.MipLevels; ++level) {
				UINT index = D3D11CalcSubresource(level, item, desc.MipLevels);
				pDeviceContext->ResolveSubresource(pTemp, index, pSrcResource, index, fmt);
			}
		}

		desc.BindFlags = 0;
		desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;

		hr = pDevice->CreateTexture2D(&desc, NULL, &pDstBuffer);
		if (FAILED(hr)) {
			GA_DBG("Failed to create tex2D 0x%x", hr);
			return hr;
		}
		assert(pDstBuffer);
		pDeviceContext->CopyResource(pDstBuffer, pTemp);
	}
	else if ((desc.Usage == D3D11_USAGE_STAGING) && (desc.CPUAccessFlags & D3D10_CPU_ACCESS_READ)) {
		GA_DBG("(desc.Usage == D3D11_USAGE_STAGING) && (desc.CPUAccessFlags & D3D10_CPU_ACCESS_READ)");
		// Handle case where the source is already a staging texture we can use directly
		pDstBuffer = pSrcBuffer;
	}
	else {
		GA_DBG("Create new dest texture2D");
		GA_DBG("desc:{ArraySize:%d,Height:%d,Width:%d,MipLevels:%d,MiscFlags:%d,BindFlags:%d,SampleDesc:{Count:%d,Quality:%d}}", 
			desc.ArraySize, desc.Height, desc.Width, desc.MipLevels, desc.MiscFlags, desc.BindFlags,
			desc.CPUAccessFlags, desc.SampleDesc.Count, desc.SampleDesc.Quality);
		// Otherwise, create a staging texture from the non-MSAA source
		desc.BindFlags = 0;
		desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;

		hr = pDevice->CreateTexture2D(&desc, 0, &pDstBuffer);
		if (FAILED(hr)) {
			GA_DBG("Create Text2D failed: 0x%x", hr);
			return hr;
		}
		assert(pDstBuffer);
		pDeviceContext->CopyResource(pDstBuffer, pSrcBuffer);
	}

	/*GA_DBG("desc:{ArraySize:%u,Height:%u,Width:%u,MipLevels:%u,MiscFlags:%u,BindFlags:%u}",
		desc.ArraySize, desc.Height, desc.Width, desc.MipLevels, desc.MiscFlags, desc.BindFlags);*/

	//GA_DBG("Map data");
	D3D11_MAPPED_SUBRESOURCE mapped_screen;
	hr = pDeviceContext->Map(pDstBuffer, 0, D3D11_MAP_READ, 0, &mapped_screen);
	if (FAILED(hr)) {
		OutputDebugString("Failed to map from DeviceContext");
		GA_DBG("Map Tex2D error: 0x%x", hr);
		//assert(exp_state == exp_none);
		return hr;
	}
	//GA_DBG("mapped_screen:{RowPitch:%u,DepthPitch:%u}", mapped_screen.RowPitch, mapped_screen.DepthPitch);

	// copy image 
	do {
		unsigned char *src, *dst;
		data = dpipe_get(g_pipe[0]);
		frame = (vsource_frame_t*)data->pointer;
		frame->pixelformat = PIX_FMT_BGRA;//PIX_FMT_RGBA;
		frame->realwidth = desc.Width;
		frame->realheight = desc.Height;
		frame->realstride = desc.Width << 2;
		frame->realsize = frame->realheight * frame->realstride;
		frame->linesize[0] = frame->realstride;
		//
		src = (unsigned char*)mapped_screen.pData;
		dst = (unsigned char*)frame->imgbuf;
		for (i = 0; i < encoder_height; i++) {
			CopyMemory(dst, src, frame->realstride);
			src += mapped_screen.RowPitch;
			dst += frame->realstride;
		}
		frame->imgpts = pcdiff_us(captureTv, initialTv, freq) / frame_interval;
		gettimeofday(&frame->timestamp, NULL);
	} while (0);

	// duplicate from channel 0 to other channels
	for (i = 1; i < SOURCES; i++) {
		int j;
		dpipe_buffer_t *dupdata;
		vsource_frame_t *dupframe;
		dupdata = dpipe_get(g_pipe[i]);
		dupframe = (vsource_frame_t*)dupdata->pointer;
		//
		vsource_dup_frame(frame, dupframe);
		//
		dpipe_store(g_pipe[i], dupdata);
	}
	dpipe_store(g_pipe[0], data);

	pDeviceContext->Unmap(pDstBuffer, 0);

	pDevice->Release();
	pDeviceContext->Release();
	pRTV->Release();
	pSrcResource->Release();
	pSrcBuffer->Release();
	pDstBuffer->Release();
	return hr;
}