#include "Camera.h"
#include "../System/ProjectConfig.h"

// カメラが動き始めるデッドゾーン
#define D_DEAD_ZONE_X (80.0f)
#define D_DEAD_ZONE_Y (60.0f)

static float camera_x;
static float camera_y;

void CameraInit(void)
{
	camera_x = 0.0f;
	camera_y = 0.0f;
}

void CameraUpdate(float target_x, float target_y)
{
	// 画面上でのプレイヤーの位置（カメラからの相対座標）
	float player_screen_x = target_x - camera_x;
	float player_screen_y = target_y - camera_y;

	// 画面中央からのずれ
	float offset_x = player_screen_x - (D_WINDOW_SIZE_X / 2.0f);
	float offset_y = player_screen_y - (D_WINDOW_SIZE_Y / 2.0f);

	// デッドゾーンを超えた分だけカメラを移動
	if (offset_x > D_DEAD_ZONE_X)
	{
		camera_x += offset_x - D_DEAD_ZONE_X;
	}
	else if (offset_x < -D_DEAD_ZONE_X)
	{
		camera_x += offset_x + D_DEAD_ZONE_X;
	}

	if (offset_y > D_DEAD_ZONE_Y)
	{
		camera_y += offset_y - D_DEAD_ZONE_Y;
	}
	else if (offset_y < -D_DEAD_ZONE_Y)
	{
		camera_y += offset_y + D_DEAD_ZONE_Y;
	}

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
