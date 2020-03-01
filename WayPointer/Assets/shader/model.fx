/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

//シャドウマップ。
Texture2D<float4> shadowMap : register(t2);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
    float3 mEmissionColor;
};


cbuffer PSEye : register(b3) {
	float3 eyePos;
}

/*!
 * @brief	ディレクションライトの定数バッファ。
 */
cbuffer PSCbDir : register(b1) {
	float4 mLightColor[4];
	float4 mLightVec[4];
};

cbuffer PSCbAmb : register(b2) {
	float4 mAmbColor;
};

//シャドウマップ用行列
cbuffer ShadowCamera : register(b6) {
    float4x4 mShadowVP;
}

/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos     : WORLD;
    float4 shadowPos     : SHADOW;
};
/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);

	psInput.worldPos = pos.xyz;

    //シャドウマップUV
    psInput.shadowPos = mul(mShadowVP, pos);

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, float4(In.Normal,0))).xyz;
	psInput.Tangent = normalize(mul(mWorld, float4(In.Tangent,0))).xyz;
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
	}
	psInput.Normal = normalize( mul(skinning, float4(In.Normal,0)) ).xyz;
	psInput.Tangent = normalize( mul(skinning, float4(In.Tangent,0)) ).xyz;
	
	psInput.worldPos = pos.xyz;

    //シャドウマップUV
    psInput.shadowPos = mul(mShadowVP, pos);

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	float4 color = albedoTexture.Sample(Sampler, In.TexCoord);
	float4 sum = float4(0, 0, 0, 0);

	for (int i = 0; i < 4; i++) {
		float3 dir = normalize(mLightVec[i].xyz);

		sum += max(dot(In.Normal, -dir), 0) * mLightColor[i] * color;
		//鏡面反射光
		{
			float3 refVec = dir + 2 * (In.Normal * dot(In.Normal, -dir));
			float3 eyeLine = normalize(In.worldPos - eyePos);
			float specPower = max(dot(refVec, -eyeLine), 0);
			sum += pow(specPower, 10) * (mLightColor[0]*0.5f);
		}
	}
    //環境光
	sum += color * mAmbColor;
    
    //シャドウマップ
    {
        //プロジェクション行列を経た座標をテクスチャ座標に変換する。
        float3 shadowPos2 = In.shadowPos.xyz / In.shadowPos.w;
        shadowPos2.xy *= float2(0.5f, -0.5f);
        shadowPos2.xy += 0.5f;
        //シャドウマップの深度とピクセルの深度を比較する。
        float mapDepth = shadowMap.Sample(Sampler, shadowPos2.xy).r;
        if (0 <= shadowPos2.x && shadowPos2.x <= 1
            && 0 <= shadowPos2.y && shadowPos2.y <= 1
            && mapDepth + 0.0003f < shadowPos2.z) {
            sum.rgb /= 2;
            //sum.rbg = 1;
        }
    }
    
    //自己発光色
    sum.xyz += mEmissionColor;
    
	return sum;
}