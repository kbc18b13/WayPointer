#pragma once
#include "RenderTarget.h"
#include "ShadowMap.h"
#include "RenderMachine.h"
#include "PostEffect/PostEffect.h"
#include "PostEffect/Bloom.h"

class RenderObjectManager;

extern RenderObjectManager g_ROManager;

class RenderObjectManager{
public:
    RenderObjectManager();
    ~RenderObjectManager();

    //�������B
    void Init();

    //�`��B
    void Render();

    //���ʂ̕`��I�u�W�F�N�g�̓o�^
    void AddDefaultRender( IRenderObject* ro ){
        m_defaultRender.AddRenderObject( ro );
    }

	//�������I�u�W�F�N�g�̓o�^
	void AddTranslucentRender( IRenderObject* ro ){
		m_translucentRender.AddRenderObject( ro );
	}

    //�V���h�E�L���X�^�[��o�^
    void AddShadowCaster( SkinModelRender* modelRender ){
        m_shadowMap.AddShadowCaster( modelRender );
    }

    void RemoveShadowCaster( SkinModelRender* modelRender ){
        m_shadowMap.RemoveShadowCaster( modelRender );
    }

    //HUD�`��I�u�W�F�N�g�̓o�^
    void AddHUDRender( IRenderObject* ro ){
        m_HUDRender.AddRenderObject( ro );
    }

	//�t�F�[�h�`��I�u�W�F�N�g�̓o�^
	void SetFadeRender( IRenderObject* ro ){
		m_fadeRender = ro;
	}

    //�V���h�E�}�b�v���擾
    ShadowMap& GetShadowMap(){
        return m_shadowMap;
    }

private:
    //�`���
    RenderTarget m_defaultTarget;

    //�V���h�E�}�b�v
    ShadowMap m_shadowMap;

    //�`��I�u�W�F�N�g
    RenderMachine m_defaultRender;
	RenderMachine m_translucentRender;
    RenderMachine m_HUDRender;

	IRenderObject* m_fadeRender = nullptr;

    //�|�X�g�G�t�F�N�g
    PostEffect m_postEffect;
    Shader m_monoShader;
	Bloom m_bloom;

	ID3D11DepthStencilState* m_noDepth; //�[�x�����[�x�X�e���V���X�e�[�g
	ID3D11BlendState* m_alphaBlend;
};

