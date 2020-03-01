#pragma once
#include "DirectXTK/Inc/SpriteFont.h"

class CFont {
public:
	CFont(const wchar_t* path);
	~CFont();

	void Begine();
	void End();
	void DrawStr(const wchar_t* str);

	void SetColor( const CVector4& c ){
		m_color = c;
	}

    void SetPivot(const CVector2& piv) {
        m_pivot = piv;
    }

    void SetPos(const CVector2& pos) {
        m_pos = pos;
    }

	void SetScale( const CVector2& scale ){
		m_scale = scale;
	}

private:
    CVector2 m_pos;
    CVector2 m_pivot;
	CVector2 m_scale = {1,1};

	CVector4 m_color = {1,1,1,1};

	DirectX::SpriteFont m_font;
};