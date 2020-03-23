#include "stdafx.h"
#include "Loot.h"
#include "WayPoint.h"
#include "physics/CollisionAttr.h"

Loot::Loot(WayPoint* start) : start(start),
collision( L"Assets/modelData/Loot.cmo" , nullptr , btCollisionObject::CollisionFlags::CF_Loot){
	collision.SetPointer( this );
	collision.SetIndex( enCollisionAttr_Loot );

	SetPos( start->GetPos() );
}

void Loot::SetTargetWayPoint( WayPoint * wp ){
	SetTargetPos( wp->GetPos() );
	end = wp;
}

void Loot::SetTargetPos( const CVector3 & pos ){
	CVector3 toVec = pos - GetPos();

	SetScale( { 1, 1, toVec.Length() } );
	SetRot( Util::LookRot( toVec ) );
}
