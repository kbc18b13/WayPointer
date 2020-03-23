#pragma once
#include "Ground/CollisionModel.h"

class WayPoint;

class Loot : public IGameObject{
public:
	Loot(WayPoint* start);

	void Update() override{
	}

	CVector3 GetPos() const{
		return collision.GetPos();
	}

	void SetPos( const CVector3& pos ){
		collision.SetPos( pos );
	}

	void SetTargetWayPoint( WayPoint* wp );

	void SetTargetPos( const CVector3& pos );

	void SetRot( const CQuaternion& rot ){
		collision.SetRot( rot );
	}

	void SetScale( const CVector3& scale ){
		collision.SetScale( scale );
	}

private:
	WayPoint* start;
	WayPoint* end = nullptr;

	CollisionModel collision;
};

