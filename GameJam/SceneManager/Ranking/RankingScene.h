#pragma once

#include "../SceneType.h"

int RankingInit(void);  //ランキングデータ読み込みとUI初期化
eSceneType RankingUpdate(void); //点滅や入力に応じた更新
void RankingDraw(void); //ランキング一覧の描画
void InitFlag(void);