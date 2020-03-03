#pragma once
#include "Ground/CollisionModel.h"

class WayPoint : public IGameObject{
public:
	WayPoint();

	void Update() override{
	}

	void SetPos( const CVector3& pos ){
		collision.SetPos( pos );
	}

private:
	CollisionModel collision;
};

