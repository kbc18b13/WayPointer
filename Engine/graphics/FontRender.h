#pragma once
#include "graphics/CFont.h"
#include "graphics/IRenderObject.h"

class FontRender : public IRenderObject{
public:
	FontRender();
	~FontRender();

	void SetText( const wchar_t* text ){
		m_text = text;
	}

	void SetPos( const CVector2& pos ){
		m_font.SetPos( pos );
	}

	void Render() override;

	void SetActive(bool active);

private:
	const wchar_t* m_text = L"\0";
	CFont m_font;
	bool m_active = true;
};

