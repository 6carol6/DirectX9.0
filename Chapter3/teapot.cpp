
#include "d3dUtility.h"
IDirect3DDevice9 *Device = 0;
const int Width = 800;
const int Height = 600;

ID3DXMesh* Teapot = 0;

bool Setup() {
	D3DXCreateTeapot(Device, &Teapot, 0);

	// Position and aim the camera
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	Device->SetTransform(D3DTS_VIEW, &V);

	// Set the projection matrix.
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
	&proj,
	D3DX_PI * 0.5f, // 90 - degree
	(float)Width / (float)Height,
	1.0f,
	1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	// Switch to wireframe mode.
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}

bool Display(float timeDelta) {
	if (Device) {
		// spin the cube:
		D3DXMATRIX Rx, Ry;

		//rotate 45 degrees on x-axis
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

		//increment y-rotation angle each frame
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y += timeDelta;

		// reset angle to zero when angle reaches 2*PI
		if (y >= 6.28f) y = 0.0f;


		Device->SetTransform(D3DTS_WORLD, &Ry);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		Teapot->DrawSubset(0);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(Teapot);
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
	::PostQuitMessage(0);
	break;

	case WM_KEYDOWN:
	if (wParam == VK_ESCAPE)
	::DestroyWindow(hwnd);
	break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	if (!d3d::InitD3D(hinstance, 800, 600, true, D3DDEVTYPE_HAL, &Device)) {
	::MessageBox(0, L"InitD3D()-FAILED", 0, 0);
	return 0;
	}
	if (!Setup()) {
	::MessageBox(0, L"InitD3D()-FAILED", 0, 0);
	return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();
	return 0;
}
