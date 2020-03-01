/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#pragma once

#include "Physics/ICollider.h"


class CapsuleCollider : public ICollider
{
public:
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~CapsuleCollider();
	/*!
	* @brief	�쐬�B
	*@param[in] radius	�J�v�Z���̔��a�B
	*@param[in]	height	�J�v�Z���̍����B
	*/
	void Create(float radius, float height)
	{
		shape = new btCapsuleShape(radius, height);
		this->radius = radius;
		this->height = height;
	}
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
	float GetRadius() const
	{
		return radius;
	}
	float GetHeight() const
	{
		return height;
	}

	void SetLocalScale( const CVector3& scale ) override{
		shape->setLocalScaling( scale );
	}

private:
	btCapsuleShape*		shape = nullptr;
	float radius = 0.0f;
	float height = 0.0f;
};