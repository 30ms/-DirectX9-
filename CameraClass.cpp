#include "CameraClass.h"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 800
#define	SCREEN_HEIGHT 600
#endif // !SCREEN_WIDTH


CameraClass::CameraClass(IDirect3DDevice9 *pd3dDevice)
{
	cameraType = 1;
	this->pd3dDevice = pd3dDevice;
	vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);//默认右向量与X正半轴重合
	vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//默认上向量与y正半轴重合
	vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);//默认观察向量与z正半轴重合
	vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


//---------------------------------------------------------------------------------------
//Name:CameraClass::CaluateViewMatrix();
//Desc:根据给定的矩阵计算出取景变换矩阵
//-------------------------------------------------------------------------------------------
VOID CameraClass::CalculateViewMatrix(D3DXMATRIX*pMatrix)
{
	//1.先把3个向量都规范化并使其互相垂直，成为一组正交矩阵
	D3DXVec3Normalize(&vLookVector, &vLookVector);//规范化观察分量
	D3DXVec3Cross(&vUpVector, &vLookVector, &vRightVector);//上向量与观察向量垂直
	D3DXVec3Normalize(&vUpVector, &vUpVector);	//规范化上向量
	D3DXVec3Cross(&vRightVector, &vUpVector, &vLookVector);	//右向量与上向量垂直
	D3DXVec3Normalize(&vRightVector, &vRightVector);	//规范化右向量

	//2.创建出取景变换矩阵
		//依次写出取景变换矩阵的第一行
	pMatrix->_11 = vRightVector.x;
	pMatrix->_12 = vUpVector.x;
	pMatrix->_13 = vLookVector.x;
	pMatrix->_14 = 0.0f;

	//依次写出取景变换矩阵的第二行
	pMatrix->_21 = vRightVector.y;
	pMatrix->_22 = vUpVector.y;
	pMatrix->_23 = vLookVector.y;
	pMatrix->_24 = 0.0f;

	//依次写出取景变换矩阵的第三行
	pMatrix->_31 = vRightVector.z;
	pMatrix->_32 = vUpVector.z;
	pMatrix->_33 = vLookVector.z;
	pMatrix->_34 = 0.0f;
	//依次写出取景变换矩阵的第四行
	pMatrix->_41 = -D3DXVec3Dot(&vRightVector, &vCameraPosition);//-P*R
	pMatrix->_42 = -D3DXVec3Dot(&vUpVector, &vCameraPosition);	//-P*U
	pMatrix->_43 = -D3DXVec3Dot(&vLookVector, &vCameraPosition);//-P*L
	pMatrix->_44 = 1.0f;
}

//设置摄像机类型
VOID CameraClass::SetCameraType(int type)
{
	cameraType = type;
}

//-------------------------------------------------------------------------------------
//Name:CameraClass:SetTargePosition()
//Desc:设置摄像机的观察位置
//---------------------------------------------------------------------------------------
VOID CameraClass::SetTargetPosition(D3DXVECTOR3 *pLookat)
{
	if (pLookat)vTargetPosition = (*pLookat);
	else vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vLookVector = vTargetPosition - vCameraPosition;//观察点位置减摄像机位置为观察方向向量
	D3DXVec3Normalize(&vLookVector, &vLookVector);//规范化

	//正交并规范化vUpVector和vRightVector
	D3DXVec3Cross(&vUpVector, &vLookVector, &vRightVector);
	D3DXVec3Normalize(&vUpVector, &vUpVector);
	D3DXVec3Cross(&vRightVector, &vUpVector, &vLookVector);
	D3DXVec3Normalize(&vRightVector, &vRightVector);
}

//--------------------------------------------------------------------------------------------
//Name:CameraClass::SetCameraPosition()
//Desc:设置摄像机所在的位置
//-------------------------------------------------------------------------------------------
VOID CameraClass::SetCameraPosition(D3DXVECTOR3 *pVector)
{
	D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	vCameraPosition = pVector ? (*pVector) : V;
}

//---------------------------------------------------------------------------------------------
//Name:CameraClass::SetViewMatrix()
//Desc:设置取景变换矩阵
//---------------------------------------------------------------------------------------------
VOID CameraClass::SetViewMatrix(D3DXMATRIX *pMatirx)
{
	if (pMatirx)matView = *pMatirx;
	else CalculateViewMatrix(&matView);
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	//把取景变换矩阵的值分下来分别给右分分量，上分量，观察分量
	vRightVector = D3DXVECTOR3(matView._11, matView._12, matView._13);
	vUpVector = D3DXVECTOR3(matView._21, matView._22, matView._23);
	vLookVector = D3DXVECTOR3(matView._31, matView._32, matView._33);
}

