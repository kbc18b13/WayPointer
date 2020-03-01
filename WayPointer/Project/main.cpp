#include "stdafx.h"
#include "system/system.h"
#include "graphics/RenderObjectManager.h"
#include "graphics/FontRender.h"
#include "World/World.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){
	//ゲームの初期化。
	InitGame( hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game" );

	g_camera3D.Init();
	g_camera3D.SetTarget( { 0.0f, 20.0f, 0.0f } );
	g_camera3D.SetFar( 10000.0f );

	//サウンドマネージャーの初期化
	g_soundEngine.Init();

	//レンダーオブジェクトマネージャーの初期化
	g_ROManager.Init();

	//ゲームオブジェクトマネージャーの初期化
	GOManager().Init( 10, 5 );

#ifdef _DEBUG
	FontRender fpsRender;
	fpsRender.SetPos( CVector2( 500, 500 ) );
	wchar_t fpsText[20];
	fpsRender.SetText( fpsText );
#endif

	bool debug = false;

	World world;

	//ゲームループ。
	while( DispatchWindowMessage() == true ){
		GameTime::UpdateTime();

		//ゲームパッドの更新。	
		for( auto& pad : g_pad ){
			pad.Update();
		}

		if( g_pad->IsTrigger( enButtonY ) ){
			debug = !debug;
			g_physics.setDebugDraw( debug );
		}

		//物理エンジンの更新。
		g_physics.Update();

		//ゲームオブジェクトマネージャーの更新。
		GOManager().Update();

		world.Update();

#ifdef _DEBUG
		
		swprintf( fpsText, L"FPS:%5f\n", GameTime::GetFPS() );
#endif

		//描画開始。
		g_graphicsEngine->BegineRender();

		//描画オブジェクトの描画。
		g_ROManager.Render();

		//描画終了。
		g_graphicsEngine->EndRender();

		//サウンドエンジンの更新。
		g_soundEngine.Update();
	}
}