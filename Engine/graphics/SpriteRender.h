#pragma once
#include "Sprite.h"

class SpriteRender : public IRenderObject {
public:

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render() override;

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="path">dds�̃t�@�C���p�X</param>
	void Init(const wchar_t* path, UINT width, UINT height);

	/// <summary>
	/// �ʒu��ݒ�
	/// </summary>
	/// <param name="pos">�ʒu�B�s�N�Z���P�ʁB</param>
	void SetPosPixel(const CVector2& pos) {
		m_pos = pos;
	}

	//���K�����W�n�ňʒu��ݒ�B
	void SetPosNormalized( const CVector2& pos ){
		m_pos.x = pos.x * FRAME_BUFFER_W * 0.5f;
		m_pos.y = pos.y * FRAME_BUFFER_H * 0.5f;
	}

	/// <summary>
	/// �ʒu���擾
	/// </summary>
	/// <returns></returns>
	CVector2 GetPos() {
		return m_pos;
	}

	/// <summary>
	/// �g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦�B</param>
	void SetScale(const CVector2& scale) {
		m_scale = scale;
	}

	/// <summary>
	/// �g�嗦���擾
	/// </summary>
	/// <returns></returns>
	CVector2 GetScale() {
		return m_scale;
	}

	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rot">��]�N�H�[�^�j�I��</param>
	void SetRot(const CQuaternion& rot) {
		m_rot = rot;
	}

	/// <summary>
	/// ��]���擾
	/// </summary>
	/// <returns></returns>
	CQuaternion GetRot() {
		return m_rot;
	}

    /// <summary>
    /// �s�{�b�g��ݒ�(���オ(0,0),�E����(1,1))
    /// </summary>
    void SetPivot(const CVector2& pivot) {
        m_pivot = pivot;
    }

    /// <summary>
    /// �s�{�b�g��ݒ�
    /// </summary>
    CVector2 GetPivot() {
        return m_pivot;
    }

	//��Z�J���[��ݒ�
	void SetMulColor(const CVector4& color){
		m_sprite.SetMulColor( color );
	}

	//�`��̉ۂ�ݒ�
	void SetIsDraw( bool isDraw ){
		m_isDraw = isDraw;
	}

private:
	CVector2 m_pos = CVector2::Zero();            //�ʒu
	CVector2 m_scale = CVector2::One();           //�g�嗦
	CQuaternion m_rot = CQuaternion::Identity();  //��]
    CVector2 m_pivot = CVector2::Zero();
	bool m_isDraw = true;
	Sprite m_sprite; //Sprite
};