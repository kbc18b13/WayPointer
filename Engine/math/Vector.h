/*ベクトル
 * @details
 * DirectMathを使いやすくしたベクトルクラス。 */


#pragma once


#include "kMath.h"

class CMatrix;

class CVector2{
public:
	static constexpr char This_is_Vector = 0;

	CVector2() : x( 0 ), y( 0 ){}

	CVector2( float x, float y ) : x( x ), y( y ){}

	CVector2( const CVector2& v ) : x( v.x ), y( v.y ){}

	//代入演算子
	CVector2& operator=( const CVector2& v ){
		this->x = v.x;
		this->y = v.y;
		return *this;
	}

	union{
		DirectX::XMFLOAT2 vec;
		struct{ float x, y; };
		float v[2];
	};

	operator DirectX::XMVECTOR() const{
		return DirectX::XMLoadFloat2( &vec );
	}

	//-------------代入演算子--------------------//

	const CVector2& operator+=( const CVector2& _v ){
		x += _v.x; y += _v.y;
		return *this;
	}

	const CVector2& operator-=( const CVector2& _v ){
		x -= _v.x; y -= _v.y;
		return *this;
	}

	const CVector2& operator*=( float _v ){
		x *= _v; y *= _v;
		return *this;
	}

	const CVector2& operator/=( float _v ){
		x /= _v; y /= _v;
		return *this;
	}

	float LengthSq(){
		return x * x + y * y;
	}

	float Length(){
		return sqrt( LengthSq() );
	}

	float Dot( const CVector2& _v ){
		return x * _v.x + y * _v.y;
	}

	void Normalize(){
		*this /= Length();
	}

	//線形補間。
	void Lerp( float t, const CVector2& v0, const CVector2& v1 ){
		x = v0.x + ( v1.x - v0.x ) * t;
		y = v0.y + ( v1.y - v0.y ) * t;
	}
	static CVector2 Zero(){
		static const CVector2 zero = { 0.0f,  0.0f };
		return zero;
	}

	static CVector2 One(){
		static const CVector2 one = { 1.0f,  1.0f };
		return one;
	}
};
//ベクトル。
class CVector3{
public:
	static constexpr char This_is_Vector = 0;

	union{
		DirectX::XMFLOAT3 vec;
		float v[3];
		struct{ float x, y, z; };
	};

public:
	//XMVECTORへの暗黙の変換。
	operator DirectX::XMVECTOR() const{
		return DirectX::XMLoadFloat3( &vec );
	}

	//btVector3への暗黙の変換。
	operator btVector3() const{
		return btVector3( x, y, z );
	}

	//operator D3DXVECTOR3(void) { return s_cast<D3DXVECTOR3>(*this); }

	//代入演算子。
	CVector3& operator=( const CVector3& _v ){
		vec = _v.vec;
		return *this;
	}

	CVector3& operator=( const btVector3& _v ){
		x = _v.x(); y = _v.y(); z = _v.z();
		return *this;
	}

	//コンストラクタ。
	CVector3() : x( 0 ), y( 0 ), z( 0 ){}

	CVector3( float x, float y, float z ){
		Set( x, y, z );
	}

	CVector3( const btVector3& v ){
		Set( v.x(), v.y(), v.z() );
	}

	//線形補間。
	void Lerp( float t, const CVector3& v0, const CVector3& v1 ){
		DirectX::XMVECTOR _v = DirectX::XMVectorLerp(
			DirectX::XMLoadFloat3( &v0.vec ),
			DirectX::XMLoadFloat3( &v1.vec ),
			t );
		DirectX::XMStoreFloat3( &vec, _v );
	}
	template<class TVector>
	void CopyTo( TVector& dst ) const{
		dst.x = x;
		dst.y = y;
		dst.z = z;
	}
	//ベクトルの各要素を設定。
	void Set( float _x, float _y, float _z ){
		vec.x = _x;
		vec.y = _y;
		vec.z = _z;
	}
	template<class TVector>
	void Set( TVector& _v ){
		Set( _v.x, _v.y, _v.z );
	}
	template<>
	void Set( btVector3& _v ){
		Set( _v.x(), _v.y(), _v.z() );
	}

	//加算代入演算子。
	const CVector3& operator+=( const CVector3& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd( xmv0, xmv1 );
		DirectX::XMStoreFloat3( &vec, xmvr );
		return *this;
	}


	//減算代入演算子。
	const CVector3& operator-=( const CVector3& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract( xmv0, xmv1 );
		DirectX::XMStoreFloat3( &vec, xmvr );
		return *this;
	}


	//拡大代入演算子。
	const CVector3& operator*=( float s ){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		xmv = DirectX::XMVectorScale( xmv, s );
		DirectX::XMStoreFloat3( &vec, xmv );
		return *this;
	}

	//除算代入演算子
	const CVector3& operator/=( float d ){
		float scale = 1.0f / d;
		return operator*=( scale );
	}

