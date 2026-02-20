#include"Nuts.h"
#include "../Utility/InputManager.h"
#include "../Camera/Camera.h"
#include "DxLib.h"
#include <math.h>

int Nuts_active[99];//木の実表示
int Nuts_Position_X[99];//木の実座標
int Nuts_Position_Y[99];
int Nut_MAX_Pieces[99];//木の実最大個数

int Nuts_image;//画像ー実
int Nuts_image2;//画像ー種
static Position2D Nuts_pos;//木の実位置
int pos_rand_X[99];//ランダム設定
int pos_rand_Y[99];//ランダム設定

int pos_Item_No;//アイテムナンバー
void NutsInit(void)
{
	//画像読み込み
	Nuts_image = LoadGraph("Images/Item/seed.jpg");
	Nuts_image2 = LoadGraph("Images/Item/128.jpg");
	/////////////////////初期化//////////////////////////////////////
	//pos_Item_No = 0;
	//Nuts_Position_X[0] \=100;
	//Nuts_Position_X[1] = 300;
	Nuts_Position_Y[0] = 800;
	Nuts_active[0] = TRUE;
/////////////////////////////////////////////////////////////////////////
	for (pos_Item_No = 0; pos_Item_No < 10; pos_Item_No++)
	{
		pos_rand_X[pos_Item_No] = GetRand(2400);
		pos_rand_Y[pos_Item_No] = GetRand(2400);
		Nuts_Position_X[pos_Item_No] = D_MAP_WIDTH - pos_rand_X[pos_Item_No];
		Nuts_Position_Y[pos_Item_No] = D_MAP_WIDTH - pos_rand_Y[pos_Item_No];
	}
}

void NutsUpdate(void)
{

	Nuts_active[pos_Item_No] = TRUE;



}

void NutsDraw(float camera_x, float camera_y)//スクリーン座標の取得
{
	for (pos_Item_No = 0; pos_Item_No <10; pos_Item_No++)
	{
	int draw_x = (int)(Nuts_Position_X[pos_Item_No] - camera_x);//「ワールド座標」を「スクリーン座標」に変換
	int draw_y = (int)(Nuts_Position_Y[pos_Item_No] - camera_y);
	
	//if (Nuts_active[pos_Item_No] == TRUE)//表示されるなら
	//{
	//}
			DrawRotaGraphF(draw_x, draw_y, 0.07, 0, Nuts_image2, TRUE);
		
	}
	//DrawRotaGraphF(Nuts_Position_X[1], Nuts_Position_Y[0], 0.07, 0, Nuts_image2, TRUE);

	DrawFormatString(400, 4, GetColor(255, 255, 0),"X:%d  Y:%d ",Nuts_Position_X[0], Nuts_Position_Y[0]);
	DrawFormatString(400, 18, GetColor(255, 255, 0),"X:%d  Y:%d ",Nuts_Position_X[1], Nuts_Position_Y[0]);
}

void NutsDestroy(void)
{
	Nuts_active[pos_Item_No] = FALSE;
}


Position2D GetNutsPosition(void)
{
	return Nuts_pos;
}

















