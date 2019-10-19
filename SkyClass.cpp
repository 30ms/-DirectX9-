#include "SkyClass.h"



SkyClass::SkyClass(LPDIRECT3DDEVICE9 pDevice)
{
	pd3dd = pDevice;
	pVertexBuffer = NULL;
	for (int i = 0; i < 5; i++)
		pTexture[i] = NULL;
	Length = 0.0f;
}


BOOL SkyClass::InitSky(float length)
{
	Length = length;
	pd3dd->CreateVertexBuffer(20 * sizeof(SKYBOXVERITEX), 0,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &pVertexBuffer, 0);

	//用一个结构体把顶点数据 准备好
	SKYBOXVERITEX vertices[] =
	{
		//前面的四个顶点
		{-Length / 2,	0.0f,		Length / 2,		0.0f,1.0f},
		{-Length / 2,	Length,	Length / 2,		0.0f,0.0f},
		{Length / 2,	0.0f,		Length / 2,		1.0f,1.0f},
		{Length / 2,	Length,	Length / 2,		1.0f,0.0f},
		//背面的四个顶点
		{Length/2,		0.0f,		-Length/2,		0.0f,1.0f},
		{Length/2,		Length,	-Length/2,		0.0f,0.0f},
		{-Length/2,		0.0f,		-Length/2,		1.0f,1.0f},
		{-Length/2,		Length,	-Length/2,		1.0f,0.0f},
		//左面的四个顶点
		{-Length/2,		0.0f,		-Length/2,		0.0f,1.0f},
		{-Length/2,		Length,	-Length/2,		0.0f,0.0f},
		{-Length/2,		0.0f,		Length/2,		1.0f,1.0f},
		{-Length/2,		Length,	Length/2,		1.0f,0.0f},
		//右面的四个顶点
		{Length/2,		0.0f,		Length/2,		0.0f,1.0f},
		{Length/2,		Length,	Length/2,		0.0f,0.0f},
		{Length/2,		0.0f,		-Length/2,		1.0f,1.0f},
		{Length/2,		Length,	-Length/2,		1.0f,0.0f},
		//上面的四个顶点
		{Length/2,		Length,	-Length/2,		1.0f,0.0f},
		{Length/2,		Length,	Length/2,		1.0f,1.0f},
		{-Length/2,		Length,	-Length/2,		0.0f,0.0f},
		{-Length / 2,	Length,	Length/2,		0.0f,1.0f }
	};
	void* pVertices;
	pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	pVertexBuffer->Unlock();
	return TRUE;
}


BOOL SkyClass::LoadSkyTextureFromFile(LPCTSTR pFrontTextureFile,
	LPCTSTR pBackTextureFile, 
	LPCTSTR pLeftTextureFile, 
	LPCTSTR pRightTextureFile, 
	LPCTSTR pUpTextureFile )
{
	D3DXCreateTextureFromFile(pd3dd, pFrontTextureFile, &pTexture[0]);
	D3DXCreateTextureFromFile(pd3dd, pBackTextureFile, &pTexture[1]);
	D3DXCreateTextureFromFile(pd3dd, pLeftTextureFile, &pTexture[2]);
	D3DXCreateTextureFromFile(pd3dd, pRightTextureFile, &pTexture[3]);
	D3DXCreateTextureFromFile(pd3dd, pUpTextureFile, &pTexture[4]);

	return TRUE;
}


VOID SkyClass::RenderSky(D3DXMATRIX *pMatWorld, BOOL bRenderFrame)
{
	pd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dd->SetTransform(D3DTS_WORLD, pMatWorld);
	pd3dd->SetStreamSource(0, pVertexBuffer, 0, sizeof(SKYBOXVERITEX));
	pd3dd->SetFVF(D3DFVF_SKYBOX);

	for (int i = 0; i < 5; i++)
	{
		pd3dd->SetTexture(0, pTexture[i]);
		pd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
	if (bRenderFrame)
	{
		pd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		for (int i = 0; i < 5; i++)
		{
			pd3dd->SetTexture(0, pTexture[i]);
			pd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
		pd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
SkyClass::~SkyClass()
{
	pVertexBuffer->Release();
	for (int i = 0; i < 5; i++)
		pTexture[i]->Release();
}
