#include "InGame.h"
#include "../../Utility/InputManager.h"
#include "../../Player/Player.h"
#include "../../Camera/Camera.h"
#include "DxLib.h"

int Cr2 = GetColor(255, 255, 255);
void InGameInit(void)
{
	PlayerInit();
	CameraInit();
}

eSceneType InGameUpdate(void)
{
	PlayerUpdate();

	Position2D pos = GetPlayerPosition();
	CameraUpdate(pos.x, pos.y);

	//スペース押されたらリザルト画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eResult;
	}
	 return eInGame;
}
void InGameDraw(void)
{
	PlayerDraw(CameraGetX(), CameraGetY());
	DrawString(200, 400, "スペースでリザルト", Cr2);
	DrawString(200, 200, "インゲーム",Cr2);
}