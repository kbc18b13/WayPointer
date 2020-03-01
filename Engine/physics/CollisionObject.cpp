#include "stdafx.h"
#include "CollisionObject.h";
#include "ICollider.h"

CollisionObject::CollisionObject() {
}

CollisionObject::~CollisionObject() {
}

void CollisionObject::Create(const ICollider& collider,const CVector3 pos,const CQuaternion rot) {
	m_collision.setCollisionShape(collider.GetBody());
	m_collision.getWorldTransform().setOrigin(pos);
	m_collision.getWorldTransform().setRotation(rot);
}
