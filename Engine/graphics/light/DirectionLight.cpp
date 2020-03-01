#include "stdafx.h"
#include "DirectionLight.h"

DirectionLight::DirectionLight() {
}

DirectionLight::~DirectionLight() {
}

void DirectionLight::Init(UINT bufferNum) {
	UINT byteWidth = ((sizeof(m_cbStruct) - 1) / 16 + 1) * 16;
	m_CBuf.Init(byteWidth, false);
	m_bufferNum = bufferNum;
}

void DirectionLight::Apply() {
	m_CBuf.UpdateData(&m_cbStruct);
	m_CBuf.SetToContext( ShaderType::PS, m_bufferNum);
}
