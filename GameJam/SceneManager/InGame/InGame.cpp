#include "InGame.h"
#include "Timer.h"
#include "../../Utility/InputManager.h"
#include "ScoreManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "../../Nuts/Nuts.h"
#include "DxLib.h"

static int prevTime = 0;

int Cr2 = GetColor(255, 255, 255);
void InGameInit(void)
{
	PlayerInit();
	CameraInit();
	ScoreReset();
	NutsInit();
	TimerInit(30.0f);        //制限時間
	prevTime = GetNowCount(); // deltaTime
}

eSceneType InGameUpdate(void)
{
	// deltaTime
	int nowTime = GetNowCount();
	float deltaTime = (nowTime - prevTime) / 1000.0f;
	prevTime = nowTime;

	PlayerUpdate();
	NutsUpdate();
	Position2D pos = GetPlayerPosition();
	CameraUpdate(pos.x, pos.y);

	//触れたらスコア加算
	int score = NutsCheckCollect(pos.x, pos.y, 16.0f);
	if (score != 0)
	{
		ScoreAdd(score); // 種類別スコア加算（腐りはマイナス）
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

}