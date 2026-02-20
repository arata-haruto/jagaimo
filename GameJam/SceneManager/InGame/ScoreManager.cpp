#include "ScoreManager.h"

// 合計スコア
static int totalScore = 0;

void ScoreReset(void)
{
	totalScore = 0;
}

void ScoreAdd(int points)
{
	totalScore += points;
}

int ScoreGetTotal(void)
{
	return totalScore;
}
