#pragma once
#include "graphics/SkinModelRender.h"

class WayPoint : public IGameObject{
public:
	WayPoint();

	void Update() override{
		r.Update();
	}

	void SetPos( const CVector3& pos ){
		r.SetPos( pos );
	}

private:
	SkinModelRender r;
};

