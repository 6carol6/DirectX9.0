#include "d3dUtility.h"
#include "stdio.h"
#include "iostream"

IDirect3DDevice9* Device = 0;
const int Width = 800;
const int Height = 600;

ID3DXMesh* Text = 0;
D3DXVECTOR3 TextPosition(-2.0f, 0.0f, 0.0f);
D3DMATERIAL9 TextMtrl = d3d::WHITE_MTRL;

bool Setup() {
	HDC hdc = CreateCompatibleDC(0);
	HFONT hFont;
	HFONT hFontOld;

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 25;
	lf.lfWidth = 12;
	lf.lfEscapement = 0;
	lf.lfOrientation - 0;
	lf.lfWeight = 500;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	wcscpy(lf.lfFaceName, L"Times New Roman");

	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	D3DXCreateText(Device, hdc, L"Direct3D", 0.001f, 0.4f, &Text, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	// Light
	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

	Device->SetLight(0, &light);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

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


	return true;
}
void Cleanup() {
	d3d::Release<ID3DXMesh*>(Text);
}

bool Display(float timeDelta) {
	if (Device) {

		D3DXMATRIX W;
		D3DXMatrixTranslation(&W,
			TextPosition.x,
			TextPosition.y,
			TextPosition.z);

		Device->SetTransform(D3DTS_WORLD, &W);

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

		W *= Ry;

		Device->SetTransform(D3DTS_WORLD, &W);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0L);
		Device->BeginScene();
		Device->SetMaterial(&TextMtrl);
		Text->DrawSubset(0);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}