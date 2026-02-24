#include"Nuts.h"
#include "../Utility/InputManager.h"
#include "../Camera/Camera.h"
#include "DxLib.h"
#include <math.h>
#include "../SceneManager/InGame/Timer.h"
#include"../Player/Player.h"
#include"../SceneManager/InGame/Timer.h"

int Nuts_active[999];//木の実表示
int Nuts_Position_X[999];//木の実X座標
int Nuts_Position_Y[999];//木の実Y座標
int Nut_MAX_Pieces;//木の実最大個数

int Nuts_image[9];//画像

static Position2D Nuts_pos;//木の実位置
int pos_rand_X[999];//X軸ランダム設定
int pos_rand_Y[999];//Y軸ランダム設定



int pos_Item_No;//アイテムナンバー
int Image_rand[999];//アイテム画像ランダム出力番号
//int Fresh_Pieces[999];//新鮮な実の個数
bool Rot_Pieces[999];//腐の実の個数
int Rot_Timer[999];//時間差で腐を消す
int FEVER_img;//フィーバー用の画像


float Nuts_Scale[999];//Nut_MAX_Pieces
int Speed_Cange;
// --- フィーバー用に追加 ---
int Fresh_Nuts_Count = 0; // 集めた新鮮な実の数
bool Is_Fever = false;    // フィーバー中かどうかフラグ
int Fever_Time_Count = 0; // フィーバーの時間
int Shine_image=0;//フィーバー木の実を強調
float Shine_Angle = 0.0f; // エフェクトの回転角度

// フィーバー前の状態を保存するためのバックアップ
int Backup_Nuts_active[999];
int Backup_Image_rand[999];
int Backup_Nut_MAX_Pieces;
// ------------------------

// 縮小アニメーション用
bool Nuts_IsVanishing[999];  // それぞれの実が縮小中かどうか
bool Is_FeverEnding = false; // フィーバー終了演出中かどうか


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
	Nuts_image[8] = LoadGraph("Images/Item/Gold.png");//フィーバー用の木の実
	Shine_image=LoadGraph("Images/Item/shine.png");//フィーバー用の木の実強調
	FEVER_img= LoadGraph("Images/Item/FEVER.png");
	/////////////////////初期化//////////////////////////////////////
 
	Nut_MAX_Pieces = 0;
	Is_Fever = false;
	Is_FeverEnding = false;
	Fresh_Nuts_Count = 0;
	for (int i = 0; i < 999; i++)
	{
		Nuts_IsVanishing[i] = FALSE; // 初期化
		Nuts_Scale[i] = 0.0f;        // 念のためスケールも初期化
	}

/////////////////////////////////ランダム配置////////////////////////////////////////
	for (pos_Item_No = 0; pos_Item_No < 999; pos_Item_No++)//３０個配置
	{
		pos_rand_X[pos_Item_No] = GetRand(2700)+80;//X軸ランダム設定(最小値調整）
		pos_rand_Y[pos_Item_No] = GetRand(2030)+80;//Y軸ランダム設定
		Image_rand[pos_Item_No] = GetRand(7);//画像ランダム設定
		Nuts_Position_X[pos_Item_No] = D_MAP_WIDTH - pos_rand_X[pos_Item_No];//ワールド内のX位置
		Nuts_Position_Y[pos_Item_No] = D_MAP_HEIGHT - pos_rand_Y[pos_Item_No];//ワールド内のY位置
		bool Is_overlap = false; // 重なり判定フラグ
		
		// 2. これまでに配置した確定した木の実と重なっていないかチェック
			for (int s = 0; s < pos_Item_No; s++)
			{
				int s_posX1 = Nuts_Position_X[s] - 50;
				int s_posX2 = Nuts_Position_X[s] + 50;
				int s_posY1 = Nuts_Position_Y[s] - 50;
				int s_posY2 = Nuts_Position_Y[s] + 50;

				// 重なり判定
				if (s_posX1 < Nuts_Position_X[pos_Item_No] && Nuts_Position_X[pos_Item_No] < s_posX2 &&s_posY1 < Nuts_Position_Y[pos_Item_No] && Nuts_Position_Y[pos_Item_No] < s_posY2)
				{
					Is_overlap = true; // 重なっている
					break;             //ループを抜ける
				}
			}
		
			// 3. 重なり判定の結果で処理を分ける
			if (Is_overlap == false)
			{
				Image_rand[pos_Item_No] = GetRand(7);
				Nuts_active[pos_Item_No] = TRUE;
				Nut_MAX_Pieces++;

				if (Image_rand[pos_Item_No] == 1 || Image_rand[pos_Item_No] == 3 || Image_rand[pos_Item_No] == 5 || Image_rand[pos_Item_No] == 7)
				{
					Rot_Pieces[pos_Item_No] = TRUE;
				}
				else 
				{
					Rot_Pieces[pos_Item_No] = FALSE;
				}
			}
			else
			{
				//重なっていたらーしてランダムやり直す
				pos_Item_No--;
			}
	}
	//最初100個のうち、どれか1つだけフィーバー実にする
	int fever_set_flag = 0;
	while (fever_set_flag == 0)//セットさせる
	{
		int r = GetRand(99); //最初の100個の中からランダムで選ぶ

		// その場所の木の実がちゃんと表示(配置)されているものなら
		if (Nuts_active[r] == TRUE)
		{
			Image_rand[r] = 8;     // フィーバー木の実に替える
			Rot_Pieces[r] = FALSE; // 新鮮な木の実に設定
			fever_set_flag = 1;    // 1つ設定完了したのでループを抜ける
		}
	}
}

