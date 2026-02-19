#include "InGame.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

int Cr2 = GetColor(255, 255, 255);
void InGameInit(void)
{

}

eSceneType InGameUpdate(void)
{
	//スペース押されたらリザルト画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eResult;
	}
	 return eInGame;
}
void InGameDraw(void)
{
	DrawString(200, 400, "スペースでリザルト", Cr2);
	DrawString(200, 200, "インゲーム",Cr2);
}