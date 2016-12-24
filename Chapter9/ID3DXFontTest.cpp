#include "d3dUtility.h"
#include "stdio.h"
#include "iostream"

IDirect3DDevice9* Device = 0;
const int Width = 800;
const int Height = 600;

ID3DXFont* font = 0;
DWORD FrameCnt = 0;
float TimeElapsed = 0;
float FPS = 0;
char tmp[9];
WCHAR FPSString[10];

bool Setup() {
	D3DXFONT_DESC df;
	ZeroMemory(&df, sizeof(D3DXFONT_DESC));
	df.Height = 25;
	df.Width = 12;
	df.Weight = 500;
	df.MipLevels = D3DX_DEFAULT;
	df.Italic = false;
	df.CharSet = DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality = 0;
	df.PitchAndFamily = 0;
	wcscpy(df.FaceName, L"Times New Roman");
	
	D3DXCreateFontIndirect(Device, &df, &font);
	
	return true;
}
void Cleanup() {
	d3d::Release<ID3DXFont*>(font);
}

bool Display(float timeDelta) {
	if (Device) {
		FrameCnt++;
		TimeElapsed += timeDelta;
		if (TimeElapsed >= 1.0f) {
			FPS = (float)FrameCnt / TimeElapsed;
			std::cout << FPS << std::endl;
			swprintf(FPSString, L"%f", FPS);
			FPSString[9] = '\0'; // mark end of string
			//wcscpy(FPSString, (const)tmp);
			
			TimeElapsed = 0.0f;
			FrameCnt = 0;
			
		}
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		RECT rect = { 0, 0, Width, Height };

		font->DrawText(NULL, FPSString, -1, &rect, DT_TOP | DT_LEFT, 0xff000000);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
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