	//内積。
	float Dot( const CVector3& _v ) const{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		return DirectX::XMVector3Dot( xmv0, xmv1 ).m128_f32[0];
	}
	//外積。
	void Cross( const CVector3& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross( xmv0, xmv1 );
		DirectX::XMStoreFloat3( &vec, xmvr );
	}
	static CVector3 Cross( const CVector3& v0, const CVector3& v1 ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &v0.vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &v1.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross( xmv0, xmv1 );
		CVector3 temp;
		DirectX::XMStoreFloat3( &temp.vec, xmvr );
		return temp;
	}
	//長さを取得

	float Length() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		return DirectX::XMVector3Length( xmv ).m128_f32[0];
	}
	//長さの二乗を取得

	float LengthSq() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		return DirectX::XMVector3LengthSq( xmv ).m128_f32[0];
	}

	//法線を正規化。
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		xmv = DirectX::XMVector3Normalize( xmv );
		DirectX::XMStoreFloat3( &vec, xmv );
	}

	//最大値を設定。
	void Max( const CVector3& vMax ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &vMax.vec );
		DirectX::XMStoreFloat3( &vec, DirectX::XMVectorMax( xmv0, xmv1 ) );
	}
	//最小値を設定。
	void Min( const CVector3& vMin ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &vMin.vec );
		DirectX::XMStoreFloat3( &vec, DirectX::XMVectorMin( xmv0, xmv1 ) );
	}

	//CVector2を生成
	CVector2 xy() const{
		return CVector2( x, y );
	}
	CVector2 xz() const{
		return CVector2( x, z );
	}
	CVector2 yx() const{
		return CVector2( y, x );
	}
	CVector2 yz() const{
		return CVector2( y, z );
	}
	CVector2 zx() const{
		return CVector2( z, x );
	}
	CVector2 zy() const{
		return CVector2( z, y );
	}

public:
	static CVector3 Zero(){
		static const CVector3 zero = { 0.0f,  0.0f,  0.0f };
		return zero;
	}
	static CVector3 Right(){
		static const CVector3 right = { 1.0f,  0.0f,  0.0f };
		return right;
	}
	static CVector3 Left(){
		static const CVector3 left = { -1.0f,  0.0f,  0.0f };
		return left;
	}
	static CVector3 Up(){
		static const CVector3 up = { 0.0f,  1.0f,  0.0f };
		return up;
	}
	static CVector3 Down(){
		static const CVector3 down = { 0.0f, -1.0f,  0.0f };
		return down;
	}
	static CVector3 Front(){
		static const CVector3 front = { 0.0f,   0.0f,  1.0f };
		return front;
	}
	static CVector3 Back(){
		static const CVector3 back = { 0.0f,   0.0f, -1.0f };
		return back;
	}
	static CVector3 AxisX(){
		static const CVector3 axisX = { 1.0f,  0.0f,  0.0f };
		return axisX;
	}
	static CVector3 AxisY(){
		static const CVector3 axisY = { 0.0f,  1.0f,  0.0f };
		return axisY;
	}
	static CVector3 AxisZ(){
		static const CVector3 axisZ = { 0.0f,  0.0f,  1.0f };
		return axisZ;
	}
	static CVector3 One(){
		static const CVector3 one = { 1.0f, 1.0f, 1.0f };
		return one;
	}
};
//4要素のベクトルクラス。
class CVector4{
public:
	static constexpr char This_is_Vector = 0;

	union{
		DirectX::XMFLOAT4 vec;
		struct{ float x, y, z, w; };
		float v[4];
	};
public:
	operator DirectX::XMVECTOR() const{
		return DirectX::XMLoadFloat4( &vec );
	}

	operator CVector3() const{
		return CVector3( x, y, z );
	}
	//代入演算子。
	CVector4& operator=( const CVector4& _v ){
		vec = _v.vec;
		return *this;
	}

	CVector4() : v{ 0,0,0,1 }{}
	//コンストラクタ

	CVector4( float x, float y, float z, float w ){
		Set( x, y, z, w );
	}

	//コンストラクタ
	//wには1.0が格納されます。
	CVector4( const CVector3& v ){
		Set( v );
	}
	//ベクトルの各要素を設定。
	void Set( float _x, float _y, float _z, float _w ){
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}
	//法線を正規化。
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		xmv = DirectX::XMVector4Normalize( xmv );
		DirectX::XMStoreFloat4( &vec, xmv );
	}

	//ベクトルを設定。
	void Set( const CVector4& _v ){
		*this = _v;
	}

	//ベクトルを設定。
	//wには1.0が格納されます。
	void Set( const CVector3& _v ){
		this->x = _v.x;
		this->y = _v.y;
		this->z = _v.z;
		this->w = 1.0f;
	}
	//加算代入演算子。
	const CVector4& operator+=( const CVector4& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd( xmv0, xmv1 );
		DirectX::XMStoreFloat4( &vec, xmvr );
		return *this;
	}

	//減算代入演算子。
	const CVector4& operator-=( const CVector4& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract( xmv0, xmv1 );
		DirectX::XMStoreFloat4( &vec, xmvr );
		return *this;
	}

	//拡大代入演算子。
	const CVector4& operator*=( float s ){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		xmv = DirectX::XMVectorScale( xmv, s );
		DirectX::XMStoreFloat4( &vec, xmv );
		return *this;
	}

	//除算代入演算子。
	const CVector4& operator/=( float s ){
		float div = 1.0f / s;
		return operator*=( div );
	}

	//内積
	float Dot( const CVector4& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4( &_v.vec );
		return DirectX::XMVector4Dot( xmv0, xmv1 ).m128_f32[0];
	}

	//長さを取得
	float Length() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		return DirectX::XMVector4Length( xmv ).m128_f32[0];
	}

	//長さの二乗を取得
	float LengthSq() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		return DirectX::XMVector4LengthSq( xmv ).m128_f32[0];
	}

	//CVector2を生成
	CVector2 xy() const{
		return CVector2( x, y );
	}

	static CVector4 White(){
		static const CVector4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
		return white;

	}
};

