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

class OllEnemyData
{
	
private:
	int RandomExtraction=0;
	int OneBattleEnemy = 0;
	int monsterX =	300;
	int monster2X =	 450;
	int monsterY = 300;
	int monster2Y = 450;
public:

	
	int normalenemy[9];
	//èâä˙âª
	void Init();
	NormalEnemyName Rand();
	void  Draw();
	NormalEnemyName Stutus();
};
