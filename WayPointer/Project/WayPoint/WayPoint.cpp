#include "stdafx.h"
#include "WayPoint.h"
#include "Loot.h"
#include "physics/CollisionAttr.h"
#include "util/FileUtil.h"

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

void WayPoint::WriteData( std::ofstream & ofs ){

	BinaryOFS bfs( ofs );

	bfs.writeBinary( GetPos() );
	bfs.writeBinary( static_cast<uint32_t>( connections.size() ) );
	for( Edge& e : connections ){
		bfs.writeBinary( e.wp->GetNumber() );
		bfs.writeBinary( e.length );
	}

}
