#include "Result.h"
#include "../InGame/ScoreManager.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

int Cr3 = GetColor(255, 255, 255);
int score;
int flag;

void ResultInit(void)
{
	score = 0;
	score = ScoreGetTotal();
	flag = 0;
}

eSceneType ResultUpdate(void)
{
	if (score > 99)
	{
		flag = 1;
	}
	else if (score > 69 && score < 100)
	{
		flag = 2;
	}
	else if (score > 34 && score < 70)
	{
		flag = 3;
	}
	else if (score > 0 && score < 35)
	{
		flag = 4;
	}

	//スペース押されたらタイトル画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eTitle;
	}
	if (GetKeyInputState(KEY_INPUT_Z) == eRelease)
	{
		return eRanking;
	}
	return eResult;
}
void ResultDraw(void)
{
	DrawString(200, 200, "リザルト", Cr3);
	DrawString(200, 400, "スペースでタイトル", Cr3);
	DrawString(200, 600, "Zキーでランキング", Cr3);

	DrawString(850, 200, "スコア", Cr3);
	DrawFormatString(820, 250, Cr3, "%4d", score);

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