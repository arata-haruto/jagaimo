#include "Player.h"
#include "../Utility/InputManager.h"
#include "../Camera/Camera.h"
#include "../System/ProjectConfig.h"
#include "DxLib.h"
#include <math.h>

#define D_PLAYER_WIDTH  (128.0f)
#define D_PLAYER_HEIGHT (128.0f)
#define D_PLAYER_SPEED  (4.0f)
#define D_ANIM_INTERVAL (10)

// 8方向: 0=右 1=右上 2=上 3=左上 4=左 5=左下 6=下 7=右下
static Position2D player_pos;
static int is_moving;
static int anim_frame;
static int player_dir;

static int img_right[2];     // 右
static int img_up[2];        // 上
static int img_down[2];      // 下
static int img_upper_left[2]; // 左上
static int img_lower_left[2]; // 左下
static int img_floor;
static int img_floor2;

#define D_STICK_DEADZONE (0.2f)
#define D_STICK_MAX (32767.0f)

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

	// 左スティックの値を -1.0?1.0 で返す
	float GetStickX(void) const
	{
		return (float)now_pad.ThumbLX / D_STICK_MAX;
	}
	float GetStickY(void) const
	{
		return (float)now_pad.ThumbLY / D_STICK_MAX;
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
	player_dir = 0;

	img_right[0] = LoadGraph("Images/Player/りす横.png");
	img_right[1] = LoadGraph("Images/Player/横2.png");
	img_up[0] = LoadGraph("Images/Player/上1.png");
	img_up[1] = LoadGraph("Images/Player/上2.png");
	img_down[0] = LoadGraph("Images/Player/下1.png");
	img_down[1] = LoadGraph("Images/Player/下2.png");
	img_upper_left[0] = LoadGraph("Images/Player/左上1.png");
	img_upper_left[1] = LoadGraph("Images/Player/左上2.png");
	img_lower_left[0] = LoadGraph("Images/Player/左下1.png");
	img_lower_left[1] = LoadGraph("Images/Player/左下2.png");
	img_floor = LoadGraph("Images/map/floor.png");
	img_floor2 = LoadGraph("Images/map/floor_02.png");
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

	// キーボード＋D-Pad入力
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

	// スティック入力（キーボード入力がなければスティックを使う）
	if (dx == 0.0f && dy == 0.0f)
	{
		float sx = input->GetStickX();
		float sy = -input->GetStickY(); // Y軸反転（スティック上が正、画面上が負）
		if (sx > D_STICK_DEADZONE || sx < -D_STICK_DEADZONE ||
			sy > D_STICK_DEADZONE || sy < -D_STICK_DEADZONE)
		{
			dx = sx;
			dy = sy;
		}
	}

	// 正規化（斜めでも同じ速度になるように）
	len = sqrtf(dx * dx + dy * dy);
	if (len > 1.0f)
	{
		dx /= len;
		dy /= len;
	}
	else if (len > 0.0f && len < 1.0f)
	{
		// スティックの傾き具合を速度に反映（軽く倒すと遅く）
		// ただしキーボードは常に1.0なのでそのまま
	}

	is_moving = (len > 0.001f) ? TRUE : FALSE;

	// 8方向の判定
	if (is_moving == TRUE)
	{
		if (dx > 0.4f && dy < -0.4f)       player_dir = 1; // 右上
		else if (dx < -0.4f && dy < -0.4f)  player_dir = 3; // 左上
		else if (dx < -0.4f && dy > 0.4f)   player_dir = 5; // 左下
		else if (dx > 0.4f && dy > 0.4f)    player_dir = 7; // 右下
		else if (dx > 0.4f)                  player_dir = 0; // 右
		else if (dx < -0.4f)                 player_dir = 4; // 左
		else if (dy < -0.4f)                 player_dir = 2; // 上
		else if (dy > 0.4f)                  player_dir = 6; // 下
	}

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

void PlayerDraw(float camera_x, float camera_y)
{
	// プレイヤー描画
	int px = (int)(player_pos.x - camera_x);
	int py = (int)(player_pos.y - camera_y);

	int anim_idx = (anim_frame / D_ANIM_INTERVAL) % 2;
	int img;
	int flip = FALSE;

	switch (player_dir)
	{
	case 0: // 右
		img = img_right[anim_idx];
		flip = FALSE;
		break;
	case 1: // 右上（左上画像を反転）
		img = img_upper_left[anim_idx];
		flip = TRUE;
		break;
	case 2: // 上
		img = img_up[anim_idx];
		flip = FALSE;
		break;
	case 3: // 左上
		img = img_upper_left[anim_idx];
		flip = FALSE;
		break;
	case 4: // 左（右画像を反転）
		img = img_right[anim_idx];
		flip = TRUE;
		break;
	case 5: // 左下
		img = img_lower_left[anim_idx];
		flip = FALSE;
		break;
	case 6: // 下
		img = img_down[anim_idx];
		flip = FALSE;
		break;
	case 7: // 右下（左下画像を反転）
		img = img_lower_left[anim_idx];
		flip = TRUE;
		break;
	default:
		img = img_right[0];
		flip = FALSE;
		break;
	}

	double scale = D_PLAYER_WIDTH / 128.0;
	DrawRotaGraphF((float)px, (float)py, scale, 0.0, img, TRUE, flip);

	// world position debug
	DrawFormatString(4, 4, GetColor(255, 255, 0),
		"X:%.0f / %.0f  Y:%.0f / %.0f",
		player_pos.x, D_MAP_WIDTH, player_pos.y, D_MAP_HEIGHT);
}

Position2D GetPlayerPosition(void)
{
	return player_pos;
}
