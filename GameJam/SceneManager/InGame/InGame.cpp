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
	TimerInit(10.0f);        // 制限時間
	prevTime = GetNowCount(); // deltaTime
}

eSceneType InGameUpdate(void)
{
	// deltaTime
	int nowTime = GetNowCount();
	float deltaTime = (nowTime - prevTime) / 1000.0f;
	prevTime = nowTime;

	PlayerUpdate();

	Position2D pos = GetPlayerPosition();
	CameraUpdate(pos.x, pos.y);

	// プレイヤーと木の実の当たり判定（プレイヤー半径16px）
	int collected = NutsCheckCollect(pos.x, pos.y, 16.0f);
	if (collected > 0)
	{
		ScoreAdd(collected * 10); // 1個+10点
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
	PlayerDraw(CameraGetX(), CameraGetY());
	NutsDraw(CameraGetX(), CameraGetY());
	DrawString(200, 400, "スペースでリザルト", Cr2);
	DrawString(200, 200, "インゲーム",Cr2);

	
	char timeBuf[16];
	sprintf_s(timeBuf, "Time: %d", TimerGetRemainingTime());
	DrawString(10, 50, timeBuf, Cr2);

	char scoreBuf[32];
	sprintf_s(scoreBuf, "Score: %d", ScoreGetTotal());
	DrawString(10, 80, scoreBuf, Cr2);

}