#pragma once

#define D_RANK_NAME_MAX (6) //ランキングデータ数
#define D_RANK_DATA_MAX (5) //名前最大数

//ランキングデータ構造体
struct RankData
{
	int num;
	int score;
};

int LoadRankData(void); //ランキングデータの読み込み
RankData* GetRankData(int index); //ランキングデータの取得
void AddData(RankData new_data); //ランキングデータの追加
void SortRankData(void); //なんか順番変えるやつ
void SaveRankData(void); //ランキングのデータの保存とかもろもろやってる