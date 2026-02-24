#pragma once
#include "../Utility/Geometry.h"
//struct TargetObject
//{
//	int is_active;
//	Position2D position;
//	CapsuleCollision collision;
//	float Tvectol_X;
//	float Tvectol_Y;
//};

void NutsInit(void);
void NutsDraw(float camera_x, float camera_y);
void NutsUpdate(void);
void NutsDestroy(void);
Position2D GetNutsPosition(void);
void NutsMagnetPull(float player_x, float player_y, float radius, float pull_speed);
int NutsCheckCollect(float player_x, float player_y, float player_radius);
bool GetIsFever(void);