#include<d3d9.h>
#include<d3dx9.h>
#include<time.h>
#include<tchar.h>
#include<dinput.h>
#include<string>
#include"CameraClass.h"
#include"TerrainClass.h"
#include"SkyClass.h"

HWND hwnd;
LPDIRECT3D9 pd3d9 = NULL;
LPDIRECT3DDEVICE9 pd3dd = NULL;

D3DXMATRIX matWorld;//�������

LPDIRECT3DTEXTURE9 pt;//����

LPD3DXFONT  pFont_FPS; //����
LPD3DXFONT	pFont_STR;

float FPS = 0.0f;//֡��
WCHAR strFPS[50];//֡���ַ���

LPDIRECTINPUT8 pDirectInput;  //directinput8 �ӿ�
LPDIRECTINPUTDEVICE8  pMouseDevice;//��������豸�ӿ�
DIMOUSESTATE	diMouseState;
LPDIRECTINPUTDEVICE8	pKeyboardDevice;
char pKeyStateBuffer[256] = { 0 };

LPD3DXMESH	pMesh = NULL;//�������
D3DMATERIAL9	*pMaterials = NULL;//���������Ϣָ��
LPDIRECT3DTEXTURE9 *pTextures = NULL;//����������Ϣָ���ָ��
DWORD	dwNumMtrls;//������Ŀ

CameraClass *pCamera;	//�����ָ��
TerrainClass *pTerrain;//����ָ��
SkyClass *pSkyBox;		//���ָ��

