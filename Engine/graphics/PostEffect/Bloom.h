#pragma once
#include "..\RenderTarget.h"
#include "Blur.h"

class Bloom{
public:
    Bloom();
    ~Bloom();

    void Init();
	void SetSource( ID3D11ShaderResourceView* srv ){
		m_source = srv;
	}
	void SetTarget( RenderTarget* rtg ){
		m_target = rtg;
	}
    void ApplyEffect( PostEffect& postEffect );

private:
	ID3D11ShaderResourceView* m_source; //ブルームの対象

	RenderTarget luminanceTarget; //輝度抽出用ターゲット
	Shader luminanceShader; //輝度抽出用シェーダー

    static constexpr UINT downSumpleNum = 4;
	ID3D11ShaderResourceView* downResources[downSumpleNum]; //ダウンサンプリングソース
	Blur downSumples[downSumpleNum];//ダウンサンプリング用ブラーオブジェクト

    RenderTarget hurfTarget; //ダウンサンプルの平均をフレームの半分サイズのターゲットに描く
	Shader hurfShader; //それ用のシェーダー

	ID3D11BlendState* blendState;
	Shader lastShader; //最後に加算合成をするシェーダー

	RenderTarget* m_target;//対象のターゲット
};

