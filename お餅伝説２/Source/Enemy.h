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
	ATK,		//�U��
	DEF,	//�h��
	AGI,		//�f����
	MP,			//�}�W�b�N�|�C���g
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
	// ������
	void Init();
	// �����_���ɓG�𒊏o���ĕԂ�
	std::vector<NormalEnemyName> Rand();

	void Draw();
};
