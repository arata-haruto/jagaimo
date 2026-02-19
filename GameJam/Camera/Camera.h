#pragma once

#define D_MAP_WIDTH  (2880.0f)
#define D_MAP_HEIGHT (2160.0f)

void CameraInit(void);
void CameraUpdate(float target_x, float target_y);

float CameraGetX(void);
float CameraGetY(void);
