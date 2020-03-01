#pragma once

class IGameObject;

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	/// <summary>
	/// 初期化関数。クラスを使用する前に呼ぶこと。
	/// </summary>
	/// <param name="defaultArraySize">初期の優先度ごとの配列の長さ</param>
	/// <param name="defaultPrioSize">初期の最大優先度</param>
	void Init(UINT defaultArraySize, UINT defaultPrioSize);

	/// <summary>
	/// 更新関数。毎フレーム呼ぶこと。
	/// </summary>
	void Update();

	/// <summary>
	/// 自分でメモリ確保したゲームオブジェクトを登録する。
	/// </summary>
	/// <param name="go">ゲームオブジェクトのポインタ</param>
	/// <param name="prio">優先度</param>
	void AddGO(IGameObject* go, UINT prio) {
		m_newGOArray[prio].push_back(go);
	}

	template<class T>
	/// <summary>
	/// ゲームオブジェクトを生成し、登録する。
	/// </summary>
	/// <param name="prio">優先度</param>
	decltype(T::This_is_IGameObject, new T) NewGO(UINT prio) {
		T* goP = new T();
		goP->needDelete = true;
		AddGO(goP, prio);
		return goP;
	}

	template<class T, class... Args>
	/// <summary>
	/// ゲームオブジェクトを生成し、登録する。コンストラクタ引数付き。
	/// </summary>
	/// <param name="prio">優先度</param>
	/// <param name="...args">コンストラクタ引数</param>
	auto NewGO(UINT prio, Args... args) -> decltype(T::This_is_IGameObject, new T(args...)) {
		T* goP = new T(args...);
		goP->needDelete = true;
		AddGO(goP, prio);
		return goP;
	}

	/// <summary>
	/// ゲームオブジェクトを登録解除、NewGOで生成した物のみメモリを開放する。
	/// </summary>
	/// <param name="go"></param>
	void DeleteGO(IGameObject* go);

private:
	std::vector<std::vector<IGameObject*>> m_newGOArray;
	std::vector<std::vector<IGameObject*>> m_goArray;
};

GameObjectManager& GOManager();

/// <summary>
/// 自分でメモリ確保したゲームオブジェクトを登録する。
/// </summary>
/// <param name="go">ゲームオブジェクトのポインタ</param>
/// <param name="prio">優先度</param>
static inline void AddGO(IGameObject* go, UINT prio) {
	GOManager().AddGO(go, prio);
}

template<class T>
/// <summary>
/// ゲームオブジェクトを生成し、登録する。
/// </summary>
/// <param name="prio">優先度</param>
inline decltype(T::This_is_IGameObject, new T) NewGO(UINT prio) {
	return GOManager().NewGO<T>(prio);
}

template<class T, class... Args>
/// <summary>
/// ゲームオブジェクトを生成し、登録する。コンストラクタ引数付き。
/// </summary>
/// <param name="prio">優先度</param>
/// <param name="...args">コンストラクタ引数</param>
inline auto NewGO(UINT prio, Args... args) -> decltype(T::This_is_IGameObject, new T(args...)) {
	return GOManager().NewGO<T, Args...>(prio, args...);
}

/// <summary>
/// ゲームオブジェクトを登録解除、NewGOで生成した物のみメモリを開放する。
/// </summary>
/// <param name="go"></param>
static inline void DeleteGO(IGameObject* go) {
	GOManager().DeleteGO(go);
}