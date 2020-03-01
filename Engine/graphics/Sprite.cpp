#include "stdafx.h"
#include "Sprite.h"

/// <summary>
/// Spriteの頂点
/// </summary>
struct SpriteVertex{
	CVector3 pos;
	CVector2 uv;
};

Sprite::Sprite(){}


Sprite::~Sprite(){
	m_vertex->Release();
}

void Sprite::Init( const wchar_t * path, UINT width, UINT height ){
	HRESULT result;

	m_width = width;
	m_height = height;

	int w = width / 2;
	int h = height / 2;

	SpriteVertex vertexData[4]{
		{CVector3( w, h, 0.0f ),CVector2( 1,0 ) },
		{CVector3( -w, h, 0.0f ),CVector2( 0,0 ) },
		{CVector3( w,-h, 0.0f ),CVector2( 1,1 ) },
		{CVector3( -w,-h, 0.0f ),CVector2( 0,1 ) },
	};

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof( vertexData );
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertexData;


	result = g_graphicsEngine->GetD3DDevice()->CreateBuffer( &desc, &data, &m_vertex );

	if( FAILED( result ) ){
		abort();
	}

	m_effect.loadTexture( path );

	if( FAILED( result ) ){
		abort();
	}

	m_CBStruct.worldMat.MakeScaling( { 2 / FRAME_BUFFER_W, 2 / FRAME_BUFFER_H, 1 } );
	m_CBStruct.worldMat.m[3][2] = 0.5f;
	m_CBStruct.mulColor = CVector4( 1, 1, 1, 1 );
	m_CBuf.Init( Util::AlignSize(sizeof( m_CBStruct ), 16), false, &m_CBStruct );
}

void Sprite::Draw(){
	UINT zero = 0;
	UINT stride = sizeof( SpriteVertex );
	g_graphicsEngine->ResetDepthStencilState();
	ID3D11DeviceContext* context = g_graphicsEngine->GetD3DDeviceContext();
	m_effect.Apply( context );
	context->IASetVertexBuffers( 0, 1, &m_vertex, &stride, &zero );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	m_CBuf.SetToContext( ShaderType::VS, 0 );
	m_CBuf.SetToContext( ShaderType::PS, 0 );
	context->Draw( 4, 0 );
}

void Sprite::UpdateWorldMatrix( CVector2 pos, const CVector2 & scale,
								const CQuaternion & rot, CVector2 pivot, float depth){
	//ピボット計算
	pivot.x *= GetWidth() * scale.x;
	pivot.y *= GetHeight() * scale.y;
	pos -= pivot;

	//ワールド行列作成
	CMatrix scaleMat;
	scaleMat.MakeScaling( { scale.x, scale.y, 1 } );
	CMatrix rotMat;
	rotMat.MakeRotationFromQuaternion( rot );
	CMatrix posMat;
	posMat.MakeTranslation( { pos.x, pos.y, depth } );

	m_CBStruct.worldMat.Mul( scaleMat, rotMat );
	m_CBStruct.worldMat.Mul( m_CBStruct.worldMat, posMat );

	CMatrix projMat;//x -1 ～ 1, y -1 ～ 1の2*2XY平面に収めるための行列
	projMat.MakeScaling( { 2 / FRAME_BUFFER_W, 2 / FRAME_BUFFER_H, 1 } );

	m_CBStruct.worldMat.Mul( m_CBStruct.worldMat, projMat );

	m_CBuf.UpdateData( &m_CBStruct );
}
