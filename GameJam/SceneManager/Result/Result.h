#pragma once

#include "../SceneType.h"
#include "../Ranking/RankingData.h"

void ResultInit(void);

eSceneType ResultUpdate(void);

void ResultDraw(void);

int GetScore(void);

int GetFlag(void);

void InitFlag(void);