#pragma once

class GameObjectManager;

class IGameObject {
public:
	static constexpr char This_is_IGameObject = 0; //NewGO��IGameObject�����ʂ���p

	IGameObject();
	virtual ~IGameObject();

    virtual void Start() {};
    virtual void Update() {};

private:
	friend GameObjectManager;
	bool isRemoved = false;
	bool needDelete = false;
};

