#pragma once
#include "graphics\Parts\ConstantBuffer.h"

class AmbientLight {
public:

	void Init(UINT bufferNum);

	void SetColor(const CVector4& c) {
		m_color = c;
	}

	void Apply();

private:
	ConstantBuffer m_CBuf;
	UINT m_bufferNum;
	CVector4 m_color;
};