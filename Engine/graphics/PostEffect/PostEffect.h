#pragma once
#include "..\Shader.h"


class PostEffect{
public:
	PostEffect();
	~PostEffect();

	void Init();


	void DrawScreenRect( ID3D11ShaderResourceView * srv,
						 ID3D11PixelShader* pixelShader,
						 ID3D11VertexShader* vertexShader = nullptr ){
		DrawScreenRect( &srv, 1, pixelShader, vertexShader );
	}

	void DrawScreenRect( ID3D11ShaderResourceView** srvArray,
						 UINT srvArrayNum,
						 ID3D11PixelShader* pixelShader,
						 ID3D11VertexShader* vertexShader = nullptr );
private:
	Shader vsShader;
	ID3D11Buffer* vertexBuf;
};

