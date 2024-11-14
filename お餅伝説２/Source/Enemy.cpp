#include <ctime>
#include<cstdlib>
#include <vector>
#include <tuple>
#include "Enemy.h"

/// @brief �S�Ă̓G�̏�����
void AllEnemyData::Init()
{
	normalenemy[0] = LoadGraph("Data/monster/c01_001.png", TRUE);
	normalenemy[1] = LoadGraph("Data/monster/c01_003.png", TRUE);
	normalenemy[2] = LoadGraph("Data/monster/c01_006.png", TRUE);
	normalenemy[3] = LoadGraph("Data/monster/c01_009.png", TRUE);
	normalenemy[4] = LoadGraph("Data/monster/c01_010.png", TRUE);
	normalenemy[5] = LoadGraph("Data/monster/c01_012.png", TRUE);
	normalenemy[6] = LoadGraph("Data/monster/c01_021.png", TRUE);
	normalenemy[7] = LoadGraph("Data/monster/c02_003.png", TRUE);
}

/// @brief �G�̃����_���Ȓ��o
/// @return�@���̃o�g���ɏo������G�̐���Ԃ�
std::vector<NormalEnemyName> AllEnemyData::Rand()
{
	std::srand(std::time(0));

	RandomExtraction = std::rand() % 4; // 0, 1, 2, 3 �̂����ꂩ

	// ���I���ꂽ���ɉ����ēG��I��
	switch (RandomExtraction)
	{
	case 0:
		// 1�̂������I
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 1:
		// 2�̒��I
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 2:
		// 3�̒��I
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 3:
		// 4�̒��I
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	}
	return enemies;
}

/// @brief �G�������_���ɑI�΂ꂽ���ʁA�`��ƃX�e�[�^�X�𐶐�
/// @return �G�̐����t���O
/// �X�e�[�^�X�̐����i�Œ�j
void AllEnemyData::Draw(EnemyNamberDraw& enemycoordinate)
{
	EnemyStutas stutas;

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies.size() == 0)
		{
			enemycoordinate.monsterX = 450; enemycoordinate.monster2X = 750; enemycoordinate.monsterY = 250; enemycoordinate.monster2Y = 500;
		}
		if (enemies.size() == 1)
		{
			enemycoordinate.monsterX =800; enemycoordinate.monster2X = 1100; enemycoordinate.monsterY = 250; enemycoordinate.monster2Y = 500;
		}
		if (enemies.size() == 2)
		{
			enemycoordinate.monsterX = 1150; enemycoordinate.monster2X = 1450; enemycoordinate.monsterY = 250; enemycoordinate.monster2Y = 500;
		}
		if (enemies.size() == 3)
		{
			enemycoordinate.monsterX = 1500; enemycoordinate.monster2X = 1800; enemycoordinate.monsterY = 250; enemycoordinate.monster2Y = 500;
		}
		//�h���S����������
		if (NormalEnemyName::DRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[0], TRUE);
			{
				stutas.AGI = 80;
				stutas.ATK = 100;
				stutas.DEF = 60;
				stutas.MP = 60;
				stutas.HP = 700;
				stutas.ALIVEENEMY = true;
			}
		}
		//�Ԃ��h���S����������
		if (NormalEnemyName::REDDRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[1], TRUE);
			{
				stutas.AGI = 200;
				stutas.ATK = 300;
				stutas.DEF = 200;
				stutas.MP = 150;
				stutas.HP = 1500;
				stutas.ALIVEENEMY = true;
			}
		}
		//�S�[������������
		if (NormalEnemyName::GOLEM == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[2], TRUE);
			{
				stutas.AGI = 20;
				stutas.ATK = 60;
				stutas.DEF = 160;
				stutas.MP = 200;
				stutas.HP = 600;
				stutas.ALIVEENEMY = true;
			}
		}
		//�~�j�h���S����������
		if (NormalEnemyName::MINIDRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[3], TRUE);
			{
				stutas.AGI = 20;
				stutas.ATK = 30;
				stutas.DEF = 60;
				stutas.MP = 50;
				stutas.HP = 110;
				stutas.ALIVEENEMY = true;
			}
		}
		//�h���S���o�[����������
		if (NormalEnemyName::DRAGONBARN == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[4], TRUE);
			stutas.AGI = 200;
			stutas.ATK = 60;
			stutas.DEF = 70;
			stutas.MP = 300;
			stutas.HP = 260;
			stutas.ALIVEENEMY = true;
		}
		//�L������������
		if (NormalEnemyName::CHIMWRA == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[5], TRUE);
			{
				stutas.AGI = 20;
				stutas.ATK = 60;
				stutas.DEF = 160;
				stutas.MP = 200;
				stutas.HP = 600;
				stutas.ALIVEENEMY = true;
			}
		}
		//�h���S�����C�h��������
		if (NormalEnemyName::DRAGONSERVANT == enemies[i])
		{
			DxLib::DrawExtendGraph(enemycoordinate.monsterX, enemycoordinate.monsterY, enemycoordinate.monster2X, enemycoordinate.monster2Y, normalenemy[6], TRUE);
			{
				stutas.AGI = 100;
				stutas.ATK = 30;
				stutas.DEF = 110;
				stutas.MP = 600;
				stutas.HP = 100;
				stutas.ALIVEENEMY = true;
			}
		}
	}
}
