#include"RankingScene.h"
#include"RankingData.h"
#include"../../Utility/InputManager.h"
#include"../Result/Result.h"
#include<DxLib.h>

int Rankingflag;
int RankingBGM;
int guri;
int num[10];

int RankingInit(void)
{
	RankingBGM = LoadSoundMem("sounds/BGM/Ranking_BGM.mp3");
	guri = LoadGraph("images/result/moridon.png");
	LoadDivGraph("images/UI/NUMBER.png", 10, 5, 2, 160, 160, num);


	//ランキングデータ読み込み
	if (LoadRankData() != TRUE)
	{
		return FALSE;
	}

	Rankingflag = GetFlag();
	SaveRankData();

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
	
	
	//タイトルランキング一覧を描画
	SetFontSize(50);
	int now_score = GetScore();
	int now_score_num[4] = { now_score / 1000 % 10,
							 now_score / 100 % 10,
							 now_score / 10 % 10,
							 now_score % 10 };
	
	DrawGraph(-200, 0, guri, TRUE);

	switch (Rankingflag)
	{
	case 0:
		for (int i = 0; i < D_RANK_DATA_MAX; i++)
		{
			RankData* deta = GetRankData(i);
			
			int score = deta->score;
			int number = deta->num;
				
			int digit_1[4] = { score / 1000 % 10,
							   score / 100 % 10,
							   score / 10 % 10,
							   score % 10 };

			int digit_2 = number % 10;

			DrawRotaGraph(400, 200 + i * 70, 0.4, 0.0, num[digit_2], TRUE);
			for (int j = 0; j < 4; j++)
			{
				DrawRotaGraph(600 + j * 45, 200 + i * 70, 0.4, 0.0, num[digit_1[j]], TRUE);
			}
		}
		DrawString(350, 600, "Aボタンでタイトルへ戻る", GetColor(255, 255, 255));
		break;
	case 1:
		for (int i = 0; i < D_RANK_DATA_MAX; i++)
		{
			const RankData* deta = GetRankData(i);

			int score = deta->score;
			int score2 = deta->score;
			int number = deta->num;

			int digit_1[4] = { score / 1000 % 10,
							   score / 100 % 10,
							   score / 10 % 10,
							   score % 10 };

			int digit_2 = number % 10;

			DrawRotaGraph(500, 200 + i * 70, 0.4, 0.0, num[digit_2], TRUE);
			for (int j = 0; j < 4; j++)
			{
				DrawRotaGraph(620 + j * 40, 200 + i * 70, 0.4, 0.0, num[digit_1[j]], TRUE);
			}

		}
		for (int j = 0; j < 4; j++)
		{
			DrawRotaGraph(200 + j * 45, 350, 0.4, 0.0, num[now_score_num[j]], TRUE);
		}
		
		DrawString(350, 600, "Aボタンでタイトルへ戻る", GetColor(255, 255, 255));
		
		break;
	}

	

	SetFontSize(20);
}