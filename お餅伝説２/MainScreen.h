#pragma once
#include "DxLib.h"


class MeinScreen
{
public:
	int mauseX, mauseY;

	bool rangeATK = true;

	int ColorHandle = 100;

	int ColorHnadleChange = 255;
	/// @brief スタート画面関数
	/// @param change 
	/// @param MeinscreenVertical 
	/// @param MeinscreenHorizontal 
	void MeinDrawScreen(int* change);	


};

static MeinScreen Mein;