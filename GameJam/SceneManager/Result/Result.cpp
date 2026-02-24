#include "Result.h"
#include "../InGame/ScoreManager.h"
#include "../../Utility/InputManager.h"

#include "DxLib.h"

int Cr3 = GetColor(255, 255, 255);
RankData now = {0,0};
int flag;
int rankflag = 0;
int resultSE1;
int resultSE2;
int resultSE3;
int kakuninn = 1;
int	donguri;
int rissu;

void ResultInit(void)
{
	now.score = 0;
	now.score = ScoreGetTotal();
	flag = 0;

	//前回情報の削除
	DeleteSoundMem(resultSE1);
	DeleteSoundMem(resultSE2);
	DeleteSoundMem(resultSE3);
	DeleteGraph(donguri);

	//SE読み込み
	resultSE1 = LoadSoundMem("sounds/SE/result_min.mp3");
	resultSE2 = LoadSoundMem("sounds/SE/result.mp3");
	resultSE3 = LoadSoundMem("sounds/SE/result_max.mp3");

	rissu = LoadGraph("images/result/risutati.png");

	//評価付け
	if (now.score > 1499)
	{
		flag = 1;
	}
	else if (now.score > 999 && now.score < 1500)
	{
		flag = 2;
	}
	else if (now.score > 499 && now.score < 1000)
	{
		flag = 3;
	}
	else if (now.score > 0 && now.score < 500)
	{
		flag = 4;
	}

	//評価によってSEと出す画像を変化
	switch (flag)
	{
	case 0:
		donguri = LoadGraph("images/result/donnashi.png");
		PlaySoundMem(resultSE1, DX_PLAYTYPE_BACK);
		break;
	case 1:
		donguri = LoadGraph("images/result/moridon.png");
		PlaySoundMem(resultSE3, DX_PLAYTYPE_BACK);
		break;
	case 2:
		donguri = LoadGraph("images/result/sokodon.png");
		PlaySoundMem(resultSE2, DX_PLAYTYPE_BACK);
		break;
	case 3:
		donguri = LoadGraph("images/result/tyoidon.png");
		PlaySoundMem(resultSE2, DX_PLAYTYPE_BACK);
		break;
	case 4:
		donguri = LoadGraph("images/result/tyoidon.png");
		PlaySoundMem(resultSE1, DX_PLAYTYPE_BACK);
		break;
	}
}

eSceneType ResultUpdate(void)
{
	//スペース押されたらタイトル画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		rankflag = 1;
		AddData(now);
		StopSoundMem(resultSE1);
		StopSoundMem(resultSE2);
		StopSoundMem(resultSE3);
		return eRanking;
	}
	if (GetKeyInputState(KEY_INPUT_R) == eRelease)
	{
		AddData(now);
		StopSoundMem(resultSE1);
		StopSoundMem(resultSE2);
		StopSoundMem(resultSE3);
		return eInGame;
	}
	return eResult;
}
void ResultDraw(void)
{
	/*DrawString(200, 200, "リザルト", Cr3);
	DrawString(200, 400, "スペースでランキング", Cr3);*/

	

	switch (flag)
	{
	case 0:
		DrawGraph(-200, 0, donguri, TRUE);
		DrawString(400, 500, "そういう日もある", Cr3);
		break;
	case 1:
		DrawGraph(-200, 0, donguri, TRUE);
		DrawString(400, 500, "なかなかやるやん", Cr3);
		break;
	case 2:
		DrawGraph(-200, 0, donguri, TRUE);
		DrawString(400, 500, "ふつう", Cr3);
		break;
	case 3:
		DrawGraph(-200, 0, donguri, TRUE);
		DrawString(400, 500, "そこそこ", Cr3);
		break;
	case 4:
		DrawGraph(-200, 0, donguri, TRUE);
		DrawString(400, 500, "もうちょいがんばろっか", Cr3);
		break;
	}

	DrawGraph(800, 600, rissu, TRUE);

	DrawString(850, 200, "スコア", Cr3);
	DrawFormatString(820, 250, Cr3, "%4d", now.score);

	
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