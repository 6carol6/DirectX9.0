#include <iostream>
#include <vector>
#include "d3dUtility.h"

IDirect3DDevice9 *Device = 0;
const int Height = 600;
const int Width = 800;

ID3DXMesh* Mesh = 0;
DWORD NumSubsets = 3;

IDirect3DTexture9* tex[3] = {0, 0, 0};


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
	HRESULT hr = 0;
	hr = D3DXCreateMeshFVF(
		12,
		24,
		D3DXMESH_MANAGED,
		Vertex::FVF,
		Device,
		&Mesh);

	Vertex* vertices = 0;
	Mesh->LockVertexBuffer(0, (void**)&vertices);

	// front
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// back
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	// left
	vertices[8] = Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[9] = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[10] = Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices[11] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// right
	vertices[12] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[13] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[14] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices[15] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// up
	vertices[16] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[17] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[18] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	vertices[19] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	// down
	vertices[20] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[21] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[22] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	vertices[23] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

	Mesh->UnlockVertexBuffer();

	// define the triangles of the cube;
	WORD* indices = 0;
	Mesh->LockIndexBuffer(0, (void**)&indices);

	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 8; indices[13] = 10; indices[14] = 9;
	indices[15] = 8; indices[16] = 11; indices[17] = 10;

	// right side
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// top
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// bottom
	indices[30] = 20; indices[31] = 22; indices[32] = 21;
	indices[33] = 20; indices[34] = 23; indices[35] = 22;

	Mesh->UnlockIndexBuffer();

	DWORD* attributeBuffer = 0;
	Mesh->LockAttributeBuffer(0, &attributeBuffer);

	for (int a = 0; a < 4; a++) {
		attributeBuffer[a] = 0;
	}
	for (int b = 4; b < 8; b++) {
		attributeBuffer[b] = 1;
	}
	for (int c = 8; c < 12; c++) {
		attributeBuffer[c] = 2;
	}

	Mesh->UnlockAttributeBuffer();

	std::vector<DWORD> adjacencyBuffer(Mesh->GetNumFaces()*3);
	Mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	hr = Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&adjacencyBuffer[0],
		0, 0, 0);

	D3DXATTRIBUTERANGE* table = new D3DXATTRIBUTERANGE[NumSubsets];
	Mesh->GetAttributeTable(table, &NumSubsets);

	D3DXCreateTextureFromFile(Device, L"crate.jpg", &tex[0]);
	D3DXCreateTextureFromFile(Device, L"checker.jpg", &tex[1]);
	D3DXCreateTextureFromFile(Device, L"brick0.jpg", &tex[2]);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	Device->SetRenderState(D3DRS_LIGHTING, false);
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

		// combine x- and y-axis rotation transformations.
		D3DXMATRIX p = Rx * Ry;
		Device->SetTransform(D3DTS_WORLD, &p);

		//draw the scene:
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();


		for (int i = 0; i < NumSubsets; i++) {
			Device->SetTexture(0, tex[i]);
			Mesh->DrawSubset(i);
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}
void Cleanup() {
	d3d::Release<ID3DXMesh*>(Mesh);
	for (int i = 0; i < NumSubsets; i++)
		d3d::Release<IDirect3DTexture9*>(tex[i]);
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
