#include "stdafx.h"
#include "AmbientLight.h"

void AmbientLight::Init(UINT bufferNum) {
	m_CBuf.Init(sizeof(CVector4), false);
	m_bufferNum = bufferNum;
}

void AmbientLight::Apply() {
	m_CBuf.UpdateData(&m_color);
	m_CBuf.SetToContext( ShaderType::PS, m_bufferNum);
}
