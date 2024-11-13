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
	ATTACK,		//�U��
	DEFENCE,	//�h��
	AGI,		//�f����
	FLEE,		//������
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

	// ������
	void Init();

	// �����_���ɓG�𒊏o���ĕԂ�
	std::vector<NormalEnemyName> Rand();

	void Draw();
	void Stutus(EnemyStutas& stutas);
};
