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
int nownum[10];

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
	LoadDivGraph("images/UI/NUMBER.png", 10, 5, 2, 160, 160, nownum);

	//評価付け
	if (now.score > 2999)
	{
		flag = 1;
	}
	else if (now.score > 1999 && now.score < 3000)
	{
		flag = 2;
	}
	else if (now.score > 999 && now.score < 2000)
	{
		flag = 3;
	}
	else if (now.score > 0 && now.score < 1000)
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
	//Bボタンでタイトルへ
	if (GetControllerState(eButtonB) == eRelease)
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
		break;
	case 1:
		DrawGraph(-200, 0, donguri, TRUE);
		break;
	case 2:
		DrawGraph(-200, 0, donguri, TRUE);
		break;
	case 3:
		DrawGraph(-200, 0, donguri, TRUE);
		break;
	case 4:
		DrawGraph(-200, 0, donguri, TRUE);
		break;
	}

	DrawGraph(800, 600, rissu, TRUE);

	int digit_1[4] = { now.score / 1000 % 10,
					   now.score / 100 % 10,
					   now.score / 10 % 10,
					   now.score % 10 };


	for (int j = 0; j < 4; j++)
	{
		DrawRotaGraph(750 + j * 45, 100, 0.4, 0.0, nownum[digit_1[j]], TRUE);
	}


	DrawString(750, 50, "スコア", Cr3);
	//DrawFormatString(820, 250, Cr3, "%4d", now.score);

	
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