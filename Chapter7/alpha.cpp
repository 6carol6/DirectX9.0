#include "d3dUtility.h"
IDirect3DDevice9 *Device = 0;
const int Width = 800;
const int Height = 600;

ID3DXMesh* Teapot = 0;
D3DMATERIAL9 TeapotMtrl;

IDirect3DVertexBuffer9* BkGndQuad = 0;
IDirect3DTexture9* BkGndTex = 0;
D3DMATERIAL9 BkGndMtrl;

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
	D3DXCreateTeapot(Device, &Teapot, 0);
	// Init Materials
	TeapotMtrl = d3d::RED_MTRL;
	TeapotMtrl.Diffuse.a = 0.5f;

	BkGndMtrl = d3d::WHITE_MTRL;

	// Create background quad snipped
	Device->CreateVertexBuffer(
		6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&BkGndQuad,
		0);

	Vertex* v;
	BkGndQuad->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-10.0f, -10.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[2] = Vertex(10.0f, -10.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f, 10.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-10.0f, 10.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(10.0f, -10.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(10.0f, 10.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);

	BkGndQuad->Unlock();

	D3DXCreateTextureFromFile(Device, L"crate.jpg", &BkGndTex);
	Device->SetTexture(0, BkGndTex);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	
	// Position and aim the camera
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	Device->SetTransform(D3DTS_VIEW, &V);
	
	// directional light
	D3DXVECTOR3 dir(0.0f, 0.0f, 1.0f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);
	
	//Device->SetRenderState(D3DRS_LIGHTING, false);
	// use alpha in material's diffuse component for alpha
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// set blending factors so that alpha component determines transparency
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
		// Update

		// increase/decrease alpha via keyboard input
		if (::GetAsyncKeyState('A') & 0x8000f)
			TeapotMtrl.Diffuse.a += 0.01f;
		else if (::GetAsyncKeyState('S') & 0x8000f)
			TeapotMtrl.Diffuse.a -= 0.01f;

		// force alpha to [0, 1] interval
		if (TeapotMtrl.Diffuse.a > 1.0f)
			TeapotMtrl.Diffuse.a = 1.0f;
		else if (TeapotMtrl.Diffuse.a < 0.0f)
			TeapotMtrl.Diffuse.a = 0.0f;

		// Render
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		// Draw the background
		D3DXMATRIX W;
		D3DXMatrixIdentity(&W);
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetFVF(Vertex::FVF);
		Device->SetStreamSource(0, BkGndQuad, 0, sizeof(Vertex));
		Device->SetMaterial(&BkGndMtrl);
		Device->SetTexture(0, BkGndTex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// Draw the teapot
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		D3DXMatrixScaling(&W, 1.5f, 1.5f, 1.5f);
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&TeapotMtrl);
		Device->SetTexture(0, 0);
		Teapot->DrawSubset(0);

		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}
void Cleanup() {
	d3d::Release<IDirect3DVertexBuffer9*>(BkGndQuad);
	d3d::Release<IDirect3DTexture9*>(BkGndTex);
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
