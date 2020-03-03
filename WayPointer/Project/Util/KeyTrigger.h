#pragma once
class KeyTrigger{
public:
	KeyTrigger(int keycode) : keycode(keycode){}

	void Update(){
		bool press = GetAsyncKeyState( keycode );
		isTrigger = !isPress && press;
		isPress = press;
	}

	bool IsPress(){
		return isPress;
	}

	bool IsTrigger(){
		return isTrigger;
	}

private:
	bool isPress = false;
	bool isTrigger = false;
	int keycode;
};

