#include "CameraClass.h"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 800
#define	SCREEN_HEIGHT 600
#endif // !SCREEN_WIDTH


CameraClass::CameraClass(IDirect3DDevice9 *pd3dDevice)
{
	cameraType = 1;
	this->pd3dDevice = pd3dDevice;
	vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);//Ĭ����������X�������غ�
	vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//Ĭ����������y�������غ�
	vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);//Ĭ�Ϲ۲�������z�������غ�
	vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


//---------------------------------------------------------------------------------------
//Name:CameraClass::CaluateViewMatrix();
//Desc:���ݸ����ľ�������ȡ���任����
//-------------------------------------------------------------------------------------------
VOID CameraClass::CalculateViewMatrix(D3DXMATRIX*pMatrix)
{
	//1.�Ȱ�3���������淶����ʹ�以�ഹֱ����Ϊһ����������
	D3DXVec3Normalize(&vLookVector, &vLookVector);//�淶���۲����
	D3DXVec3Cross(&vUpVector, &vLookVector, &vRightVector);//��������۲�������ֱ
	D3DXVec3Normalize(&vUpVector, &vUpVector);	//�淶��������
	D3DXVec3Cross(&vRightVector, &vUpVector, &vLookVector);	//����������������ֱ
	D3DXVec3Normalize(&vRightVector, &vRightVector);	//�淶��������

	//2.������ȡ���任����
		//����д��ȡ���任����ĵ�һ��
	pMatrix->_11 = vRightVector.x;
	pMatrix->_12 = vUpVector.x;
	pMatrix->_13 = vLookVector.x;
	pMatrix->_14 = 0.0f;

	//����д��ȡ���任����ĵڶ���
	pMatrix->_21 = vRightVector.y;
	pMatrix->_22 = vUpVector.y;
	pMatrix->_23 = vLookVector.y;
	pMatrix->_24 = 0.0f;

	//����д��ȡ���任����ĵ�����
	pMatrix->_31 = vRightVector.z;
	pMatrix->_32 = vUpVector.z;
	pMatrix->_33 = vLookVector.z;
	pMatrix->_34 = 0.0f;
	//����д��ȡ���任����ĵ�����
	pMatrix->_41 = -D3DXVec3Dot(&vRightVector, &vCameraPosition);//-P*R
	pMatrix->_42 = -D3DXVec3Dot(&vUpVector, &vCameraPosition);	//-P*U
	pMatrix->_43 = -D3DXVec3Dot(&vLookVector, &vCameraPosition);//-P*L
	pMatrix->_44 = 1.0f;
}

//�������������
VOID CameraClass::SetCameraType(int type)
{
	cameraType = type;
}

//-------------------------------------------------------------------------------------
//Name:CameraClass:SetTargePosition()
//Desc:����������Ĺ۲�λ��
//---------------------------------------------------------------------------------------
VOID CameraClass::SetTargetPosition(D3DXVECTOR3 *pLookat)
{
	if (pLookat)vTargetPosition = (*pLookat);
	else vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vLookVector = vTargetPosition - vCameraPosition;//�۲��λ�ü������λ��Ϊ�۲췽������
	D3DXVec3Normalize(&vLookVector, &vLookVector);//�淶��

	//�������淶��vUpVector��vRightVector
	D3DXVec3Cross(&vUpVector, &vLookVector, &vRightVector);
	D3DXVec3Normalize(&vUpVector, &vUpVector);
	D3DXVec3Cross(&vRightVector, &vUpVector, &vLookVector);
	D3DXVec3Normalize(&vRightVector, &vRightVector);
}

//--------------------------------------------------------------------------------------------
//Name:CameraClass::SetCameraPosition()
//Desc:������������ڵ�λ��
//-------------------------------------------------------------------------------------------
VOID CameraClass::SetCameraPosition(D3DXVECTOR3 *pVector)
{
	D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	vCameraPosition = pVector ? (*pVector) : V;
}

//---------------------------------------------------------------------------------------------
//Name:CameraClass::SetViewMatrix()
//Desc:����ȡ���任����
//---------------------------------------------------------------------------------------------
VOID CameraClass::SetViewMatrix(D3DXMATRIX *pMatirx)
{
	if (pMatirx)matView = *pMatirx;
	else CalculateViewMatrix(&matView);
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	//��ȡ���任�����ֵ�������ֱ���ҷַ������Ϸ������۲����
	vRightVector = D3DXVECTOR3(matView._11, matView._12, matView._13);
	vUpVector = D3DXVECTOR3(matView._21, matView._22, matView._23);
	vLookVector = D3DXVECTOR3(matView._31, matView._32, matView._33);
}

