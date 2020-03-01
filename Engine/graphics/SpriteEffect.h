#pragma once
#include "graphics/Shader.h"

class SpriteEffect : DirectX::IEffect {
public:
	SpriteEffect();
	~SpriteEffect();

	void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;

	void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override {
		*pShaderByteCode = m_vsShader.GetByteCode();
		*pByteCodeLength = m_vsShader.GetByteCodeSize();
	}

	/// <summary>
	/// テクスチャをロード
	/// </summary>
	/// <param name="texPath">テクスチャパス</param>
	void loadTexture(const wchar_t * texPath);

private:
	Shader m_vsShader;
	Shader m_psShader;
	ID3D11ShaderResourceView* m_tex = nullptr;
	ID3D11SamplerState* m_sampler;
};
