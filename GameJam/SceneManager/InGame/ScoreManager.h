#pragma once

// スコア管理（グローバル関数）
void ScoreReset(void);           // スコアを0にリセット
void ScoreAdd(int points);       // 加点
int  ScoreGetTotal(void);        // 現在の合計スコアを取得
