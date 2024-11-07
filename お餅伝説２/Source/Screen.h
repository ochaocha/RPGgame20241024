#pragma once
#include "DxLib.h"
#include <time.h>
#include <stdlib.h>
#include "player.h"




class MeinScreenChanger
{
public:
	
	static const int startefectheight = 18;
	static const int startefectwidht = 26;

	

	int mauseX=0, mauseY=0;


	int ColorHandle = 100;

	int ColorHandleChange = 255;
	/// @brief スタート画面関数
	/// @param change 
	/// @param MeinscreenVertical 
	/// @param MeinscreenHorizontal 
	void Draw(int* screenchange);	
	
};

static MeinScreenChanger Mein;

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
	/// @brief キャラクターのステータスなどの情報を描画
	void PlayerInfomationDraw();
	/// @brief 敵キャラクターの描画
	void EnemyDraw();

};

static BattleScreenChanger Battle;
