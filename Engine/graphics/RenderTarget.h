#pragma once
class RenderTarget {
public:
    RenderTarget();
    ~RenderTarget();

    /// <summary>
    /// リソースの解放。デストラクタからも呼ばれる。
    /// </summary>
    void Release();

    /// <summary>
    /// 指定した色でクリア
    /// </summary>
    void Clear(CVector4 Color);

    /// <summary>
    /// 初期化
    /// </summary>
    void Init(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

    void SetToContext(ID3D11DeviceContext* dc);

    /// <summary>
    /// レンダリングターゲットビューを取得。
    /// </summary>
    /// <returns>レンダリングターゲットビュー</returns>
    ID3D11RenderTargetView* GetRenderTargetView() {
        return m_renderTargetView;
    }

    /// <summary>
    /// デプスステンシルビューを取得。
    /// </summary>
    /// <returns>デプスステンシルビュー</returns>
    ID3D11DepthStencilView* GetDepthStensilView() {
        return m_depthStencilView;
    }

    /// <summary>
    /// レンダリングターゲットのSRVを取得。
    /// </summary>
    /// <returns>レンダリングターゲットのSRV</returns>
    ID3D11ShaderResourceView* GetRenderTargetSRV() {
        return m_renderTargetSRV;
    }
    /// <summary>
    /// ビューポートの取得。
    /// </summary>
    /// <returns>ビューポート</returns>
    D3D11_VIEWPORT* GetViewport() {
        return &m_viewport;
    }

private:
    //レンダーターゲット
    ID3D11Texture2D* m_renderTargetTex;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11ShaderResourceView* m_renderTargetSRV;
    D3D11_VIEWPORT m_viewport;

    //深度バッファ
    ID3D11Texture2D* m_depthStencilTex;
    ID3D11DepthStencilView* m_depthStencilView;
};

