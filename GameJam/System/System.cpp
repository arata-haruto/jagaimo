#include "System.h"
#include "ProjectConfig.h"
#include "DxLib.h"

LONGLONG old_time;
LONGLONG now_time;
float delta_second;

int WakeUp(void)
{
	//Log.Extを出力」しない
	SetOutApplicationLogValidFlag(FALSE);

	//Windowモードで起動
	ChangeWindowMode(TRUE);

	//ウィンドウのサイズ指定
	SetGraphMode(D_WINDOW_SIZE_X, D_WINDOW_SIZE_Y, D_COLOR_BIT);

	//非アクティブ時でも動作させる
	SetAlwaysRunFlag(TRUE);

	//DXライブラリの初期化
	if (DxLib_Init() == -1)
	{
		return FALSE;
	}

	//異常を検知
	SetDrawScreen(DX_SCREEN_BACK);

	old_time = 0LL;
	now_time = 0LL;
	delta_second = 0.0f;

	return TRUE;
}

void ShutDown(void)
{
	DxLib_End();
}

void CalcFreamTime(void)
{
	now_time = GetNowHiPerformanceCount();
	delta_second = (float)(now_time - old_time) * 1.0e-6f;
	old_time = now_time;
}

const float GetDeltaSecond(void)
{
	return delta_second;
}








