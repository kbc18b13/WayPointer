/*!
 * @brief	ボックスコライダー。
 */

#include "stdafx.h"
#include "Physics/CylinderCollider.h"

/*!
	* @brief	デストラクタ。
	*/
CylinderCollider::~CylinderCollider()
{
	delete shape;
}
