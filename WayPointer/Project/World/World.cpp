#include "stdafx.h"
#include "World.h"
#include "graphics/SkinModelRender.h"
#include "WayPoint/WayPoint.h"

World::World() : ground( L"Assets/modelData/FirstStage.cmo" ){
	g_graphicsEngine->GetDirectionLight().SetColor( 0, { 1,1,1,1 } );
	g_graphicsEngine->GetDirectionLight().SetDirection( 0, { -1,-1,-1 } );
	g_graphicsEngine->GetDirectionLight().Apply();

	g_graphicsEngine->GetAmbientLight().SetColor( { 0.5f,0.5f,0.5f,1 } );
	g_graphicsEngine->GetAmbientLight().Apply();
}

void World::Update(){
	camera.Update();

	if( GetAsyncKeyState( VK_LBUTTON ) ){
		CVector3 from = g_camera3D.GetPosition();
		CVector3 to = from + camera.GetFrontVec() * 1000;
		btCollisionWorld::ClosestRayResultCallback cb(from, to);
		g_physics.GetDynamicWorld()->rayTest( from, to, cb );

		if( cb.hasHit() ){
			CVector3 pos = cb.m_hitPointWorld;
			NewGO<WayPoint>(0)->SetPos(pos);
		}
	}
}