HRESULT InitializeObjects() {

	//��������
	D3DXCreateFont(pd3dd, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Calibri"), &pFont_FPS);
	D3DXCreateFont(pd3dd, 20, 0, 0, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("�л�����"), &pFont_STR);

	//����X�ļ�����������
	LPD3DXBUFFER pAdjBuffer = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;

	D3DXLoadMeshFromX(TEXT("Xfile\\A2.x"), D3DXMESH_MANAGED, pd3dd, &pAdjBuffer,
		&pMtrlBuffer, NULL, &dwNumMtrls, &pMesh);

	//��ȡ���ʺ���������
	D3DXMATERIAL *pMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
	pMaterials = new D3DMATERIAL9[dwNumMtrls];
	pTextures = new LPDIRECT3DTEXTURE9[dwNumMtrls];


	for (DWORD i = 0; i < dwNumMtrls; i++)
	{
		//��ȡ���ʣ������û��������ɫֵ
		pMaterials[i] = pMtrls[i].MatD3D;
		pMaterials[i].Ambient = pMaterials[i].Diffuse;

		//�����������
		pTextures[i] = NULL;
		D3DXCreateTextureFromFileA(pd3dd, pMtrls[i].pTextureFilename, &pTextures[i]);
	}
	pAdjBuffer->Release();
	pMtrlBuffer->Release();



	//���ù���
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	pd3dd->SetLight(0, &light);
	pd3dd->LightEnable(0, true);
	pd3dd->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pd3dd->SetRenderState(D3DRS_SPECULARENABLE, true);

	//��������ʼ�������
	pCamera = new CameraClass(pd3dd);
	pCamera->SetCameraPosition(&D3DXVECTOR3(0.0f, 50.0f, -10.0f));
	pCamera->SetTargetPosition(&D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	pCamera->SetViewMatrix();
	pCamera->SetProjMatrix();

	//��ʼ������
	pTerrain = new TerrainClass(pd3dd);
	pTerrain->LoadTerrainFromFile(TEXT("heighmap.raw"),
		TEXT("1.jpg"));
	pTerrain->InitTerrain(112, 108, 1.0f, 1.0f);


	//��ʼ�����
	pSkyBox = new SkyClass(pd3dd);
	pSkyBox->LoadSkyTextureFromFile(TEXT("sky\\sky_front.jpg"), TEXT("sky\\sky_back.jpg"),
		TEXT("sky\\sky_left.jpg"), TEXT("sky\\sky_right.jpg"),TEXT("sky\\sky_up.jpg"));
	pSkyBox->InitSky(512);

	pd3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);//���ø�������������ˣ���С
	pd3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);//�Ŵ�
	pd3dd->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);//�������Եȼ�

	pd3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return  S_OK;
}
HRESULT InitD3D(HWND hWnd, HINSTANCE hInstance)
{
	D3DDISPLAYMODE	displayMode;
	if (NULL == (pd3d9 = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

	if (FAILED(pd3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode))) return E_FAIL;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.BackBufferHeight = 800;
	d3dpp.BackBufferWidth = 600;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 2;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(pd3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3dd))) return E_FAIL;
	//����DirectInput�ӿ�
	DirectInput8Create(hInstance, 0x0800, IID_IDirectInput8, (void**)&pDirectInput, NULL);

	//���������豸�ӿ�
	pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboardDevice, NULL);

	//�������ݸ�ʽ��Э������
	pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);//��̨�Ƕ�ռģʽ

	//��������豸�ӿ�
	pDirectInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);

	pMouseDevice->SetDataFormat(&c_dfDIMouse);
	pMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);//ǰ̨��ռģʽ



	if (FAILED(InitializeObjects())) return  E_FAIL;

	pd3d9->Release();
	pDirectInput->Release();

	return S_OK;
}
VOID Cleanup()
{
	for (DWORD i = 0; i < dwNumMtrls; i++)
		pTextures[i]->Release();
	pMesh->Release();
	pFont_FPS->Release();
	pFont_STR->Release();
	pd3dd->Release();
}
float Get_FPS() {
	static float fps = 0;
	static int frameCount = 0;
	static float currentTime = 0.0f;
	static float lastTime = 0.0f;

	frameCount++;
	currentTime = timeGetTime()*0.001f;
	if (currentTime - lastTime > 1.0f)
	{
		fps = (float)frameCount / (currentTime - lastTime);
		lastTime = currentTime;
		frameCount = 0;
	}
	return fps;
}
//��ȡ�豸��������
BOOL Device_Read(IDirectInputDevice8*pDIDevice, void*pBuffer, long lSize) {
	HRESULT hr;
	while (true)
	{
		pDIDevice->Poll();
		pDIDevice->Acquire();
		if (SUCCEEDED(hr = pDIDevice->GetDeviceState(lSize, pBuffer))) break;
		if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED) return FALSE;
		if (FAILED(pDIDevice->Acquire())) return FALSE;
	}
	return TRUE;
}
VOID Direct3D_Update() {
	//��ȡ�������
	::ZeroMemory(&diMouseState, sizeof(diMouseState));
	Device_Read(pMouseDevice, (LPVOID)&diMouseState, sizeof(diMouseState));
	//��ȡ��������
	::ZeroMemory(pKeyStateBuffer, sizeof(pKeyStateBuffer));
	Device_Read(pKeyboardDevice, (LPVOID)pKeyStateBuffer, sizeof(pKeyStateBuffer));

	//���������
	if (pKeyStateBuffer[DIK_1] & 0x80) pCamera->SetCameraType(1);//������������
	if (pKeyStateBuffer[DIK_2] & 0x80)pCamera->SetCameraType(2);//����������

	//��Ӱ���������ƶ��ӽ�
	if (pKeyStateBuffer[DIK_A] & 0x80) pCamera->strafe(-0.01f);
	if (pKeyStateBuffer[DIK_D] & 0x80)pCamera->strafe(0.01f);
	if (pKeyStateBuffer[DIK_W] & 0x80)pCamera->walk(0.01f);
	if (pKeyStateBuffer[DIK_S] & 0x80)pCamera->walk(-0.01f);
	if (pKeyStateBuffer[DIK_SPACE] & 0x80)pCamera->fly(0.01f);
	if (pKeyStateBuffer[DIK_LCONTROL] & 0x80)pCamera->fly(-0.01f);

	//��Ӱ���ӽ���б
	if (pKeyStateBuffer[DIK_Q] & 0x80) pCamera->RotationLookVec(0.01f);
	if (pKeyStateBuffer[DIK_E] & 0x80)pCamera->RotationLookVec(-0.01f);

	//������������������������ת
	pCamera->yaw(diMouseState.lX*0.001f);
	pCamera->pitch(diMouseState.lY*0.001f);

	//���㲢����ȡ���任����
	D3DXMATRIX matView;
	pCamera->CalculateViewMatrix(&matView);
	pd3dd->SetTransform(D3DTS_VIEW, &matView);

	//����ȷ������任����浽matWorld��
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);

	//����������ƶ�����
	POINT lt, rb;
	RECT rect;
	GetClientRect(hwnd, &rect);
	//�����������������
	lt.x = rect.left;
	lt.y = rect.top;
	//�����������������
	rb.x = rect.right;
	rb.y = rect.bottom;
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);
	//����Ļ���������趨��������
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);
}
VOID Render()
{

	pd3dd->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffc0c0c0, 1.0f, 0);
	pd3dd->BeginScene();

	//����
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	int charCount = swprintf_s(strFPS, 20, TEXT("FPS:%0.3f"), Get_FPS());
	//���FPS
	pFont_FPS->DrawText(NULL, strFPS, charCount, &formatRect, DT_TOP, D3DCOLOR_XRGB(255, 255, 255));
	//�����������
	formatRect.top += 30;
	pFont_STR->DrawText(NULL, TEXT("��1Ϊ���������������������2Ϊ���������������"), -1, &formatRect, DT_SINGLELINE |
		DT_NOCLIP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));
	formatRect.top += 20;
	pFont_STR->DrawText(NULL, TEXT("W��S��A��D������ƶ�"), -1, &formatRect, DT_SINGLELINE |
		DT_NOCLIP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));
	formatRect.top += 20;
	pFont_STR->DrawText(NULL, TEXT("LCrtl���ո�������½�������"), -1, &formatRect, DT_SINGLELINE |
		DT_NOCLIP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));
	formatRect.top += 20;
	pFont_STR->DrawText(NULL, TEXT("����ƶ����ӽ���ת"), -1, &formatRect, DT_SINGLELINE |
		DT_NOCLIP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));
	formatRect.top += 20;
	pFont_STR->DrawText(NULL, TEXT("Q��E�ӽ���б"), -1, &formatRect, DT_SINGLELINE |
		DT_NOCLIP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));
	formatRect.top += 20;
	pFont_STR->DrawText(NULL, TEXT("ESC�˳�����"), -1, &formatRect, DT_SINGLELINE |
		DT_NOCLIP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));


	//��������
	D3DXMATRIX Hero;
	D3DXMatrixTranslation(&Hero, 0, 50.0f, 0);
	Hero *= matWorld;
	pd3dd->SetTransform(D3DTS_WORLD, &Hero);
	for (DWORD i = 0; i < dwNumMtrls; i++)
	{
		pd3dd->SetMaterial(&pMaterials[i]);
		pd3dd->SetTexture(0, pTextures[i]);
		pMesh->DrawSubset(i);
	}

	//���Ƶ���
	pTerrain->RendeTerrain(&matWorld, false);

	//�������
	D3DXMATRIX matSky;
	D3DXMatrixTranslation(&matSky, 0.0f, -200.0f, 0.0f);
	pSkyBox->RenderSky(&matSky, false);

	pd3dd->EndScene();
	pd3dd->Present(NULL, NULL, NULL, NULL);

}
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParrm, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParrm == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParrm, lParam);
	}
	return 0;
}
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,MsgProc,0L,0L,GetModuleHandle(NULL),NULL,NULL,NULL,NULL,
					TEXT("D3D������"),NULL };
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(TEXT("D3D������"), TEXT("D3D ʵ�� --���"), WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);
	hwnd = hWnd;
	if (SUCCEEDED(InitD3D(hWnd, hInst)))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Direct3D_Update();
				Render();
			}
		}
	}
	return 0;
}