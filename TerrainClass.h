#pragma once

#include<d3d9.h>
#include<d3dx9.h>
#include<vector>
#include<fstream>


class TerrainClass
{
private:
	LPDIRECT3DDEVICE9 pd3dd;					//D3D�豸
	LPDIRECT3DTEXTURE9 pTexture;				//����
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;		//���㻺��
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;		//��������

	int nCellsPerRow;	//ÿ�еĵ�Ԫ����
	int nCellsPerCol;	//ÿ�еĵ�Ԫ����
	int nVertsPerRow;	//ÿ�еĶ������
	int nVertsPerCol;	//ÿ�еĶ������
	int nNumVertices;	//��������
	FLOAT fTerrainWidth;	//���εĿ��
	FLOAT fTerrainDepth;	//���ε����
	FLOAT fCellSpacing;	//��Ԫ��ļ��
	FLOAT fHeightScale;	//�߶�����ϵ��

	std::vector<FLOAT> vHeightInfo;	//���ڴ�Ÿ߶���Ϣ

	//����һ�����ε�FVF�����ʽ
	struct TERRAINVERTEX
	{
		FLOAT _x, _y, _z;
		FLOAT _u, _v;
		TERRAINVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) :_x(x), _y(y), _z(z), _u(u), _v(v) {}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};

public:
	TerrainClass(IDirect3DDevice9*pd3dd);
	~TerrainClass(void);

	BOOL LoadTerrainFromFile(LPCTSTR pRawFileName, LPCTSTR pTextureFile); //���ļ����ظ߶�ͼ������ĺ���
	BOOL InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale);//���γ�ʼ��
	BOOL RendeTerrain(D3DXMATRIX*pMatWorld, BOOL bDrawFrame = FALSE);//������Ⱦ
};

