#include "stdafx.h"
#include "GhostObject.h";
#include "ICollider.h"

GhostObject::GhostObject() {
}

GhostObject::~GhostObject() {
}

void GhostObject::Create(const ICollider& collider,const CVector3 pos,const CQuaternion rot) {
	m_ghost.setCollisionShape(collider.GetBody());
	m_ghost.getWorldTransform().setOrigin(pos);
	m_ghost.getWorldTransform().setRotation(rot);
}
