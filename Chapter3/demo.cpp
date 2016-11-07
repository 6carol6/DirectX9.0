#include "d3dUtility.h"
IDirect3DDevice9 *Device = 0;
IDirect3DVertexBuffer9* VB = 0;
IDirect3DIndexBuffer9* IB = 0;
const int Width = 800;
const int Height = 600;

struct Vertex {
	Vertex() {}
	Vertex(float x, float y, float z) {
		_x = x; _y = y; _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;

bool Setup() {
	// Create vertex and index buffers.
	Device->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0);

	// Fill the buffers with the cube data.
	Vertex* vertices;
	VB->Lock(0, 0, (void**)&vertices, 0);

	// vertices of a unit cube
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f,  1.0f, -1.0f);
	vertices[2] = Vertex( 1.0f,  1.0f, -1.0f);
	vertices[3] = Vertex( 1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f,  1.0f);
	vertices[5] = Vertex(-1.0f,  1.0f,  1.0f);
	vertices[6] = Vertex( 1.0f,  1.0f,  1.0f);
	vertices[7] = Vertex( 1.0f, -1.0f, -1.0f);

	VB->Unlock();

	// define the triangles of the cube;
	WORD* indices = 0;

	return true;
}

void Cleanup() {}

bool Display(float timeDelta) {
	if (Device) {
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->Present(0, 0, 0, 0);
	}
	return true;
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