#include "Help.h"
#include<DxLib.h>

#include"../../Utility/InputManager.h"

//色を指定
int R = GetColor(255, 120, 0);
int C = GetColor(255, 190, 0);
int K = GetColor(0, 255, 0);
int B = GetColor(100, 0, 255);
int T = GetColor(255, 0, 180);
int CancelSE;//戻る際の効果音
int Help_back;//ヘルプ画面の背景

/// <summary>
/// 更新処理
/// </summary>
eSceneType HelpUpdate(void)
{
	CancelSE = LoadSoundMem("sounds/SE/cancel.wav");//効果音を読み込む
	Help_back = LoadGraph("images/background/Nature-others-7.png");//画像を読み込む
	//Aボタンが押されたらタイトルへ
	if (GetControllerState(eButtonB) == ePress)
	{
		PlaySoundMem(CancelSE, DX_PLAYTYPE_BACK);//効果音を鳴らす
		return eTitle;
	}

	//留まる
	return eHelp;
}

/// <summary>
/// 描画処理
/// </summary>
void HelpDraw(void)
{
	DrawGraph(0, 0, Help_back, TRUE);
	//文を描画する
	DrawString(200, 180, "リスを動かして木の実を集めよう！", R);
	DrawString(200, 230, "Lスティックでリスを動かします", C);
	DrawString(200, 260, "木の実やキノコに触れると得点がもらえます", C);
	DrawString(200, 290, "傷んでいる木の実やキノコに触れると得点が下がってしまいます", C);
	DrawString(200, 320, "たくさん木の実を集めたり、特殊な木の実を取るとフィーバータイムになります", C);
	DrawString(200, 350, "得点を消費することでアイテム寄せ、ダッシュ、得点ブーストが使えます", C);
	DrawString(200, 400, "Xボタン:アイテム寄せ　Yボタン:ダッシュ　Bボタン:得点ブースト", K);
	DrawString(200, 450, "稼いだスコアは最後にどんぐりに変換されます", T);
	DrawString(200, 470, "たくさんスコアを稼いで山盛りのどんぐりを目指そう!", T);
	DrawString(200, 540, "Bボタンでタイトルに戻ります", B);
}

