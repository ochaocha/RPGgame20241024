#include "Battle.h"
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>

//�G�ɑ�������m��
constexpr int MINNUM = 20;

void BattleData::InputEncounterCheck(
	bool& battle,
	int* screenchange,
	bool MoveLeftCount,
	bool MoveRightCount,
	bool MoveDownCount,
	bool MoveUpCount
)

{
	if (MoveLeftCount == true || MoveRightCount == true || MoveDownCount == true || MoveUpCount == true)
	{
		Time += 1.0f / 60.f;
		if (Time <= 3)
		{
			//�����̏�����
			srand((unsigned int)time(NULL));
			//player�������Ă���Ƃ�

			randomEncounter = rand() % 100;
			if (randomEncounter < MINNUM)
			{
				battle = TRUE;
				if (battle == TRUE)
				{
					*screenchange = 3;

				}
			}
			Time = 0.f;
		}
	}
	

}

void BattleScreenChanger::Scene()
{

	DrawExtendGraph(0, 0, 1920, 1080, BattleScreenHandle, TRUE);
	DrawExtendGraph(1000, 800, 1800, 1000, BattleUIHandle, TRUE);
	DrawExtendGraph(1000, 800, 1800, 1000, BattleUIHandle, TRUE);
	DrawExtendGraph(1000, 800, 1800, 1000, BattleUIHandle, TRUE);
}


void BattleScreenChanger::Imginitialize()
{
	BattleScreenHandle = LoadGraph("Data/img/pipo-bg004c.png", TRUE);
	BattleUIHandle = LoadGraph("Data/img/UI_Flat_Frame02a.png", TRUE);


}

void BattleScreenChanger::PlayerDraw()
{

}

void BattleScreenChanger::EnemyDraw()
{

}
void BattleData::PlayerInfomationDraw()
{

}

void BattleData::PlayerStatusInit()
{

}
//�o�g���J�n�Ɠ����ɃL�����N�^�[�̃X�e�[�^�X���Q�Ƃł���悤�ɂ��Ă���
void BattleData::StatusUI(int& CharacterHP,
	int& CharacterATK,
	int& CharacterMP,
	int& CharacterDEF,
	int& CharacterLUCK,
	int& CharacterAGI
)
{
	int Cr = GetColor(255, 255, 255);

	DrawFormatString(250, 240, Cr, "%d", CharacterAGI);
	DrawFormatString(250, 280, Cr, "%d", CharacterATK);
	DrawFormatString(250, 320, Cr, "%d", CharacterMP);
	DrawFormatString(250, 360, Cr, "%d", CharacterHP);
	DrawFormatString(250, 400, Cr, "%d", CharacterLUCK);
	DrawFormatString(250, 440, Cr, "%d", CharacterDEF);
}