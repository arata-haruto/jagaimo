#include "InGame.h"
#include "Timer.h"
#include "../../Utility/InputManager.h"
#include "ScoreManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "../../Nuts/Nuts.h"
#include "../../Ability/Ability.h"
#include "../../System/ProjectConfig.h"
#include "DxLib.h"
#include <string.h>

static int prevTime = 0;
static int ingame_bgm;
static int item_se;
static int itemrot_se;
static int time_se;
static int time_warned;
static int fontLoaded = FALSE;

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

	ingame_bgm     = LoadSoundMem("sounds/BGM/InGame_bgm.mp3");
	item_se  = LoadSoundMem("sounds/SE/itemget.wav");
	itemrot_se  = LoadSoundMem("sounds/SE/itemrot.wav");
	time_se     = LoadSoundMem("sounds/SE/time.wav");
	time_warned  = FALSE;
	if (fontLoaded == FALSE)
	{
		AddFontResourceEx("images/UI/x10y12pxDonguriDuel.ttf", FR_PRIVATE, NULL);
		fontLoaded = TRUE;
	}
	PlaySoundMem(ingame_bgm, DX_PLAYTYPE_LOOP);
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
		if (score > 0)
			PlaySoundMem(item_se, DX_PLAYTYPE_BACK);
		else
			PlaySoundMem(itemrot_se, DX_PLAYTYPE_BACK);
	}

	TimerUpdate(deltaTime);

	if (time_warned == FALSE && TimerGetRemainingTime() <= 5)
	{
		PlaySoundMem(time_se, DX_PLAYTYPE_BACK);
		time_warned = TRUE;
	}

	if (TimerIsTimeUp())
	{
		StopSoundMem(ingame_bgm);
		return eResult;
	}

	 return eInGame;
}
void InGameDraw(void)
{
	MapDraw(CameraGetX(), CameraGetY());
	NutsDraw(CameraGetX(), CameraGetY());
	PlayerDraw(CameraGetX(), CameraGetY());

	ChangeFont("x10y12pxDonguriDuel");
	ChangeFontType(DX_FONTTYPE_NORMAL);

	// --- Score (right-top) ---
	{
		SetFontSize(24);
		char scoreBuf[32];
		sprintf_s(scoreBuf, "Score: %d", ScoreGetTotal());
		int textW = GetDrawStringWidth(scoreBuf, (int)strlen(scoreBuf));
		int panelW = textW + 20;
		int panelH = 40;
		int panelX = D_WINDOW_SIZE_X - panelW - 10;
		int panelY = 10;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
		DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawFormatString(panelX + 10, panelY + 8, GetColor(255, 255, 255), "%s", scoreBuf);
		SetFontSize(16);
	}

	// --- �^�C�}�[�\�� (�㕔����) ---
	{
		int remaining = TimerGetRemainingTime();
		float remainF = TimerGetRemainingTimeF();
		float total = TimerGetTotalTime();
		float ratio = (total > 0.0f) ? (remainF / total) : 0.0f;
		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;

		int panelW = 220, panelH = 55;
		int panelX = (D_WINDOW_SIZE_X - panelW) / 2;
		int panelY = 10;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
		DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �F�ω�
		unsigned int textColor, barColor;
		if (remaining > 10)
		{
			textColor = GetColor(255, 255, 255);
			barColor = GetColor(0, 200, 0);
		}
		else if (remaining > 5)
		{
			textColor = GetColor(255, 255, 0);
			barColor = GetColor(255, 255, 0);
		}
		else
		{
			// 5�b�ȉ�: �� + �_��
			int blink = (GetNowCount() / 300) % 2;
			textColor = blink ? GetColor(255, 50, 50) : GetColor(255, 150, 150);
			barColor = GetColor(255, 50, 50);
		}

		SetFontSize(28);
		DrawFormatString(panelX + 10, panelY + 4, textColor, "Time: %d", remaining);
		SetFontSize(16);

		// �^�C�}�[�o�[
		int barX = panelX + 10;
		int barY = panelY + 36;
		int barMaxW = 200;
		int barH = 10;
		DrawBox(barX, barY, barX + barMaxW, barY + barH, GetColor(60, 60, 60), TRUE);
		int barW = (int)(barMaxW * ratio);
		if (barW > 0)
			DrawBox(barX, barY, barX + barW, barY + barH, barColor, TRUE);
	}

	AbilityDraw();

	ChangeFont("�l�r �S�V�b�N");
}