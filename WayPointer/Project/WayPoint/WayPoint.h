#pragma once
#include "Ground/CollisionModel.h"

class WayPoint : public IGameObject{
public:
	WayPoint();

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

private:
	bool isSelect = false;

	CollisionModel collision;
};

