#pragma once
#include "DxLib.h"
#include <time.h>
#include <stdlib.h>
#include "player.h"




class MeinScreenChanger
{

public:
	 int StartEfectNum = 89;
	static const int StartEfectXNum = 10;
	static const int StartEfectYNum = 9;
	 int StartEfectXSize = 64;
	 int StartEfectYSize = 64;
	int EfectNowIndex = 0;
	int EfectImg[StartEfectXNum * StartEfectYNum] = { 0 };
	float time = 0.f;
	int StartSound = LoadSoundMem("Sound/�J�[�\���ړ�6.mp3");

	

	int mauseX=0, mauseY=0;


	int ColorHandle = 255;

	int ColorHandleChange = 255;


	void ScreenInit();
	/// @brief �X�^�[�g��ʊ֐�
	/// @param change 
	/// @param MeinscreenVertical 
	/// @param MeinscreenHorizontal 
	void Draw(int* screenchange);	
	
};

static MeinScreenChanger Mein;

class BattleScreenChanger
{
private:
	int BattleProbability;
	bool BattlePlayFlag = false;
	int BattleScreenHandle = LoadGraph("Data/img/pipo-bg004c.png");
	
public:
	/// @brief �o�g����ʂ̕`��
	void Scene();
	/// @brief �L�����N�^�[�̕`��
	void PlayerDraw();
	/// @brief �L�����N�^�[�̃X�e�[�^�X�Ȃǂ̏���`��
	void PlayerInfomationDraw();
	/// @brief �G�L�����N�^�[�̕`��
	void EnemyDraw();

};

static BattleScreenChanger Battle;
