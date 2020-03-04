#include "stdafx.h"
#include "physics/CollisionAttr.h"
#include "World.h"
#include "graphics/SkinModelRender.h"
#include "WayPoint/WayPoint.h"
#include "WayPoint/Loot.h"

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
	mouseRB.Update();
	if( creatingLoot ){
		auto cb = rayTest(1000, ~btCollisionObject::CollisionFlags::CF_Loot);

		CVector3 targetPos;
		bool isLookWayP = false;
		if( cb.hasHit() ){
			if(cb.m_collisionObject->getUserIndex() == enCollisionAttr_WayPoint ){
				targetPos = static_cast<WayPoint*>( cb.m_collisionObject->getUserPointer() )->GetPos();
				isLookWayP = true;
			} else{
				targetPos = cb.m_hitPointWorld;
			}
		} else{
			targetPos = g_camera3D.GetPosition() + camera.GetFrontVec() * 1000;
		}

		creatingLoot->SetTargetPos( targetPos );

		if( mouseLB.IsTrigger() ){
			if( !isLookWayP ){
				DeleteGO( creatingLoot );
			}
			creatingLoot = nullptr;
			selectWayPoint->TurnSelected();
			selectWayPoint = nullptr;
		}

	} else if( mouseLB.IsTrigger() || mouseRB.IsTrigger() ){
		auto cb = rayTest(1000);

		if( cb.hasHit() ){
			const btCollisionObject* col = cb.m_collisionObject;
			CVector3 pos = cb.m_hitPointWorld;

			if( mouseRB.IsTrigger() ){
				//地面にクリックしたらウェイポイント設置
				if( col->getUserIndex() == enCollisionAttr_Ground ){
					NewGO<WayPoint>( 0 )->SetPos( pos );
				}
			}

			if( mouseLB.IsTrigger() ){
				//ウェイポイントにクリックしたら選択
				if( col->getUserIndex() == enCollisionAttr_WayPoint ){
					if( selectWayPoint ){
						selectWayPoint->TurnSelected();
						selectWayPoint = nullptr;
					} else{
						selectWayPoint = static_cast<WayPoint*>( col->getUserPointer() );
						selectWayPoint->TurnSelected();
						creatingLoot = NewGO<Loot>( 0 );
						creatingLoot->SetPos( selectWayPoint->GetPos());
					}
				}
			}
		}
	}
}

btCollisionWorld::ClosestRayResultCallback World::rayTest(float length, int mask){
	CVector3 from = g_camera3D.GetPosition();
	CVector3 to = from + camera.GetFrontVec() * length;
	btCollisionWorld::ClosestRayResultCallback cb( from, to );
	cb.m_collisionFilterMask = mask;
	g_physics.GetDynamicWorld()->rayTest( from, to, cb );

	return cb;
}
