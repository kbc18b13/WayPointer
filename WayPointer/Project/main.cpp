#include "stdafx.h"
#include "system/system.h"
#include "graphics/RenderObjectManager.h"
#include "graphics/FontRender.h"
#include "World/World.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){
	//�Q�[���̏������B
	InitGame( hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game" );

	g_camera3D.Init();
	g_camera3D.SetTarget( { 0.0f, 20.0f, 0.0f } );
	g_camera3D.SetFar( 10000.0f );

	//�T�E���h�}�l�[�W���[�̏�����
	g_soundEngine.Init();

	//�����_�[�I�u�W�F�N�g�}�l�[�W���[�̏�����
	g_ROManager.Init();

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̏�����
	GOManager().Init( 10, 5 );

#ifdef _DEBUG
	FontRender fpsRender;
	fpsRender.SetPos( CVector2( 500, 500 ) );
	wchar_t fpsText[20];
	fpsRender.SetText( fpsText );
#endif

	bool debug = false;

	World world;

	//�Q�[�����[�v�B
	while( DispatchWindowMessage() == true ){
		GameTime::UpdateTime();

		//�Q�[���p�b�h�̍X�V�B	
		for( auto& pad : g_pad ){
			pad.Update();
		}

		if( g_pad->IsTrigger( enButtonY ) ){
			debug = !debug;
			g_physics.setDebugDraw( debug );
		}

		//�����G���W���̍X�V�B
		g_physics.Update();

		//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�B
		GOManager().Update();

		world.Update();

#ifdef _DEBUG
		
		swprintf( fpsText, L"FPS:%5f\n", GameTime::GetFPS() );
#endif

		//�`��J�n�B
		g_graphicsEngine->BegineRender();

		//�`��I�u�W�F�N�g�̕`��B
		g_ROManager.Render();

		//�`��I���B
		g_graphicsEngine->EndRender();

		//�T�E���h�G���W���̍X�V�B
		g_soundEngine.Update();
	}
}