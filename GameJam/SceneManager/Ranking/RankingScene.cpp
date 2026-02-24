#include"RankingScene.h"
#include"RankingData.h"
#include"../../Utility/InputManager.h"
#include"../Result/Result.h"
#include<DxLib.h>

int Rankingflag;
int RankingBGM;
int guri;

int RankingInit(void)
{
	RankingBGM = LoadSoundMem("sounds/BGM/Ranking_BGM.mp3");
	guri = LoadGraph("images/result/moridon.png");

	//ランキングデータ読み込み
	if (LoadRankData() != TRUE)
	{
		return FALSE;
	}

	Rankingflag = GetFlag();

	return TRUE;
}

eSceneType RankingUpdate(void)
{
	if (CheckSoundMem(RankingBGM) != TRUE)
	{
		PlaySoundMem(RankingBGM, DX_PLAYTYPE_BACK);
	}
	//Aボタンでタイトルへ戻る
	if (GetControllerState(eButtonA) == ePress)
	{
		StopSoundMem(RankingBGM);
		return eTitle;
	}
	if (GetKeyInputState(KEY_INPUT_B) == ePress)
	{
		StopSoundMem(RankingBGM);
		return eTitle;
	}
	return eRanking;
}

void RankingDraw(void)
{
	
	SaveRankData();
	//タイトルランキング一覧を描画
	SetFontSize(50);
	int now_score = GetScore();
	
	DrawGraph(-200, 0, guri, TRUE);

	switch (Rankingflag)
	{
	case 0:
		for (int i = 0; i < D_RANK_DATA_MAX; i++)
		{
			const RankData* deta = GetRankData(i);
			if (now_score == deta->score)
			{
				DrawFormatString(200, 200 + i * 70, GetColor(255, 255, 0), "%2d %10d",
					deta->num, deta->score);
			}
			else
			{
				DrawFormatString(200, 200 + i * 70, GetColor(0, 0, 0), "%2d %10d",
					deta->num, deta->score);
			}
		}
		DrawString(350, 600, "Aボタンでタイトルへ戻る", GetColor(255, 255, 255));
		break;
	case 1:
		for (int i = 0; i < D_RANK_DATA_MAX; i++)
		{
			const RankData* deta = GetRankData(i);
			if (now_score == deta->score)
			{
				DrawFormatString(500, 200 + i * 70, GetColor(255, 255, 0), "%2d %10d",
					deta->num, deta->score);
			}
			else
			{
				DrawFormatString(500, 200 + i * 70, GetColor(0, 0, 0), "%2d %10d",
					deta->num, deta->score);
			}
		}
		DrawFormatString(0, 300, GetColor(0, 0, 0), "%10d", now_score);
		DrawString(350, 600, "Aボタンでタイトルへ戻る", GetColor(255, 255, 255));
		
		break;
	}

	

	SetFontSize(20);
}