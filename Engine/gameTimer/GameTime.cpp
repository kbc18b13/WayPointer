#include "stdafx.h"
#include "GameTime.h"
#include <chrono>
using namespace std::chrono;
namespace GameTime {

static time_point<system_clock> beforeFrame;//前フレームの時刻
static float deltaTime = 1;//前フレームとの差

void UpdateTime() {
	auto now = system_clock::now();
	long long mili = duration_cast<milliseconds>(now - beforeFrame).count();
	deltaTime = mili / 1000.0f;
	beforeFrame = now;
}

float GetFPS(){
	return 1.0f / deltaTime;
}

float GetDeltaTime() {
	return std::min(deltaTime, 1.0f/20.0f);
}

}