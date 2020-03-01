/*!
 * @brief	ブラーシェーダー。
 */

 //テクスチャ。
Texture2D<float4> Texture0: register( t0 );
Texture2D<float4> Texture1: register( t1 );
Texture2D<float4> Texture2: register( t2 );
Texture2D<float4> Texture3: register( t3 );
// SamplerState
sampler Sampler: register( s0 );

//頂点入力
struct PSInput {
    float4 pos : SV_POSITION;
    float2 uv : UV;
};


float4 PSluminance( PSInput input ): SV_Target
{
	float4 color = Texture0.Sample(Sampler, input.uv ) - float4(1, 1, 1, 0);
    color.r = max(0, color.r);
    color.g = max(0, color.g);
    color.b = max(0, color.b);
	return color;
}

float4 PSAverage( PSInput input ): SV_Target
{
	float4 color = float4(0, 0, 0, 0);
    const float dev = 1.0f / 4;
    color += Texture0.Sample(Sampler, input.uv) * dev;
    color += Texture1.Sample(Sampler, input.uv) * dev;
    color += Texture2.Sample(Sampler, input.uv) * dev;
    color += Texture3.Sample(Sampler, input.uv) * dev;
    color.a = 1.0f;
    return color;
}

float4 PSPlus( PSInput input ): SV_Target
{
	return Texture0.Sample(Sampler, input.uv);
}