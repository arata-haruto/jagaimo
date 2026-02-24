#include "Help.h"
#include<DxLib.h>

#include"../../Utility/InputManager.h"

//色を指定
int C = GetColor(255, 255, 255);
int CancelSE;

/// <summary>
/// 更新処理
/// </summary>
eSceneType HelpUpdate(void)
{
	CancelSE = LoadSoundMem("sounds/SE/cancel.wav");
	//Aボタンが押されたらタイトルへ
	if (GetControllerState(eButtonA) == ePress)
	{
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
	//文を描画する
	DrawString(200, 180, "リスを動かして木の実を集めよう！", C);
	DrawString(200, 230, "Lスティックでリスを動かします", C);
	DrawString(200, 260, "木の実やキノコに触れると得点がもらえます", C);
	DrawString(200, 290, "傷んでいる木の実やキノコに触れると得点が下がってしまいます", C);
	DrawString(200, 320, "稼いだスコアは最後にどんぐりに変換されます", C);
	DrawString(200, 400, "たくさんスコアを稼いで山盛りのどんぐりを目指そう!", C);
	DrawString(200, 470, "Aボタンでタイトルに戻ります", C);
}

