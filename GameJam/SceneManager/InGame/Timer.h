#pragma once

// 制限時間タイマー
void TimerInit(float limitSeconds);
void TimerUpdate(float deltaTime);
int  TimerGetRemainingTime(void);  // 残り秒数（整数）
int  TimerIsTimeUp(void);          // 1: 時間切れ, 0: まだ
