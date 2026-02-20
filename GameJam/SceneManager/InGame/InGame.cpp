#include "InGame.h"
#include "Timer.h"
#include "../../Utility/InputManager.h"
#include "ScoreManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "DxLib.h"

static int prevTime = 0;

int Cr2 = GetColor(255, 255, 255);
void InGameInit(void)
{
	PlayerInit();
	CameraInit();
	ScoreReset();
	TimerInit(5.0f);        // 制限時間60秒
	prevTime = GetNowCount(); // deltaTime
}

eSceneType InGameUpdate(void)
{
	// deltaTime）
	int nowTime = GetNowCount();
	float deltaTime = (nowTime - prevTime) / 1000.0f;
	prevTime = nowTime;

	PlayerUpdate();

	Position2D pos = GetPlayerPosition();
	CameraUpdate(pos.x, pos.y);

	if (GetKeyInputState(KEY_INPUT_Z) == ePress)
	{
		ScoreAdd(10);
	}


	// タイマー更新＆時間切れ判定
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
	DrawString(200, 400, "スペースでリザルトへ", Cr2);
	DrawString(200, 200, "インゲームシーン",Cr2);

	// 残り時間を左上に表示
	char timeBuf[16];
	sprintf_s(timeBuf, "Time: %d", TimerGetRemainingTime());
	DrawString(10, 50, timeBuf, Cr2);

	char scoreBuf[32];
	sprintf_s(scoreBuf, "Score: %d", ScoreGetTotal());
	DrawString(10, 80, scoreBuf, Cr2);

}