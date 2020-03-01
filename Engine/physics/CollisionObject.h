#pragma once
#include "ICollision.h"

class ICollider;

class CollisionObject : public ICollision{
public:
	CollisionObject();
	~CollisionObject();

	void Create(const ICollider& collider, const CVector3 pos = CVector3(), const CQuaternion rot = CQuaternion());

	btCollisionObject* GetBody() override{
		return &m_collision;
	}

	void SetUserPointer(void* p) {
		m_collision.setUserPointer(p);
	}

	void SetUserIndex(int index) {
		m_collision.setUserIndex(index);
	}

	void SetMask(short mask){
		m_mask = mask;
	}

	void SetGroup(short group){
		m_group = group;
	}

	short GetMask() override {
		return m_mask;
	}

	short GetGroup() override {
		return m_group;
	}

private:
	btCollisionObject m_collision;
	short m_mask;
	short m_group;
};