#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;

	float Time;
	float randomEncounter;
public:
	/// @brief キャラクターのステータスなどの情報を描画
	void PlayerInfomationDraw();
	void PlayerStatusInit();
	/// @brief 
	void StatusUI(int& CharacterHP,
		int& CharacterATK,
		int& CharacterMP,
		int& CharacterDEF,
		int& CharacterLUCK,
		int& CharacterAGI);
	void InputEncounterCheck(
		bool& battle,
		int* screenchange,
		bool MoveLeftCount,
		bool MoveRightCount,
		bool MoveDownCount,
		bool MoveUpCount
	);
};

class BattleScreenChanger
{
private:
	int BattleProbability;
	bool BattlePlayFlag = false;
	int BattleScreenHandle;
	int BattleUIStatusHandle;
	int BattleUICommandHandle;
	int BattleUIAttckTypeHandle;
public:

	/// @brief バトル画面の描画
	void Scene();
	/// @brief キャラクターの描画
	void PlayerDraw();
	/// @brief 敵キャラクターの描画
	void EnemyDraw();
	/// @brief 画像の初期化
	void Imginitialize();
};
