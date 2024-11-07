#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;
public:
	//エンカウントの実装
	void InputEncounterCheck(
		bool& BattleEncounterFlag,
		bool moveLeft,
		bool moveRight,
		bool moveDown,
		bool moveUp
	);
};
