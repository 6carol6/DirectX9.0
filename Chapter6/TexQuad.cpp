#include "d3dUtility.h"

IDirect3DDevice9 *Device = 0;
IDirect3DVertexBuffer9* Quad = 0;
IDirect3DTexture9* Tex = 0;
const int Width = 800;
const int Height = 600;

struct Vertex {
	Vertex() {}
	Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v) {
		x = _x; y = _y; z = _z;
		nx = _nx; ny = _ny; nz = _nz;
		u = _u; v = _v;
	}
	float x, y, z;
	float nx, ny, nz;
	float u, v;

	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Setup() {
	// Create the Quad.
	Device->CreateVertexBuffer(
		6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Quad,
		0);
	Vertex* v;
	Quad->Lock(0, 0, (void**)&v, 0);

	// quad built from two triangles:
	v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f);
	v[1] = Vertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f);

	v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f);
	v[4] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 3.0f);

	Quad->Unlock();

	// Create the texture and set texture filters.
	D3DXCreateTextureFromFile(
		Device,
		L"dx5_logo.bmp",
		&Tex);

	Device->SetTexture(0, Tex);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	// Don't use lighting for this sample.
	Device->SetRenderState(D3DRS_LIGHTING, false);

	// Set the projection matrix.
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool Display(float timeDelta) {
	if (Device) {
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, Quad, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		// set wrap adress mode
		if (::GetAsyncKeyState('W') & 0x80000f) {
			Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}
		// set border color mode
		if (::GetAsyncKeyState('B') & 0x80000f) {
			Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			Device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);
		}
		// set clamp address mode
		if (::GetAsyncKeyState('C') & 0x80000f) {
			Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}
		if (::GetAsyncKeyState('M') & 0x80000f) {
			Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}

		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

void Cleanup() {
	d3d::Release<IDirect3DVertexBuffer9*>(Quad);
	d3d::Release<IDirect3DTexture9*>(Tex);
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
