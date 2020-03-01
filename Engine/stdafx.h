#pragma once

//MIN��MAX�̃}�N���Ȃ�ď����Ă��܂�
#define NOMINMAX

//�W�����C�u����
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>

//�T�E���h
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//BulletPhysics
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTK�̃C���N���[�h�t�@�C���B
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

//���w�n
#include "math/Vector.h"
#include "math/Matrix.h"

#include "physics/Physics.h"

//���[�e�B���e�B
#include "util/Util.h"

//����
#include "HID/Pad.h"

//�O���t�B�b�N�n
#include "graphics/GraphicsEngine.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"

//Effekseer
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"

//�Q�[���I�u�W�F�N�g
#include "gameObject/GameObjectManager.h"
#include "gameObject/IGameObject.h"

//�f���^�^�C���p
#include "gameTimer/GameTime.h"

#include "graphics/SkinModelRender.h"

const float FRAME_BUFFER_W = 1280.0f;				//�t���[���o�b�t�@�̕��B
const float FRAME_BUFFER_H = 720.0f;				//�t���[���o�b�t�@�̍����B
const DXGI_FORMAT FRAME_BUFFER_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM; //�t���[���o�b�t�@�̃t�H�[�}�b�g�B

static const int MAX_BONE = 512;	//!<�{�[���̍ő吔�B
