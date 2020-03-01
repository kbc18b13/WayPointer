#pragma once
#include "..\RenderTarget.h"
#include "Blur.h"

class Bloom{
public:
    Bloom();
    ~Bloom();

    void Init();
	void SetSource( ID3D11ShaderResourceView* srv ){
		m_source = srv;
	}
	void SetTarget( RenderTarget* rtg ){
		m_target = rtg;
	}
    void ApplyEffect( PostEffect& postEffect );

private:
	ID3D11ShaderResourceView* m_source; //�u���[���̑Ώ�

	RenderTarget luminanceTarget; //�P�x���o�p�^�[�Q�b�g
	Shader luminanceShader; //�P�x���o�p�V�F�[�_�[

    static constexpr UINT downSumpleNum = 4;
	ID3D11ShaderResourceView* downResources[downSumpleNum]; //�_�E���T���v�����O�\�[�X
	Blur downSumples[downSumpleNum];//�_�E���T���v�����O�p�u���[�I�u�W�F�N�g

    RenderTarget hurfTarget; //�_�E���T���v���̕��ς��t���[���̔����T�C�Y�̃^�[�Q�b�g�ɕ`��
	Shader hurfShader; //����p�̃V�F�[�_�[

	ID3D11BlendState* blendState;
	Shader lastShader; //�Ō�ɉ��Z����������V�F�[�_�[

	RenderTarget* m_target;//�Ώۂ̃^�[�Q�b�g
};

