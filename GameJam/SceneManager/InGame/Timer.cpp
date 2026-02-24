#include "Timer.h"

// 残り時間（秒）
static float remainingTime = 0.0f;
static float totalTime = 0.0f;

void TimerInit(float limitSeconds)
{
	remainingTime = limitSeconds;
	totalTime = limitSeconds;
}

void TimerUpdate(float deltaTime)
{
	remainingTime -= deltaTime;
	if (remainingTime < 0.0f)
	{
		remainingTime = 0.0f;
	}
}

int TimerGetRemainingTime(void)
{
	return (int)remainingTime;
}

float TimerGetRemainingTimeF(void)
{
	return remainingTime;
}

float TimerGetTotalTime(void)
{
	return totalTime;
}

int TimerIsTimeUp(void)
{
	return (remainingTime <= 0.0f) ? 1 : 0;
}
void TimerAddTime(float add_time)
{
	remainingTime += add_time;

	// タイマーの上限制限
	if (remainingTime > 30.0f) {
		remainingTime = 30.0f;
	 }
}