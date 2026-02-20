#include "RankingData.h"
#include  <DxLib.h>

RankData ranking_data[D_RANK_DATA_MAX] = {1,100,2,200,3,300,4,400,5,500};


int LoadRankData(void)
{
	FILE* fp = NULL;                //ファイル読み込み
	errno_t result = fopen_s(&fp, "SceneManager/Ranking/ranking_data.txt", "r");
	if (result != NULL)
	{
		return FALSE;
	}

	//既定の件数分読み込み
	for (int i = 0; i < D_RANK_DATA_MAX; i++)
	{
		fscanf_s(fp, "%2d  %10d\n",
			&ranking_data[i].num,
			&ranking_data[i].score);
	}
	fclose(fp);

	return TRUE;
}

const RankData* GetRankData(int index)
{
	//範囲内なら該当データを返す
	if (0 <= index && index < D_RANK_DATA_MAX)
	{
		return &ranking_data[index];
	}
	return NULL;
}

void AddData(RankData new_data)
{
	const int tail = (D_RANK_DATA_MAX - 1);

	//最下位よりスコアが大きいなら最後に追加し整列する
	if (new_data.score > ranking_data[tail].score)
	{
		ranking_data[tail] = new_data;
		SortRankData();
	}
}

void SortRankData(void)
{
	RankData tmp = {};

	//順位の初期化（1で初期化）
	for (int i = 0; i < D_RANK_DATA_MAX; i++)
	{
		ranking_data[i].num = 1;
	}

	//スコア降順で交換
	for (int i = 0; i < D_RANK_DATA_MAX - 1; i++)
	{
		for (int j = i + 1; j < D_RANK_DATA_MAX; j++)
		{
			if (ranking_data[i].score <= ranking_data[j].score)
			{
				tmp = ranking_data[i];
				ranking_data[i] = ranking_data[j];
				ranking_data[j] = tmp;
			}
		}
	}

	//同スコア時の順位補正
	for (int i = 0; i < D_RANK_DATA_MAX - 1; i++)
	{
		for (int j = i + 1; j < D_RANK_DATA_MAX; j++)
		{
			if (ranking_data[i].score > ranking_data[j].score)
			{
				ranking_data[j].num++;
			}
		}
	}
	SaveRankData();  //結果を保存
}

void SaveRankData()
{
	FILE* fp = NULL;
	errno_t result = fopen_s(&fp, "SceneManager/Ranking/ranking_data.txt", "w");
	if (result != 0);
	{
		return;
	}

	for (int i = 0; i < D_RANK_DATA_MAX; i++)
	{
		fprintf(fp, "%2d  %10d\n",
			ranking_data[i].num,
			ranking_data[i].score);
	}
	fclose(fp);
}