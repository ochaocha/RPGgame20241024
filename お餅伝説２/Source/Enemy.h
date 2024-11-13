#pragma once
#include "DxLib.h"

enum class NormalEnemyName
{
	SLIME,          //0
	GOLEM,			//1
	CHIMERA,		//2
	DRAGON,			//3
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
