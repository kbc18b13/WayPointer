#pragma once

class RigidBody;
class ICollision;

class PhysicsWorld
{
	btDefaultCollisionConfiguration*		collisionConfig = nullptr;
	btCollisionDispatcher*					collisionDispatcher = nullptr;	//!<�Փˉ��������B
	btBroadphaseInterface*					overlappingPairCache = nullptr;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	btSequentialImpulseConstraintSolver*	constraintSolver = nullptr;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	btDiscreteDynamicsWorld*				dynamicWorld = nullptr;			//!<���[���h�B
public:
	PhysicsWorld();
	~PhysicsWorld();
	void Init();
	void Update();
	void Release();

	void setDebugDraw(bool isDraw);

	void DebugDraw();

	/*!
	* @brief	�_�C�i�~�b�N���[���h���擾�B
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return dynamicWorld;
	}
	/*!
	* @brief	���̂�o�^�B
	*/
	void AddRigidBody(RigidBody& rb);
	/*!
	* @brief	���̂�j���B
	*/
	void RemoveRigidBody(RigidBody& rb);

	/*!
	* @brief	�R���W�����I�u�W�F�N�g��o�^�B
	*/
	void AddCollision(ICollision& cl);
	/*!
	* @brief	�R���W�����I�u�W�F�N�g���폜�B
	*/
	void RemoveCollision(ICollision& cl);

	/// <summary>
	/// �Ώۂ�AABB���A�b�v�f�[�g����B�R���W�����V�F�C�v�̃X�P�[�����O��ύX������ɌĂ΂Ȃ���΂Ȃ�Ȃ��B
	/// </summary>
	void UpdateSingleAABB( RigidBody& rb );
	void UpdateSingleAABB( ICollision& cl );

	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}
	void ContactTest(
		btCollisionObject* colObj, 
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		dynamicWorld->contactTest(colObj, resultCallback);
	}
};

extern PhysicsWorld g_physics;


