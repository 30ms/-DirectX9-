#pragma once
#include<d3d9.h>
#include<d3dx9.h>

struct SKYBOXVERITEX
{
	float x, y, z;
	float u, v;
};
#define D3DFVF_SKYBOX D3DFVF_XYZ|D3DFVF_TEX1
class SkyClass
{
private:
	LPDIRECT3DDEVICE9 pd3dd;
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;//���㻺��
	LPDIRECT3DTEXTURE9 pTexture[5];		//5������ӿڶ���
	float Length;				//��պб߳�
public:
	SkyClass(LPDIRECT3DDEVICE9 pDevice);
	~SkyClass(void);
	BOOL InitSky(float Length);
	BOOL LoadSkyTextureFromFile(LPCTSTR pFrontTextureFile,
		LPCTSTR pBackTextureFile,
		LPCTSTR pleftTextureFile,
		LPCTSTR pRightTextureFile,
		LPCTSTR pUpTextureFile);
	VOID RenderSky(D3DXMATRIX *pMatWorld, BOOL bRenderFrame);
};

