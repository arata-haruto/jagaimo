#include "Title.h"
#include "../../Utility/InputManager.h"
#include "Dxlib.h"

int Cr = GetColor(255, 255, 255);
void TitleInit(void)
{
	
}

eSceneType TitleUpdate(void)
{

	//スペース押されたらインゲーム画面へ
	if (GetKeyInputState(KEY_INPUT_SPACE) == eRelease)
	{
		return eInGame;
	}
	return eTitle;
}
void TitleDraw(void)
{
	
	DrawString(200, 200, "タイトル",Cr);
	DrawString(200, 400, "スペースでインゲーム",Cr);

}