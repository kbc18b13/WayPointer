/*!
 * @brief	スプライトシェーダー。
 */

//テクスチャ。
Texture2D<float4> Texture : register(t0);	

// SamplerState
sampler Sampler : register(s0);

cbuffer ConstBuffer : register(b0) {
	float4x4 worldMatrix;
    float4 mulColor;
}

struct VSInput {
	float3 pos : POS;
	float2 uv : UV;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PSInput VSMain(VSInput input) {
	PSInput output;
	float4 pos4 = float4(input.pos, 1);
	output.pos = mul(worldMatrix, pos4);
	output.uv = input.uv;
	return output;
}

float4 PSMain(PSInput input) : SV_Target{
	float4 col = Texture.Sample(Sampler, input.uv) * mulColor;
	return col;
}