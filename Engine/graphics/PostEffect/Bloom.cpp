#include "stdafx.h"
#include "Bloom.h"


Bloom::Bloom(){}


Bloom::~Bloom(){}

void Bloom::Init(){
    UINT w = FRAME_BUFFER_W;
    UINT h = FRAME_BUFFER_H;

	//輝度抽出用ターゲットを初期化
	luminanceTarget.Init( w, h, DXGI_FORMAT_R16G16B16A16_FLOAT );
	//輝度抽出用シェーダーを初期化
	luminanceShader.Load( "Assets/shader/Bloom.fx", "PSluminance", Shader::EnType::PS );

    //最終一歩手前用ターゲットを初期化
    hurfTarget.Init( w/2, h/2 , FRAME_BUFFER_FORMAT);
	//一歩手前用シェーダーを初期化
	hurfShader.Load( "Assets/shader/Bloom.fx", "PSAverage", Shader::EnType::PS );

    //ダウンサンプリング用ターゲットを初期化
	ID3D11ShaderResourceView* srv = luminanceTarget.GetRenderTargetSRV();
    for( Blur& bl : downSumples ){
		bl.Init( w, h, DXGI_FORMAT_R16G16B16A16_FLOAT );
		bl.SetSource( srv );
		srv = bl.GetResult();
        w /= 2;
        h /= 2;
    }

	//加算合成ブレンドステート作成
	D3D11_BLEND_DESC blendDesc{};
	D3D11_RENDER_TARGET_BLEND_DESC& rbd = blendDesc.RenderTarget[0];
	rbd.BlendEnable = true;
	rbd.BlendOp = D3D11_BLEND_OP_ADD;
	rbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rbd.DestBlend = D3D11_BLEND_ONE;
	rbd.DestBlendAlpha = D3D11_BLEND_ONE;
	rbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	rbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rbd.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;

	g_graphicsEngine->GetD3DDevice()->CreateBlendState( &blendDesc, &blendState );

	//加算合成用シェーダー
	lastShader.Load( "Assets/shader/Bloom.fx", "PSPlus", Shader::EnType::PS );
}

void Bloom::ApplyEffect( PostEffect& postEffect ){
	using PS = ID3D11PixelShader;
    ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

	//輝度抽出
	luminanceTarget.SetToContext( dc );
	luminanceTarget.Clear( CVector4( 0, 0, 0, 0 ) );
	postEffect.DrawScreenRect( m_source, (PS*)luminanceShader.GetBody() );

	//ダウンサンプリング
	ID3D11ShaderResourceView* downSampleSRVs[downSumpleNum];
	for( int i = 0; i < downSumpleNum; i++){
		downSumples[i].ApplyEffect( postEffect );
		downSampleSRVs[i] = downSumples[i].GetResult();
	}

	//フレームの2分の1サイズのターゲットにダウンサンプルした複数の絵の平均を出す。
	hurfTarget.SetToContext( dc );
	hurfTarget.Clear( CVector4( 0, 0, 0, 0 ) );
	postEffect.DrawScreenRect(downSampleSRVs, downSumpleNum, (PS*)hurfShader.GetBody() );

	//描画先を出力先に変える
	m_target->SetToContext(dc);

	//ブレンドステートをバックアップ
	ID3D11BlendState* beforeBS;
	float facter[4];
	UINT mask;
	dc->OMGetBlendState(&beforeBS, facter, &mask);
	//ブレンドステートを変更
	dc->OMSetBlendState( blendState, nullptr, 0xffffffff );

	//最終加算合成
	postEffect.DrawScreenRect(hurfTarget.GetRenderTargetSRV() , (PS*)lastShader.GetBody() );

	//ブレンドステートを戻す
	dc->OMSetBlendState( beforeBS, facter, mask );
}
