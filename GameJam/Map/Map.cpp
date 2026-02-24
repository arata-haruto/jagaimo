#include "Map.h"
#include "../System/ProjectConfig.h"
#include "DxLib.h"

static int img_floor;
static int img_floor2;

void MapInit(void)
{
	img_floor = LoadGraph("Images/map/floor.png");
	img_floor2 = LoadGraph("Images/map/floor_02.png");
}

void MapDraw(float camera_x, float camera_y)
{
	int tile_w = 64;
	int tile_h = 64;
	int start_tx = (int)(camera_x / tile_w);
	int start_ty = (int)(camera_y / tile_h);
	int end_tx = (int)((camera_x + D_WINDOW_SIZE_X) / tile_w);
	int end_ty = (int)((camera_y + D_WINDOW_SIZE_Y) / tile_h);

	for (int ty = start_ty; ty <= end_ty; ty++)
	{
		for (int tx = start_tx; tx <= end_tx; tx++)
		{
			int draw_x = (int)(tx * tile_w - camera_x);
			int draw_y = (int)(ty * tile_h - camera_y);
			unsigned int hash = (unsigned int)(tx * 7919 + ty * 104729);
			int use_flower = ((hash % 10) < 3) ? TRUE : FALSE;
			DrawGraph(draw_x, draw_y, use_flower ? img_floor2 : img_floor, FALSE);
		}
	}
}
