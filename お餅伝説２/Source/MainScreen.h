#pragma once
#include "DxLib.h"


class MeinScreen
{
public:
	int mauseX=0, mauseY=0;

	bool rangeATK = true;

	int ColorHandle = 100;

	int ColorHnadleChange = 255;
	/// @brief �X�^�[�g��ʊ֐�
	/// @param change 
	/// @param MeinscreenVertical 
	/// @param MeinscreenHorizontal 
	void Draw(int* change);	


};

static MeinScreen Mein;