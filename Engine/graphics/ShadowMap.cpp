#include "stdafx.h"
#include "ShadowMap.h"
#include "SkinModelRender.h"

ShadowMap::ShadowMap(){}


ShadowMap::~ShadowMap(){}

void ShadowMap::Init( unsigned int w, unsigned int h ){
    m_renderTarget.Init( w, h, DXGI_FORMAT_R32_FLOAT );
    m_vpMatCB.Init( sizeof( CMatrix ), false );
}

void ShadowMap::UpdateLight( const CVector3 & pos, const CVector3 & dir ){
    m_lightViewMatrix = DirectX::XMMatrixLookToLH( pos, dir, CVector3::Up() );
    m_lightProjMatrix = DirectX::XMMatrixOrthographicLH( 800, 800, 10, 5000 );
}

void ShadowMap::RemoveShadowCaster( SkinModelRender * render ){
    for( auto shadowItr = m_shadowCasters.begin(); shadowItr != m_shadowCasters.end(); shadowItr++ ){
        if( render == *shadowItr ){
            m_shadowCasters.erase( shadowItr );
            return;
        }
    }
}

void ShadowMap::RenderToShadowMap( ID3D11DeviceContext * dc ){
    //�����_�[�^�[�Q�b�g�ƃr���[�|�[�g���X�V
    auto rt = m_renderTarget.GetRenderTargetView();
    dc->OMSetRenderTargets( 1, &rt, m_renderTarget.GetDepthStensilView() );
    dc->RSSetViewports( 1, m_renderTarget.GetViewport() );

    //�V���h�E�}�b�v���N���A�B
    //��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
    m_renderTarget.Clear( CVector4( 1.0f, 1.0f, 1.0f, 1.0f ) );

    //�V���h�E�L���X�^�[���V���h�E�}�b�v�Ƀ����_�����O�B
    for( auto& caster : m_shadowCasters ){
        caster->Render( EnRenderMode::ShadowMap, m_lightViewMatrix, m_lightProjMatrix );
    }

    CMatrix mat;
    mat.Mul( m_lightViewMatrix, m_lightProjMatrix );
    m_vpMatCB.UpdateData( &mat );
    m_vpMatCB.SetToContext( ShaderType::VS, 6 );
}
