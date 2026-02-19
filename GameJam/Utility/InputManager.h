#pragma once

enum eInputState
{
	eNone,
	ePress,
	eRelease,
	eHold,
	eUP,
	eDOWN,
};

enum eInputController
{
	eStick,
	eButtonA,
	eButtonB
};


void InputManagerUpdate(void);
eInputState GetKeyInputState(int Keycode);

//コントローラーの入力状態取得
eInputState GetControllerState(eInputController button);










