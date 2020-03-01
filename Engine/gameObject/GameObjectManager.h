#pragma once

class IGameObject;

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	/// <summary>
	/// �������֐��B�N���X���g�p����O�ɌĂԂ��ƁB
	/// </summary>
	/// <param name="defaultArraySize">�����̗D��x���Ƃ̔z��̒���</param>
	/// <param name="defaultPrioSize">�����̍ő�D��x</param>
	void Init(UINT defaultArraySize, UINT defaultPrioSize);

	/// <summary>
	/// �X�V�֐��B���t���[���ĂԂ��ƁB
	/// </summary>
	void Update();

	/// <summary>
	/// �����Ń������m�ۂ����Q�[���I�u�W�F�N�g��o�^����B
	/// </summary>
	/// <param name="go">�Q�[���I�u�W�F�N�g�̃|�C���^</param>
	/// <param name="prio">�D��x</param>
	void AddGO(IGameObject* go, UINT prio) {
		m_newGOArray[prio].push_back(go);
	}

	template<class T>
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�𐶐����A�o�^����B
	/// </summary>
	/// <param name="prio">�D��x</param>
	decltype(T::This_is_IGameObject, new T) NewGO(UINT prio) {
		T* goP = new T();
		goP->needDelete = true;
		AddGO(goP, prio);
		return goP;
	}

	template<class T, class... Args>
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�𐶐����A�o�^����B�R���X�g���N�^�����t���B
	/// </summary>
	/// <param name="prio">�D��x</param>
	/// <param name="...args">�R���X�g���N�^����</param>
	auto NewGO(UINT prio, Args... args) -> decltype(T::This_is_IGameObject, new T(args...)) {
		T* goP = new T(args...);
		goP->needDelete = true;
		AddGO(goP, prio);
		return goP;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g��o�^�����ANewGO�Ő����������̂݃��������J������B
	/// </summary>
	/// <param name="go"></param>
	void DeleteGO(IGameObject* go);

private:
	std::vector<std::vector<IGameObject*>> m_newGOArray;
	std::vector<std::vector<IGameObject*>> m_goArray;
};

GameObjectManager& GOManager();

/// <summary>
/// �����Ń������m�ۂ����Q�[���I�u�W�F�N�g��o�^����B
/// </summary>
/// <param name="go">�Q�[���I�u�W�F�N�g�̃|�C���^</param>
/// <param name="prio">�D��x</param>
static inline void AddGO(IGameObject* go, UINT prio) {
	GOManager().AddGO(go, prio);
}

template<class T>
/// <summary>
/// �Q�[���I�u�W�F�N�g�𐶐����A�o�^����B
/// </summary>
/// <param name="prio">�D��x</param>
inline decltype(T::This_is_IGameObject, new T) NewGO(UINT prio) {
	return GOManager().NewGO<T>(prio);
}

template<class T, class... Args>
/// <summary>
/// �Q�[���I�u�W�F�N�g�𐶐����A�o�^����B�R���X�g���N�^�����t���B
/// </summary>
/// <param name="prio">�D��x</param>
/// <param name="...args">�R���X�g���N�^����</param>
inline auto NewGO(UINT prio, Args... args) -> decltype(T::This_is_IGameObject, new T(args...)) {
	return GOManager().NewGO<T, Args...>(prio, args...);
}

/// <summary>
/// �Q�[���I�u�W�F�N�g��o�^�����ANewGO�Ő����������̂݃��������J������B
/// </summary>
/// <param name="go"></param>
static inline void DeleteGO(IGameObject* go) {
	GOManager().DeleteGO(go);
}