#include"Nuts.h"
#include "../Utility/InputManager.h"
#include "DxLib.h"
#include <math.h>

int Nuts_active[99];
int Nuts_Position_X[99];
int Nuts_Position_Y[99];
int Nut_pieces[99];
int x;
void NutsInit(void)
{ 
	int x = 0;
	 Nuts_Position_X[0] =100;
	Nuts_Position_Y[0] =200;
	Nuts_active[0] = TRUE;
}

void NutsUpdate(void)
{
	//Nuts_active[x] = TRUE;
}

void NutsDraw(void)
{
	if (Nuts_active[x] == TRUE)
	{
	DrawCircle(100, 200,5, GetColor(0, 0, 255),TRUE);
	}
		DrawCircle(300, 200,5, GetColor(0, 0, 255),TRUE);
	
}

void NutsDestroy(void)
{ 
	Nuts_active[x] = FALSE;
}




















