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
	bool ALIVEENEMY;
};

struct EnemyNamberDraw
{
	int monsterX = 750;
	int monster2X = 1050;
	int monsterY = 250;
	int monster2Y = 500;
};
class AllEnemyData
{
private:
	int RandomExtraction = 0;
	
public:

	
	std::vector<NormalEnemyName> enemies;
	int normalenemy[9];	
	// ������
	void Init();
	// �����_���ɓG�𒊏o���ĕԂ�
	std::vector<NormalEnemyName> Rand();
	//�G�ɂ���Ă̕`��ƃX�e�[�^�X�̐���	
	void Draw(EnemyNamberDraw& enemycoordinate);


	
};
