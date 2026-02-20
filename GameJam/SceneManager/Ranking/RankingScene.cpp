#include"RankingScene.h"
#include"RankingData.h"
#include"../../Utility/InputManager.h"
#include"../Result/Result.h"
#include<DxLib.h>

float ranking_blink_time;    //点滅タイマー
int ranking_blink_flag;      //点滅ON/OFFフラグ
int ranking_cursor;          //カーソル画像ハンドル

int RankingInit(void)
{
	//ランキングデータ読み込み
	if (LoadRankData() != TRUE)
	{
		return FALSE;
	}

	//カーソル画像読み込み
	/*ranking_cursor = LoadGraph("assets/");
	if (ranking_cursor == -1)
	{
		return FALSE;
	}*/

	//点滅状態初期化
	/*ranking_blink_time = 0.0f;
	ranking_blink_flag = TRUE;*/
	return TRUE;
}

eSceneType RankingUpdate(void)
{
	//0.5秒ごとに点滅
	/*ranking_blink_time += delta_second;
	if (ranking_blink_time >= 0.0f)
	{
		ranking_blink_time = 0.0f;
		ranking_blink_flag = ranking_blink_flag ^ TRUE;
	}*/

	//Aボタンでタイトルへ戻る
	if (GetControllerState(eButtonA) == ePress)
	{
		LoadRankData();
		return eTitle;
	}
	return eRanking;
}

void RankingDraw(void)
{
	SaveRankData();
	//タイトルランキング一覧を描画
	SetFontSize(30);
	DrawString(0, 10, "ランキング画面", GetColor(255, 255, 255));
	for (int i = 0; i < D_RANK_DATA_MAX; i++)
	{
		const RankData* data = GetRankData(i);
		DrawFormatString(700, 100 + i * 60, GetColor(255, 255, 255), "%2d %10d",
			data->num, data->score);
	}
	int now_score = GetScore();
	DrawFormatString(0, 300, GetColor(255, 255, 255), "%10d", now_score);
	DrawString(500, 600, "Aボタンでタイトルへ戻る", GetColor(255, 255, 255));
	SetFontSize(20);

	//点滅中は「A」ボタンで戻る案内を表示
	/*if (ranking_blink_flag == TRUE)
	{
		DrawRotaGraph(500, 450, 1.0, 0.0, ranking_cursor, TRUE);
		SetFontSize(25);
		DrawString(530, 440, ":Title", GetColor(255, 0, 0));
	}*/
}