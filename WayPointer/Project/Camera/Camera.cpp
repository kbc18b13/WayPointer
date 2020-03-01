#include "stdafx.h"
#include "Camera.h"

void MyCamera::Update(){
	if( GetAsyncKeyState( VK_MBUTTON ) ){
		POINT p;
		GetCursorPos(&p);

		POINT center;
		{
			HWND hWnd = g_graphicsEngine->GetWindowHundle();
			RECT rect;
			GetClientRect( hWnd, &rect );
			center.x = ( rect.left + rect.right ) / 2;
			center.y = ( rect.top + rect.bottom ) / 2;
		}

		CVector2 cursorVec( p.x - center.x, p.y - center.y );

		if( cursorVec.LengthSq() < pow2( 300 ) ){

			CQuaternion rot;
			rot.SetRotationDeg( CVector3::AxisY(), cursorVec.x  * 0.1f);

			CVector3 right = CVector3::AxisY();
			right.Cross( -cameraVec );
			right.Normalize();

			rot.AddRotationDeg( right, -cursorVec.y * 0.1f);

			rot.Multiply( cameraVec );
		}
		SetCursorPos( center.x, center.y );
	}

	CVector3 moveVec;
	{
		if( GetAsyncKeyState( 'A' ) ){
			moveVec.x -= moveSpeed;
		}
		if( GetAsyncKeyState( 'D' ) ){
			moveVec.x += moveSpeed;
		}
		if( GetAsyncKeyState( 'W' ) ){
			moveVec.z += moveSpeed;
		}
		if( GetAsyncKeyState( 'S' ) ){
			moveVec.z -= moveSpeed;
		}
		if( GetAsyncKeyState( VK_SPACE ) ){
			moveVec.y += moveSpeed;
		}
		if( GetAsyncKeyState( VK_SHIFT ) ){
			moveVec.y -= moveSpeed;
		}
	}

	Util::LookRotXZ(g_camera3D.GetFront()).Multiply(moveVec);
	g_camera3D.SetPosition( g_camera3D.GetPosition() + moveVec );
	g_camera3D.SetTarget( g_camera3D.GetPosition() + cameraVec );
	g_camera3D.Update();

	
}