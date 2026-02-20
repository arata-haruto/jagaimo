#include "Result.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

int Cr3 = GetColor(255, 255, 255);
void ResultInit(void)
{

}

eSceneType ResultUpdate(void)
{
	//スペース押されたらタイトル画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eTitle;
	}
	if (GetKeyInputState(KEY_INPUT_Z) == eRelease)
	{
		return eRanking;
	}
	return eResult;
}
void ResultDraw(void)
{
	DrawString(200, 200, "リザルト", Cr3);
	DrawString(200, 400, "スペースでタイトル", Cr3);
	DrawString(200, 600, "Zキーでランキング", Cr3);
}