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

    //初期化。
    void Init();

    //描画。
    void Render();

    //普通の描画オブジェクトの登録
    void AddDefaultRender( IRenderObject* ro ){
        m_defaultRender.AddRenderObject( ro );
    }

	//半透明オブジェクトの登録
	void AddTranslucentRender( IRenderObject* ro ){
		m_translucentRender.AddRenderObject( ro );
	}

    //シャドウキャスターを登録
    void AddShadowCaster( SkinModelRender* modelRender ){
        m_shadowMap.AddShadowCaster( modelRender );
    }

    void RemoveShadowCaster( SkinModelRender* modelRender ){
        m_shadowMap.RemoveShadowCaster( modelRender );
    }

    //HUD描画オブジェクトの登録
    void AddHUDRender( IRenderObject* ro ){
        m_HUDRender.AddRenderObject( ro );
    }

	//フェード描画オブジェクトの登録
	void SetFadeRender( IRenderObject* ro ){
		m_fadeRender = ro;
	}

    //シャドウマップを取得
    ShadowMap& GetShadowMap(){
        return m_shadowMap;
    }

private:
    //描画先
    RenderTarget m_defaultTarget;

    //シャドウマップ
    ShadowMap m_shadowMap;

    //描画オブジェクト
    RenderMachine m_defaultRender;
	RenderMachine m_translucentRender;
    RenderMachine m_HUDRender;

	IRenderObject* m_fadeRender = nullptr;

    //ポストエフェクト
    PostEffect m_postEffect;
    Shader m_monoShader;
	Bloom m_bloom;

	ID3D11DepthStencilState* m_noDepth; //深度無し深度ステンシルステート
	ID3D11BlendState* m_alphaBlend;
};

