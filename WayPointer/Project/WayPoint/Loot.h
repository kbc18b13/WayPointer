#pragma once
#include "Ground/CollisionModel.h"

class Loot : public IGameObject{
public:
	Loot();

	void Update() override{
	}

	CVector3 GetPos() const{
		return collision.GetPos();
	}

	void SetPos( const CVector3& pos ){
		collision.SetPos( pos );
	}

	void SetTargetPos( const CVector3& pos );

	void SetRot( const CQuaternion& rot ){
		collision.SetRot( rot );
	}

	void SetScale( const CVector3& scale ){
		collision.SetScale( scale );
	}

private:
	CollisionModel collision;
};

