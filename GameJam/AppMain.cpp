#include "DxLib.h"
#include "System/System.h"
#include "Utility/Inputmanager.h"
#include "SceneManager/SceneManager.h"

//メイン関数
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	//起動確認
	if (WakeUp() != TRUE)
	{
		//異常があった
		OutputDebugString("アプリケーションの初期化に失敗しました\n");
		return -1;
	}
	//シーンマネージャー機能の初期化
	SceneManagerInitialize();

	//ローカル変数
	float fixed_time = 0.0f;

	//メインループ（更新と描画をつかさどる）
	while (ProcessMessage() != -1 && IsFinish() != TRUE)
	{
		//１フレームあたりの時間を計測
		CalcFreamTime();

		//１フレームにかかった時間を加算
		fixed_time += GetDeltaSecond();

		//1/60s経過したときに更新の描画
		if (fixed_time >= (1.0f / 60.0f))
		{
			fixed_time = 0.0f;
			//入力値の更新
			InputManagerUpdate();

			//画面の初期化
			ClearDrawScreen();

			//シーンマネージャーの更新
			SceneManagerUpdate();

			//裏画面の内容を表画面に反映
			ScreenFlip();

			//ESOキーが離されたときに、ループを終了
			if (GetKeyInputState(KEY_INPUT_ESCAPE) == eRelease)
			{
				break;
			}
		}
	}
	//後始末
	ShutDown();
	//正常に終了したことを通知
	return 0;

}