/*!
 * @brief	ベクトルクラス。
 */

#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"




/*!
*@brief	行列からクォータニオンを作成。
*/
void CQuaternion::SetRotation(const CMatrix& m)
{
	DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
}

void CQuaternion::SetRotationVec( CVector3 from, CVector3 to ){
	from.Normalize();
	to.Normalize();
	float angle = acosf( from.Dot( to ) );
	from.Cross( to );
	from.Normalize();

	SetRotation( from, angle );
}
