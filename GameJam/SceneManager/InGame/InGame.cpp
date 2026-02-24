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
static int snd_bgm;
static int snd_itemget;
static int snd_itemrot;
static int snd_time;
static int time_warned;

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

	snd_bgm     = LoadSoundMem("sounds/BGM/InGame_bgm.mp3");
	snd_itemget  = LoadSoundMem("sounds/SE/itemget.wav");
	snd_itemrot  = LoadSoundMem("sounds/SE/itemrot.wav");
	snd_time     = LoadSoundMem("sounds/SE/time.wav");
	time_warned  = FALSE;
	PlaySoundMem(snd_bgm, DX_PLAYTYPE_LOOP);
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

	//if (IsAbilityMagnetActive())
	{
		//NutsMagnetPull(pos.x, pos.y, 200.0f, 5.0f);
	}

	int score = NutsCheckCollect(pos.x, pos.y, 16.0f);
	if (score != 0)
	{
		ScoreAdd(score * GetAbilityScoreMul());
		if (score > 0)
			PlaySoundMem(snd_itemget, DX_PLAYTYPE_BACK);
		else
			PlaySoundMem(snd_itemrot, DX_PLAYTYPE_BACK);
	}

	TimerUpdate(deltaTime);

	if (time_warned == FALSE && TimerGetRemainingTime() <= 5)
	{
		PlaySoundMem(snd_time, DX_PLAYTYPE_BACK);
		time_warned = TRUE;
	}

	if (TimerIsTimeUp())
	{
		StopSoundMem(snd_bgm);
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