#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"
#include "RenderObjectManager.h"

ModelEffect::ModelEffect() {
    m_psShader.Load("Assets/shader/model.fx", "PSMain", Shader::EnType::PS);
    m_psShadow.Load("Assets/shader/shadowMap.fx", "PSMain", Shader::EnType::PS);
}

ModelEffect::~ModelEffect() {
    if (m_albedoTex) {
        m_albedoTex->Release();
    }
}

void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{
    switch (m_renderMode) {
    case EnRenderMode::Default: {
        deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
        deviceContext->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);

        auto shadowMapTex = g_ROManager.GetShadowMap().GetShadowMapSRV();
        deviceContext->PSSetShaderResources(enSkinModelSRVReg_ShadowMap, 1, &shadowMapTex);
        deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
        break;
    }
    case EnRenderMode::ShadowMap:
        deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadow.GetBody(), NULL, 0);
        deviceContext->PSSetShader((ID3D11PixelShader*) m_psShadow.GetBody(), NULL, 0);
        break;
    }
}