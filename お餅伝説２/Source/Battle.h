#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;
public:
	/// @brief �L�����N�^�[�̃X�e�[�^�X�Ȃǂ̏���`��
	void PlayerInfomationDraw();
	//�G���J�E���g�̎���
	void InputEncounterCheck(
		bool& BattleEncounterFlag,
		bool moveLeft,
		bool moveRight,
		bool moveDown,
		bool moveUp
	);

	void PlayerEncounterCheck(int* battlescreen);
};

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
	/// @brief �G�L�����N�^�[�̕`��
	void EnemyDraw();
	/// @brief 
	void UI();

};

static BattleScreenChanger Battle;
