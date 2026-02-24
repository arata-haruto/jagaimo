#include "InGame.h"
#include "Timer.h"
#include "../../Utility/InputManager.h"
#include "ScoreManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "../../Nuts/Nuts.h"
#include "../../Ability/Ability.h"
#include "DxLib.h"

static int prevTime = 0;

int Cr2 = GetColor(255, 255, 255);
void InGameInit(void)
{
	PlayerInit();
	CameraInit();
	ScoreReset();
	NutsInit();
	AbilityInit();
	TimerInit(30.0f);        
	prevTime = GetNowCount(); // deltaTime
}

eSceneType InGameUpdate(void)
{
	// deltaTime
	int nowTime = GetNowCount();
	float deltaTime = (nowTime - prevTime) / 1000.0f;
	prevTime = nowTime;

	AbilityUpdate(deltaTime);
	PlayerSetSpeedMultiplier(GetAbilityDashSpeedMul());

	PlayerUpdate();
	NutsUpdate();
	Position2D pos = GetPlayerPosition();
	CameraUpdate(pos.x, pos.y);

	if (IsAbilityMagnetActive())
	{
		NutsMagnetPull(pos.x, pos.y, 200.0f, 5.0f);
	}

	int score = NutsCheckCollect(pos.x, pos.y, 16.0f);
	if (score != 0)
	{
		ScoreAdd(score * GetAbilityScoreMul()); 
	}


	
	TimerUpdate(deltaTime);
	if (TimerIsTimeUp())
	{
		return eResult;
	}

	 return eInGame;
}
void InGameDraw(void)
{
	MapDraw(CameraGetX(), CameraGetY());
	NutsDraw(CameraGetX(), CameraGetY());
	PlayerDraw(CameraGetX(), CameraGetY());

	
	char timeBuf[16];
	sprintf_s(timeBuf, "Time: %d", TimerGetRemainingTime());
	DrawString(10, 50, timeBuf, Cr2);

	char scoreBuf[32];
	sprintf_s(scoreBuf, "Score: %d", ScoreGetTotal());
	DrawString(10, 80, scoreBuf, Cr2);

	AbilityDraw();
}