//---------------------------------------------------------------------------------------------
//Name:CameraClass::SetProjMatrix()
//Desc:设置投影变换矩阵
//--------------------------------------------------------------------------------------------
VOID CameraClass::SetProjMatrix(D3DXMATRIX *pMatrix)
{
	if (pMatrix) matProj = *pMatrix;
	else D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 1.0f, 3000.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


//---------------------------------------------------------------------------------------------
//Name:CameraClass::MoveAlongRightVec()
//Desc:沿右向量平移fUnits个单位
//-----------------------------------------------------------------------------------------------
VOID CameraClass::MoveAlongRightVec(FLOAT fUnits)
{
	vCameraPosition += vRightVector * fUnits;
	vTargetPosition += vRightVector * fUnits;
}

//---------------------------------------------------------------------------------------------
//Name:CameraClass::MoveAlongUpVec()
//Desc:沿上向量平移fUnits个单位
//----------------------------------------------------------------------------------------------
VOID CameraClass::MoveAlongUpVec(FLOAT fUnits)
{
	vCameraPosition += vUpVector * fUnits;
	vTargetPosition += vUpVector * fUnits;
}

//---------------------------------------------------------------------------------------------
//Name::CameraClass::MoveAlongLookvec()
//Desc:沿观察向量平移fUnits个单位
//-----------------------------------------------------------------------------------------------
VOID CameraClass::MoveAlongLookVec(FLOAT fUnits)
{
	vCameraPosition += vLookVector * fUnits;
	vTargetPosition += vLookVector * fUnits;
}

//----------------------------------------------------------------------------------------------
//Name:CameraClass::RotationRightVec()
//Desc:沿右向量旋转fAngle个弧度单位的角度
//----------------------------------------------------------------------------------------------
VOID CameraClass::RotationRightVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vRightVector, fAngle);//创建出绕vRightVector旋转fAngle角度的矩阵R
	D3DXVec3TransformCoord(&vUpVector, &vUpVector, &R);//让vUpVector向量绕vRightVector旋转fAngle角度
	D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);//让vLookVector向量绕vRightVector旋转fAngle角度

	//更新观察点位置 （方向乘以模长=向量）
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);
}

//----------------------------------------------------------------------------------------------
//Name:CameraClass::RotationUpVec()
//Desc:沿上向量旋转fAngle个弧度单位的角度
//----------------------------------------------------------------------------------------------
VOID CameraClass::RotationUpVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vUpVector, fAngle);//创建出绕vUpVector旋转fAngle角度的矩阵R
	D3DXVec3TransformCoord(&vRightVector, &vRightVector, &R);//让vRightVector向量绕vUpVector旋转fAngle角度
	D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);//让vLookVector向量绕vRightVector旋转fAngle角度

	//更新观察点位置 （方向乘以模长=向量）
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);
}

//----------------------------------------------------------------------------------------------
//Name:CameraClass::RotationLookVec()
//Desc:沿观察向量旋转fAngle个弧度单位的角度
//----------------------------------------------------------------------------------------------
VOID CameraClass::RotationLookVec(FLOAT fAngle)
{

	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &vLookVector, fAngle);//创建出绕vLookVector旋转fAngle角度的矩阵R
	D3DXVec3TransformCoord(&vRightVector, &vRightVector, &R);//让vRightVector向量绕vLookVector旋转fAngle角度
	D3DXVec3TransformCoord(&vUpVector, &vUpVector, &R);//让vUpVector向量绕vLookVector旋转fAngle角度

	//更新观察点位置 （方向乘以模长=向量）
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
	D3DXMatrixRotationAxis(&R, &vRightVector, fAngle);//创建出绕vRightVector旋转fAngle角度的矩阵R
	D3DXVec3TransformCoord(&vUpVector, &vUpVector, &R);//让vUpVector向量绕vRightVector旋转fAngle角度
	D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);//让vLookVector向量绕vRightVector旋转fAngle角度

	//更新观察点位置 （方向乘以模长=向量）
	vTargetPosition = vLookVector * D3DXVec3Length(&vCameraPosition);

}

VOID CameraClass::yaw(FLOAT fAngle)
{
	if (cameraType == 1) {
		D3DXMATRIX R;
		D3DXMatrixRotationY(&R, fAngle);//绕世界Y轴旋转矩阵
		D3DXVec3TransformCoord(&vRightVector, &vRightVector, &R);
		D3DXVec3TransformCoord(&vLookVector, &vLookVector, &R);
	}
	else if (cameraType == 2) RotationUpVec(fAngle);
}

CameraClass::~CameraClass()
{
}
