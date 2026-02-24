#include "InGame.h"
#include "Timer.h"
#include "../../Utility/InputManager.h"
#include "ScoreManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "../../Nuts/Nuts.h"
#include "../../Ability/Ability.h"
#include "../../Map/Map.h"
#include "../../System/ProjectConfig.h"
#include "DxLib.h"

static int prevTime = 0;
static int sndBGM;
static int sndItemGet;
static int sndItemRot;
static int sndTime;
static int prevTimeSec;

int Cr2 = GetColor(255, 255, 255);
void InGameInit(void)
{
	MapInit();
	PlayerInit();
	CameraInit();
	ScoreReset();
	NutsInit();
	TimerInit(60.0f);
	prevTime = GetNowCount();
	AbilityInit();
	sndBGM = LoadSoundMem("sounds/BGM/InGame_bgm.mp3");
	sndItemGet = LoadSoundMem("sounds/SE/itemget.wav");
	sndItemRot = LoadSoundMem("sounds/SE/itemrot.wav");
	sndTime = LoadSoundMem("sounds/SE/time.wav");
	prevTimeSec = -1;
	PlaySoundMem(sndBGM, DX_PLAYTYPE_LOOP);
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

	int score = NutsCheckCollect(pos.x, pos.y, GetPlayerRadius());
	if (score != 0)
	{
		ScoreAdd(score * GetAbilityScoreMul());
		if (score > 0)
			PlaySoundMem(sndItemGet, DX_PLAYTYPE_BACK);
		else
			PlaySoundMem(sndItemRot, DX_PLAYTYPE_BACK);
	}



	TimerUpdate(deltaTime);

	int curTimeSec = TimerGetRemainingTime();
	if (curTimeSec != prevTimeSec && curTimeSec <= 5 && curTimeSec >= 1)
	{
		PlaySoundMem(sndTime, DX_PLAYTYPE_BACK);
	}
	prevTimeSec = curTimeSec;

	if (TimerIsTimeUp())
	{
		StopSoundMem(sndBGM);
		return eResult;
	}

	return eInGame;
}
void InGameDraw(void)
{
	MapDraw(CameraGetX(), CameraGetY());
	NutsDraw(CameraGetX(), CameraGetY());
	PlayerDraw(CameraGetX(), CameraGetY());

	// --- Score / Time �p�l�� (�E��) ---
	int panelW = 180;
	int panelX = D_WINDOW_SIZE_X - panelW - 8;
	int panelY = 8;
	int lineH = 34;
	int panelH = lineH * 2 + 8;

	// ���������w�i
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �g��
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(80, 80, 80), FALSE);

	SetFontSize(28);

	// Time �`��i�c��5�b�ȉ��Őԓ_�Łj
	int remaining = TimerGetRemainingTime();
	unsigned int timeColor;
	if (remaining <= 5)
	{
		// �t���[���J�E���^�œ_�Łi��30f���Ƃɐؑցj
		int blink = (GetNowCount() / 300) % 2;
		timeColor = blink ? GetColor(255, 50, 50) : GetColor(200, 0, 0);
	}
	else
	{
		timeColor = GetColor(255, 255, 255);
	}

	char timeBuf[16];
	sprintf_s(timeBuf, "Time: %d", remaining);
	DrawString(panelX + 10, panelY + 6, timeBuf, timeColor);

	// Score �`��
	char scoreBuf[32];
	sprintf_s(scoreBuf, "Score: %d", ScoreGetTotal());
	DrawString(panelX + 10, panelY + 6 + lineH, scoreBuf, GetColor(255, 255, 255));

	SetFontSize(16);

	AbilityDraw();
}