void NutsUpdate(void)
{
	int time = TimerGetRemainingTime();
	
	//強調エフェクト回転
	Shine_Angle += 0.05f;
	// 角度が大きくなりすぎないようにリセット
	if (Shine_Angle > 3.141592f * 2.0f) {
		Shine_Angle -= 3.141592f * 2.0f;
	}

	//フィーバーのカウントダウンと終了処理
	if (Is_Fever==TRUE)
	{
		Fever_Time_Count--;
		if (Fever_Time_Count <= 0) 
		{
			// 5秒経過したら元の配置に完全に戻す
			Is_Fever = false;//フィーバー終了
			Is_FeverEnding = true;//消えるモーション中
		
			// 現在マップにいる全ての実を消す
			for (int i = 0; i < 999; i++) 
			{
				if (Nuts_active[i] == TRUE) 
				{
					Nuts_IsVanishing[i] = TRUE;
				}
			}

		}
	}
	//フィーバー終了演出
	else if (Is_FeverEnding) {
		// すべての実が消え終わったかチェック
		bool allVanished = true;
		for (int i = 0; i < 999; i++)
		{
			// まだ表示されていて、かつ縮小中のものがあれば
			if (Nuts_active[i] == TRUE && Nuts_IsVanishing[i] == TRUE)
			{
				allVanished = false; // まだ消え終わっていない
				break;
			}
		}

		// 全員消え終わったら、元の配置を復元する
		if (allVanished) 
		{
			Is_FeverEnding = false; // 終了演出完了
			Nut_MAX_Pieces = Backup_Nut_MAX_Pieces;
			for (int i = 0; i < 999; i++) 
			{
				Nuts_active[i] = Backup_Nuts_active[i];
				Image_rand[i] = Backup_Image_rand[i];
				Nuts_IsVanishing[i] = FALSE; // リセット
				// 復帰した実もふわっと出現させるためにスケールを0にする
				if (Nuts_active[i] == TRUE) 
				{
					Nuts_Scale[i] = 0.0f;
				}
			}
		}
	}
	//通常時
	else {
		// フィーバーしていない時だけ、通常の個数調整を行う
		switch (time)//木の実の数調整
		{
		case 5:  Nut_MAX_Pieces = 500; break;
		case 10: Nut_MAX_Pieces = 450; break;
		case 15: Nut_MAX_Pieces = 300; break;
		case 20: Nut_MAX_Pieces = 200; break;
		case 25: Nut_MAX_Pieces = 150; break;
		case 30: Nut_MAX_Pieces = 100; break;
		default: break;
		}
	}

	
	// 拡大・縮小アニメーション
	for (int i = 0; i < 999; i++) // 全要素チェック
	{
		if (Nuts_active[i] == TRUE)
		{
			// 消える　縮小
			if (Nuts_IsVanishing[i] == TRUE)
			{
				Nuts_Scale[i] -= 0.05f; // 縮小スピード

				if (Nuts_Scale[i] <= 0.0f) {
					Nuts_Scale[i] = 0.0f;
					Nuts_active[i] = FALSE;      // 完全に消す
					Nuts_IsVanishing[i] = FALSE; // 縮小モード解除
				}
			}
			// 出現　拡大
			else if (Nuts_Scale[i] < 0.5f) 
			{
				Nuts_Scale[i] += 0.02f; // 拡大スピード
				if (Nuts_Scale[i] > 0.5f) 
				{
					Nuts_Scale[i] = 0.5f;
				}
			}
		}
	}
}

