/*!
 * @brief	BulletPhysicsデバッグ用ワイヤーフレームシェーダー。
 */

cbuffer ConstBuffer : register(b0) {
	float4x4 vpMat;
}

struct VSInput {
	float3 pos : POS;
	float3 col : COLOR;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float3 col : COLOR;
};

PSInput VSMain(VSInput input){
	PSInput output;
	output.pos = float4(input.pos, 1);
	output.pos = mul(vpMat, output.pos);
	output.col = input.col;
	return output;
}

float4 PSMain(PSInput input) : SV_Target{
	return float4(input.col, 1);
}