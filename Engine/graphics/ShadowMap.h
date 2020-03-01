#pragma once
#include "RenderTarget.h"
#include "graphics/Parts/ConstantBuffer.h"

class SkinModelRender;

class ShadowMap {
public:
    ShadowMap();
    ~ShadowMap();

    void Init(unsigned int w, unsigned int h);

    void UpdateLight(const CVector3& pos, const CVector3& dir);

    void AddShadowCaster(SkinModelRender* render) {
        m_shadowCasters.push_back(render);
    }
    void RemoveShadowCaster(SkinModelRender* render);


    /// <summary>
    /// �V���h�E�}�b�v�ɕ`��B
    /// </summary>
    void RenderToShadowMap(ID3D11DeviceContext* dc);

    /// <summary>
    /// �V���h�E�}�b�v��SRV���擾�B
    /// </summary>
    /// <returns>�V���h�E�}�b�v��SRV</returns>
    ID3D11ShaderResourceView* GetShadowMapSRV() {
        return m_renderTarget.GetRenderTargetSRV();
    }

private:

    CVector3 m_lightCameraPosition = CVector3::Zero();	//���C�g�J�����̎��_�B
    CVector3 m_lightCameraTarget = CVector3::Zero();	//���C�g�J�����̒����_�B
    CMatrix m_lightViewMatrix = CMatrix::Identity();	//���C�g�r���[�s��B
    CMatrix m_lightProjMatrix = CMatrix::Identity();	//���C�g�v���W�F�N�V�����s��B

    RenderTarget m_renderTarget;

    ConstantBuffer m_vpMatCB;

    std::vector<SkinModelRender*> m_shadowCasters;
};