class CQuaternion : public CVector4{
public:
	CQuaternion(){}
	CQuaternion( float x, float y, float z, float w ) :
		CVector4( x, y, z, w ){}

	operator btQuaternion() const{
		return btQuaternion( x, y, z, w );
	}

	//単項マイナス演算子
	const CQuaternion operator-(){
		return CQuaternion(-x, -y, -z, -w);
	}

	//任意の軸周りの回転クォータニオンを作成。
	void SetRotation( const CVector3& axis, float angle ){
		float s;
		float halfAngle = angle * 0.5f;
		s = sin( halfAngle );
		w = cos( halfAngle );
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	void SetRotationDeg( const CVector3& axis, float angle ){
		float s;
		float halfAngle = CMath::DegToRad( angle ) * 0.5f;
		s = sin( halfAngle );
		w = cos( halfAngle );
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	void SetRotationVec( CVector3 from, CVector3 to );

	//クォータニオンに回転をプラス。
	void AddRotation( const CVector3& axis, float angle ){
		CQuaternion rot;
		rot.SetRotation( axis, angle );
		Multiply( rot );
	}
	void AddRotationDeg( const CVector3& axis, float angle ){
		CQuaternion rot;
		rot.SetRotationDeg( axis, angle );
		Multiply( rot );
	}

	//行列からクォータニオンを作成。
	void SetRotation( const CMatrix& m );
	//球面線形補完。
	void Slerp( float t, CQuaternion q1, CQuaternion q2 ){
		DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
			DirectX::XMLoadFloat4( &q1.vec ),
			DirectX::XMLoadFloat4( &q2.vec ),
			t
		);
		DirectX::XMStoreFloat4( &vec, xmv );
	}
	//クォータニオン同士の積。
	void Multiply( const CQuaternion& rot ){
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		pw = w; px = x; py = y; pz = z;
		qw = rot.w; qx = rot.x; qy = rot.y; qz = rot.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;

	}

	//クォータニオン同士の乗算。
	void Multiply( const CQuaternion& rot0, const CQuaternion& rot1 ){
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		pw = rot0.w; px = rot0.x; py = rot0.y; pz = rot0.z;
		qw = rot1.w; qx = rot1.x; qy = rot1.y; qz = rot1.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;
	}

	//ベクトルにクォータニオンを適用する。
	void Multiply( CVector4& _v ){
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate( _v, *this );
		DirectX::XMStoreFloat4( &_v.vec, xmv );
	}
	void Multiply( CVector3& _v ){
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate( _v, *this );
		DirectX::XMStoreFloat3( &_v.vec, xmv );
	}

	static CQuaternion Identity(){
		static const CQuaternion identity = { 0.0f,  0.0f, 0.0f, 1.0f };
		return identity;
	}

	//クラスから直接、任意の軸周りの回転クォータニオンを作成。
	static CQuaternion CreateRot( const CVector3& axis, float angle ){
		CQuaternion q;
		q.SetRotation( axis, angle );
		return q;
	}
	static CQuaternion CreateRotDeg( const CVector3& axis, float angle ){
		CQuaternion q;
		q.SetRotationDeg( axis, angle );
		return q;
	}

	static CQuaternion CreateRotVec(const CVector3& from, const CVector3& to ){
		CQuaternion q;
		q.SetRotationVec( from, to );
		return q;
	}

};
//整数型のベクトルクラス。
__declspec( align( 16 ) ) class CVector4i{
public:
	static constexpr char This_is_Vector = 0;

	union{
		struct{ int x, y, z, w; };
		int v[4];
	};
};
//ベクトル同士の加算。
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator+( const TVector& v0, const TVector& v1 ){
	TVector result = v0;
	return result += v1;
}
//ベクトルのスケール倍。
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator*( const TVector& v, float s ){
	TVector result = v;
	return result *= s;
}
//ベクトルの除算。
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator/( const TVector& v, float s ){
	TVector result = v;
	return result /= s;
}
//ベクトル同士の減算。
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator-( const TVector& v0, const TVector& v1 ){
	TVector result = v0;
	return result -= v1;
}

