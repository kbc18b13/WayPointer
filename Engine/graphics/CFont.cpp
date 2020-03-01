#include "stdafx.h"
#include "CFont.h"

static DirectX::SpriteBatch* GetSpriteBatch() {
	static DirectX::SpriteBatch spriteBatch(g_graphicsEngine->GetD3DDeviceContext());
	return &spriteBatch;
}

CFont::CFont(const wchar_t* path) : m_font(g_graphicsEngine->GetD3DDevice(), path){
}

CFont::~CFont() {
}

void CFont::Begine() {
	ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11BlendState* blendState;
	float dummy[4];
	UINT dummy2;
	dc->OMGetBlendState( &blendState, dummy, &dummy2 );
	GetSpriteBatch()->Begin( DirectX::SpriteSortMode_Deferred , blendState);
	blendState->Release();
}

void CFont::End() {
	GetSpriteBatch()->End();
	g_graphicsEngine->ResetRasterizerState();
	g_graphicsEngine->ResetDepthStencilState();
}

void CFont::DrawStr(const wchar_t * str) {
    CVector2 pos = m_pos;

    CVector2 pivot;
    DirectX::XMStoreFloat2(&pivot.vec , m_font.MeasureString(str));
    pivot.x *= m_pivot.x;
    pivot.y *= m_pivot.y;

	m_font.DrawString(GetSpriteBatch(), str, pos, m_color, 0.0f, pivot, m_scale,
					   DirectX::SpriteEffects_None, 0.5f);
}
