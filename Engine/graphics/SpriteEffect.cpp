#include "stdafx.h"
#include "SpriteEffect.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

SpriteEffect::SpriteEffect() {
	HRESULT result;

	bool shaderResult = m_psShader.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	if (!shaderResult) abort();
	shaderResult = m_vsShader.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
	if (!shaderResult) abort();

	auto dev = g_graphicsEngine->GetD3DDevice();

	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	result = dev->CreateSamplerState(&sampDesc, &m_sampler);
	if (FAILED(result))abort();
}

SpriteEffect::~SpriteEffect() {
	m_tex->Release();
	m_sampler->Release();
}

void __cdecl SpriteEffect::Apply(ID3D11DeviceContext * deviceContext) {
	deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
	deviceContext->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);
	deviceContext->PSSetShaderResources(0, 1, &m_tex);

	deviceContext->IASetInputLayout(m_vsShader.GetInputLayout());
	deviceContext->PSSetSamplers(0, 1, &m_sampler);
}

void SpriteEffect::loadTexture(const wchar_t * texPath) {
	ID3D11Resource* res;
	HRESULT result = DirectX::CreateDDSTextureFromFile(g_graphicsEngine->GetD3DDevice(), texPath, &res, &m_tex);
	if (FAILED(result)) {
		abort();
	}
	res->Release();
}
