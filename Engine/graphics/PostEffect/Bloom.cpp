#include "stdafx.h"
#include "Bloom.h"


Bloom::Bloom(){}


Bloom::~Bloom(){}

void Bloom::Init(){
    UINT w = FRAME_BUFFER_W;
    UINT h = FRAME_BUFFER_H;

	//�P�x���o�p�^�[�Q�b�g��������
	luminanceTarget.Init( w, h, DXGI_FORMAT_R16G16B16A16_FLOAT );
	//�P�x���o�p�V�F�[�_�[��������
	luminanceShader.Load( "Assets/shader/Bloom.fx", "PSluminance", Shader::EnType::PS );

    //�ŏI�����O�p�^�[�Q�b�g��������
    hurfTarget.Init( w/2, h/2 , FRAME_BUFFER_FORMAT);
	//�����O�p�V�F�[�_�[��������
	hurfShader.Load( "Assets/shader/Bloom.fx", "PSAverage", Shader::EnType::PS );

    //�_�E���T���v�����O�p�^�[�Q�b�g��������
	ID3D11ShaderResourceView* srv = luminanceTarget.GetRenderTargetSRV();
    for( Blur& bl : downSumples ){
		bl.Init( w, h, DXGI_FORMAT_R16G16B16A16_FLOAT );
		bl.SetSource( srv );
		srv = bl.GetResult();
        w /= 2;
        h /= 2;
    }

	//���Z�����u�����h�X�e�[�g�쐬
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

	//���Z�����p�V�F�[�_�[
	lastShader.Load( "Assets/shader/Bloom.fx", "PSPlus", Shader::EnType::PS );
}

void Bloom::ApplyEffect( PostEffect& postEffect ){
	using PS = ID3D11PixelShader;
    ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

	//�P�x���o
	luminanceTarget.SetToContext( dc );
	luminanceTarget.Clear( CVector4( 0, 0, 0, 0 ) );
	postEffect.DrawScreenRect( m_source, (PS*)luminanceShader.GetBody() );

	//�_�E���T���v�����O
	ID3D11ShaderResourceView* downSampleSRVs[downSumpleNum];
	for( int i = 0; i < downSumpleNum; i++){
		downSumples[i].ApplyEffect( postEffect );
		downSampleSRVs[i] = downSumples[i].GetResult();
	}

	//�t���[����2����1�T�C�Y�̃^�[�Q�b�g�Ƀ_�E���T���v�����������̊G�̕��ς��o���B
	hurfTarget.SetToContext( dc );
	hurfTarget.Clear( CVector4( 0, 0, 0, 0 ) );
	postEffect.DrawScreenRect(downSampleSRVs, downSumpleNum, (PS*)hurfShader.GetBody() );

	//�`�����o�͐�ɕς���
	m_target->SetToContext(dc);

	//�u�����h�X�e�[�g���o�b�N�A�b�v
	ID3D11BlendState* beforeBS;
	float facter[4];
	UINT mask;
	dc->OMGetBlendState(&beforeBS, facter, &mask);
	//�u�����h�X�e�[�g��ύX
	dc->OMSetBlendState( blendState, nullptr, 0xffffffff );

	//�ŏI���Z����
	postEffect.DrawScreenRect(hurfTarget.GetRenderTargetSRV() , (PS*)lastShader.GetBody() );

	//�u�����h�X�e�[�g��߂�
	dc->OMSetBlendState( beforeBS, facter, mask );
}
