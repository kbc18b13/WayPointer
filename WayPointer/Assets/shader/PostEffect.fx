/*!
 * @brief	モノクロポストエフェクトシェーダー。
 */

 //テクスチャ。
Texture2D<float4> Texture : register(t0);
// SamplerState
sampler Sampler : register(s0);

struct VSInput {
    float2 pos : POS;
    float2 uv : UV;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PSInput VSMain(VSInput input) {
    PSInput output;
    output.pos = float4(input.pos, 0.5f,1);
    output.uv = input.uv;
    return output;
}

float4 PSMonochrome(PSInput input) : SV_Target{
    float4 col = Texture.Sample(Sampler, input.uv);
    //0.299*R + 0.587*G + 0.114*B
    float gray = dot(col.rgb, float3(0.299f, 0.587f, 0.114f));
    return float4(gray, gray, gray, 1);
}

float4 PSDefault( PSInput input ) : SV_Target{
    float4 col = Texture.Sample( Sampler, input.uv );
    return col;
}