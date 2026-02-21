#include "SceneManager.h"
#include "SceneType.h"
#include "DxLib.h"

#include "Title/Title.h"
#include "InGame/InGame.h"
#include "Result/Result.h"
#include"Ranking/RankingScene.h"
#include"Help/Help.h"




eSceneType current_scene_type;
int is_end_flag;

void SceneManagerDraw(void);
void ChangeScene(eSceneType new_scene_type);
void SceneInit(eSceneType new_scene_type);


void SceneManagerInitialize(void)
{
	//終了フラグを折っておく
	is_end_flag = FALSE;
	//タイトルから始める
	ChangeScene(eTitle);
}


//メインループ
void SceneManagerUpdate(void)
{
	//現在シーンの更新
	eSceneType next_scene_type = eEnd;

	switch (current_scene_type)
	{
		//タイトル画面の更新
	case eTitle:
		next_scene_type = TitleUpdate();
		break;
		//インゲーム画面の更新
	case eInGame:
		next_scene_type = InGameUpdate();
		break;
	case eResult:
		next_scene_type = ResultUpdate();
		break;
	case eRanking:
		next_scene_type = RankingUpdate();
		break;
	case eHelp:
		next_scene_type = HelpUpdate();
		break;
	}
	//シーンの描画
	SceneManagerDraw();

	//現在のシーンと次のシーンが違っていたら、切り替える
	if (current_scene_type != next_scene_type)
	{
		ChangeScene(next_scene_type);
	}
}

int IsFinish(void)
{	//終了フラグの情報を返す
	return is_end_flag;
}

void SceneManagerDraw(void)
{
	//シーン情報に従って描画を行う
	switch (current_scene_type)
	{
		//タイトル画面の描画
	case eTitle:
		TitleDraw();
		break;
		//インゲーム画面の更新
	case eInGame:
		InGameDraw();
		break;
	case eResult:
		ResultDraw();
		break;
	case eRanking:
		RankingDraw();
		break;
	case eHelp:
		HelpDraw();
		break;
		//それ以外
	default:
		break;
	}

}

void ChangeScene(eSceneType new_scene_type)
{
	//１回だけ実行
	static int is_once = TRUE;

	//エンドに切り替えが必要か？
	if (is_once == TRUE)
	{
		is_once = FALSE;
		SceneInit(new_scene_type);
		return;
	}

	//終了フラグ
	if (new_scene_type == eEnd)
	{
		is_end_flag = TRUE;
		return;
	}

	//新しいシーンの初期化
	SceneInit(new_scene_type);
	//現在シーンの上書き
	current_scene_type = new_scene_type;
}

void SceneInit(eSceneType new_scene_type)
{
	switch (new_scene_type)
	{
	case eTitle:
		TitleInit();
		break;
	case eInGame:
		InGameInit();
		break;
	case eResult:
		ResultInit();
		break;
	case eRanking:
		RankingInit();
		break;
	case eHelp:
		HelpInit();
		break;
	default:
		break;

	}
}

















