#pragma once
#include "Ground/CollisionModel.h"

class WayPoint;
class Loot;

struct Edge{
	Edge( WayPoint* wp, float length ) : wp( wp ), length( length ){}
	WayPoint* wp = nullptr;
	float length = 0;
};

class WayPoint : public IGameObject{
public:
	WayPoint();
	~WayPoint();

	void Update() override{}

	CVector3 GetPos() const{
		return collision.GetPos() + CVector3::Up() * 22;
	}

	void SetPos( const CVector3& pos ){
		collision.SetPos( pos );
	}

	void TurnSelected(){
		isSelect = !isSelect;
		if( isSelect ){
			collision.GetModel().GetModel().setEmissionColor( { 5,0,0,1 } );
		} else{
			collision.GetModel().GetModel().setEmissionColor( { 0,0,0,0 } );
		}
	}

	float Distance( WayPoint* wp ){
		return ( wp->GetPos() - GetPos() ).Length();
	}

	void Connect( WayPoint* wayP ){
		ConnectOneWay( wayP );
		wayP->ConnectOneWay( this );
	}

	void ConnectOneWay( WayPoint* wayP ){
		connections.emplace_back( wayP ,Distance(wayP));
	}

	Loot* CreateLoot();

	void DeleteLoot( Loot* );

private:
	bool isSelect = false;

	std::vector<Edge> connections;

	std::vector<Loot*> loots;

	CollisionModel collision;
};

