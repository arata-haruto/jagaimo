#include "Title.h"
#include "../../Utility/InputManager.h"
#include "Dxlib.h"

int Cr = GetColor(255, 255, 255);
int title_image;
int cursor_number;
int blink_flag;
float blink_time;

void TitleInit(void)
{
	cursor_number = 0;

	blink_time = TRUE;
	blink_time = 0.0f;
}

eSceneType TitleUpdate()
{
	if (GetKeyInputState(KEY_INPUT_UP) == ePress || eStick == eUP)
	{
		cursor_number--;
		if (cursor_number < 0)
		{
			cursor_number = 2;
		}
	}

	if (GetKeyInputState(KEY_INPUT_DOWN) == ePress || eStick == eDOWN)
	{
		cursor_number++;
	}
	//スペース押されたらインゲーム画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eInGame;
	}
	if (GetKeyInputState(KEY_INPUT_Z) == ePress)
	{
		switch (cursor_number)
		{
		case 0:
			return eHelp;
		case 1:
			return eRanking;
		case 2:
			return eEnd;
		}
	}
	return eTitle;
}

void TitleDraw(void)
{
	
	DrawString(200, 200, "タイトル",Cr);
	DrawString(200, 300, "スペースでインゲーム",Cr);
	DrawString(200, 500, "ヘルプ", Cr);
	DrawString(200, 550, "ランキング", Cr);
	DrawString(200, 600, "ゲームを終わる", Cr);

}