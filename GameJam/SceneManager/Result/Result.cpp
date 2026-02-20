#include "Result.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

int Cr3 = GetColor(255, 255, 255);
int score;
int flag;

void ResultInit(void)
{
	score = 10;
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

	//Rキーが押されたらリスタート
	if (GetKeyInputState(KEY_INPUT_R) == eRelease)
	{
		return eInGame;
	}

	//スペース押されたらタイトル画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eTitle;

	}
	return eResult;
}
void ResultDraw(void)
{
	DrawString(200, 200, "リザルト", Cr3);
	DrawString(200, 400, "スペースでタイトル", Cr3);

	DrawString(850, 100, "スコア",Cr3);
	DrawFormatString(870, 170, Cr3, "%4d", score);
	DrawFormatString(870, 300, Cr3, "%d", flag);

	switch (flag)
	{
	case 0:
		DrawString(400, 500, "放置すな", Cr3);
		break;
	case 1:
		DrawString(400, 500, "めっちゃいいね", Cr3);
		break;
	case 2:
		DrawString(400, 500, "いい感じ", Cr3);
		break;
	case 3:
		DrawString(400, 500, "ふつう", Cr3);
		break;
	case 4:
		DrawString(400, 500, "がんばろっか", Cr3);
		break;
	}
}