void NutsDraw(float camera_x, float camera_y)//スクリーン座標の取得
{
	int draw_x;
	int draw_y;
	int s_posX1;
	int s_posX2;
	int s_posY1;
	int s_posY2;
	int s;
	for (pos_Item_No = 0; pos_Item_No < Nut_MAX_Pieces; pos_Item_No++)//4０個配置
	{

		draw_x = (int)(Nuts_Position_X[pos_Item_No] - camera_x);//「ワールド座標」を「スクリーン座標」に変換
		draw_y = (int)(Nuts_Position_Y[pos_Item_No] - camera_y);

		if (Nuts_active[pos_Item_No] == TRUE)//表示されるなら
		{
			DrawRotaGraphF(draw_x, draw_y, Nuts_Scale[pos_Item_No], 0, Nuts_image[Image_rand[pos_Item_No]], TRUE);
			if (Image_rand[pos_Item_No] == 8)
			{
				// Shine_Angleを使って回転させながら描画
				DrawRotaGraphF(draw_x, draw_y, 1.2, Shine_Angle, Shine_image, TRUE);

			}
		}
		/////////////////////画像が重なるなら消す////////////////////////////////
		for (s = 0; s < Nut_MAX_Pieces; s++)//今までの画像を０から全部位置判別する
		{
			if (s != pos_Item_No)//自身以外該当
			{
				if (Nuts_active[s] == TRUE)//その中の表示されているものが
				{
					s_posX1 = Nuts_Position_X[s] - 50;//左
					s_posX2 = Nuts_Position_X[s] + 50;//右
					s_posY1 = Nuts_Position_Y[s] - 50;//上
					s_posY2 = Nuts_Position_Y[s] + 50;//下
					//↑と今表示させたい画像の座標が重ならないか
					if (s_posX1 < Nuts_Position_X[pos_Item_No] && Nuts_Position_X[pos_Item_No] < s_posX2 && s_posY1 < Nuts_Position_Y[pos_Item_No] && Nuts_Position_Y[pos_Item_No] < s_posY2)
					{
						// フィーバー木の実が重なっても消えないようにする
						if (Image_rand[pos_Item_No] != 8)
						{
							Nuts_active[pos_Item_No] = FALSE;//重なるなら消す
						}
					}
				}
			}
		}

	}
	/////////////////////////////////////////////////////////////////////////////

	int gauge_x = 30;           // ゲージの左上X座標
	int gauge_y = 150;          // ゲージの左上Y座標
	int gauge_width = 20;       // ゲージの幅
	int gauge_max_height = 300; // ゲージの最大高さ

	float ratio = 0.0f;         // ゲージの割合 (0.0 - 1.0)
	unsigned int gauge_color;   // ゲージの色

	if (Is_Fever == false)
	{
		// フィーバーではない時：集めた数の割合 (最大20)
		ratio = (float)Fresh_Nuts_Count / 5.0f;
		gauge_color = GetColor(0, 255, 100); // 緑色
	}
	else
	{
		// フィーバー中：残り時間の割合 (最大300フレーム = 60FPS × 5秒)
		ratio = (float)Fever_Time_Count / (60.0f * 5.0f);
		gauge_color = GetColor(255, 150, 0); // フィーバー中はオレンジ色
	}

	// 割合が0から1の範囲に収まるように
	if (ratio < 0.0f) ratio = 0.0f;
	if (ratio > 1.0f) ratio = 1.0f;

	// 現在のゲージの高さを計算
	int current_height = (int)(gauge_max_height * ratio);

	// ゲージの一番下のY座標
	int gauge_bottom_y = gauge_y + gauge_max_height;

	//ゲージの背景を描画
	DrawBox(gauge_x, gauge_y, gauge_x + gauge_width, gauge_bottom_y, GetColor(50, 50, 50), TRUE);

	//ゲージの中身を描画
	if (current_height > 0) {
		// Y座標の開始位置(底辺 - 現在の高さ)
		DrawBox(gauge_x, gauge_bottom_y - current_height, gauge_x + gauge_width, gauge_bottom_y, gauge_color, TRUE);
	}

	// 3. ゲージの枠線を描画
	DrawBox(gauge_x, gauge_y, gauge_x + gauge_width, gauge_bottom_y, GetColor(255, 255, 255), FALSE);

	// 4. 文字情報（ゲージの上に配置）
	if (Is_Fever)
	{
		DrawString(gauge_x - 15, gauge_y - 20, "FEVER!", GetColor(255, 255, 0));
		DrawRotaGraphF(450, 120, 0.8, 0, FEVER_img, TRUE);//フィーバーの表記
	}
	else
	{
		DrawFormatString(gauge_x - 5, gauge_y - 20, GetColor(255, 255, 255), "%d/5", Fresh_Nuts_Count);
	}
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

	
	if (image_id == 8) return 0;// フィーバー木の実のスコア

	int is_rot = (image_id % 2 == 1); // 奇数番号 = 腐っている
	switch (image_id / 2) // 0,1=seed 2,3=nuts 4,5=mush 6,7=walnut
	{
	case 0: return is_rot ? -20 : 10;   // seed（種）
	case 1: return is_rot ? -40 : 20;   // nuts（木の実）
	case 2: return is_rot ? -80 : 50;   // mush（きのこ）
	case 3: return is_rot ? -120 : 100;  // walnut（くるみ）
	default: return 10; 10;
	}
}

