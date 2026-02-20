#include"Nuts.h"
#include "../Utility/InputManager.h"
#include "../Camera/Camera.h"
#include "DxLib.h"
#include <math.h>

int Nuts_active[99];//木の実表示
int Nuts_Position_X[99];//木の実座標
int Nuts_Position_Y[99];
int Nut_MAX_Pieces[99];//木の実最大個数

int Nuts_image[8];//画像

static Position2D Nuts_pos;//木の実位置
int pos_rand_X[99];//X軸ランダム設定
int pos_rand_Y[99];//Y軸ランダム設定

float Appear_Time;//時間差で現れる
bool Appear_Time_flag;//時間差で現れるフラグ
int pos_Item_No;//アイテムナンバー
int Image_rand[99];//アイテム画像ランダム出力番号
void NutsInit(void)
{
	//画像読み込み
	Nuts_image[0] = LoadGraph("Images/Item/seed.png");
	Nuts_image[1] = LoadGraph("Images/Item/seed_rot.png");
	Nuts_image[2] = LoadGraph("Images/Item/nuts.png");
	Nuts_image[3] = LoadGraph("Images/Item/nuts_rot.png");
	Nuts_image[4] = LoadGraph("Images/Item/mush.png");
	Nuts_image[5] = LoadGraph("Images/Item/mush_rot.png");
	Nuts_image[6] = LoadGraph("Images/Item/walnut.png");
	Nuts_image[7] = LoadGraph("Images/Item/walnut_rot.png");
	/////////////////////初期化//////////////////////////////////////
	
	Nuts_active[0] = TRUE;//木の実表示
	Appear_Time = 0;//時間差で現れる
	Appear_Time_flag == FALSE;
/////////////////////////////////ランダム配置////////////////////////////////////////
	for (pos_Item_No = 0; pos_Item_No < 30; pos_Item_No++)//３０個配置
	{
		pos_rand_X[pos_Item_No] = GetRand(2400);//X軸ランダム設定
		pos_rand_Y[pos_Item_No] = GetRand(2100);//Y軸ランダム設定
		Image_rand[pos_Item_No] = GetRand(8);//画像ランダム設定
		Nuts_Position_X[pos_Item_No] = D_MAP_WIDTH - pos_rand_X[pos_Item_No];//ワールド内のX位置
		Nuts_Position_Y[pos_Item_No] = D_MAP_WIDTH - pos_rand_Y[pos_Item_No];//ワールド内のY位置
		Nuts_active[pos_Item_No] = TRUE;//表示
	}
}

void NutsUpdate(void)
{
	//時間差で現れる
	//Appear_Time = +0.001;
	//if (Appear_Time == 10)
	//{
	//	Appear_Time_flag = TRUE;
	//}

}

void NutsDraw(float camera_x, float camera_y)//スクリーン座標の取得
{
	for (pos_Item_No = 0; pos_Item_No < 30;pos_Item_No++)//３０個配置
	{
		int draw_x = (int)(Nuts_Position_X[pos_Item_No] - camera_x);//「ワールド座標」を「スクリーン座標」に変換
		int draw_y = (int)(Nuts_Position_Y[pos_Item_No] - camera_y);

		if (Nuts_active[pos_Item_No] == TRUE)//表示されるなら
		{
			DrawRotaGraphF(draw_x, draw_y, 0.5, 0, Nuts_image[Image_rand[pos_Item_No]], TRUE);
		}
	}

	DrawFormatString(400, 4, GetColor(255, 255, 0),"X:%d  Y:%d ",Nuts_Position_X[0], Nuts_Position_Y[0]);//一つ目の位置
	DrawFormatString(400, 18, GetColor(255, 255, 0),"X:%d  Y:%d ",Nuts_Position_X[1], Nuts_Position_Y[1]);//二つ目の位置
}

void NutsDestroy(void)
{
	Nuts_active[pos_Item_No] = FALSE;//壊す
}


Position2D GetNutsPosition(void)
{
	return Nuts_pos;//位置
}

// 画像番号から種類別スコアを返す
static int GetNutsScore(int image_id)
{
	int is_rot = (image_id % 2 == 1); // 奇数番号 = 腐っている
	switch (image_id / 2) // 0,1=seed 2,3=nuts 4,5=mush 6,7=walnut
	{
	case 0: return is_rot ? -10 : 10;   // seed（種）
	case 1: return is_rot ? -20 : 20;   // nuts（木の実）
	case 2: return is_rot ? -50 : 50;   // mush（きのこ）
	case 3: return is_rot ? -100 : 100;  // walnut（くるみ）
	default: return 10; 10;
	}
}

int NutsCheckCollect(float player_x, float player_y, float player_radius)
{
	for (pos_Item_No = 0; pos_Item_No < 30; pos_Item_No++)
	{
		if (Nuts_active[pos_Item_No] == TRUE)
		{
			float dx = player_x - Nuts_Position_X[pos_Item_No];
			float dy = player_y - Nuts_Position_Y[pos_Item_No];
			float distance = sqrtf(dx * dx + dy * dy);
			if (distance < player_radius + 16.0f) // 木の実の半径16px
			{
				Nuts_active[pos_Item_No] = FALSE; // 木の実を消す
				return GetNutsScore(Image_rand[pos_Item_No]); // 種類別スコア
			}
		}
	}
	return 0; // 収集なし
}

















