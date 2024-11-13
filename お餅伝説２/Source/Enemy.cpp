#include <ctime>
#include<cstdlib>
#include <vector>
#include <tuple>
#include "Enemy.h"

void OllEnemyData::Init()
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

std::vector<NormalEnemyName> OllEnemyData::Rand()
{
	std::srand(std::time(0));

	RandomExtraction = std::rand() % 4; // 0, 1, 2, 3 のいずれか

	std::vector<NormalEnemyName> enemies;

	// 抽選された数に応じて敵を選ぶ
	switch (RandomExtraction)
	{
	case 0:
		// 1体だけ抽選
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 1:
		// 2体抽選
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 2:
		// 3体抽選
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 3:
		// 4体抽選
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.push_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	}

	return enemies;
}


//敵が増えるとその分　横に描画させるために後から修正する
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

void Stutus(EnemyStutas& stutas)
{
	


}












