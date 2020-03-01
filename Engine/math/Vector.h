/*�x�N�g��
 * @details
 * DirectMath���g���₷�������x�N�g���N���X�B */


#pragma once


#include "kMath.h"

class CMatrix;

class CVector2{
public:
	static constexpr char This_is_Vector = 0;

	CVector2() : x( 0 ), y( 0 ){}

	CVector2( float x, float y ) : x( x ), y( y ){}

	CVector2( const CVector2& v ) : x( v.x ), y( v.y ){}

	//������Z�q
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

	//-------------������Z�q--------------------//

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

	//���`��ԁB
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
//�x�N�g���B
class CVector3{
public:
	static constexpr char This_is_Vector = 0;

	union{
		DirectX::XMFLOAT3 vec;
		float v[3];
		struct{ float x, y, z; };
	};

public:
	//XMVECTOR�ւ̈Öق̕ϊ��B
	operator DirectX::XMVECTOR() const{
		return DirectX::XMLoadFloat3( &vec );
	}

	//btVector3�ւ̈Öق̕ϊ��B
	operator btVector3() const{
		return btVector3( x, y, z );
	}

	//operator D3DXVECTOR3(void) { return s_cast<D3DXVECTOR3>(*this); }

	//������Z�q�B
	CVector3& operator=( const CVector3& _v ){
		vec = _v.vec;
		return *this;
	}

	CVector3& operator=( const btVector3& _v ){
		x = _v.x(); y = _v.y(); z = _v.z();
		return *this;
	}

	//�R���X�g���N�^�B
	CVector3() : x( 0 ), y( 0 ), z( 0 ){}

	CVector3( float x, float y, float z ){
		Set( x, y, z );
	}

	CVector3( const btVector3& v ){
		Set( v.x(), v.y(), v.z() );
	}

	//���`��ԁB
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
	//�x�N�g���̊e�v�f��ݒ�B
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

	//���Z������Z�q�B
	const CVector3& operator+=( const CVector3& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd( xmv0, xmv1 );
		DirectX::XMStoreFloat3( &vec, xmvr );
		return *this;
	}


	//���Z������Z�q�B
	const CVector3& operator-=( const CVector3& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract( xmv0, xmv1 );
		DirectX::XMStoreFloat3( &vec, xmvr );
		return *this;
	}


	//�g�������Z�q�B
	const CVector3& operator*=( float s ){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		xmv = DirectX::XMVectorScale( xmv, s );
		DirectX::XMStoreFloat3( &vec, xmv );
		return *this;
	}

	//���Z������Z�q
	const CVector3& operator/=( float d ){
		float scale = 1.0f / d;
		return operator*=( scale );
	}

	//���ρB
	float Dot( const CVector3& _v ) const{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &_v.vec );
		return DirectX::XMVector3Dot( xmv0, xmv1 ).m128_f32[0];
	}
	//�O�ρB
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
	//�������擾