//---------------------------------------------------------------------------------------------
//Name:CameraClass::SetProjMatrix()
//Desc:����ͶӰ�任����
//--------------------------------------------------------------------------------------------
VOID CameraClass::SetProjMatrix(D3DXMATRIX *pMatrix)
{
	if (pMatrix) matProj = *pMatrix;
	else D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 1.0f, 3000.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


//---------------------------------------------------------------------------------------------
//Name:CameraClass::MoveAlongRightVec()
//Desc:��������ƽ��fUnits����λ
//-----------------------------------------------------------------------------------------------
VOID CameraClass::MoveAlongRightVec(FLOAT fUnits)
{
	vCameraPosition += vRightVector * fUnits;
	vTargetPosition += vRightVector * fUnits;
}

//---------------------------------------------------------------------------------------------
//Name:CameraClass::MoveAlongUpVec()
//Desc:��������ƽ��fUnits����λ
//----------------------------------------------------------------------------------------------
VOID CameraClass::MoveAlongUpVec(FLOAT fUnits)
{
	vCameraPosition += vUpVector * fUnits;
	vTargetPosition += vUpVector * fUnits;
}

//---------------------------------------------------------------------------------------------
//Name::CameraClass::MoveAlongLookvec()
//Desc:�ع۲�����ƽ��fUnits����λ
//-----------------------------------------------------------------------------------------------
VOID CameraClass::MoveAlongLookVec(FLOAT fUnits)
{
	vCameraPosition += vLookVector * fUnits;
	vTargetPosition += vLookVector * fUnits;
}

//----------------------------------------------------------------------------------------------
//Name:CameraClass::RotationRightVec()
//Desc:����������תfAngle�����ȵ�λ�ĽǶ�
//----------------------------------------------------------------------------------------------
VOID CameraClass::RotationRightVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vRightVector, fAngle);//��������vRightVector��תfAngle�Ƕȵľ���R
	D3DXVec3TransformCoord(&vUpVector, &vUpVector, &R);//��vUpVector������vRightVector��תfAngle�Ƕ�
	D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);//��vLookVector������vRightVector��תfAngle�Ƕ�

	//���¹۲��λ�� ���������ģ��=������
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);
}

//----------------------------------------------------------------------------------------------
//Name:CameraClass::RotationUpVec()
//Desc:����������תfAngle�����ȵ�λ�ĽǶ�
//----------------------------------------------------------------------------------------------
VOID CameraClass::RotationUpVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vUpVector, fAngle);//��������vUpVector��תfAngle�Ƕȵľ���R
	D3DXVec3TransformCoord(&vRightVector, &vRightVector, &R);//��vRightVector������vUpVector��תfAngle�Ƕ�
	D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);//��vLookVector������vRightVector��תfAngle�Ƕ�

	//���¹۲��λ�� ���������ģ��=������
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);
}

//----------------------------------------------------------------------------------------------
//Name:CameraClass::RotationLookVec()
//Desc:�ع۲�������תfAngle�����ȵ�λ�ĽǶ�
//----------------------------------------------------------------------------------------------
VOID CameraClass::RotationLookVec(FLOAT fAngle)
{

	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vLookVector, fAngle);//��������vLookVector��תfAngle�Ƕȵľ���R
	D3DXVec3TransformCoord(&vRightVector, &vRightVector, &R);//��vRightVector������vLookVector��תfAngle�Ƕ�
	D3DXVec3TransformCoord(&vUpVector, &vUpVector, &R);//��vUpVector������vLookVector��תfAngle�Ƕ�

	//���¹۲��λ�� ���������ģ��=������
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);
}



VOID CameraClass::walk(FLOAT units)
{
	if (cameraType == 1) {
		vCameraPosition += D3DXVECTOR3(vLookVector.x, 0.0f, vLookVector.z)*units;
		vTargetPosition += D3DXVECTOR3(vLookVector.x, 0.0f, vLookVector.z)*units;
	}
	else if (cameraType == 2) MoveAlongLookVec(units);

}

VOID CameraClass::strafe(FLOAT units)
{
	if (cameraType == 1) {
		vCameraPosition += D3DXVECTOR3(vRightVector.x, 0.0f, vRightVector.z)*units;
		vTargetPosition += D3DXVECTOR3(vRightVector.x, 0.0f, vRightVector.z)*units;
	}
	else if (cameraType == 2) MoveAlongRightVec(units);
}

VOID CameraClass::fly(FLOAT units)
{
	if (cameraType == 1) {
		vCameraPosition.y += units;
		vTargetPosition.y += units;
	}
	else if (cameraType == 2) MoveAlongUpVec(units);
}


VOID CameraClass::pitch(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vRightVector, fAngle);//��������vRightVector��תfAngle�Ƕȵľ���R
	D3DXVec3TransformCoord(&vUpVector, &vUpVector, &R);//��vUpVector������vRightVector��תfAngle�Ƕ�
	D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);//��vLookVector������vRightVector��תfAngle�Ƕ�

	//���¹۲��λ�� ���������ģ��=������
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);

}

VOID CameraClass::yaw(FLOAT fAngle)
{
	if (cameraType == 1) {
		D3DXMATRIX R;
		D3DXMatrixRotationY(&R, fAngle);//������Y����ת����
		D3DXVec3TransformCoord(&vRightVector, &vRightVector, &R);
		D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);
	}
	else if (cameraType == 2) RotationUpVec(fAngle);
}

CameraClass::~CameraClass()
{
}
