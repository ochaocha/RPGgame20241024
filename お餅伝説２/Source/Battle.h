#pragma once
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

class BattleData
{
	int Battle;

	float Time;
	float encounter;
	float randomEncounter;
public:
	/// @brief �L�����N�^�[�̃X�e�[�^�X�Ȃǂ̏���`��
	void PlayerInfomationDraw();
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
	int BattleProbability=0;
	bool BattlePlayFlag = false;
	int BattleScreenHandle=0;
	int BattleUIStatusHandle=0;
	int BattleUICommandHandle=0;
	int BattleUIAttckTypeHandle=0;
public:

	/// @brief �o�g����ʂ̕`��
	void Scene();
	/// @brief �L�����N�^�[�̕`��
	void PlayerDraw();
	/// @brief �G�L�����N�^�[�̕`��
	void EnemyDraw();
	/// @brief �摜�̏�����
	void Imginitialize();
};
