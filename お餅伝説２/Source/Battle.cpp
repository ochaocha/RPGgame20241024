#include "Battle.h"
#include "Dxlib.h"
#include <ctime>
#include <cstdlib>


void BattleData::InputEncounterCheck(
	bool& Battlecount,
	bool MoveLeftCount,
	bool MoveRightCount,
	bool MoveDownCount,
	bool MoveUpCount
)
{
	Battlecount = true;
}

void BattleScreenChanger::Scene()
{
	DrawExtendGraph(0, 0, 1920, 1080, BattleScreenHandle, TRUE);
	

}
void BattleData::PlayerEncounterCheck(int* battlescreen)
{

	if ((CheckHitKey(KEY_INPUT_P))==TRUE);
	*battlescreen = 3;
}
void BattleScreenChanger::PlayerDraw()
{


}