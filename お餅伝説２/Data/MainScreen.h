#pragma once
#include "DxLib.h"


class MeinScreen
{
public:
	int mauseX=0, mauseY=0;

	bool rangeATK = true;

	int ColorHandle = 100;

	int ColorHnadleChange = 255;
	/// @brief スタート画面関数
	/// @param change 
	/// @param MeinscreenVertical 
	/// @param MeinscreenHorizontal 
	void Draw(int* change);	


};

static MeinScreen Mein;