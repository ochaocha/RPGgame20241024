#include "player.h"
#include "DxLib.h"

void PlayerFunction ::PlayerInit(int displaywidth, int displayhight)
{
	px = float(displaywidth / 2);
	py = float(displayhight / 2);
	animNowType;
	animNowIndex = 0;
	animDirectionOffset = 0;

	//initRect(playerHit,hitSizeX,hitSize)
	LoadDivGraph("chara/walk.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX);
	drawOffsetY = (hitSizeY - animSizeY);

}

void PlayerFunction ::Playerappdate()
{
	time += 1.0f / 60.f;						//60FPS‚Í‚P•bŠÔ‚¾‚©‚ç‚»‚ê‚ğtime‚ÉŠÔ‚ğ“ü‚ê‚Ä‚¢‚é

	//0,1,2
	//3,4,5
	//6,7,8
	//9,10,11

	if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1•bˆÈã‚É‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚é
		time -= 1.0f;							//‚P•bˆÈã‚É‚È‚é‚Æ1•bƒ}ƒCƒiƒX‚³‚¹‚é
		animNowIndex += 1;						//‚P•b‚É‚È‚é‚Æ”z—ñ‚Ì—v‘f”‚ğ‚P‘‚â‚µ‚Ä‚¢‚é
		if (animNowIndex >= animPatternNum)		//animNowIndex‚ªƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‰¡—v‘f”‚É‚È‚é‚Æ
		{
			animNowIndex -= animPatternNum;		//‚RanimPatternNum‚ğanimIndex‚Éƒ}ƒCƒiƒX‚³‚¹‚Ä0`‚É‚µ‚Ä‚¢‚é
		}
	}


	if (CheckHitKey(KEY_INPUT_A))
	{
		px-=1.0f;

		charaMove = true;
		animDirectionOffset = 8;

	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		px+=1.0f;
		charaMove = true;
		animDirectionOffset = 40;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		py+=1.0;
		charaMove = true;
		animDirectionOffset = 0;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		py-=1.0f;
		charaMove = true;
		animDirectionOffset = 24;
	}
	//if(CheckHitKey(KEY_INPUT_))
	if (CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0 && CheckHitKey(KEY_INPUT_D) == 0 && CheckHitKey(KEY_INPUT_A) == 0)
	{
		charaMove = false;
	}
}

void PlayerFunction::PlayerDraw()
 {

	DrawExtendGraph(static_cast<int>(px) + drawOffsetX, static_cast<int>(py) + drawOffsetY,px+drawOffsetX+110,py+drawOffsetY+110,charaimg[animDirectionOffset + animNowIndex], TRUE);


 }

 void PlayerFunction::PlayerFinalize()
 {
	 if (charaimg[0] != -1)
	 {
		 for (int i = 0; i < animPatternNum * animTypeNum; i++)
		 {
			 DeleteGraph(charaimg[i]);			
		 }
	 }
 }

 void PlayerFunction::PlayerStatusEffect()
 {
	 if()
 }