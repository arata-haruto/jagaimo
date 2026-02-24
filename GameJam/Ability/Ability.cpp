#include "Ability.h"
#include "../Utility/InputManager.h"
#include "../SceneManager/InGame/ScoreManager.h"
#include "../System/ProjectConfig.h"
#include "DxLib.h"

#define MAGNET_COST   50
#define MAGNET_TIME   4.0f
#define DASH_COST     20
#define DASH_TIME     1.5f
#define DASH_SPEED    3.0f
#define BOOST_COST    100
#define BOOST_TIME    5.0f
#define BOOST_MUL     2

static float magnetTimer;
static float dashTimer;
static float boostTimer;

static XINPUT_STATE pad_now;
static XINPUT_STATE pad_old;

static int PadPressed(int button)
{
	return (pad_old.Buttons[button] == 0 && pad_now.Buttons[button] != 0);
}

void AbilityInit(void)
{
	magnetTimer = 0.0f;
	dashTimer   = 0.0f;
	boostTimer  = 0.0f;
	pad_now = {};
	pad_old = {};
}

void AbilityUpdate(float deltaTime)
{
	pad_old = pad_now;
	GetJoypadXInputState(DX_INPUT_PAD1, &pad_now);

	int score = ScoreGetTotal();

	// Magnet: Z key or X button
	if (magnetTimer <= 0.0f)
	{
		if ((GetKeyInputState(KEY_INPUT_Z) == ePress) || PadPressed(XINPUT_BUTTON_X))
		{
			if (score >= MAGNET_COST)
			{
				ScoreAdd(-MAGNET_COST);
				magnetTimer = MAGNET_TIME;
			}
		}
	}

	// Dash: X key or Y button
	if (dashTimer <= 0.0f)
	{
		if ((GetKeyInputState(KEY_INPUT_X) == ePress) || PadPressed(XINPUT_BUTTON_Y))
		{
			if (score >= DASH_COST)
			{
				ScoreAdd(-DASH_COST);
				dashTimer = DASH_TIME;
			}
		}
	}

	// Boost: C key or B button
	if (boostTimer <= 0.0f)
	{
		if ((GetKeyInputState(KEY_INPUT_C) == ePress) || PadPressed(XINPUT_BUTTON_B))
		{
			if (score >= BOOST_COST)
			{
				ScoreAdd(-BOOST_COST);
				boostTimer = BOOST_TIME;
			}
		}
	}

	if (magnetTimer > 0.0f) magnetTimer -= deltaTime;
	if (dashTimer > 0.0f)   dashTimer   -= deltaTime;
	if (boostTimer > 0.0f)  boostTimer  -= deltaTime;
}

void AbilityDraw(void)
{
	int x = D_WINDOW_SIZE_X - 200;
	int y = 10;
	unsigned int white = GetColor(255, 255, 255);
	unsigned int yellow = GetColor(255, 255, 0);

	// Magnet
	if (magnetTimer > 0.0f)
		DrawFormatString(x, y, yellow, "[Z] Magnet  %.1fs", magnetTimer);
	else
		DrawFormatString(x, y, white, "[Z] Magnet  -%d", MAGNET_COST);

	y += 20;

	// Dash
	if (dashTimer > 0.0f)
		DrawFormatString(x, y, yellow, "[X] Dash    %.1fs", dashTimer);
	else
		DrawFormatString(x, y, white, "[X] Dash    -%d", DASH_COST);

	y += 20;

	// Boost
	if (boostTimer > 0.0f)
		DrawFormatString(x, y, yellow, "[C] Boost   %.1fs", boostTimer);
	else
		DrawFormatString(x, y, white, "[C] Boost   -%d", BOOST_COST);
}

int IsAbilityMagnetActive(void)
{
	return (magnetTimer > 0.0f) ? TRUE : FALSE;
}

float GetAbilityDashSpeedMul(void)
{
	return (dashTimer > 0.0f) ? DASH_SPEED : 1.0f;
}

int GetAbilityScoreMul(void)
{
	return (boostTimer > 0.0f) ? BOOST_MUL : 1;
}
