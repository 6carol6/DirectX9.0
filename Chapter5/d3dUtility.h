#ifndef _D3DUTILITY_H__
#define _D3DUTILITY_H__
#include <d3dx9.h>

namespace d3d {
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	bool InitD3D(
		HINSTANCE hInstance,
		int width, int height,
		bool windowed,
		D3DDEVTYPE deviceType,
		IDirect3DDevice9** device);

	int EnterMsgLoop(
		bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t) {
		if (t) {
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t) {
		if (t) {
			delete t;
			t = 0;
		}
	}
}

#endif