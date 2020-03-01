#include "stdafx.h"
#include "RenderObjectManager.h"

//extern Effekseer::Manager* g_effectManager;

RenderObjectManager g_ROManager;

RenderObjectManager::RenderObjectManager(){}


RenderObjectManager::~RenderObjectManager(){
	m_alphaBlend->Release();
	m_noDepth->Release();
}

void RenderObjectManager::Init(){
    m_shadowMap.Init( 2048, 2048 );
    m_postEffect.Init();

    //ポストエフェクト前に書き込むレンダリングターゲット
    m_defaultTarget.Init( FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);

    //モノクロシェーダー
    m_monoShader.Load( "Assets/shader/PostEffect.fx", "PSDefault", Shader::EnType::PS );

	m_bloom.Init();
    
	//2D用深度ステンシルステート
	D3D11_DEPTH_STENCIL_DESC dpDesc{};
	dpDesc.DepthEnable = false;
	dpDesc.StencilEnable = false;
	g_graphicsEngine->GetD3DDevice()->CreateDepthStencilState( &dpDesc, &m_noDepth );

	//アルファ有効ブレンドステート
	D3D11_BLEND_DESC blDesc{};
	auto& r0 = blDesc.RenderTarget[0];
	r0.BlendEnable = true;
	r0.BlendOp = D3D11_BLEND_OP_ADD;
	r0.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	r0.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	r0.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	r0.SrcBlendAlpha = D3D11_BLEND_ZERO;
	r0.DestBlendAlpha = D3D11_BLEND_ZERO;
	r0.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_graphicsEngine->GetD3DDevice()->CreateBlendState( &blDesc, &m_alphaBlend );
}

void RenderObjectManager::Render(){
    ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

    //シャドウマップの描画。
    m_shadowMap.RenderToShadowMap( dc );

    //描画先を通常ターゲットへ。
    m_defaultTarget.SetToContext( dc );
    m_defaultTarget.Clear(CVector4(0, 54.f/255, 106.f/255, 1));

    //通常描画オブジェクトの描画
    m_defaultRender.Render();

	//半透明描画オブジェクトの描画
	m_translucentRender.Render();

	//エフェクト描画
	//g_effectManager->Draw();

    //物理ワイヤーフレーム
    g_physics.DebugDraw();
	//BulletPhysicsに描画を任せたらブレンドステート変えられてたんでデフォルトに戻す。
	dc->OMSetBlendState( nullptr, nullptr, 0xffffffff );

	//ブルーム
	m_bloom.SetSource( m_defaultTarget.GetRenderTargetSRV() );
	m_bloom.SetTarget( &m_defaultTarget );
	m_bloom.ApplyEffect( m_postEffect );

	//描画先をフレームバッファへ。
	g_graphicsEngine->BegineFrameBuffer();

    //メインターゲットをフレームバッファへ
    m_postEffect.DrawScreenRect(m_defaultTarget.GetRenderTargetSRV() , (ID3D11PixelShader*)m_monoShader.GetBody());

	//アルファ有効ブレンドに変更。
	dc->OMSetBlendState( m_alphaBlend, nullptr, 0xffffffff );
	//深度ステートをリセット
	g_graphicsEngine->ResetDepthStencilState();

    //HUD描画オブジェクトの描画。
    m_HUDRender.Render();

	if( m_fadeRender )
		m_fadeRender->Render();

	//深度ステートをデフォルトに戻す
	dc->OMSetDepthStencilState( nullptr, 0 );
	//ブレンドステートをデフォルトに戻す。
	dc->OMSetBlendState( nullptr, nullptr, 0 );
}