	float Length() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		return DirectX::XMVector3Length( xmv ).m128_f32[0];
	}
	//�����̓����擾

	float LengthSq() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		return DirectX::XMVector3LengthSq( xmv ).m128_f32[0];
	}

	//�@���𐳋K���B
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3( &vec );
		xmv = DirectX::XMVector3Normalize( xmv );
		DirectX::XMStoreFloat3( &vec, xmv );
	}

	//�ő�l��ݒ�B
	void Max( const CVector3& vMax ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &vMax.vec );
		DirectX::XMStoreFloat3( &vec, DirectX::XMVectorMax( xmv0, xmv1 ) );
	}
	//�ŏ��l��ݒ�B
	void Min( const CVector3& vMin ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3( &vMin.vec );
		DirectX::XMStoreFloat3( &vec, DirectX::XMVectorMin( xmv0, xmv1 ) );
	}

	//CVector2�𐶐�
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
//4�v�f�̃x�N�g���N���X�B
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
	//������Z�q�B
	CVector4& operator=( const CVector4& _v ){
		vec = _v.vec;
		return *this;
	}

	CVector4() : v{ 0,0,0,1 }{}
	//�R���X�g���N�^

	CVector4( float x, float y, float z, float w ){
		Set( x, y, z, w );
	}

	//�R���X�g���N�^
	//w�ɂ�1.0���i�[����܂��B
	CVector4( const CVector3& v ){
		Set( v );
	}
	//�x�N�g���̊e�v�f��ݒ�B
	void Set( float _x, float _y, float _z, float _w ){
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}
	//�@���𐳋K���B
	void Normalize(){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		xmv = DirectX::XMVector4Normalize( xmv );
		DirectX::XMStoreFloat4( &vec, xmv );
	}

	//�x�N�g����ݒ�B
	void Set( const CVector4& _v ){
		*this = _v;
	}

	//�x�N�g����ݒ�B
	//w�ɂ�1.0���i�[����܂��B
	void Set( const CVector3& _v ){
		this->x = _v.x;
		this->y = _v.y;
		this->z = _v.z;
		this->w = 1.0f;
	}
	//���Z������Z�q�B
	const CVector4& operator+=( const CVector4& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd( xmv0, xmv1 );
		DirectX::XMStoreFloat4( &vec, xmvr );
		return *this;
	}

	//���Z������Z�q�B
	const CVector4& operator-=( const CVector4& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4( &_v.vec );
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract( xmv0, xmv1 );
		DirectX::XMStoreFloat4( &vec, xmvr );
		return *this;
	}

	//�g�������Z�q�B
	const CVector4& operator*=( float s ){
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		xmv = DirectX::XMVectorScale( xmv, s );
		DirectX::XMStoreFloat4( &vec, xmv );
		return *this;
	}

	//���Z������Z�q�B
	const CVector4& operator/=( float s ){
		float div = 1.0f / s;
		return operator*=( div );
	}

	//����
	float Dot( const CVector4& _v ){
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4( &vec );
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4( &_v.vec );
		return DirectX::XMVector4Dot( xmv0, xmv1 ).m128_f32[0];
	}

	//�������擾
	float Length() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		return DirectX::XMVector4Length( xmv ).m128_f32[0];
	}

	//�����̓����擾
	float LengthSq() const{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4( &vec );
		return DirectX::XMVector4LengthSq( xmv ).m128_f32[0];
	}

	//CVector2�𐶐�
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

	//�P���}�C�i�X���Z�q
	const CQuaternion operator-(){
		return CQuaternion(-x, -y, -z, -w);
	}

	//�C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
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

	//�N�H�[�^�j�I���ɉ�]���v���X�B
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

	//�s�񂩂�N�H�[�^�j�I�����쐬�B
	void SetRotation( const CMatrix& m );
	//���ʐ��`�⊮�B
	void Slerp( float t, CQuaternion q1, CQuaternion q2 ){
		DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
			DirectX::XMLoadFloat4( &q1.vec ),
			DirectX::XMLoadFloat4( &q2.vec ),
			t
		);
		DirectX::XMStoreFloat4( &vec, xmv );
	}
	//�N�H�[�^�j�I�����m�̐ρB
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

	//�N�H�[�^�j�I�����m�̏�Z�B
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

	//�x�N�g���ɃN�H�[�^�j�I����K�p����B
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

	//�N���X���璼�ځA�C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
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
//�����^�̃x�N�g���N���X�B
__declspec( align( 16 ) ) class CVector4i{
public:
	static constexpr char This_is_Vector = 0;

	union{
		struct{ int x, y, z, w; };
		int v[4];
	};
};
//�x�N�g�����m�̉��Z�B
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator+( const TVector& v0, const TVector& v1 ){
	TVector result = v0;
	return result += v1;
}
//�x�N�g���̃X�P�[���{�B
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator*( const TVector& v, float s ){
	TVector result = v;
	return result *= s;
}
//�x�N�g���̏��Z�B
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator/( const TVector& v, float s ){
	TVector result = v;
	return result /= s;
}
//�x�N�g�����m�̌��Z�B
template<class TVector>
static inline decltype( TVector::This_is_Vector, std::declval<TVector>() ) operator-( const TVector& v0, const TVector& v1 ){
	TVector result = v0;
	return result -= v1;
}

