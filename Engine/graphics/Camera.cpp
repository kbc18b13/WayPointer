#include "stdafx.h"
#include "Camera.h"

Camera g_camera3D;		//3D�J�����B

void Camera::Init() {
	m_cbuffer.Init(sizeof(CVector3) + 4, false, &m_position);
	m_cbuffer.SetToContext(ShaderType::PS, 3);
}

void Camera::Update()
{
	//�r���[�s����v�Z�B
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	//�v���W�F�N�V�����s����v�Z�B
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//��p�B
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
		m_near,
		m_far
	);
	m_cbuffer.UpdateData(&m_position);
}

CVector4 Camera::GetProjectedPos(const CVector3& pos3 ){
	CVector4 pos4 = CVector4( pos3.x, pos3.y, pos3.z, 1 );
	m_viewMatrix.Mul( pos4 );
	m_projMatrix.Mul( pos4 );
	pos4.x = pos4.x / pos4.w;
	pos4.y = pos4.y / pos4.w;
	pos4.z = pos4.z / pos4.w;
	return pos4;
}
