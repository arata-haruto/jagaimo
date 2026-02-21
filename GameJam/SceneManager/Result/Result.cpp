#include "Result.h"
#include "../InGame/ScoreManager.h"
#include "../../Utility/InputManager.h"

#include "DxLib.h"

int Cr3 = GetColor(255, 255, 255);
RankData now = {0,0};
int flag;
int rankflag = 0;

void ResultInit(void)
{
	now.score = 0;
	now.score = ScoreGetTotal();
	flag = 0;
}

eSceneType ResultUpdate(void)
{
	if (now.score > 99)
	{
		flag = 1;
	}
	else if (now.score > 69 && now.score < 100)
	{
		flag = 2;
	}
	else if (now.score > 34 && now.score < 70)
	{
		flag = 3;
	}
	else if (now.score > 0 && now.score < 35)
	{
		flag = 4;
	}

	//スペース押されたらタイトル画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		rankflag = 1;
		AddData(now);
		return eRanking;
	}
	if (GetKeyInputState(KEY_INPUT_R) == eRelease)
	{
		AddData(now);
		return eInGame;
	}
	return eResult;
}
void ResultDraw(void)
{
	DrawString(200, 200, "リザルト", Cr3);
	DrawString(200, 400, "スペースでランキング", Cr3);

	DrawString(850, 200, "スコア", Cr3);
	DrawFormatString(820, 250, Cr3, "%4d", now.score);

	switch (flag)
	{
	case 0:
		DrawString(400, 500, "放置すんなよ", Cr3);
		break;
	case 1:
		DrawString(400, 500, "なかなかやるやん", Cr3);
		break;
	case 2:
		DrawString(400, 500, "ふつう", Cr3);
		break;
	case 3:
		DrawString(400, 500,"そこそこ", Cr3);
		break;
	case 4:
		DrawString(400, 500, "もうちょいがんばろっか", Cr3);
		break;
	}
}

int GetScore(void)
{
	return now.score;
}

int GetFlag(void)
{
	return rankflag;
}

void InitFlag(void)
{
	rankflag = 0;
}