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

    //�|�X�g�G�t�F�N�g�O�ɏ������ރ����_�����O�^�[�Q�b�g
    m_defaultTarget.Init( FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);

    //���m�N���V�F�[�_�[
    m_monoShader.Load( "Assets/shader/PostEffect.fx", "PSDefault", Shader::EnType::PS );

	m_bloom.Init();
    
	//2D�p�[�x�X�e���V���X�e�[�g
	D3D11_DEPTH_STENCIL_DESC dpDesc{};
	dpDesc.DepthEnable = false;
	dpDesc.StencilEnable = false;
	g_graphicsEngine->GetD3DDevice()->CreateDepthStencilState( &dpDesc, &m_noDepth );

	//�A���t�@�L���u�����h�X�e�[�g
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

    //�V���h�E�}�b�v�̕`��B
    m_shadowMap.RenderToShadowMap( dc );

    //�`����ʏ�^�[�Q�b�g�ցB
    m_defaultTarget.SetToContext( dc );
    m_defaultTarget.Clear(CVector4(0, 54.f/255, 106.f/255, 1));

    //�ʏ�`��I�u�W�F�N�g�̕`��
    m_defaultRender.Render();

	//�������`��I�u�W�F�N�g�̕`��
	m_translucentRender.Render();

	//�G�t�F�N�g�`��
	//g_effectManager->Draw();

    //�������C���[�t���[��
    g_physics.DebugDraw();
	//BulletPhysics�ɕ`���C������u�����h�X�e�[�g�ς����Ă���Ńf�t�H���g�ɖ߂��B
	dc->OMSetBlendState( nullptr, nullptr, 0xffffffff );

	//�u���[��
	m_bloom.SetSource( m_defaultTarget.GetRenderTargetSRV() );
	m_bloom.SetTarget( &m_defaultTarget );
	m_bloom.ApplyEffect( m_postEffect );

	//�`�����t���[���o�b�t�@�ցB
	g_graphicsEngine->BegineFrameBuffer();

    //���C���^�[�Q�b�g���t���[���o�b�t�@��
    m_postEffect.DrawScreenRect(m_defaultTarget.GetRenderTargetSRV() , (ID3D11PixelShader*)m_monoShader.GetBody());

	//�A���t�@�L���u�����h�ɕύX�B
	dc->OMSetBlendState( m_alphaBlend, nullptr, 0xffffffff );
	//�[�x�X�e�[�g�����Z�b�g
	g_graphicsEngine->ResetDepthStencilState();

    //HUD�`��I�u�W�F�N�g�̕`��B
    m_HUDRender.Render();

	if( m_fadeRender )
		m_fadeRender->Render();

	//�[�x�X�e�[�g���f�t�H���g�ɖ߂�
	dc->OMSetDepthStencilState( nullptr, 0 );
	//�u�����h�X�e�[�g���f�t�H���g�ɖ߂��B
	dc->OMSetBlendState( nullptr, nullptr, 0 );
}
