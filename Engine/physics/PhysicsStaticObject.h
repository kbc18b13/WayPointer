/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"


/*!
* @brief	�ÓI�����I�u�W�F�N�g
*/
class PhysicsStaticObject{
public:
	/*!
		* @brief	�R���X�g���N�^�B
		*/
	PhysicsStaticObject();
	/*!
		* @brief	�f�X�g���N�^�B
		*/
	~PhysicsStaticObject();
	/*!
		* @brief	���b�V���̐ÓI�I�u�W�F�N�g���쐬�B
		*@param[in]	skinModel	�X�L�����f���B
		*@param[in]	pos			���W�B
		*@param[in]	rot			��]�B
		*/
	void CreateMeshObject( SkinModel& skinModel,const CVector3& pos,const CQuaternion& rot ){
		//���b�V���R���C�_�[���쐬�B
		m_meshCollider.CreateFromSkinModel( skinModel );
		CreateRigidBody( pos, rot );
	}

	void CreateMeshObject( const wchar_t* modelPath, const CVector3& pos, const CQuaternion& rot ){
		//���b�V���R���C�_�[���쐬�B
		m_meshCollider.CreateFromCMOFile( modelPath );
		CreateRigidBody( pos, rot );
	}
	
	//�ʒu��ݒ�
	void SetPos(const CVector3& pos) {
		btTransform trans = m_rigidBody.GetBody()->getWorldTransform();
		trans.setOrigin(pos);
		m_rigidBody.GetBody()->setWorldTransform(trans);
	}

	void SetRot( const CQuaternion& rot ){
		btTransform trans = m_rigidBody.GetBody()->getWorldTransform();
		trans.setRotation( rot );
		m_rigidBody.GetBody()->setWorldTransform( trans );
	}

	void SetScale( const CVector3& scale ){
		m_meshCollider.SetLocalScale( scale );
		g_physics.UpdateSingleAABB( m_rigidBody );
	}

	MeshCollider* GetShape(){
		return &m_meshCollider;
	}

	RigidBody* GetRigidBody(){
		return &m_rigidBody;
	}

private:
	void CreateRigidBody( CVector3 pos, CQuaternion rot );
	MeshCollider m_meshCollider;		//!<���b�V���R���C�_�[�B
	RigidBody m_rigidBody;				//!<���́B
};
