#pragma once

#include<d3d9.h>
#include<d3dx9.h>
#include<vector>
#include<fstream>


class TerrainClass
{
private:
	LPDIRECT3DDEVICE9 pd3dd;					//D3D设备
	LPDIRECT3DTEXTURE9 pTexture;				//纹理
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;		//顶点缓存
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;		//索引缓存

	int nCellsPerRow;	//每行的单元格数
	int nCellsPerCol;	//每列的单元格数
	int nVertsPerRow;	//每行的顶点个数
	int nVertsPerCol;	//每列的顶点个数
	int nNumVertices;	//顶点总是
	FLOAT fTerrainWidth;	//地形的宽度
	FLOAT fTerrainDepth;	//地形的深度
	FLOAT fCellSpacing;	//单元格的间距
	FLOAT fHeightScale;	//高度缩放系数

	std::vector<FLOAT> vHeightInfo;	//用于存放高度信息

	//定义一个地形的FVF顶点格式
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

	BOOL LoadTerrainFromFile(LPCTSTR pRawFileName, LPCTSTR pTextureFile); //从文件加载高度图和纹理的函数
	BOOL InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale);//地形初始化
	BOOL RendeTerrain(D3DXMATRIX*pMatWorld, BOOL bDrawFrame = FALSE);//地形渲染
};

