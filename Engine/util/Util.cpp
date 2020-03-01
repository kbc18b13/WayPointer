#include "stdafx.h"
#include "Util.h"
#include <random>
#include <time.h>

static std::mt19937& getRand() {
	static std::mt19937 mt(static_cast<UINT>(time(0)));
	return mt;
}

int Util::RandomInt(int begine, int end) {
	std::uniform_int_distribution<int> l_rand(begine, end);
	return l_rand(getRand());
}

float Util::RandomFloat(float begine, float end) {
	std::uniform_real_distribution<float> l_rand(begine, end);
	return l_rand(getRand());
}

CQuaternion Util::LookRotXZ(CVector3 lookVec, CVector3 nowFront) {
	lookVec.y = 0;
	nowFront.y = 0;
	return LookRot( lookVec, nowFront );
}

CQuaternion Util::LookRot( CVector3 lookVec, CVector3 nowFront ){
	lookVec.Normalize();
	nowFront.Normalize();
	float d = lookVec.Dot( nowFront );
	float rot = acosf( d );
	lookVec.Cross( nowFront );
	lookVec.Normalize();
	CQuaternion qRot;
	qRot.SetRotation( -lookVec, rot );

	return qRot;
}

CQuaternion Util::LookRotSLerp( CVector3 lookVec, CVector3 nowFront, float lerp ){
	lookVec.Normalize();
	nowFront.Normalize();
	float d = lookVec.Dot( nowFront );
	d = CMath::Clamp( d, -1, 1 );
	float rot = acosf( d ) * lerp;
	lookVec.Cross( nowFront );
	lookVec.Normalize();
	CQuaternion qRot;
	qRot.SetRotation( -lookVec, rot );

	return qRot;
}
