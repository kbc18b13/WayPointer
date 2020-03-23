#pragma once
#include "Ground/CollisionModel.h"
#include "Camera/Camera.h"
#include "Util/KeyTrigger.h"

class WayPoint;
class Loot;

class World{
public:
	World();
	~World();

	void Update();

private:
	btCollisionWorld::ClosestRayResultCallback rayTest(float length, int mask = 0xffffffff);
	WayPoint* AddWayPoint( const CVector3& pos );

	MyCamera camera;

	CollisionModel ground;

	KeyTrigger mouseLB{ VK_LBUTTON};
	KeyTrigger mouseRB{ VK_RBUTTON };

	std::vector<WayPoint*> wayPoints;

	WayPoint* selectWayPoint = nullptr;
	Loot* creatingLoot = nullptr;
};

