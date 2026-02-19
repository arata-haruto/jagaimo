#include "Camera.h"
#include "../System/ProjectConfig.h"

static float camera_x;
static float camera_y;

void CameraInit(void)
{
	camera_x = 0.0f;
	camera_y = 0.0f;
}

void CameraUpdate(float target_x, float target_y)
{
	camera_x = target_x - (D_WINDOW_SIZE_X / 2.0f);
	camera_y = target_y - (D_WINDOW_SIZE_Y / 2.0f);

	if (camera_x < 0.0f)
	{
		camera_x = 0.0f;
	}
	if (camera_y < 0.0f)
	{
		camera_y = 0.0f;
	}
	if (camera_x > D_MAP_WIDTH - D_WINDOW_SIZE_X)
	{
		camera_x = D_MAP_WIDTH - D_WINDOW_SIZE_X;
	}
	if (camera_y > D_MAP_HEIGHT - D_WINDOW_SIZE_Y)
	{
		camera_y = D_MAP_HEIGHT - D_WINDOW_SIZE_Y;
	}
}

float CameraGetX(void)
{
	return camera_x;
}

float CameraGetY(void)
{
	return camera_y;
}
