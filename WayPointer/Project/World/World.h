#pragma once
#include "Ground/CollisionModel.h"
#include "Camera/Camera.h"

class World{
public:
	World();

	void Update();

private:
	MyCamera camera;
	CollisionModel ground;
};

