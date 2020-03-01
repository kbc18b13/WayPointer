#include "stdafx.h"
#include "Blur.h"


Blur::Blur(){}


Blur::~Blur(){}

void Blur::Init( UINT sourceWidth, UINT sourceHeight, DXGI_FORMAT format ){
	//レンダーターゲットを初期化
	m_widthBlurTarget.Init( sourceWidth / 2, sourceHeight, format );
	m_lastBlurTarget.Init( sourceWidth / 2, sourceHeight / 2, format );

	//シェーダーを初期化
	m_vShaderX.Load( "Assets/shader/Blur.fx", "VSBlurX", Shader::EnType::VS );
	m_vShaderY.Load( "Assets/shader/Blur.fx", "VSBlurY", Shader::EnType::VS );
	m_pShader.Load( "Assets/shader/Blur.fx", "PSBlur", Shader::EnType::PS );

	CBData data;
	data.width = sourceWidth;
	data.height = sourceHeight;

	//ガウス関数でウェイトを計算
	static constexpr float a = 1.8f; //分散。大きいほどまんべんなく、小さいほど中心に集まる。
	float sum = 0;
	for( int i = 0; i < blurRange; i++ ){
		data.weights[i] = expf( -( i * i ) / ( 2 * a * a ) );
		sum += data.weights[i];
	}
	//ウェイトの合計を1にするために合計で割る
	//ウェイトは正方向と負方向で2回使われるためさらに半分にする。
	for( int i = 0; i < blurRange; i++ ){
		data.weights[i] /= sum * 2;
	}

	//定数バッファを初期化
	m_weightCB.Init( Util::AlignSize( sizeof( data ), 16 ), false, &data );
}

void Blur::ApplyEffect(PostEffect& postEffect ){
	ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

	using VS = ID3D11VertexShader;
	using PS = ID3D11PixelShader;

	m_weightCB.SetToContext( ShaderType::VS, 0 );
	m_weightCB.SetToContext( ShaderType::PS, 0 );

	//横ブラー
	m_widthBlurTarget.SetToContext( dc );
	m_widthBlurTarget.Clear( CVector4( 0, 0, 0, 1 ) );
	postEffect.DrawScreenRect( m_source, (PS*)m_pShader.GetBody(), (VS*)m_vShaderX.GetBody() );

	//縦ブラー
	m_lastBlurTarget.SetToContext( dc );
	m_lastBlurTarget.Clear( CVector4( 0, 0, 0, 1 ) );
	postEffect.DrawScreenRect( m_widthBlurTarget.GetRenderTargetSRV()
							   , (PS*)m_pShader.GetBody(), (VS*)m_vShaderY.GetBody() );

}
