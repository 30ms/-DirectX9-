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
	//���ļ��ж�ȡ�߶���Ϣ
	std::ifstream inFile;
	inFile.open(pRawFileName, std::ios::binary);
	inFile.seekg(0, std::ios::end);//�ļ�ָ���ƶ����ļ�ĩβ
	std::vector<BYTE> inData(inFile.tellg());//��ģ�嶨��һ��vector<BYTE>���͵ı�������ʼ������ֵΪ�������Ĵ�С
	inFile.seekg(std::ios::beg);//���ļ�ָ���ƶ����ļ��Ŀ�ͷ
	inFile.read((char*)&inData[0], inData.size());//��ȡ�����߶���Ϣ
	inFile.close();

	vHeightInfo.resize(inData.size());//��vHeightInfo�ߴ�ȡΪ�������ĳߴ�
	//������������������inData�е�ֵ����vHeightInfo
	for (size_t i = 0; i < inData.size(); i++)
		vHeightInfo[i] = inData[i];
	//���ص�������
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

	//ͨ��ѭ��������ѵ���ԭʼ�߶ȳ�������ϵ�����õ����ź�ĸ߶�
	for (size_t i = 0; i < vHeightInfo.size(); i++)
		vHeightInfo[i] *= fHeightScale;

	//������εĶ���
	if (FAILED(pd3dd->CreateVertexBuffer(nNumVertices * sizeof(TERRAINVERTEX),
		D3DUSAGE_WRITEONLY, TERRAINVERTEX::FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0)))
		return FALSE;

	TERRAINVERTEX *pVertices = NULL;
	pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	FLOAT fStarX = -fTerrainWidth / 2.0f, fEndX = fTerrainWidth / 2.0f;
	FLOAT fStarZ = fTerrainDepth / 2.0f, fEndZ = -fTerrainDepth / 2.0f;//ָ����ʼ����ֹ��������
	FLOAT fCoordU = 1.0f / (FLOAT)nCellsPerRow;//ָ������ĺ�����
	FLOAT fCoordV = 1.0f / (FLOAT)nCellsPerCol;//ָ�������������

	int nIndex = 0, i = 0, j = 0;
	for (float z = fStarZ; z > fEndZ; z -= fCellSpacing, i++)
	{
		j = 0;
		for (float x = fStarX; x < fEndX; x += fCellSpacing, j++)
		{
			nIndex = i * nCellsPerRow + j;//ָ����ǰ�����ڶ��㻺���е�λ��
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
			//������ABC���϶���
			pIndices[nIndex] = row * nCellsPerRow + col;//����A
			pIndices[nIndex + 1] = row * nCellsPerRow + col + 1;//����B
			pIndices[nIndex + 2] = (row + 1)*nCellsPerRow + col;//����C
			//������CBD����������
			pIndices[nIndex + 3] = (row + 1)*nCellsPerRow + col;//����C
			pIndices[nIndex + 4] = row * nCellsPerRow + col + 1;//����B
			pIndices[nIndex + 5] = (row + 1)*nCellsPerRow + col + 1;//����D
			//������һ����Ԫ��������6
			nIndex += 6;
		}
	}
	pIndexBuffer->Unlock();
	return TRUE;
}
BOOL TerrainClass::RendeTerrain(D3DXMATRIX*pMatWorld, BOOL bRendFrame)
{
	pd3dd->SetStreamSource(0, pVertexBuffer, 0, sizeof(TERRAINVERTEX));
	pd3dd->SetFVF(TERRAINVERTEX::FVF);//ָ�������ʽ
	pd3dd->SetIndices(pIndexBuffer);//������������
	pd3dd->SetTexture(0, pTexture);//��������

	pd3dd->SetRenderState(D3DRS_LIGHTING, FALSE);//�رչ���
	pd3dd->SetTransform(D3DTS_WORLD, pMatWorld);//�����������
	pd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nNumVertices, 0, nNumVertices * 2);//���ƶ���

	pd3dd->SetRenderState(D3DRS_LIGHTING, TRUE);//�򿪹���
	pd3dd->SetTexture(0, 0);//����Ϊ��

	if (bRendFrame)//�������Ⱦ���߿�Ļ�
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
