#include "stdafx.h"
#include "PostEffect.h"

PostEffect::PostEffect() {
}


PostEffect::~PostEffect() {
}

void PostEffect::Init() {
    D3D11_BUFFER_DESC desc{};
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = sizeof(float) * (2 + 2) * 4;
    desc.CPUAccessFlags = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;

    struct Vertex{
        float pos[2];
        float uv[2];
    };

    Vertex vArray[4]{
        {{ 1, 1} , {1, 0}},//‰Eã
        {{-1, 1} , {0, 0}},//¶ã
        {{ 1,-1} , {1, 1}},//‰E‰º
        {{-1,-1} , {0, 1}},//¶‰º
    };

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = vArray;

    g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, &data, &vertexBuf);

    vsShader.Load("Assets/shader/PostEffect.fx", "VSMain", Shader::EnType::VS);
}

void PostEffect::DrawScreenRect( ID3D11ShaderResourceView ** srvArray,
								 UINT srvArrayNum,
								 ID3D11PixelShader* pixelShader,
                                 ID3D11VertexShader* vertexShader) {
    ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();
    UINT stride = sizeof(float) * (2 + 2);
    UINT zero = 0;

	dc->PSSetShaderResources( 0, srvArrayNum, srvArray );
    dc->PSSetShader(pixelShader, nullptr, 0);

    if( vertexShader == nullptr ){
        vertexShader = (ID3D11VertexShader*)vsShader.GetBody();
    }
    dc->VSSetShader( vertexShader, nullptr, 0);

    dc->IASetInputLayout(vsShader.GetInputLayout());
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    dc->IASetVertexBuffers(0, 1, &vertexBuf, &stride, &zero);

    dc->Draw(4, 0);
}