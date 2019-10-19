#pragma once
#pragma once
#include<d3d9.h>
#include<d3dx9.h>
class CameraClass
{
private:
	int cameraType;			//摄影机类型 1为地面物体类，2为飞行器类
	D3DXVECTOR3 vRightVector;//右分量向量
	D3DXVECTOR3 vUpVector;	//上分量向量
	D3DXVECTOR3 vLookVector;//观察方向向量

	D3DXVECTOR3 vCameraPosition; //摄像机位置向量

	D3DXVECTOR3 vTargetPosition; //目标观察点位置向量
	D3DXMATRIX matView;			//取景变换矩阵
	D3DXMATRIX matProj;			//投影变换矩阵
	LPDIRECT3DDEVICE9 pd3dDevice; //Diect3D设备对象

public:
	//计算取景变换的函数
	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);

	/*Get函数*/

	//返回当前投影变换矩阵
	VOID GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = matProj; }

	//返回当前摄像机位置向量
	VOID GetCameraPosition(D3DXVECTOR3 *pVector) { *pVector = vCameraPosition; }

	//返回当前观察点位置向量
	VOID GetLookVector(D3DXVECTOR3 *pVector) { *pVector = vLookVector; }

	/*Set函数*/
	//设置摄像机类型
	VOID SetCameraType(int type);

	//设置摄像机的目标观察位置向量
	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);

	//设置摄像机所在位置向量
	VOID SetCameraPosition(D3DXVECTOR3 *pVector = NULL);

	//设置取景变换矩阵
	VOID SetViewMatrix(D3DXMATRIX *pMatrix = NULL);

	//设置投影变换矩阵
	VOID SetProjMatrix(D3DXMATRIX *pMatrix = NULL);

public:
	/*沿各个分量平移的三个函数*/

	//沿right向量移动
	VOID MoveAlongRightVec(FLOAT funits);
	//沿up向量移动
	VOID MoveAlongUpVec(FLOAT fUnits);
	//沿Look向量移动
	VOID MoveAlongLookVec(FLOAT fUnits);

	/*绕各个分量旋转的三个函数*/

	//绕right向量旋转
	VOID RotationRightVec(FLOAT fAngle);
	//绕Up向量旋转
	VOID RotationUpVec(FLOAT fAngle);
	//绕Look向量旋转
	VOID RotationLookVec(FLOAT fAngle);

	//行走
	VOID walk(FLOAT fAngle);
	//扫视
	VOID strafe(FLOAT fAngle);
	//升降
	VOID fly(FLOAT units);

	//俯仰
	VOID pitch(FLOAT fAngle);
	//偏航
	VOID yaw(FLOAT fAngle);
	//滚动
	VOID roll(FLOAT fAngle);
public:
	CameraClass(IDirect3DDevice9 *pd3dDevice);
	virtual~CameraClass(void);
};


