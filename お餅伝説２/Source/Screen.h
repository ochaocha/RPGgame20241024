#pragma once
#include "DxLib.h"
#include <time.h>
#include <stdlib.h>
#include "player.h"




class MeinScreenChanger
{
public:
	
	static const int startefectheight = 18;
	static const int startefectwidht = 26;

	

	int mauseX=0, mauseY=0;


	int ColorHandle = 100;

	int ColorHandleChange = 255;
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
