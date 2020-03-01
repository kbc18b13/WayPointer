#pragma once
class RenderTarget {
public:
    RenderTarget();
    ~RenderTarget();

    /// <summary>
    /// ���\�[�X�̉���B�f�X�g���N�^������Ă΂��B
    /// </summary>
    void Release();

    /// <summary>
    /// �w�肵���F�ŃN���A
    /// </summary>
    void Clear(CVector4 Color);

    /// <summary>
    /// ������
    /// </summary>
    void Init(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

    void SetToContext(ID3D11DeviceContext* dc);

    /// <summary>
    /// �����_�����O�^�[�Q�b�g�r���[���擾�B
    /// </summary>
    /// <returns>�����_�����O�^�[�Q�b�g�r���[</returns>
    ID3D11RenderTargetView* GetRenderTargetView() {
        return m_renderTargetView;
    }

    /// <summary>
    /// �f�v�X�X�e���V���r���[���擾�B
    /// </summary>
    /// <returns>�f�v�X�X�e���V���r���[</returns>
    ID3D11DepthStencilView* GetDepthStensilView() {
        return m_depthStencilView;
    }

    /// <summary>
    /// �����_�����O�^�[�Q�b�g��SRV���擾�B
    /// </summary>
    /// <returns>�����_�����O�^�[�Q�b�g��SRV</returns>
    ID3D11ShaderResourceView* GetRenderTargetSRV() {
        return m_renderTargetSRV;
    }
    /// <summary>
    /// �r���[�|�[�g�̎擾�B
    /// </summary>
    /// <returns>�r���[�|�[�g</returns>
    D3D11_VIEWPORT* GetViewport() {
        return &m_viewport;
    }

private:
    //�����_�[�^�[�Q�b�g
    ID3D11Texture2D* m_renderTargetTex;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11ShaderResourceView* m_renderTargetSRV;
    D3D11_VIEWPORT m_viewport;

    //�[�x�o�b�t�@
    ID3D11Texture2D* m_depthStencilTex;
    ID3D11DepthStencilView* m_depthStencilView;
};

