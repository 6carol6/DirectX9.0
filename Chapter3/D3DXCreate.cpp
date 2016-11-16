#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;
const int Width = 800;
const int Height = 600;

ID3DXMesh* Box = 0;
ID3DXMesh* Sphere = 0;
ID3DXMesh* Cylinder = 0;
ID3DXMesh* Torus = 0;

bool Setup() {
	D3DXCreateBox(Device, 1.0f, 1.0f, 1.0f, &Box, 0);
	D3DXCreateSphere(Device, 2.0f, 4, 4, &Sphere, 0);
	D3DXCreateCylinder(Device, 1.0f, 1.0f, 3.0f, 3, 6, &Cylinder, 0);
	D3DXCreateTorus(Device, 1.0f, 2.0f, 10, 10, &Torus, 0);

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
		//draw the scene:
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Box->DrawSubset(0);
		Sphere->DrawSubset(0);
		Cylinder->DrawSubset(0);
		Torus->DrawSubset(0);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}
void Cleanup()
{
	d3d::Release<ID3DXMesh*>(Box);
	d3d::Release<ID3DXMesh*>(Sphere);
	d3d::Release<ID3DXMesh*>(Cylinder);
	d3d::Release<ID3DXMesh*>(Torus);
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