int NutsCheckCollect(float player_x, float player_y, float player_radius)//あたり判定
{
	for (pos_Item_No = 0; pos_Item_No < Nut_MAX_Pieces; pos_Item_No++)//最大数まで繰り返す
	{
		if (Nuts_active[pos_Item_No] == TRUE)//表示されているもの
		{
			float dx = player_x - Nuts_Position_X[pos_Item_No];
			float dy = player_y - Nuts_Position_Y[pos_Item_No];
			float distance = sqrtf(dx * dx + dy * dy);
			
			if (distance < player_radius + 16.0f) // 木の実の半径16px
			{
				Nuts_active[pos_Item_No] = FALSE; // 木の実を消す
				int score = GetNutsScore(Image_rand[pos_Item_No]); // スコアを取得する

				if (Is_Fever == false)//フィーバーではない時
				{
					bool triggerFever = false; //フィーバーを発動させるかどうかのフラグ

					//フィーバー木の実を取ったか
					if (Image_rand[pos_Item_No] == 8)
					{
						
						triggerFever = true; // フィーバー木の実ならフィーバータイム
					}
					else if (Image_rand[pos_Item_No] % 2 == 0)
					{
						// 新鮮な実を５個集めても発動
						Fresh_Nuts_Count++;
						if (Fresh_Nuts_Count >= 5)
						{
							triggerFever = true; // 5個集まってもフィーバータイム
						}
					}
					else if (Image_rand[pos_Item_No] % 2 == 1)
					{
						if (Fresh_Nuts_Count >= 0)
						{
							// 腐った実を取ったらマイナス
							Fresh_Nuts_Count--;
						}
						
					}

					//フィーバー中
						if (triggerFever == true) 
						{
							Is_Fever = true;
							Fever_Time_Count = 60 * 5; // 60FPSで5秒間
							Fresh_Nuts_Count = 0;      // カウントリセット
							TimerAddTime(3.0f); // 5秒プラス
							Backup_Nut_MAX_Pieces = Nut_MAX_Pieces; // 現在の最大表示数を保存

							Nut_MAX_Pieces = 800; // フィーバー中は綺麗な木の実だけたくさんだす

							for (int i = 0; i < 999; i++) {
								Backup_Nuts_active[i] = Nuts_active[i]; // 元の表示状態を保存
								Backup_Image_rand[i] = Image_rand[i];   // 元の画像番号を保存

								if (i < Nut_MAX_Pieces)
								{
									if (Backup_Nuts_active[i] == TRUE) 
									{
										// もともと表示されているものが腐った実なら消す
										if (Image_rand[i] % 2 != 0)
										{
											Nuts_IsVanishing[i] = TRUE;
										}
									}
									else 
									{
										// もともと非表示だった場所を「新鮮な実」として大量発生させる
										Nuts_active[i] = TRUE;
										Nuts_Scale[i] = 0.0f; // 0から拡大アニメーションさせる
										Image_rand[i] = (GetRand(3)) * 2; //新鮮な実だけにする
										Nuts_IsVanishing[i] = FALSE;
									}
								}
							}
						}
				}
	
				return GetNutsScore(Image_rand[pos_Item_No]); // 種類別スコア
			}
		}
	}
	return 0; // 収集なし
}

void NutsMagnetPull(float px, float py, float radius, float pull_speed)
{
	for (int i = 0; i < Nut_MAX_Pieces; i++)
	{
		if (Nuts_active[i] != TRUE) continue;
		if (Rot_Pieces[i] == TRUE) continue;
		float dx = px - Nuts_Position_X[i];
		float dy = py - Nuts_Position_Y[i];
		float dist = sqrtf(dx * dx + dy * dy);
		if (dist < radius && dist > 1.0f)
		{
			float nx = dx / dist;
			float ny = dy / dist;
			Nuts_Position_X[i] += (int)(nx * pull_speed);
			Nuts_Position_Y[i] += (int)(ny * pull_speed);
		}
	}
}

bool GetIsFever(void)//インゲームにフィーバーの判定を送る
{
	return Is_Fever; // フィーバー中ならTRUE
}


