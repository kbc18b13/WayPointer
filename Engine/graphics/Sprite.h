#pragma once
#include "SpriteEffect.h"
#include "Parts/ConstantBuffer.h"

class Sprite{
public:
	Sprite();
	~Sprite();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="path">�e�N�X�`���p�X</param>
	/// <param name="width">��</param>
	/// <param name="hight">����</param>
	void Init( const wchar_t* path, UINT width, UINT height );

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*@param[in]	pivot		���f���̃s�{�b�g�B
	*/
	void UpdateWorldMatrix( CVector2 pos, const CVector2& scale,
							const CQuaternion& rot, CVector2 pivot = CVector2::Zero(), float depth = 0.5f);

	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	mat        ���f���̃��[���h���W
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

	SpriteEffect m_effect;              //�e�N�X�`����V�F�[�_�[�Ȃ�
	ID3D11Buffer* m_vertex;             //���_�o�b�t�@
	CBStruct m_CBStruct;            //�萔�o�b�t�@�\����
	ConstantBuffer m_CBuf;              //�萔�o�b�t�@
	UINT m_width = 0;
	UINT m_height = 0;
};

