/*!
 * @brief	�A�j���[�V�����Đ��R���g���[���B
 */

#include "stdafx.h"
#include "graphics/animation/AnimationPlayController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"


	
	
void AnimationPlayController::Init(Skeleton* skeleton)
{
	int numBones = skeleton->GetNumBones();
	//�{�[���s����o�V�b�Ɗm�ہB
	m_boneMatrix.resize(numBones);
}
	
void AnimationPlayController::NormalStartLoop()
{
	m_currentKeyFrameNo = 0;
	m_time = 0.0f;
	m_nextEventNumber = 0;
}

void AnimationPlayController::NormalUpdate(float deltaTime, Animation* animation)
{
	if(m_animationClip == nullptr){	
		return ;
	}
	const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();
	m_time += deltaTime;

	//�⊮���Ԃ��i�߂Ă����B
	m_interpolateTime = std::min( m_interpolateEndTime, m_interpolateTime + deltaTime);
	while (true) {
		if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
			//�I�[�܂ōs�����B
			if (m_animationClip->IsLoop()) {
				//���[�v�B
				NormalStartLoop();
			}
			else {
				//�����V���b�g�Đ��B
				m_currentKeyFrameNo--;
				m_isPlaying = false;	//�Đ��I���B
			}
			break;
		}
		if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
			//�I���B
			break;
		}
		//���ցB
		m_currentKeyFrameNo++;
	}
	//�{�[���s����v�Z���Ă����B
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for (const auto& keyFrameList : keyFramePtrListArray) {
		if (keyFrameList.size() == 0) {
			continue;
		}
		//���ݍĐ����̃L�[�t���[��������Ă���B
		Keyframe* keyframe = keyFrameList.at(m_currentKeyFrameNo);
		if (keyframe->boneIndex < m_boneMatrix.size()) {
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		}
		else {
#ifdef _DEBUG			
			MessageBox(NULL, "AnimationPlayController::Update : ���݂��Ȃ��{�[���ɒl�������������Ƃ��Ă��܂��B���̂悤�Ȍ������l�����܂��B\n"
				"�@ tka�t�@�C�����o�͂��鎞�ɁA�I���������[�g�{�[�����X�P���g���̃��[�g�{�[���ƈقȂ��Ă���B\n"
				"�A �قȂ�X�P���g���̃A�j���[�V�����N���b�v���g���Ă���B\n"
				"������xtka�t�@�C�����o�͂��Ȃ����Ă��������B", "error", MB_OK);
			std::abort();
#endif
		}
	}

	//�A�j���[�V�����C�x���g�̏���
	if (m_animationClip->GetEventSize() > m_nextEventNumber) {
		const AnimationEventData& nowEvent = m_animationClip->GetEvent(m_nextEventNumber);
		if (nowEvent.invokeTime < m_time) {
			for (auto ev : animation->GetEventList()) {
				if (strcmp(ev.name, nowEvent.eventName) == 0) {
					ev.func();
				}
			}
			m_nextEventNumber++;
		}
	}
}

void AnimationPlayController::ReverseStartLoop(){
	m_currentKeyFrameNo = m_animationClip->GetKeySize() - 1;
	m_time = m_animationClip->GetLength();
	m_nextEventNumber = m_animationClip->GetEventSize() - 1;
}

void AnimationPlayController::ReverseUpdate( float deltaTime, Animation * animation ){
	if( m_animationClip == nullptr ){
		return;
	}
	const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();
	m_time -= deltaTime;

	//�⊮���Ԃ��i�߂Ă����B
	m_interpolateTime = std::min( m_interpolateEndTime, m_interpolateTime + deltaTime );
	while( true ){
		if( m_currentKeyFrameNo < 0 ){
			//�I�[�܂ōs�����B
			if( m_animationClip->IsLoop() ){
				//���[�v�B
				ReverseStartLoop();
			} else{
				//�����V���b�g�Đ��B
				m_currentKeyFrameNo++;
				m_isPlaying = false;	//�Đ��I���B
			}
			break;
		}
		if(m_currentKeyFrameNo != 0 && topBoneKeyFrameList.at( m_currentKeyFrameNo - 1 )->time < m_time ){
			//�I���B
			break;
		}
		//���ցB
		m_currentKeyFrameNo--;
	}
	//�{�[���s����v�Z���Ă����B
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for( const auto& keyFrameList : keyFramePtrListArray ){
		if( keyFrameList.size() == 0 ){
			continue;
		}
		//���ݍĐ����̃L�[�t���[��������Ă���B
		Keyframe* keyframe = keyFrameList.at( m_currentKeyFrameNo );
		if( keyframe->boneIndex < m_boneMatrix.size() ){
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		} else{
#ifdef _DEBUG			
			MessageBox( NULL, "AnimationPlayController::Update : ���݂��Ȃ��{�[���ɒl�������������Ƃ��Ă��܂��B���̂悤�Ȍ������l�����܂��B\n"
						"�@ tka�t�@�C�����o�͂��鎞�ɁA�I���������[�g�{�[�����X�P���g���̃��[�g�{�[���ƈقȂ��Ă���B\n"
						"�A �قȂ�X�P���g���̃A�j���[�V�����N���b�v���g���Ă���B\n"
						"������xtka�t�@�C�����o�͂��Ȃ����Ă��������B", "error", MB_OK );
			std::abort();
#endif
		}
	}

	//�A�j���[�V�����C�x���g�̏���
	if( 0 <= m_nextEventNumber ){
		const AnimationEventData& nowEvent = m_animationClip->GetEvent( m_nextEventNumber );
		if( nowEvent.invokeTime >= m_time ){
			for( auto ev : animation->GetEventList() ){
				if( strcmp( ev.name, nowEvent.eventName ) == 0 ){
					ev.func();
				}
			}
			m_nextEventNumber--;
		}
	}
}

