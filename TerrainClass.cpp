#include "TerrainClass.h"



TerrainClass::TerrainClass(IDirect3DDevice9*pd3dDevice)
{
	pd3dd = pd3dDevice;
	pTexture = NULL;
	pIndexBuffer = NULL;
	pVertexBuffer = NULL;
	nCellsPerCol = 0;
	nCellsPerRow = 0;
	nVertsPerCol = 0;
	nVertsPerRow = 0;
	fTerrainDepth = 0.0f;
	fTerrainWidth = 0.0f;
	fCellSpacing = 0.0f;
	fHeightScale = 0.0f;
}
BOOL TerrainClass::LoadTerrainFromFile(LPCTSTR pRawFileName, LPCTSTR pTextureName)
{
	//从文件中读取高度信息
	std::ifstream inFile;
	inFile.open(pRawFileName, std::ios::binary);
	inFile.seekg(0, std::ios::end);//文件指针移动到文件末尾
	std::vector<BYTE> inData(inFile.tellg());//用模板定义一个vector<BYTE>类型的变量并初始化，其值为输入流的大小
	inFile.seekg(std::ios::beg);//将文件指针移动到文件的开头
	inFile.read((char*)&inData[0], inData.size());//读取整个高度信息
	inFile.close();

	vHeightInfo.resize(inData.size());//将vHeightInfo尺寸取为缓冲区的尺寸
	//遍历整个缓存区，将inData中的值赋给vHeightInfo
	for (size_t i = 0; i < inData.size(); i++)
		vHeightInfo[i] = inData[i];
	//加载地形纹理
	if (FAILED(D3DXCreateTextureFromFile(pd3dd, pTextureName, &pTexture)))
		return FALSE;
	return TRUE;
}


BOOL TerrainClass::InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale)
{
	nCellsPerRow = nRows;
	nCellsPerCol = nCols;
	fCellSpacing = fSpace;
	fHeightScale = fScale;
	fTerrainDepth = nCols * fSpace;
	fTerrainWidth = nRows * fSpace;
	nVertsPerRow = nCellsPerRow + 1;
	nVertsPerCol = nCellsPerCol + 1;
	nNumVertices = nVertsPerRow * nVertsPerCol;

	//通过循环，逐个把地形原始高度乘以缩放系数，得到缩放后的高度
	for (size_t i = 0; i < vHeightInfo.size(); i++)
		vHeightInfo[i] *= fHeightScale;

	//处理地形的顶点
	if (FAILED(pd3dd->CreateVertexBuffer(nNumVertices * sizeof(TERRAINVERTEX),
		D3DUSAGE_WRITEONLY, TERRAINVERTEX::FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0)))
		return FALSE;

	TERRAINVERTEX *pVertices = NULL;
	pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	FLOAT fStarX = -fTerrainWidth / 2.0f, fEndX = fTerrainWidth / 2.0f;
	FLOAT fStarZ = fTerrainDepth / 2.0f, fEndZ = -fTerrainDepth / 2.0f;//指定起始和终止顶点坐标
	FLOAT fCoordU = 1.0f / (FLOAT)nCellsPerRow;//指定纹理的横坐标
	FLOAT fCoordV = 1.0f / (FLOAT)nCellsPerCol;//指定纹理的纵坐标

	int nIndex = 0, i = 0, j = 0;
	for (float z = fStarZ; z > fEndZ; z -= fCellSpacing, i++)
	{
		j = 0;
		for (float x = fStarX; x < fEndX; x += fCellSpacing, j++)
		{
			nIndex = i * nCellsPerRow + j;//指定当前顶点在顶点缓存中的位置
			pVertices[nIndex] = TERRAINVERTEX(x, vHeightInfo[nIndex], z, j*fCoordU, i*fCoordV);
			nIndex++;
		}
	}
	pVertexBuffer->Unlock();

	if (FAILED(pd3dd->CreateIndexBuffer(nNumVertices * 6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, 0)))
		return FALSE;
	WORD*pIndices = NULL;
	pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	nIndex = 0;
	for (int row = 0; row < nCellsPerRow - 1; row++)
	{
		for (int col = 0; col < nCellsPerCol - 1; col++)
		{
			//三角形ABC的上顶点
			pIndices[nIndex] = row * nCellsPerRow + col;//顶点A
			pIndices[nIndex + 1] = row * nCellsPerRow + col + 1;//顶点B
			pIndices[nIndex + 2] = (row + 1)*nCellsPerRow + col;//顶点C
			//三角形CBD的三个顶点
			pIndices[nIndex + 3] = (row + 1)*nCellsPerRow + col;//顶点C
			pIndices[nIndex + 4] = row * nCellsPerRow + col + 1;//顶点B
			pIndices[nIndex + 5] = (row + 1)*nCellsPerRow + col + 1;//顶点D
			//处理完一个单元格，索引加6
			nIndex += 6;
		}
	}
	pIndexBuffer->Unlock();
	return TRUE;
}
BOOL TerrainClass::RendeTerrain(D3DXMATRIX*pMatWorld, BOOL bRendFrame)
{
	pd3dd->SetStreamSource(0, pVertexBuffer, 0, sizeof(TERRAINVERTEX));
	pd3dd->SetFVF(TERRAINVERTEX::FVF);//指定灵活顶点格式
	pd3dd->SetIndices(pIndexBuffer);//设置索引缓存
	pd3dd->SetTexture(0, pTexture);//设置纹理

	pd3dd->SetRenderState(D3DRS_LIGHTING, FALSE);//关闭光照
	pd3dd->SetTransform(D3DTS_WORLD, pMatWorld);//设置世界矩阵
	pd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nNumVertices, 0, nNumVertices * 2);//绘制顶点

	pd3dd->SetRenderState(D3DRS_LIGHTING, TRUE);//打开光照
	pd3dd->SetTexture(0, 0);//纹理为空

	if (bRendFrame)//如果熬渲染出线框的话
	{
		pd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nNumVertices, 0, nNumVertices * 2);
		pd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	return TRUE;

}
TerrainClass::~TerrainClass()
{
	pTexture->Release();
	pIndexBuffer->Release();
	pVertexBuffer->Release();
}
