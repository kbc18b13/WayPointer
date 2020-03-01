/*!
 * @brief	コライダー。
 */

#pragma once

class btCollisionShape;

/*!
 * @brief	コライダーのインターフェースクラス。
 */
class ICollider {
public:
	virtual void SetLocalScale( const CVector3& scale ) = 0;
	virtual btCollisionShape* GetBody() const = 0;
};
