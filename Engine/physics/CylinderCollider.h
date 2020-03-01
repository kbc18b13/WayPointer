/*!
 * @brief	カプセルコライダー。
 */

#pragma once

#include "Physics/ICollider.h"


class CylinderCollider : public ICollider
{
public:
	/*!
	* @brief	デストラクタ。
	*/
	~CylinderCollider();
	
	/// <summary>
	/// 作成。
	/// </summary>
	/// <param name="radius"></param>
	/// <param name="height"></param>
	void Create(float height, float radius)
	{
		shape = new btCylinderShape(btVector3(radius, height, radius));
	}
	btCollisionShape* GetBody() const override
	{
		return shape;
	}

	void SetLocalScale( const CVector3& scale ) override{
		shape->setLocalScaling( scale );
	}

private:
	btCylinderShape* shape = nullptr;
};
