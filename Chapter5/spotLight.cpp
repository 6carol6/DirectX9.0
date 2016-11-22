
#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;
const int Width = 800;
const int Height = 600;

ID3DXMesh* Box = 0;
ID3DXMesh* Sphere = 0;
ID3DXMesh* Cylinder = 0;
ID3DXMesh* Torus = 0;

bool Setup() {
	D3DXCreateBox(Device, 2.0f, 2.0f, 2.0f, &Box, 0);
	D3DXCreateSphere(Device, 2.0f, 20, 20, &Sphere, 0);
	D3DXCreateCylinder(Device, 1.0f, 1.0f, 3.0f, 20, 6, &Cylinder, 0);
	D3DXCreateTorus(Device, 1.0f, 2.0f, 10, 10, &Torus, 0);

	// Position and aim the camera
	D3DXVECTOR3 position(0.0f, 0.0f, -10.0f);
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

	/*
	// directional light
	D3DXVECTOR3 dir(1.0f, 0.0f, 0.0f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
	Device->SetLight(0, &dirLight);
	*/
	/*
	// point light
	D3DXVECTOR3 poi(0.0f, 0.0f, 0.0f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 poiLight = d3d::InitPointLight(&poi, &c);
	Device->SetLight(0, &poiLight);
	*/
	// spot light
	D3DXVECTOR3 poi(-5.0f, 0.0f, 0.0f);
	D3DXVECTOR3 dir(1.0f, 0.0f, 0.0f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 spotLight = d3d::InitSpotLight(&poi, &dir, &c);
	Device->SetLight(0, &spotLight);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	return true;
}

bool Display(float timeDelta) {
	if (Device) {
		// translate to world space
		D3DXMATRIX boxWorldMatrix;
		D3DXMatrixTranslation(&boxWorldMatrix, -2.0f, -2.0f, 0.0f);
		D3DXMATRIX sphereWorldMatrix;
		D3DXMatrixTranslation(&sphereWorldMatrix, -2.0f, 2.0f, 0.0f);
		D3DXMATRIX cylinderWorldMatrix;
		D3DXMatrixTranslation(&cylinderWorldMatrix, 2.0f, 2.0f, 0.0f);
		D3DXMATRIX torusWorldMatrix;
		D3DXMatrixTranslation(&torusWorldMatrix, 2.0f, -2.0f, 0.0f);

		D3DXMATRIX yRot;

		static float y = 0.0f;

		D3DXMatrixRotationY(&yRot, y);
		y += timeDelta;

		if (y >= 6.28f)
			y = 0.0f;

		//Device->SetTransform(D3DTS_WORLD, &yRot);

		//draw the scene:
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		boxWorldMatrix *= yRot;
		Device->SetTransform(D3DTS_WORLD, &boxWorldMatrix);
		Device->SetMaterial(&d3d::RED_MTRL);
		Box->DrawSubset(0);
		sphereWorldMatrix *= yRot;
		Device->SetTransform(D3DTS_WORLD, &sphereWorldMatrix);
		Device->SetMaterial(&d3d::BLUE_MTRL);
		Sphere->DrawSubset(0);
		cylinderWorldMatrix *= yRot;
		Device->SetTransform(D3DTS_WORLD, &cylinderWorldMatrix);
		Device->SetMaterial(&d3d::YELLO_MTRL);
		Cylinder->DrawSubset(0);
		torusWorldMatrix *= yRot;
		Device->SetTransform(D3DTS_WORLD, &torusWorldMatrix);
		Device->SetMaterial(&d3d::GREEN_MTRL);
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
