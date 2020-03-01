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
    /// シャドウマップに描画。
    /// </summary>
    void RenderToShadowMap(ID3D11DeviceContext* dc);

    /// <summary>
    /// シャドウマップのSRVを取得。
    /// </summary>
    /// <returns>シャドウマップのSRV</returns>
    ID3D11ShaderResourceView* GetShadowMapSRV() {
        return m_renderTarget.GetRenderTargetSRV();
    }

private:

    CVector3 m_lightCameraPosition = CVector3::Zero();	//ライトカメラの視点。
    CVector3 m_lightCameraTarget = CVector3::Zero();	//ライトカメラの注視点。
    CMatrix m_lightViewMatrix = CMatrix::Identity();	//ライトビュー行列。
    CMatrix m_lightProjMatrix = CMatrix::Identity();	//ライトプロジェクション行列。

    RenderTarget m_renderTarget;

    ConstantBuffer m_vpMatCB;

    std::vector<SkinModelRender*> m_shadowCasters;
};

