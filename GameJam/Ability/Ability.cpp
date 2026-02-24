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

class AbilityInputBridge
{
public:
	void Update(void)
	{
		old_pad = now_pad;
		GetJoypadXInputState(DX_INPUT_PAD1, &now_pad);
	}

	eInputState GetButtonState(int button) const
	{
		int now_pressed = (now_pad.Buttons[button] == TRUE);
		int old_pressed = (old_pad.Buttons[button] == TRUE);

		if (old_pressed == FALSE && now_pressed == TRUE)
			return ePress;
		if (old_pressed == TRUE && now_pressed == TRUE)
			return eHold;
		if (old_pressed == TRUE && now_pressed == FALSE)
			return eRelease;
		return eNone;
	}

private:
	XINPUT_STATE now_pad = {};
	XINPUT_STATE old_pad = {};
};

static AbilityInputBridge g_abil_input;

void AbilityInit(void)
{
	magnetTimer = 0.0f;
	dashTimer   = 0.0f;
	boostTimer  = 0.0f;
	g_abil_input = AbilityInputBridge();
}

void AbilityUpdate(float deltaTime)
{
	AbilityInputBridge* input = &g_abil_input;
	input->Update();

	int score = ScoreGetTotal();

	// Magnet: Z key or X button
	if (magnetTimer <= 0.0f)
	{
		if ((GetKeyInputState(KEY_INPUT_Z) == ePress) ||
			input->GetButtonState(XINPUT_BUTTON_X) == ePress)
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
		if ((GetKeyInputState(KEY_INPUT_X) == ePress) ||
			input->GetButtonState(XINPUT_BUTTON_Y) == ePress)
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
		if ((GetKeyInputState(KEY_INPUT_C) == ePress) ||
			input->GetButtonState(XINPUT_BUTTON_B) == ePress)
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
	int slotW = 200;
	int slotH = 50;
	int gap = 10;
	int totalW = slotW * 3 + gap * 2;
	int panelX = (D_WINDOW_SIZE_X - totalW) / 2 - 10;
	int panelY = D_WINDOW_SIZE_Y - slotH - 20;
	int panelW = totalW + 20;
	int panelH = slotH + 10;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	unsigned int white  = GetColor(255, 255, 255);
	unsigned int yellow = GetColor(255, 255, 0);
	unsigned int green  = GetColor(0, 200, 0);
	int barMaxW = slotW - 10;
	int barH = 6;

	SetFontSize(18);

	int sx = panelX + 10;
	int sy = panelY + 5;

	// Magnet (X button)
	if (magnetTimer > 0.0f)
	{
		DrawFormatString(sx, sy, yellow, "[X] Magnet %.1fs", magnetTimer);
		float ratio = magnetTimer / MAGNET_TIME;
		DrawBox(sx, sy + 24, sx + barMaxW, sy + 24 + barH, GetColor(60, 60, 60), TRUE);
		DrawBox(sx, sy + 24, sx + (int)(barMaxW * ratio), sy + 24 + barH, green, TRUE);
	}
	else
		DrawFormatString(sx, sy, white, "[X] Magnet  -%d", MAGNET_COST);

	sx += slotW + gap;

	// Dash (Y button)
	if (dashTimer > 0.0f)
	{
		DrawFormatString(sx, sy, yellow, "[Y] Dash %.1fs", dashTimer);
		float ratio = dashTimer / DASH_TIME;
		DrawBox(sx, sy + 24, sx + barMaxW, sy + 24 + barH, GetColor(60, 60, 60), TRUE);
		DrawBox(sx, sy + 24, sx + (int)(barMaxW * ratio), sy + 24 + barH, green, TRUE);
	}
	else
		DrawFormatString(sx, sy, white, "[Y] Dash    -%d", DASH_COST);

	sx += slotW + gap;

	// Boost (B button)
	if (boostTimer > 0.0f)
	{
		DrawFormatString(sx, sy, yellow, "[B] Boost %.1fs", boostTimer);
		float ratio = boostTimer / BOOST_TIME;
		DrawBox(sx, sy + 24, sx + barMaxW, sy + 24 + barH, GetColor(60, 60, 60), TRUE);
		DrawBox(sx, sy + 24, sx + (int)(barMaxW * ratio), sy + 24 + barH, green, TRUE);
	}
	else
		DrawFormatString(sx, sy, white, "[B] Boost   -%d", BOOST_COST);

	SetFontSize(16);
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
