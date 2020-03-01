/*!
 *@brief	アニメーションクリップ。
 */

#pragma once

//////////////////////////////////////////////////////////////////////
// 各種構造体
//////////////////////////////////////////////////////////////////////
/*!
*@brief	アニメーションクリップのヘッダー。
*/
struct AnimClipHeader {
	std::uint32_t numKey;				//!<キーフレームの数。
	std::uint32_t numAnimationEvent;	//!<アニメーションイベントの数。
};
/*!
*@brief	アニメーションイベント
*@アニメーションイベントは未対応。
* やりたかったら自分で実装するように。
*/
struct AnimationEventData {
	float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
	const char* eventName;

	AnimationEventData(float time, char* name) : invokeTime(time), eventName(name) {}

	AnimationEventData( const AnimationEventData& data ) = delete;

	AnimationEventData( AnimationEventData&& data ) noexcept : invokeTime( data.invokeTime ), eventName( data.eventName ){
		data.eventName = nullptr;
	}

	AnimationEventData& operator=( AnimationEventData&& data ) noexcept{
		invokeTime = data.invokeTime;
		eventName = data.eventName;
		data.eventName = nullptr;
		return *this;
	}

	~AnimationEventData() {
		delete[] eventName;
	}
};
/*!
*@brief	キーフレーム。
*/
struct Keyframe {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CMatrix transform;			//!<トランスフォーム。
};
/*!
*@brief	キーフレーム。
*/
struct KeyframeRow {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CVector3 transform[4];		//!<トランスフォーム。
};

/*!
*@brief	アニメーションクリップ。
*/
class AnimationClip  {
public:
	//タイプ量が長ったらしくて、うざいのでstd::vector<KeyFrame*>の別名定義。
	using keyFramePtrList = std::vector<Keyframe*>;
	/*!
	* @brief	コンストラクタ
	*/
	AnimationClip() 
	{
	}
	/*!
	*@brief	デストラクタ。
	*/
	~AnimationClip();
	/*!
	*@brief	アニメーションクリップをロード。
	*@param[in]	filePath	ファイルパス。
	*/
	void Load(const wchar_t* filePath, bool isLoop = false);

	/*!
	*@brief	ループする？
	*/
	bool IsLoop() const
	{
		return m_isLoop;
	}
	/*!
	*@brief	ループフラグを設定する。
	*/
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	/*!
	*@brief
	*/
	const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
	{
		return m_keyFramePtrListArray;
	}
	const keyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFramList;
	}

	size_t GetKeySize() const{
		return m_topBoneKeyFramList->size();
	}

	float GetLength() const{
		return m_topBoneKeyFramList->back()->time;
	}

	int GetEventSize() const{
		return m_eventList.size();
	}

	const AnimationEventData& GetEvent(int index) const {
		return m_eventList[index];
	}
private:
	
	bool m_isLoop = false;									//!<ループフラグ。
	std::vector<Keyframe*> m_keyframes;						//全てのキーフレーム。
	std::vector<keyFramePtrList> m_keyFramePtrListArray;	//ボーンごとのキーフレームのリストを管理するための配列。
															//例えば、m_keyFramePtrListArray[0]は0番目のボーンのキーフレームのリスト、
															//m_keyFramePtrListArray[1]は1番目のボーンのキーフレームのリストといった感じ。
	keyFramePtrList* m_topBoneKeyFramList = nullptr;

	std::vector<AnimationEventData> m_eventList;
};

