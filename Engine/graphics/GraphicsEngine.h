#pragma once
#include "light/DirectionLight.h"
#include "light/AmbientLight.h"

//主に描画の開始と終了をつかさどるオブジェクトにしたい
class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	//初期化。
	void Init(HWND hWnd);

	//解放。
	void Release();

    //描画開始。
    void BegineRender();

    //描画終了。
    void EndRender();

    //フレームバッファへの描画前準備。
    void BegineFrameBuffer();

	//ラスタライザステートをリセット
	void ResetRasterizerState(){
		m_pd3dDeviceContext->RSSetState( m_rasterizerState );
	}

	//デプスステンシルステートをリセット
	void ResetDepthStencilState(){
		m_pd3dDeviceContext->OMSetDepthStencilState( m_depthStencilState, 0 );
	}

	//ブレンドステートをリセット
	void ResetBlendState(){
		m_pd3dDeviceContext->OMSetBlendState( m_alphaBlend, nullptr, 0xffffffff);
	}

	//D3D11デバイスを取得。
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}

	//D3D11デバイスコンテキストを取得。
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}

    //ディレクションライト。
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

	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;		//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;		//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。
	ID3D11DepthStencilState* m_depthStencilState = NULL;//デプスステンシルステート
	ID3D11BlendState* m_alphaBlend = NULL;
    D3D11_VIEWPORT m_viewport;

	HWND m_windowHundle;
};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン