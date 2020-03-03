#pragma once
#include "Util/KeyTrigger.h"

class MyCamera{
public:
	void Update();

	/// <summary>
	/// カメラの前方向
	/// </summary>
	/// <returns></returns>
	CVector3 GetFrontVec(){
		CVector3 front = cameraVec;
		front.Normalize();
		return front;
	}

	/// <summary>
	/// カメラの右方向
	/// </summary>
	/// <returns></returns>
	CVector3 GetRightVec(){
		CVector3 right = CVector3::AxisY();
		right.Cross( -cameraVec );
		right.Normalize();
		return right;
	}

private:
	KeyTrigger mouseM{ VK_MBUTTON };

	bool mouseLock = false;

	float moveSpeed = 10;
	CVector3 cameraVec = { 1, 0, 0 };
};

