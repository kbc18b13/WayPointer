#pragma once
#include "physics/PhysicsStaticObject.h"
class SkinModelRender;
class CollisionModel {
public:
	CollisionModel(const wchar_t* modelPath, const wchar_t* collisionModelPath = nullptr, int flag = 1);
	~CollisionModel();

	void SetPos(const CVector3& pos);

	CVector3 GetPos() const{
		return m_model.GetPos();
	}

	void SetRot( const CQuaternion& rot );

	void SetScale( const CVector3& scale );

	void SetPointer( void* p ){
		m_collider.GetRigidBody()->GetBody()->setUserPointer( p );
	}

	void SetIndex( int index ){
		m_collider.GetRigidBody()->GetBody()->setUserIndex( index );
	}

	SkinModelRender& GetModel(){
		return m_model;
	}

private:
	SkinModelRender m_model;
	PhysicsStaticObject m_collider;
};