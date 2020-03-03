#include "stdafx.h"
#include "World.h"
#include "graphics/SkinModelRender.h"
#include "WayPoint/WayPoint.h"
#include "physics/CollisionAttr.h"

World::World() : ground( L"Assets/modelData/FirstStage.cmo" ){
	ground.SetIndex( enCollisionAttr_Ground );

	g_graphicsEngine->GetDirectionLight().SetColor( 0, { 1,1,1,1 } );
	g_graphicsEngine->GetDirectionLight().SetDirection( 0, { -1,-1,-1 } );
	g_graphicsEngine->GetDirectionLight().Apply();

	g_graphicsEngine->GetAmbientLight().SetColor( { 0.5f,0.5f,0.5f,1 } );
	g_graphicsEngine->GetAmbientLight().Apply();
}

void World::Update(){
	camera.Update();
	mouseLB.Update();

	if( mouseLB.IsTrigger() ){
		CVector3 from = g_camera3D.GetPosition();
		CVector3 to = from + camera.GetFrontVec() * 1000;
		btCollisionWorld::ClosestRayResultCallback cb(from, to);
		g_physics.GetDynamicWorld()->rayTest( from, to, cb );

		if( cb.hasHit() ){
			const btCollisionObject* col = cb.m_collisionObject;

			//地面にクリックしたらウェイポイント設置
			if( col->getUserIndex() == enCollisionAttr_Ground ){
				CVector3 pos = cb.m_hitPointWorld;
				NewGO<WayPoint>( 0 )->SetPos( pos );
			}

			//ウェイポイントにクリックしたら選択
		}
	}
}
