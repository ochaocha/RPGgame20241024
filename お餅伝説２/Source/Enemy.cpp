
#include <ctime>
#include<cstdlib>
#include <vector>
#include <tuple>
#include "Enemy.h"



NormalEnemyName OllEnemyData::Rand()
{
	std::srand(std::time(0));
	
	RandomExtraction = std::rand() % 4;

	//0だった場合敵の中からランダムに１体だけ敵を抽出
	switch (RandomExtraction)
	{
	case 0:
		//構造体からランダムに敵を一体だけ抽選
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		//std::tuple<NormalEnemyName, NormalEnemyName> RandomExtraction;
		return static_cast<NormalEnemyName>(RandomExtraction);
	case 1:
		//構造体からランダムに敵を二体抽選
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		return static_cast<NormalEnemyName>(RandomExtraction);
	case 2:
		//構造体からランダムに敵を三体抽選
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		return static_cast<NormalEnemyName>(RandomExtraction);
	case 3:
		//構造体からランダムに敵を四体抽選
		RandomExtraction = std::rand() % static_cast<int>(NormalEnemyName::COUNT);
		return static_cast<NormalEnemyName>(RandomExtraction);
	}
}

void OllEnemyData::Draw()
{

	//ドラゴンだった時の処理
	if (NormalEnemyName::DRAGON == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[0], TRUE);
	}
	//赤いドラゴンだった時
	if (NormalEnemyName::REDDRAGON == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[1], TRUE);
	}
	//ゴーレムだった時
	if (NormalEnemyName::GOLEM == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[2], TRUE);
	}
	//ミニドラゴンだった時
	if (NormalEnemyName::MINIDRAGON == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[3], TRUE);
	}
	//ドラゴンバーンだった時
	if (NormalEnemyName::DRAGONBARN == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[4], TRUE);
	}
	//キメラだった時
	if (NormalEnemyName::CHIMWRA == static_cast<NormalEnemyName>(RandomExtraction))
	{
		DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[5], TRUE);
	}
	//ドラゴンメイドだった時
	if (NormalEnemyName::DRAGONSERVANT == static_cast<NormalEnemyName>(RandomExtraction))
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

