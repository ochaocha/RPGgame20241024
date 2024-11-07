#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;
public:
	/// @brief キャラクターのステータスなどの情報を描画
	void PlayerInfomationDraw();
	//エンカウントの実装
	void InputEncounterCheck(
		bool& BattleEncounterFlag,
		bool moveLeft,
		bool moveRight,
		bool moveDown,
		bool moveUp
	);

	void PlayerEncounterCheck(int* battlescreen);
};

class BattleScreenChanger
{
private:
	int BattleProbability;
	bool BattlePlayFlag = false;
	int BattleScreenHandle = LoadGraph("Data/img/pipo-bg004c.png");

public:

	

	/// @brief バトル画面の描画
	void Scene();
	/// @brief キャラクターの描画
	void PlayerDraw();
	/// @brief 敵キャラクターの描画
	void EnemyDraw();
	/// @brief 
	void UI();

};

static BattleScreenChanger Battle;
