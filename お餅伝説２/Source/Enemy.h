#pragma once
#include "DxLib.h"

enum class NormalEnemyName
{
	DRAGON,
	REDDRAGON,
	GOLEM,
	MINIDRAGON,
	DRAGONBARN,
	CHIMWRA,
	DRAGONSERVANT,
	COUNT
};

enum class EnemyStutas
{
	ATTACK,		//攻撃
	DEFENCE,	//防御
	AGI,		//素早さ
	FLEE,		//逃げる
};



class OllEnemyData
{
private:
	int RandomExtraction = 0;
	int monsterX = 300;
	int monster2X = 450;
	int monsterY = 300;
	int monster2Y = 450;

public:
	EnemyStutas* stutas;
	int normalenemy[9];

	// 初期化
	void Init();

	// ランダムに敵を抽出して返す
	std::vector<NormalEnemyName> Rand();

	void Draw();
	void Stutus(EnemyStutas& stutas);
};
