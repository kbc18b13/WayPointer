#pragma once
#include "SpriteEffect.h"
#include "Parts/ConstantBuffer.h"

class Sprite{
public:
	Sprite();
	~Sprite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="path">テクスチャパス</param>
	/// <param name="width">幅</param>
	/// <param name="hight">高さ</param>
	void Init( const wchar_t* path, UINT width, UINT height );

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*@param[in]	pivot		モデルのピボット。
	*/
	void UpdateWorldMatrix( CVector2 pos, const CVector2& scale,
							const CQuaternion& rot, CVector2 pivot = CVector2::Zero(), float depth = 0.5f);

	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	mat        モデルのワールド座標
	*/
	void UpdateWorldMatrix( const CMatrix& mat ){
		m_CBStruct.worldMat = mat;
		m_CBuf.UpdateData( &m_CBStruct );
	}

	void SetMulColor( const CVector4& color ){
		m_CBStruct.mulColor = color;
		m_CBuf.UpdateData( &m_CBStruct );
	}

	CVector4 GetMulColor(){
		return m_CBStruct.mulColor;
	}

	UINT GetWidth(){
		return m_width;
	}

	UINT GetHeight(){
		return m_height;
	}

private:
	struct CBStruct{
		CMatrix worldMat;
		CVector4 mulColor;
	};

	SpriteEffect m_effect;              //テクスチャやシェーダーなど
	ID3D11Buffer* m_vertex;             //頂点バッファ
	CBStruct m_CBStruct;            //定数バッファ構造体
	ConstantBuffer m_CBuf;              //定数バッファ
	UINT m_width = 0;
	UINT m_height = 0;
};

