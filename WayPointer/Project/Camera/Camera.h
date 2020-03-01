#pragma once
class MyCamera{
public:
	void Update();

	/// <summary>
	/// �J�����̑O����
	/// </summary>
	/// <returns></returns>
	CVector3 GetFrontVec(){
		CVector3 front = cameraVec;
		front.Normalize();
		return front;
	}

	/// <summary>
	/// �J�����̉E����
	/// </summary>
	/// <returns></returns>
	CVector3 GetRightVec(){
		CVector3 right = CVector3::AxisY();
		right.Cross( -cameraVec );
		right.Normalize();
		return right;
	}

private:

	float moveSpeed = 10;
	CVector3 cameraVec = {1, 0, 0};
};
