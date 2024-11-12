#pragma once

enum class EnemyName
{
	SLIME,          //0
	GOLEM,			//1
	CHIMERA,		//2
	DRAGON,			//3
	COUNT
};


class OllEnemyData
{
	int OneBattleEnemy=0;
private:

	EnemyName NormalEnemyRand();
};