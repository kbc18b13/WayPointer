#pragma once
#include "Ground/CollisionModel.h"
#include "Camera/Camera.h"
#include "Util/KeyTrigger.h"

class WayPoint;

class World{
public:
	World();

	void Update();

private:
	MyCamera camera;
	CollisionModel ground;
	KeyTrigger mouseLB{ VK_LBUTTON};

	WayPoint* selectWayPoint = nullptr;
};

