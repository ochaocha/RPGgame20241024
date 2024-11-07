#pragma once
#include "DxLib.h"
#include <time.h>
#include <stdlib.h>
#include "player.h"




class MeinScreenChanger
{
private:
	 int StartSound = LoadSoundMem("Sound/kurikku.mp3");
public:
	 int StartEfectNum = 89;
	static const int StartEfectXNum = 10;
	static const int StartEfectYNum = 9;
	 int StartEfectXSize = 64;
	 int StartEfectYSize = 64;
	int EfectNowIndex = 0;
	int EfectImg[StartEfectXNum * StartEfectYNum] = { 0 };
	float time = 0.f;
	

	 
	int StartDesition = LoadSoundMem("Sound/決定ボタンを押す36.mp3");
	

	int mauseX=0, mauseY=0;


	int ColorHandle = 255;

	int ColorHandleChange = 255;

	void ScreenInit();
	/// @brief スタート画面関数
	/// @param change 
	/// @param MeinscreenVertical 
	/// @param MeinscreenHorizontal 
	void Draw(int* screenchange);	
	
};


