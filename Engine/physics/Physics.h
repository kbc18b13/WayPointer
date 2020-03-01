#pragma once

class RigidBody;
class ICollision;

class PhysicsWorld
{
	btDefaultCollisionConfiguration*		collisionConfig = nullptr;
	btCollisionDispatcher*					collisionDispatcher = nullptr;	//!<衝突解決処理。
	btBroadphaseInterface*					overlappingPairCache = nullptr;	//!<ブロードフェーズ。衝突判定の枝切り。
	btSequentialImpulseConstraintSolver*	constraintSolver = nullptr;		//!<コンストレイントソルバー。拘束条件の解決処理。
	btDiscreteDynamicsWorld*				dynamicWorld = nullptr;			//!<ワールド。
public:
	PhysicsWorld();
	~PhysicsWorld();
	void Init();
	void Update();
	void Release();

	void setDebugDraw(bool isDraw);

	void DebugDraw();

	/*!
	* @brief	ダイナミックワールドを取得。
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return dynamicWorld;
	}
	/*!
	* @brief	剛体を登録。
	*/
	void AddRigidBody(RigidBody& rb);
	/*!
	* @brief	剛体を破棄。
	*/
	void RemoveRigidBody(RigidBody& rb);

	/*!
	* @brief	コリジョンオブジェクトを登録。
	*/
	void AddCollision(ICollision& cl);
	/*!
	* @brief	コリジョンオブジェクトを削除。
	*/
	void RemoveCollision(ICollision& cl);

	/// <summary>
	/// 対象のAABBをアップデートする。コリジョンシェイプのスケーリングを変更した後に呼ばなければならない。
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


