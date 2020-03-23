#include "stdafx.h"
#include "WayPoint.h"
#include "Loot.h"
#include "physics/CollisionAttr.h"

WayPoint::WayPoint() : collision( L"Assets/modelData/WayPoint.cmo" ){
	collision.SetPointer( this );
	collision.SetIndex( enCollisionAttr_WayPoint );
}

WayPoint::~WayPoint(){}

Loot * WayPoint::CreateLoot(){
	Loot* loot = NewGO<Loot>( 0, this );
	loots.push_back( loot );
	return loot;
}

void WayPoint::DeleteLoot( Loot * loot){
	loots.erase( std::remove( loots.begin(), loots.end(), loot ), loots.end() );
}
