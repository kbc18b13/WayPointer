#include "stdafx.h"
#include "FontRender.h"
#include "RenderObjectManager.h"

FontRender::FontRender() : m_font(L"Assets/font/font.spritefont"){
	g_ROManager.AddHUDRender( this );
}

FontRender::~FontRender(){}

void FontRender::Render(){
	if( m_active ){
		m_font.Begine();

		m_font.DrawStr( m_text );

		m_font.End();
	}
}

void FontRender::SetActive( bool active ){
	m_active = active;
}
