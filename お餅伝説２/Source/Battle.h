#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;
public:
	//�G���J�E���g�̎���
	void InputEncounterCheck(
		bool& BattleEncounterFlag,
		bool moveLeft,
		bool moveRight,
		bool moveDown,
		bool moveUp
	);
};
