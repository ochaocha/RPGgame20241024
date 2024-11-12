#include "Enemy.hpp"
#include <ctime>
#include<cstdlib>



EnemyName OllEnemyData::NormalEnemyRand()
{
	std::srand(std::time(0));
	OneBattleEnemy = rand() % 4;
	int random;
	if (OneBattleEnemy == 0)
	{
		random = std::rand() % static_cast<int>(EnemyName::COUNT);
		return static_cast<EnemyName>(random);
	}
}

