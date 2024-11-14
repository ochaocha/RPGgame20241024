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

/// @brief �G�������_���ɑI�΂ꂽ���ʁA�`������X�e�[�^�X�𐶐�
/// @return �G�̃X�e�[�^�X
void AllEnemyData::Draw()
{
	EnemyStutas stutas;

	for (int i = 0; i < enemies.size(); i++)
	{
		//�h���S����������
		if (NormalEnemyName::DRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[0], TRUE);
			//�X�e�[�^�X�̐����i�Œ�j
			{
				stutas.AGI = 80;
				stutas.ATK = 100;
				stutas.DEF = 60;
				stutas.MP=60;
				stutas.HP=700;
			}
		}
		//�Ԃ��h���S����������
		if (NormalEnemyName::REDDRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[1], TRUE);
			{
				stutas.AGI =200;
				stutas.ATK = 300;
				stutas.DEF = 200;
				stutas.MP = 150;
				stutas.HP = 1500;
			}
		}
		//�S�[������������
		if (NormalEnemyName::GOLEM == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[2], TRUE);
			{
				stutas.AGI = 20;
				stutas.ATK = 60;
				stutas.DEF = 160;
				stutas.MP = 200;
				stutas.HP = 600;
			}
			
		}
		//�~�j�h���S����������
		if (NormalEnemyName::MINIDRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[3], TRUE);
			{
				stutas.AGI = 20;
				stutas.ATK = 30;
				stutas.DEF = 60;
				stutas.MP = 50;
				stutas.HP = 110;
			}
		}
		//�h���S���o�[����������
		if (NormalEnemyName::DRAGONBARN == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[4], TRUE);
			stutas.AGI = 200;
			stutas.ATK = 60;
			stutas.DEF = 70;
			stutas.MP = 300;
			stutas.HP = 260;
		}
		//�L������������
		if (NormalEnemyName::CHIMWRA == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[5], TRUE);
			{
				stutas.AGI = 20;
				stutas.ATK = 60;
				stutas.DEF = 160;
				stutas.MP = 200;
				stutas.HP = 600;
			}
		}
		//�h���S�����C�h��������
		if (NormalEnemyName::DRAGONSERVANT == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[6], TRUE);
			{
				stutas.AGI = 100;
				stutas.ATK = 30;
				stutas.DEF = 110;
				stutas.MP = 600;
				stutas.HP = 100;
			}
		}
	}
}




