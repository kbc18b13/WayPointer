#pragma once
#include "Ground/CollisionModel.h"

class Loot : public IGameObject{
public:
	Loot();

	void Update() override{
	}

	void SetPos( const CVector3& pos ){
		collision.SetPos( pos );
	}

private:
	CollisionModel collision;
};

