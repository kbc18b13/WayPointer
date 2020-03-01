#pragma once
#include "graphics/Parts/ConstantBuffer.h"

class DirectionLight {
private:
	static constexpr UINT LIGHT_NUM = 4;

	struct CBStruct {
		CVector4 m_color[LIGHT_NUM];
		CVector4 m_vec[LIGHT_NUM];
	};

public:
	DirectionLight();
	~DirectionLight();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="bufferNum">�萔�o�b�t�@�̃��W�X�^�ԍ�</param>
	void Init(UINT bufferNum);

	/// <summary>
	/// ���C�g�̕�����ݒ�
	/// </summary>
	void SetDirection(UINT lightNum ,const CVector3& v) {
		m_cbStruct.m_vec[lightNum] = v;
	}

	/// <summary>
	///���C�g�̐F��ݒ�
	/// </summary>
	void SetColor(UINT lightNum, const CVector4& c) {
		m_cbStruct.m_color[lightNum] = c;
	}

	/// <summary>
	/// �萔�o�b�t�@��K�p
	/// </summary>
	void Apply();

private:
	CBStruct m_cbStruct;
	ConstantBuffer m_CBuf;
	UINT m_bufferNum;
};