#include <ctime>
#include<cstdlib>
#include <vector>
#include <tuple>
#include "Enemy.h"

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

std::vector<NormalEnemyName> AllEnemyData::Rand()
{
	std::srand(std::time(0));

	RandomExtraction = std::rand() % 4; // 0, 1, 2, 3 のいずれか

	// 抽選された数に応じて敵を選ぶ
	switch (RandomExtraction)
	{
	case 0:
		// 1体だけ抽選
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 1:
		// 2体抽選
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 2:
		// 3体抽選
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	case 3:
		// 4体抽選
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		enemies.emplace_back(static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT)));
		break;
	}
	return enemies;
}


//敵が増えるとその分　横に描画させるために後から修正する
void AllEnemyData::Draw()
{

	for (int i = 0; i < enemies.size(); i++)
	{
		//ドラゴンだった時
		if (NormalEnemyName::DRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[0], TRUE);
		}
		//赤いドラゴンだった時
		if (NormalEnemyName::REDDRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[1], TRUE);
		}
		//ゴーレムだった時
		if (NormalEnemyName::GOLEM == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[2], TRUE);
		}
		//ミニドラゴンだった時
		if (NormalEnemyName::MINIDRAGON == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[3], TRUE);
		}
		//ドラゴンバーンだった時
		if (NormalEnemyName::DRAGONBARN == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[4], TRUE);
		}
		//キメラだった時
		if (NormalEnemyName::CHIMWRA == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[5], TRUE);
		}
		//ドラゴンメイドだった時
		if (NormalEnemyName::DRAGONSERVANT == enemies[i])
		{
			DxLib::DrawExtendGraph(monsterX, monsterY, monster2X, monster2Y, normalenemy[6], TRUE);
		}
	}
}

#ifdef define(FALSE)
std::vector<EnemyStutas>AllEnemyData::StutasGeneration()
{
	stutas.emplace_back((static_cast<NormalEnemyName>(std::rand() % static_cast<int>(NormalEnemyName::COUNT))

}
#endif












