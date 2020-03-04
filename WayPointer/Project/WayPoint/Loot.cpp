#include "stdafx.h"
#include "Loot.h"
#include "physics/CollisionAttr.h"

Loot::Loot() : collision( L"Assets/modelData/Loot.cmo" , nullptr , btCollisionObject::CollisionFlags::CF_Loot){
	collision.SetPointer( this );
	collision.SetIndex( enCollisionAttr_Loot );
}

void Loot::SetTargetPos( const CVector3 & pos ){
	CVector3 toVec = pos - GetPos();

	SetScale( { 1, 1, toVec.Length() } );
	SetRot( Util::LookRot( toVec ) );
}
