#pragma once
#include "../Utility/Geometry.h"
struct TargetObject
{
	int is_active;
	Position2D position;
	CapsuleCollision collision;
	float Tvectol_X;
	float Tvectol_Y;
};

void NutsInit(void);
void NutsDraw(void);
//void NutsUpdate(void);
void NutsDestroy(void);
