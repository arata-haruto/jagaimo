#include"Nuts.h"
#include "../Utility/InputManager.h"
#include "../Camera/Camera.h"
#include "DxLib.h"
#include <math.h>
#include "../SceneManager/InGame/Timer.h"

int Nuts_active[99];//木の実表示
int Nuts_Position_X[99];//木の実X座標
int Nuts_Position_Y[99];//木の実Y座標
int Nut_MAX_Pieces;//木の実最大個数

int Nuts_image[8];//画像

static Position2D Nuts_pos;//木の実位置
int pos_rand_X[99];//X軸ランダム設定
int pos_rand_Y[99];//Y軸ランダム設定

float Appear_Time;//時間差で現れる
bool Appear_Time_flag;//時間差で現れるフラグ
int pos_Item_No;//アイテムナンバー
int Image_rand[99];//アイテム画像ランダム出力番号
//int Fresh_Pieces[99];//新鮮な実の個数
bool Rot_Pieces[99];//腐の実の個数
int Rot_Timer[99];//時間差で腐を消す
int Rot_T;
//int order;
int sorting;
int R_Pieces;
int F_Pieces;


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
	sorting = 0;
	Appear_Time = 0;//時間差で現れる
	Appear_Time_flag == FALSE;//時間差で現れるフラグ
	R_Pieces=0;
    F_Pieces=0;
	Rot_T = 0;
/////////////////////////////////ランダム配置////////////////////////////////////////
	for (pos_Item_No = 0; pos_Item_No < 99; pos_Item_No++)//３０個配置
	{
		pos_rand_X[pos_Item_No] = GetRand(2200)+100;//X軸ランダム設定(最小値調整）
		pos_rand_Y[pos_Item_No] = GetRand(1700)+100;//Y軸ランダム設定
		Image_rand[pos_Item_No] = GetRand(8);//画像ランダム設定
		Nuts_Position_X[pos_Item_No] = D_MAP_WIDTH - pos_rand_X[pos_Item_No];//ワールド内のX位置
		Nuts_Position_Y[pos_Item_No] = D_MAP_WIDTH - pos_rand_Y[pos_Item_No];//ワールド内のY位置
		Nuts_active[pos_Item_No] = TRUE;//表示
		Nut_MAX_Pieces++;
		//読み込んだ画像が腐なら(画像ランダム出力が奇数なら）
		if (Image_rand[pos_Item_No] == 1 || Image_rand[pos_Item_No] == 3 || Image_rand[pos_Item_No] == 5 || Image_rand[pos_Item_No] == 7)
		{
			Rot_Pieces[pos_Item_No]=TRUE; //腐の個数を増やす
			Rot_Timer[Rot_T]++;
		}
		else
		{
			
			Rot_Pieces[pos_Item_No]=FALSE;//新の個数を増やす
		}
		
	}
}

void NutsUpdate(void)
{
	int time = TimerGetRemainingTime();
	

	switch (time)//木の実の数調整
	{
	case 5:////残5で70
		Nut_MAX_Pieces = 70;
		break;
	case 10://残10で60
		Nut_MAX_Pieces = 60;
		break;
	case 15://残15で50
		Nut_MAX_Pieces = 50;
		break;
	case 20://残20で40
		Nut_MAX_Pieces = 40;
		break;
	case 25://残25で30
		Nut_MAX_Pieces = 30;
		break;
	case 30://最初20
		Nut_MAX_Pieces = 20;
		break;
	default:
		break;
	}
	for (sorting; sorting < Nut_MAX_Pieces; sorting++)
	{
		if (Rot_Pieces[sorting] == FALSE)
		{
			R_Pieces++;
		}
		else
		{
			F_Pieces++;
		}
	}
}

void NutsDraw(float camera_x, float camera_y)//スクリーン座標の取得
{
	int draw_x;
	int draw_y;
	for (pos_Item_No = 0; pos_Item_No < Nut_MAX_Pieces;pos_Item_No++)//4０個配置
	{
		
		 draw_x = (int)(Nuts_Position_X[pos_Item_No] - camera_x);//「ワールド座標」を「スクリーン座標」に変換
		 draw_y = (int)(Nuts_Position_Y[pos_Item_No] - camera_y);

		 if (Nuts_active[pos_Item_No] == TRUE)//表示されるなら
		 {
			 DrawRotaGraphF(draw_x, draw_y, 0.5, 0, Nuts_image[Image_rand[pos_Item_No]], TRUE);
		 }
	}

	DrawFormatString(400, 4, GetColor(255, 255, 0),"X:%d  Y:%d ",Nuts_Position_X[0], Nuts_Position_Y[0]);//一つ目の位置
	DrawFormatString(400, 18, GetColor(255, 255, 0),"R:%d F:%d", R_Pieces, F_Pieces);//二つ目の位置
	DrawFormatString(400, 38, GetColor(255, 255, 0),"time:%f", Appear_Time);//二つ目の位置
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
	for (pos_Item_No = 0; pos_Item_No < Nut_MAX_Pieces; pos_Item_No++)
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











//void OtherDraw() {
//	int time = TimerGetRemainingTime();
//}





