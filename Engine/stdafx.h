#pragma once

//MINとMAXのマクロなんて消してしまう
#define NOMINMAX

//標準ライブラリ
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

//サウンド
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//BulletPhysics
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTKのインクルードファイル。
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

//数学系
#include "math/Vector.h"
#include "math/Matrix.h"

#include "physics/Physics.h"

//ユーティリティ
#include "util/Util.h"

//入力
#include "HID/Pad.h"

//グラフィック系
#include "graphics/GraphicsEngine.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"

//Effekseer
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"

//ゲームオブジェクト
#include "gameObject/GameObjectManager.h"
#include "gameObject/IGameObject.h"

//デルタタイム用
#include "gameTimer/GameTime.h"

#include "graphics/SkinModelRender.h"

const float FRAME_BUFFER_W = 1280.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 720.0f;				//フレームバッファの高さ。
const DXGI_FORMAT FRAME_BUFFER_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM; //フレームバッファのフォーマット。

static const int MAX_BONE = 512;	//!<ボーンの最大数。
