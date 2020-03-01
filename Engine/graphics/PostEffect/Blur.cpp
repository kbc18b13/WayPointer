#include "stdafx.h"
#include "Blur.h"


Blur::Blur(){}


Blur::~Blur(){}

void Blur::Init( UINT sourceWidth, UINT sourceHeight, DXGI_FORMAT format ){
	//�����_�[�^�[�Q�b�g��������
	m_widthBlurTarget.Init( sourceWidth / 2, sourceHeight, format );
	m_lastBlurTarget.Init( sourceWidth / 2, sourceHeight / 2, format );

	//�V�F�[�_�[��������
	m_vShaderX.Load( "Assets/shader/Blur.fx", "VSBlurX", Shader::EnType::VS );
	m_vShaderY.Load( "Assets/shader/Blur.fx", "VSBlurY", Shader::EnType::VS );
	m_pShader.Load( "Assets/shader/Blur.fx", "PSBlur", Shader::EnType::PS );

	CBData data;
	data.width = sourceWidth;
	data.height = sourceHeight;

	//�K�E�X�֐��ŃE�F�C�g���v�Z
	static constexpr float a = 1.8f; //���U�B�傫���قǂ܂�ׂ�Ȃ��A�������قǒ��S�ɏW�܂�B
	float sum = 0;
	for( int i = 0; i < blurRange; i++ ){
		data.weights[i] = expf( -( i * i ) / ( 2 * a * a ) );
		sum += data.weights[i];
	}
	//�E�F�C�g�̍��v��1�ɂ��邽�߂ɍ��v�Ŋ���
	//�E�F�C�g�͐������ƕ�������2��g���邽�߂���ɔ����ɂ���B
	for( int i = 0; i < blurRange; i++ ){
		data.weights[i] /= sum * 2;
	}

	//�萔�o�b�t�@��������
	m_weightCB.Init( Util::AlignSize( sizeof( data ), 16 ), false, &data );
}

void Blur::ApplyEffect(PostEffect& postEffect ){
	ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

	using VS = ID3D11VertexShader;
	using PS = ID3D11PixelShader;

	m_weightCB.SetToContext( ShaderType::VS, 0 );
	m_weightCB.SetToContext( ShaderType::PS, 0 );

	//���u���[
	m_widthBlurTarget.SetToContext( dc );
	m_widthBlurTarget.Clear( CVector4( 0, 0, 0, 1 ) );
	postEffect.DrawScreenRect( m_source, (PS*)m_pShader.GetBody(), (VS*)m_vShaderX.GetBody() );

	//�c�u���[
	m_lastBlurTarget.SetToContext( dc );
	m_lastBlurTarget.Clear( CVector4( 0, 0, 0, 1 ) );
	postEffect.DrawScreenRect( m_widthBlurTarget.GetRenderTargetSRV()
							   , (PS*)m_pShader.GetBody(), (VS*)m_vShaderY.GetBody() );

}
