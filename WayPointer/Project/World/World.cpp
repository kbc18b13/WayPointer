#include "stdafx.h"
#include "physics/CollisionAttr.h"
#include "World.h"
#include "graphics/SkinModelRender.h"
#include "WayPoint/WayPoint.h"
#include "WayPoint/Loot.h"
#include "util/FileUtil.h"

World::World() : ground( L"Assets/modelData/FirstStage.cmo" ){
	ground.SetIndex( enCollisionAttr_Ground );

	g_graphicsEngine->GetDirectionLight().SetColor( 0, { 1,1,1,1 } );
	g_graphicsEngine->GetDirectionLight().SetDirection( 0, { -1,-1,-1 } );
	g_graphicsEngine->GetDirectionLight().Apply();

	g_graphicsEngine->GetAmbientLight().SetColor( { 0.5f,0.5f,0.5f,1 } );
	g_graphicsEngine->GetAmbientLight().Apply();
}

World::~World(){}

void World::Update(){
	camera.Update();
	mouseLB.Update();
	mouseRB.Update();
	saveB.Update();

	//���[�g���ݒ�
	if( creatingLoot ){
		auto cb = rayTest(1000, ~btCollisionObject::CollisionFlags::CF_Loot);

		//���[�g�̐�����C�e�X�g�ŏo��
		CVector3 targetPos;
		WayPoint* hitWp = nullptr;
		if( cb.hasHit() ){
			//�����̐悪�E�F�C�|�C���g�Ȃ炻�����w��
			if( cb.m_collisionObject->getUserIndex() == enCollisionAttr_WayPoint ){
				hitWp = static_cast<WayPoint*>( cb.m_collisionObject->getUserPointer() );
				targetPos = hitWp->GetPos();
			} else{
				targetPos = cb.m_hitPointWorld;
			}
		} else{
			targetPos = g_camera3D.GetPosition() + camera.GetFrontVec() * 1000;
		}
		creatingLoot->SetTargetPos( targetPos );

		//���N���b�N�Ŋm��B�E�F�C�|�C���g��I�����ĂȂ���΃��[�g���폜�B
		if( mouseLB.IsTrigger() ){
			if( hitWp != nullptr ){
				creatingLoot->SetTargetWayPoint( hitWp);
				selectWayPoint->Connect( hitWp );
			} else{
				DeleteGO( creatingLoot );
			}
			creatingLoot = nullptr;
			selectWayPoint->TurnSelected();
			selectWayPoint = nullptr;
		}

	//�������ĂȂ���ԁB�N���b�N�����m�B
	} else if( mouseLB.IsTrigger() || mouseRB.IsTrigger() ){
		auto cb = rayTest(1000);

		if( cb.hasHit() ){
			const btCollisionObject* col = cb.m_collisionObject;
			CVector3 pos = cb.m_hitPointWorld;

			if( mouseRB.IsTrigger() ){
				//�n�ʂɃN���b�N������E�F�C�|�C���g�ݒu
				if( col->getUserIndex() == enCollisionAttr_Ground ){
					AddWayPoint( pos );
				}
			}

			if( mouseLB.IsTrigger() ){
				//�E�F�C�|�C���g�ɃN���b�N������I��
				if( col->getUserIndex() == enCollisionAttr_WayPoint ){
					if( selectWayPoint ){
						selectWayPoint->TurnSelected();
						selectWayPoint = nullptr;
					} else{
						selectWayPoint = static_cast<WayPoint*>( col->getUserPointer() );
						selectWayPoint->TurnSelected();
						creatingLoot = NewGO<Loot>( 0 , selectWayPoint);
					}
				}
			}
		}
	} else if( saveB.IsTrigger() ){

		std::ofstream ofs( "wayPoint.wpdata", std::ios::binary);
		if( !ofs ){
			abort();
		}
		BinaryOFS bfs( ofs );

		bfs.writeBinary( static_cast<uint32_t>(wayPoints.size()) );

		int wayPNum = 0;
		for( WayPoint* wp : wayPoints ){
			wp->SetNumber( wayPNum );
			wayPNum++;
		}
		for( WayPoint* wp : wayPoints ){
			wp->WriteData( ofs );
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

WayPoint * World::AddWayPoint( const CVector3 & pos ){
	WayPoint* wp = NewGO<WayPoint>( 0 );
	wayPoints.push_back( wp );
	wp->SetPos( pos );
	return wp;
}
