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
	ATK,		//攻撃
	DEF,	//防御
	AGI,		//素早さ
	MP,			//マジックポイント
};


class AllEnemyData
{
private:
	int RandomExtraction = 0;
	int monsterX = 300;
	int monster2X = 450;
	int monsterY = 300;
	int monster2Y = 450;
	bool AliveEnemy = false;
	int EnemyHp=0;
	
public:
	std::vector<EnemyStutas> stutas;
	std::vector<NormalEnemyName> enemies;
	int normalenemy[9];

	std::vector<EnemyStutas>StutasGeneration();
	// 初期化
	void Init();
	// ランダムに敵を抽出して返す
	std::vector<NormalEnemyName> Rand();

	void Draw();
};
