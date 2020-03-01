#pragma once
#include "Sprite.h"

class SpriteRender : public IRenderObject {
public:

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render() override;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="path">ddsのファイルパス</param>
	void Init(const wchar_t* path, UINT width, UINT height);

	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="pos">位置。ピクセル単位。</param>
	void SetPosPixel(const CVector2& pos) {
		m_pos = pos;
	}

	//正規化座標系で位置を設定。
	void SetPosNormalized( const CVector2& pos ){
		m_pos.x = pos.x * FRAME_BUFFER_W * 0.5f;
		m_pos.y = pos.y * FRAME_BUFFER_H * 0.5f;
	}

	/// <summary>
	/// 位置を取得
	/// </summary>
	/// <returns></returns>
	CVector2 GetPos() {
		return m_pos;
	}

	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率。</param>
	void SetScale(const CVector2& scale) {
		m_scale = scale;
	}

	/// <summary>
	/// 拡大率を取得
	/// </summary>
	/// <returns></returns>
	CVector2 GetScale() {
		return m_scale;
	}

	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rot">回転クォータニオン</param>
	void SetRot(const CQuaternion& rot) {
		m_rot = rot;
	}

	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns></returns>
	CQuaternion GetRot() {
		return m_rot;
	}

    /// <summary>
    /// ピボットを設定(左上が(0,0),右下が(1,1))
    /// </summary>
    void SetPivot(const CVector2& pivot) {
        m_pivot = pivot;
    }

    /// <summary>
    /// ピボットを設定
    /// </summary>
    CVector2 GetPivot() {
        return m_pivot;
    }

	//乗算カラーを設定
	void SetMulColor(const CVector4& color){
		m_sprite.SetMulColor( color );
	}

	//描画の可否を設定
	void SetIsDraw( bool isDraw ){
		m_isDraw = isDraw;
	}

private:
	CVector2 m_pos = CVector2::Zero();            //位置
	CVector2 m_scale = CVector2::One();           //拡大率
	CQuaternion m_rot = CQuaternion::Identity();  //回転
    CVector2 m_pivot = CVector2::Zero();
	bool m_isDraw = true;
	Sprite m_sprite; //Sprite
};