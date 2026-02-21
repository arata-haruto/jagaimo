#include"RankingScene.h"
#include"RankingData.h"
#include"../../Utility/InputManager.h"
#include"../Result/Result.h"
#include<DxLib.h>

int Rankingflag;

int RankingInit(void)
{
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

	//Aボタンでタイトルへ戻る
	if (GetControllerState(eButtonA) == ePress)
	{
		return eTitle;
	}
	return eRanking;
}

void RankingDraw(void)
{
	
	SaveRankData();
	//タイトルランキング一覧を描画
	SetFontSize(50);
	DrawString(0, 10, "ランキング画面", GetColor(255, 255, 255));
	int now_score = GetScore();
	
	switch (Rankingflag)
	{
	case 0:
		for (int i = 0; i < D_RANK_DATA_MAX; i++)
		{
			const RankData* deta = GetRankData(i);
			if (now_score == deta->score)
			{
				DrawFormatString(200, 100 + i * 60, GetColor(255, 255, 0), "%2d %10d",
					deta->num, deta->score);
			}
			else
			{
				DrawFormatString(200, 100 + i * 60, GetColor(255, 255, 255), "%2d %10d",
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
				DrawFormatString(500, 100 + i * 60, GetColor(255, 255, 0), "%2d %10d",
					deta->num, deta->score);
			}
			else
			{
				DrawFormatString(500, 100 + i * 60, GetColor(255, 255, 255), "%2d %10d",
					deta->num, deta->score);
			}
		}
		DrawFormatString(0, 300, GetColor(255, 255, 255), "%10d", now_score);
		DrawString(350, 600, "Aボタンでタイトルへ戻る", GetColor(255, 255, 255));
		
		break;
	}

	SetFontSize(20);
}