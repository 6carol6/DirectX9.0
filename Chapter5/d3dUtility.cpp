#include "d3dUtility.h"

bool d3d::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device) {
	//register the window class
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"Direct3D9App", NULL
	};
	if (!RegisterClassEx(&wc)) {
		return false;
	}
	HWND hwnd = CreateWindow(L"Direct3D9App", L"Direct3D9App", WS_OVERLAPPEDWINDOW,
		                     0, 0, width, height, NULL, NULL, hInstance, NULL);

	if (!hwnd) {
		::MessageBox(0, TEXT("CreateWindow() - FAILED"), 0, 0);
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);


	//initial idx pointer
	IDirect3D9 *_d3d9;
	_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//check hardware
	D3DCAPS9 caps;
	_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//set parameters
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = windowed;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	HRESULT hr = _d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);
	if (FAILED(hr)) {
		::MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
		return false;
	}
	_d3d9->Release(); //why?
	return true;
}

int d3d::EnterMsgLoop(bool(*ptr_display)(float)) {
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT) {
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else {
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			ptr_display(timeDelta);

			lastTime = currTime;
		}
	}
	return msg.wParam;
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p) {
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color) {
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color) {
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;

	return light;
}

D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color) {
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Direction = *direction;

	return light;
}