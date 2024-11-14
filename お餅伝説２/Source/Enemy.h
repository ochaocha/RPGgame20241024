#pragma once
#include "DxLib.h"


//�G�̖��O
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

//	�G�̃X�e�[�^�X
struct EnemyStutas
{
	int ATK;		//�U��
	int DEF;		//�h��
	int AGI;		//�f����
	int MP;		    //�}�W�b�N�|�C���g
	int HP;			//�q�b�g�|�C���g
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
	std::vector<NormalEnemyName> enemies;
	int normalenemy[9];	
	// ������
	void Init();
	// �����_���ɓG�𒊏o���ĕԂ�
	std::vector<NormalEnemyName> Rand();
	//�G�ɂ���Ă̕`��ƃX�e�[�^�X�̐���
	void Draw();
};
