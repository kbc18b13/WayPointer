#pragma once
#include "light/DirectionLight.h"
#include "light/AmbientLight.h"

//��ɕ`��̊J�n�ƏI���������ǂ�I�u�W�F�N�g�ɂ�����
class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	//�������B
	void Init(HWND hWnd);

	//����B
	void Release();

    //�`��J�n�B
    void BegineRender();

    //�`��I���B
    void EndRender();

    //�t���[���o�b�t�@�ւ̕`��O�����B
    void BegineFrameBuffer();

	//���X�^���C�U�X�e�[�g�����Z�b�g
	void ResetRasterizerState(){
		m_pd3dDeviceContext->RSSetState( m_rasterizerState );
	}

	//�f�v�X�X�e���V���X�e�[�g�����Z�b�g
	void ResetDepthStencilState(){
		m_pd3dDeviceContext->OMSetDepthStencilState( m_depthStencilState, 0 );
	}

	//�u�����h�X�e�[�g�����Z�b�g
	void ResetBlendState(){
		m_pd3dDeviceContext->OMSetBlendState( m_alphaBlend, nullptr, 0xffffffff);
	}

	//D3D11�f�o�C�X���擾�B
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}

	//D3D11�f�o�C�X�R���e�L�X�g���擾�B
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}

    //�f�B���N�V�������C�g�B
	DirectionLight& GetDirectionLight() {
		return m_dirLight;
	}

	AmbientLight& GetAmbientLight() {
		return m_ambientLight;
	}

	HWND GetWindowHundle(){
		return m_windowHundle;
	}

private:
	DirectionLight m_dirLight;
	AmbientLight m_ambientLight;

	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11�f�o�C�X�B
	IDXGISwapChain*			m_pSwapChain = NULL;		//�X���b�v�`�F�C���B
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11�f�o�C�X�R���e�L�X�g�B
	ID3D11RenderTargetView* m_backBuffer = NULL;		//�o�b�N�o�b�t�@�B
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
	ID3D11Texture2D*		m_depthStencil = NULL;		//�f�v�X�X�e���V���B
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//�f�v�X�X�e���V���r���[�B
	ID3D11DepthStencilState* m_depthStencilState = NULL;//�f�v�X�X�e���V���X�e�[�g
	ID3D11BlendState* m_alphaBlend = NULL;
    D3D11_VIEWPORT m_viewport;

	HWND m_windowHundle;
};

extern GraphicsEngine* g_graphicsEngine;			//�O���t�B�b�N�X�G���W��