#include "Title.h"
#include "../../Utility/InputManager.h"
#include "Dxlib.h"

int Cr = GetColor(255, 255, 255);
void TitleInit(void)
{
	
}

eSceneType TitleUpdate(void)
{
	return eTitle;
}
void TitleDraw(void)
{
	
	DrawString(200, 200, "Title",Cr);
}