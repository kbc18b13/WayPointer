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
	/// 初期化
	/// </summary>
	/// <param name="bufferNum">定数バッファのレジスタ番号</param>
	void Init(UINT bufferNum);

	/// <summary>
	/// ライトの方向を設定
	/// </summary>
	void SetDirection(UINT lightNum ,const CVector3& v) {
		m_cbStruct.m_vec[lightNum] = v;
	}

	/// <summary>
	///ライトの色を設定
	/// </summary>
	void SetColor(UINT lightNum, const CVector4& c) {
		m_cbStruct.m_color[lightNum] = c;
	}

	/// <summary>
	/// 定数バッファを適用
	/// </summary>
	void Apply();

private:
	CBStruct m_cbStruct;
	ConstantBuffer m_CBuf;
	UINT m_bufferNum;
};