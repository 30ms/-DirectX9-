#pragma once
#pragma once
#include<d3d9.h>
#include<d3dx9.h>
class CameraClass
{
private:
	int cameraType;			//��Ӱ������ 1Ϊ���������࣬2Ϊ��������
	D3DXVECTOR3 vRightVector;//�ҷ�������
	D3DXVECTOR3 vUpVector;	//�Ϸ�������
	D3DXVECTOR3 vLookVector;//�۲췽������

	D3DXVECTOR3 vCameraPosition; //�����λ������

	D3DXVECTOR3 vTargetPosition; //Ŀ��۲��λ������
	D3DXMATRIX matView;			//ȡ���任����
	D3DXMATRIX matProj;			//ͶӰ�任����
	LPDIRECT3DDEVICE9 pd3dDevice; //Diect3D�豸����

public:
	//����ȡ���任�ĺ���
	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);

	/*Get����*/

	//���ص�ǰͶӰ�任����
	VOID GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = matProj; }

	//���ص�ǰ�����λ������
	VOID GetCameraPosition(D3DXVECTOR3 *pVector) { *pVector = vCameraPosition; }

	//���ص�ǰ�۲��λ������
	VOID GetLookVector(D3DXVECTOR3 *pVector) { *pVector = vLookVector; }

	/*Set����*/
	//�������������
	VOID SetCameraType(int type);

	//�����������Ŀ��۲�λ������
	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);

	//�������������λ������
	VOID SetCameraPosition(D3DXVECTOR3 *pVector = NULL);

	//����ȡ���任����
	VOID SetViewMatrix(D3DXMATRIX *pMatrix = NULL);

	//����ͶӰ�任����
	VOID SetProjMatrix(D3DXMATRIX *pMatrix = NULL);

public:
	/*�ظ�������ƽ�Ƶ���������*/

	//��right�����ƶ�
	VOID MoveAlongRightVec(FLOAT funits);
	//��up�����ƶ�
	VOID MoveAlongUpVec(FLOAT fUnits);
	//��Look�����ƶ�
	VOID MoveAlongLookVec(FLOAT fUnits);

	/*�Ƹ���������ת����������*/

	//��right������ת
	VOID RotationRightVec(FLOAT fAngle);
	//��Up������ת
	VOID RotationUpVec(FLOAT fAngle);
	//��Look������ת
	VOID RotationLookVec(FLOAT fAngle);

	//����
	VOID walk(FLOAT fAngle);
	//ɨ��
	VOID strafe(FLOAT fAngle);
	//����
	VOID fly(FLOAT units);

	//����
	VOID pitch(FLOAT fAngle);
	//ƫ��
	VOID yaw(FLOAT fAngle);
	//����
	VOID roll(FLOAT fAngle);
public:
	CameraClass(IDirect3DDevice9 *pd3dDevice);
	virtual~CameraClass(void);
};


