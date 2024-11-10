#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;

	float Time;
	float randomEncounter;
public:
	/// @brief �L�����N�^�[�̃X�e�[�^�X�Ȃǂ̏���`��
	void PlayerInfomationDraw();
	void PlayerEncounterCheck(int* battlescreen);

	void PlayerStatusInit();
	/// @brief 
	void StatusUI(int& CharacterHP,
		int& CharacterATK,
		int& CharacterMP,
		int& CharacterDEF,
		int& CharacterLUCK,
		int& CharacterAGI);

	void InputEncounterCheck(
		bool& battle,
		int* screenchange,
		bool MoveLeftCount,
		bool MoveRightCount,
		bool MoveDownCount,
		bool MoveUpCount
	);
};

class BattleScreenChanger
{
private:
	int BattleProbability;
	bool BattlePlayFlag = false;
	int BattleScreenHandle = LoadGraph("Data/img/pipo-bg004c.png");

	int BattleUIHandle = -1;
public:



	/// @brief �o�g����ʂ̕`��
	void Scene();
	/// @brief �L�����N�^�[�̕`��
	void PlayerDraw();
	/// @brief �G�L�����N�^�[�̕`��
	void EnemyDraw();
	/// @brief 
	void UI();

	void Imginitialize();
};

static BattleScreenChanger Battle;