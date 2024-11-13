
#include <ctime>
#include<cstdlib>
#include <vector>
#include <tuple>
#include "Enemy.h"



NormalEnemyName OllEnemyData::Rand()
{
	std::srand(std::time(0));
	
	RandomExtraction = std::rand() % 4;

	//0�������ꍇ�G�̒����烉���_���ɂP�̂����G�𒊏o
	switch (RandomExtraction)
	{
	case 0:
		//�\���̂��烉���_���ɓG����̂������I
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		//std::tuple<NormalEnemyName, NormalEnemyName> RandomExtraction;
		return static_cast<NormalEnemyName>(RandomExtraction);
	case 1:
		//�\���̂��烉���_���ɓG���̒��I
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		return static_cast<NormalEnemyName>(RandomExtraction);
	case 2:
		//�\���̂��烉���_���ɓG���O�̒��I
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		return static_cast<NormalEnemyName>(RandomExtraction);
	case 3:
		//�\���̂��烉���_���ɓG���l�̒��I
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		return static_cast<NormalEnemyName>(RandomExtraction);
	}
}

void OllEnemyData::Draw()
{

	//�h���S�����������̏���
	if (NormalEnemyName::DRAGON== static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[0], TRUE);
	}
	//�Ԃ��h���S����������
	if (NormalEnemyName::GOLEM == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[1], TRUE);
	}
	
	if (NormalEnemyName::CHIMERA== static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[2], TRUE);
	}
	if (NormalEnemyName::SLIME == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[3], TRUE);
	}
	if (NormalEnemyName::DRAGON == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[4], TRUE);
	}
	if (NormalEnemyName::DRAGON == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[5], TRUE);
	}
	if (NormalEnemyName::DRAGON == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[6], TRUE);
	}

}

void OllEnemyData::Init()
{
	 normalenemy[0]=LoadGraph("Data/monster/c01_001.png",TRUE);			
	 normalenemy[1]=LoadGraph("Data/monster/c01_003.png",TRUE);		
	 normalenemy[2]=LoadGraph("Data/monster/c01_006.png",TRUE);			
	 normalenemy[3]=LoadGraph("Data/monster/c01_009.png",TRUE);			
	 normalenemy[4]=LoadGraph("Data/monster/c01_010.png",TRUE);			
	 normalenemy[5]=LoadGraph("Data/monster/c01_012.png",TRUE);			
	 normalenemy[6]=LoadGraph("Data/monster/c01_021.png",TRUE);			
	 normalenemy[7]=LoadGraph("Data/monster/c02_003.png",TRUE);
}

