#include "Player.h"
#include "../Utility/InputManager.h"
#include "../Camera/Camera.h"
#include "../System/ProjectConfig.h"
#include "DxLib.h"
#include <math.h>

#define D_PLAYER_WIDTH  (32.0f)
#define D_PLAYER_HEIGHT (32.0f)
#define D_PLAYER_SPEED  (4.0f)
#define D_ANIM_INTERVAL (10)

static Position2D player_pos;
static int is_moving;
static int anim_frame;

class PlayerInputBridge
{
public:
	void Update(void)
	{
		old_pad = now_pad;
		GetJoypadXInputState(DX_INPUT_PAD1, &now_pad);
	}

	eInputState GetKeyState(int keycode) const
	{
		return GetKeyInputState(keycode);
	}

	eInputState GetButtonState(int button) const
	{
		int now_pressed = (now_pad.Buttons[button] == TRUE);
		int old_pressed = (old_pad.Buttons[button] == TRUE);

		if (old_pressed == FALSE && now_pressed == TRUE)
		{
			return ePress;
		}
		if (old_pressed == TRUE && now_pressed == TRUE)
		{
			return eHold;
		}
		if (old_pressed == TRUE && now_pressed == FALSE)
		{
			return eRelease;
		}
		return eNone;
	}

private:
	XINPUT_STATE now_pad = {};
	XINPUT_STATE old_pad = {};
};

static PlayerInputBridge g_input;

void PlayerInit(void)
{
	player_pos.x = D_MAP_WIDTH / 2.0f;
	player_pos.y = D_MAP_HEIGHT / 2.0f;
	is_moving = FALSE;
	anim_frame = 0;
}

void PlayerUpdate(void)
{
	PlayerInputBridge* input = &g_input;
	float dx = 0.0f;
	float dy = 0.0f;
	float len;
	float half_w = D_PLAYER_WIDTH / 2.0f;
	float half_h = D_PLAYER_HEIGHT / 2.0f;

	input->Update();

	if (input->GetKeyState(KEY_INPUT_RIGHT) == ePress ||
		input->GetKeyState(KEY_INPUT_RIGHT) == eHold ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == ePress ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_RIGHT) == eHold)
	{
		dx += 1.0f;
	}

	if (input->GetKeyState(KEY_INPUT_LEFT) == ePress ||
		input->GetKeyState(KEY_INPUT_LEFT) == eHold ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == ePress ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_LEFT) == eHold)
	{
		dx -= 1.0f;
	}

	if (input->GetKeyState(KEY_INPUT_UP) == ePress ||
		input->GetKeyState(KEY_INPUT_UP) == eHold ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == ePress ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eHold)
	{
		dy -= 1.0f;
	}

	if (input->GetKeyState(KEY_INPUT_DOWN) == ePress ||
		input->GetKeyState(KEY_INPUT_DOWN) == eHold ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == ePress ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eHold)
	{
		dy += 1.0f;
	}

	if (dx != 0.0f && dy != 0.0f)
	{
		len = sqrtf(dx * dx + dy * dy);
		dx /= len;
		dy /= len;
	}

	is_moving = (dx != 0.0f || dy != 0.0f) ? TRUE : FALSE;

	player_pos.x += dx * D_PLAYER_SPEED;
	player_pos.y += dy * D_PLAYER_SPEED;

	if (player_pos.x < half_w)
	{
		player_pos.x = half_w;
	}
	if (player_pos.y < half_h)
	{
		player_pos.y = half_h;
	}
	if (player_pos.x > D_MAP_WIDTH - half_w)
	{
		player_pos.x = D_MAP_WIDTH - half_w;
	}
	if (player_pos.y > D_MAP_HEIGHT - half_h)
	{
		player_pos.y = D_MAP_HEIGHT - half_h;
	}

	if (is_moving == TRUE)
	{
		anim_frame++;
	}
	else
	{
		anim_frame = 0;
	}
}

void PlayerDraw(float camera_x, float camera_y)
{
	// scrolling grid
	int grid = 96;
	int ox = (int)fmodf(camera_x, (float)grid);
	int oy = (int)fmodf(camera_y, (float)grid);
	int grid_col = GetColor(50, 80, 50);
	for (int x = -ox; x <= D_WINDOW_SIZE_X; x += grid)
		DrawLine(x, 0, x, D_WINDOW_SIZE_Y, grid_col);
	for (int y = -oy; y <= D_WINDOW_SIZE_Y; y += grid)
		DrawLine(0, y, D_WINDOW_SIZE_X, y, grid_col);

	// player
	int draw_x = (int)(player_pos.x - camera_x);
	int draw_y = (int)(player_pos.y - camera_y);
	int half_w = (int)(D_PLAYER_WIDTH / 2.0f);
	int half_h = (int)(D_PLAYER_HEIGHT / 2.0f);
	int color = GetColor(255, 80, 80);

	DrawBox(draw_x - half_w, draw_y - half_h,
		draw_x + half_w, draw_y + half_h, color, TRUE);
	DrawBox(draw_x - half_w, draw_y - half_h,
		draw_x + half_w, draw_y + half_h,
		GetColor(255, 255, 255), FALSE);

	// world position debug
	DrawFormatString(4, 4, GetColor(255, 255, 0),
		"X:%.0f / %.0f  Y:%.0f / %.0f",
		player_pos.x, D_MAP_WIDTH, player_pos.y, D_MAP_HEIGHT);
}

Position2D GetPlayerPosition(void)
{
	return player_pos;
}
