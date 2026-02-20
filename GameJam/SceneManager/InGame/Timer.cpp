#include "Timer.h"

// 残り時間（秒）
static float remainingTime = 0.0f;

void TimerInit(float limitSeconds)
{
	remainingTime = limitSeconds;
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

int TimerIsTimeUp(void)
{
	return (remainingTime <= 0.0f) ? 1 : 0;
}
