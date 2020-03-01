/*!
 * @brief	�A�j���[�V����
 */

#pragma once

#include "graphics/animation/AnimationClip.h"
#include "graphics/animation/AnimationPlayController.h"



class Skeleton;
class SkinModel;

/*!
* @brief	�A�j���[�V�����N���X�B
*/
class Animation{
	struct EventFunc{
		const char* name;
		std::function<void()> func;
		EventFunc( const char* n, std::function<void()> f ) : name( n ), func( f ){}
	};
public:
	Animation();
	~Animation();
	/*!
		*@brief	�������B
		*@param[in]	skinModel		�A�j���[�V����������X�L�����f���B
		*@param[in]	animeClipList	�A�j���[�V�����N���b�v�̔z��B
		*@param[in]	numAnimClip		�A�j���[�V�����N���b�v�̐��B
		*/
	void Init( SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip );


	void AddEventFunc( const char* name, std::function<void()> func ){
		m_eventFuncList.emplace_back( name, func );
	}

	const std::vector<EventFunc>& GetEventList(){
		return m_eventFuncList;
	}

	/*!
	*@brief	�A�j���[�V�����̍Đ��B
	*@param[in]	clipNo	�A�j���[�V�����N���b�v�̔ԍ��BInit�֐��ɓn����animClipList�̕��тƂȂ�B
	*@param[in]	interpolateTime		�⊮����(�P�ʁF�b)
	*/
	void Play( int clipNo, float interpolateTime = 0.0f, bool allowSameClip = false, bool reverse = false ){
		PlayCommon( m_animationClips[clipNo], interpolateTime, allowSameClip , reverse);
	}

	/*!
	* @brief	�A�j���[�V�����̍Đ����H
	*/
	bool IsPlaying() const{
		int lastIndex = GetLastAnimationControllerIndex();
		return m_animationPlayController[lastIndex].IsPlaying();
	}

	/*!
	* @brief	�A�j���[�V������i�߂�B
	*@details
	* �G���W����������Ă΂�܂��B
	* ���[�U�[�͎g�p���Ȃ��ł��������B
	*@param[in]	deltaTime		�A�j���[�V������i�߂鎞��(�P�ʁF�b)�B
	*/
	void Update( float deltaTime );

private:
	void PlayCommon( AnimationClip* nextClip, float interpolateTime, bool sameClip, bool reverse ){

		int index = GetLastAnimationControllerIndex();

		if(!sameClip && m_animationPlayController[index].GetAnimClip() == nextClip ){
			return;
		}
		if( interpolateTime == 0.0f ){
			//�⊮�Ȃ��B
			m_numAnimationPlayController = 1;
		} else{
			//�⊮����B
			m_numAnimationPlayController++;
		}
		index = GetLastAnimationControllerIndex();
		m_animationPlayController[index].ChangeAnimationClip( nextClip , reverse);
		m_animationPlayController[index].SetInterpolateTime( interpolateTime );
		m_interpolateTime = 0.0f;
		m_interpolateTimeEnd = interpolateTime;
	}
	/*!
		* @brief	���[�J���|�[�Y�̍X�V�B
		*/
	void UpdateLocalPose( float deltaTime );
	/*!
		* @brief	�O���[�o���|�[�Y�̍X�V�B
		*/
	void UpdateGlobalPose();

private:

	/*!
		*@brief	�ŏI�|�[�Y�ɂȂ�A�j���[�V�����̃����O�o�b�t�@��ł̃C���f�b�N�X���擾�B
		*/
	int GetLastAnimationControllerIndex() const{
		return GetAnimationControllerIndex( m_startAnimationPlayController, m_numAnimationPlayController - 1 );
	}
	/*!
	*@brief	�A�j���[�V�����R���g���[���̃����O�o�b�t�@��ł̃C���f�b�N�X���擾�B
	*@param[in]	startIndex		�J�n�C���f�b�N�X�B
	*@param[in]	localIndex		���[�J���C���f�b�N�X�B
	*/
	int GetAnimationControllerIndex( int startIndex, int localIndex ) const{
		return ( startIndex + localIndex ) % ANIMATION_PLAY_CONTROLLER_NUM;
	}
private:
	static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;			//!<�A�j���[�V�����R���g���[���̐��B
	std::vector<AnimationClip*>	m_animationClips;					//!<�A�j���[�V�����N���b�v�̔z��B
	Skeleton* m_skeleton = nullptr;	//!<�A�j���[�V������K�p����X�P���g���B
	AnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];	//!<�A�j���[�V�����R���g���[���B�����O�o�b�t�@�B
	int m_numAnimationPlayController = 0;		//!<���ݎg�p���̃A�j���[�V�����Đ��R���g���[���̐��B
	int m_startAnimationPlayController = 0;		//!<�A�j���[�V�����R���g���[���̊J�n�C���f�b�N�X�B
	float m_interpolateTime = 0.0f;
	float m_interpolateTimeEnd = 0.0f;
	bool m_isInterpolate = false;				//!<��Ԓ��H

	std::vector<EventFunc> m_eventFuncList;
};
