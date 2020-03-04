#include "stdafx.h"
#include "CollisionModel.h"
#include "physics/CollisionAttr.h"

CollisionModel::CollisionModel( const wchar_t* modelPath, const wchar_t* collisionModelPath, int flag){
	m_model.Init( modelPath );
	if( !collisionModelPath ){
		m_collider.CreateMeshObject( m_model.GetModel(), { 0,0,0 }, CQuaternion::Identity() , flag);
	} else{
		m_collider.CreateMeshObject( collisionModelPath, { 0,0,0 }, CQuaternion::Identity() , flag);
	}
}

CollisionModel::~CollisionModel(){}

void CollisionModel::SetPos( const CVector3 & pos ){
	m_model.SetPos( pos );
	m_model.Update();
	m_collider.SetPos( pos );
}

void CollisionModel::SetRot( const CQuaternion & rot ){
	m_model.SetRot( rot );
	m_model.Update();
	m_collider.SetRot( rot );
}

void CollisionModel::SetScale( const CVector3 & scale ){
	m_model.SetScale( scale );
	m_model.Update();
	m_collider.SetScale( scale );
}
