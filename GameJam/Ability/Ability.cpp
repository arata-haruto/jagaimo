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

static eInputState GetButtonState(int button)
{
	int now_pressed = (pad_now.Buttons[button] == TRUE);
	int old_pressed = (pad_old.Buttons[button] == TRUE);

	if (old_pressed == FALSE && now_pressed == TRUE) return ePress;
	if (old_pressed == TRUE && now_pressed == TRUE)  return eHold;
	if (old_pressed == TRUE && now_pressed == FALSE)  return eRelease;
	return eNone;
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
		if (GetKeyInputState(KEY_INPUT_Z) == ePress ||
			GetButtonState(XINPUT_BUTTON_X) == ePress)
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
		if (GetKeyInputState(KEY_INPUT_X) == ePress ||
			GetButtonState(XINPUT_BUTTON_Y) == ePress)
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
		if (GetKeyInputState(KEY_INPUT_C) == ePress ||
			GetButtonState(XINPUT_BUTTON_B) == ePress)
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

// 1???A?r???e?B?p?l????`????w???p?[
static void DrawAbilityEntry(int x, int *y, const char *key, const char *name,
	int cost, float timer, float maxTime)
{
	int panelW = 210;
	int lineH = 22;
	int score = ScoreGetTotal();
	int isActive = (timer > 0.0f);

	// ?p?l??????: ????????Q?[?W?s??????2?s??
	int panelH = isActive ? (lineH * 2 + 6) : (lineH + 6);

	// ?????????w?i
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(x, *y, x + panelW, *y + panelH, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ?g??
	DrawBox(x, *y, x + panelW, *y + panelH, GetColor(80, 80, 80), FALSE);

	// ?e?L?X?g?F??????
	unsigned int textColor;
	if (isActive)
		textColor = GetColor(255, 255, 0);   // ??????: ???F
	else if (score >= cost)
		textColor = GetColor(255, 255, 255); // ??????\: ??
	else
		textColor = GetColor(100, 100, 100); // ?X?R?A?s??: ?O???[

	// ?A?r???e?B????R?X?g
	DrawFormatString(x + 6, *y + 3, textColor, "[%s] %s  -%dpt", key, name, cost);

	// ??????: ?Q?[?W?o?[ + ?c?ŠÎ??
	if (isActive)
	{
		int barX = x + 6;
		int barY = *y + lineH + 2;
		int barMaxW = 140;
		int barH = 14;
		float ratio = timer / maxTime;
		if (ratio > 1.0f) ratio = 1.0f;
		int barW = (int)(barMaxW * ratio);

		// ?Q?[?W?w?i(????O???[)
		DrawBox(barX, barY, barX + barMaxW, barY + barH, GetColor(40, 40, 40), TRUE);
		// ?Q?[?W?{??(??????F)
		unsigned int barColor = (ratio > 0.3f) ? GetColor(0, 200, 80) : GetColor(200, 200, 0);
		DrawBox(barX, barY, barX + barW, barY + barH, barColor, TRUE);
		// ?c?ŠÎ??e?L?X?g
		DrawFormatString(barX + barMaxW + 8, barY - 1, textColor, "%.1fs", timer);
	}

	*y += panelH + 2;
}

void AbilityDraw(void)
{
	// ‰æ–Ê‰º‚É‰¡•À‚Ñ‚Å”z’u
	int panelW = 210;
	int panelCount = 3;
	int gap = 8;
	int totalW = panelW * panelCount + gap * (panelCount - 1);
	int startX = (D_WINDOW_SIZE_X - totalW) / 2;  // ’†‰›‘µ‚¦
	int baseY = D_WINDOW_SIZE_Y - 60;

	int x, y;

	x = startX;
	y = baseY;
	DrawAbilityEntry(x, &y, "Z", "Magnet", MAGNET_COST, magnetTimer, MAGNET_TIME);

	x = startX + panelW + gap;
	y = baseY;
	DrawAbilityEntry(x, &y, "X", "Dash",   DASH_COST,   dashTimer,   DASH_TIME);

	x = startX + (panelW + gap) * 2;
	y = baseY;
	DrawAbilityEntry(x, &y, "C", "Boost",  BOOST_COST,  boostTimer,  BOOST_TIME);
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
