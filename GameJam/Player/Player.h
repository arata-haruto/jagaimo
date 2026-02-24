#pragma once
#include "../Utility/Geometry.h"

void PlayerInit(void);
void PlayerUpdate(void);
void PlayerDraw(float camera_x, float camera_y);

Position2D GetPlayerPosition(void);
float GetPlayerRadius(void);
void PlayerSetSpeedMultiplier(float mul);
