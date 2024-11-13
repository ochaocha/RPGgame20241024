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
	ATTACK,		//UŒ‚
	DEFENCE,	//–hŒä
	AGI,		//‘f‘‚³
	FLEE,		//“¦‚°‚é
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

	// ‰Šú‰»
	void Init();

	// ƒ‰ƒ“ƒ_ƒ€‚É“G‚ğ’Šo‚µ‚Ä•Ô‚·
	std::vector<NormalEnemyName> Rand();

	void Draw();
	void Stutus(EnemyStutas& stutas);
};
