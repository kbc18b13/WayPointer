#include "stdafx.h"
#include "WayPoint.h"
#include "physics/CollisionAttr.h"

WayPoint::WayPoint() : collision( L"Assets/modelData/WayPoint.cmo" ){
	collision.SetPointer( this );
	collision.SetIndex( enCollisionAttr_WayPoint );
}
