#include "Title.h"
#include "../../Utility/InputManager.h"
#include "../Result/Result.h"
#include "Dxlib.h"

int Cr = GetColor(255, 190, 0);//色設定
int title_image;//タイトル画像
int cursor_number;//矢印の位置
int blink_flag;//矢印のフラグ
int TitleBGM;//タイトルのBGM
int PushSE;//決定音
int Movement;//カーソルを動かしたときの音
int Title_image;//タイトルの背景画像


/// <summary>
/// 初期化処理
/// </summary>
/// <param name=""></param>
void TitleInit(void)
{
	//効果音を読み込む
	TitleBGM = LoadSoundMem("sounds/BGM/Title_bgm.mp3");
	PushSE = LoadSoundMem("sounds/SE/buutton.wav");
	Movement = LoadSoundMem("sounds/SE/menu_move.wav");
	//画像を読み込む
	Title_image = LoadGraph("images/background/bg_natural_mori.jpg");
	title_image = LoadGraph("images/Title/Titlelogo.png");
	cursor_number = 0;//矢印の位置を一番目にする
	InitFlag();//フラグ初期化を呼び出す

	blink_flag = TRUE;//矢印フラグをいれる
}

/// <summary>
/// タイトル更新処理
/// </summary>
/// <returns>何もなければタイトル画面を維持する</returns>
eSceneType TitleUpdate()
{
	if (CheckSoundMem(TitleBGM) != TRUE)
	{
		PlaySoundMem(TitleBGM, DX_PLAYTYPE_BACK);
	}
	//スティックを下にしたら矢印を下に動かす
	if (GetKeyInputState(KEY_INPUT_UP) == ePress || GetControllerState(eStick) == eUP)
	{
		cursor_number--;
		WaitTimer(200);
		PlaySoundMem(Movement, DX_PLAYTYPE_BACK);
		//矢印が一番下なら一番上に戻す
		if (cursor_number < 0)
		{
			cursor_number = 3;
		}
	}

	//スティックを上にしたら矢印を上に動かす
	if (GetKeyInputState(KEY_INPUT_DOWN) == ePress || GetControllerState(eStick) == eDOWN)
	{
		cursor_number++;
		WaitTimer(200);
		PlaySoundMem(Movement, DX_PLAYTYPE_BACK);
		//矢印が一番上なら一番下に戻す
		if (cursor_number > 3)
		{
			cursor_number = 0;
		}
	}
	//スペース押されたらインゲーム画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		PlaySoundMem(PushSE, DX_PLAYTYPE_BACK);
		StopSoundMem(TitleBGM);
		return eInGame;
	}
	//Bボタンを押されたら特定の画面に遷移する
	if (GetKeyInputState(KEY_INPUT_Z) == ePress|| GetControllerState(eButtonA)==ePress)
	{
		switch (cursor_number)
		{
		case 0://一番上はHelp画面に
			PlaySoundMem(PushSE, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			return eInGame;
		case 1://真ん中はランキング画面に
			PlaySoundMem(PushSE, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			return eHelp;
		case 2://一番下は終了
			PlaySoundMem(PushSE, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			return eRanking;
		case 3:
			PlaySoundMem(PushSE, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			return eEnd;
		}
	}
	return eTitle;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name=""></param>
void TitleDraw(void)
{
	DrawGraph(0, 0, Title_image, TRUE);
	//文を描画する
	DrawGraph(-680, -280, title_image,TRUE);
	DrawString(200, 450, "ゲーム開始",Cr);
	DrawString(200, 500, "ヘルプ", Cr);
	DrawString(200, 550, "ランキング", Cr);
	DrawString(200, 600, "ゲームを終わる", Cr);

	//矢印の描画処理
	if (blink_flag == TRUE)
	{
		int cy = cursor_number * 50;//カーソル番号ごとに縦に動かす
		DrawTriangle(180, 450 + cy, 190, 460 + cy, 180, 470 + cy, GetColor(255, 0, 0), TRUE);//矢印を描画する
	}

}