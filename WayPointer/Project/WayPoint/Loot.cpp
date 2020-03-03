#include "stdafx.h"
#include "Loot.h"
#include "physics/CollisionAttr.h"

Loot::Loot() : collision( L"Assets/modelData/Loot.cmo" ){
	collision.SetPointer( this );
	collision.SetIndex( enCollisionAttr_Loot );
}
