#include "d3dUtility.h"

bool d3d::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device) {
	IDirect3D9 *_d3d9;
	_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.Windowed = windowed;
	
	HRESULT hr = _d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, device);
	if (FAILED(hr)) {
		::MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
		return false;
	}
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