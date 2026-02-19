#include "InputManager.h"
#include "DxLib.h"

#define D_KEYCODE_MAX (256)

char now_key[D_KEYCODE_MAX];
char old_key[D_KEYCODE_MAX];

//コントローラーステックの座標
#define STICK_MAX	(32767.0f)	
#define STICK_MIN	(-32768.0f)	


//キーを取得
int CheckKeycodeRange(int keycode)
{
	if (0 <= keycode && keycode < D_KEYCODE_MAX)
	{
		return TRUE;
	}
	return FALSE;

}
//押された
void InputManagerUpdate(void)
{
	memcpy(old_key, now_key, (sizeof(char) * D_KEYCODE_MAX));
	GetHitKeyStateAll(now_key);
}

//キーの入力状態を取得
eInputState GetKeyInputState(int keycode)
{
	if (CheckKeycodeRange(keycode) == TRUE)
	{
		if (old_key[keycode] == TRUE)
		{
			if (now_key[keycode] == TRUE)
			{
				return eHold;
			}
			else
			{
				return eRelease;
			}
		}
		else
		{
			if (now_key[keycode] == TRUE)
			{
				return ePress;
			}
		}
	}

	return eNone;
}


eInputState GetControllerState(eInputController button)
{
	/*DINPUT_JOYSTATE input;

	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);*/

	XINPUT_STATE input_controller;

	//コントローラーの情報取得
	GetJoypadXInputState(DX_INPUT_PAD1, &input_controller);

	switch (button)
	{
	case eStick:

		float Stick_L;	//leftステックの入力値
		Stick_L = (float)input_controller.ThumbLY / STICK_MAX;	//leftステックの入力値取得

		//leftステックが上下どちらに傾いているかの判定
		if (Stick_L > 0.1)
		{
			return eUP;	//上に傾いていたら
		}
		else if (Stick_L < -0.1)
		{
			return eDOWN;	//下に傾いていたら
		}
		break;

	case eButtonA:
		//Buttons[12] = Aボタンの入力状況が格納されている配列
		if (input_controller.Buttons[12] == TRUE)
		{
			return ePress;	//Aボタンが押されていたら
		}
		else
		{
			return eNone;	//Aボタンが押されていなかったら
		}
		break;
	case eButtonB:
		//Buttons[12] = Bボタンの入力状況が格納されている配列
		if (input_controller.Buttons[13] == TRUE)
		{
			return ePress;	//Aボタンが押されていたら
		}
		else
		{
			return eNone;	//Aボタンが押されていなかったら
		}
		break;
	}
}




























