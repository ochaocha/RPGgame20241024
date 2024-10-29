#include "player.h"
#include "DxLib.h"

#include "map.h"

void OllPlayerData::PlayerInit(int displaywidth, int displayhight)
{
	PlayerPos.x = float(displaywidth / 2);
	PlayerPos.y = float(displayhight / 2);
	animNowType;
	animNowIndex = 0;
	animDirectionOffset = 0;

	//initRect(playerHit,hitSizeX,hitSize)
	LoadDivGraph("chara/walk.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX);
	drawOffsetY = (hitSizeY - animSizeY);

}

void OllPlayerData::Playerappdate()
{
	time += 1.0f / 60.f;						//60FPSÇÕÇPïbä‘ÇæÇ©ÇÁÇªÇÍÇtimeÇ…éûä‘Çì¸ÇÍÇƒÇ¢ÇÈ

	//0,1,2
	//3,4,5
	//6,7,8
	//9,10,11



	//à⁄ìÆëOÇÃÉvÉåÉCÉÑÅ[ç¿ïWÇï€ë∂
	Vector2DX prevPos = PlayerPos;


	//AÇ™âüÇ≥ÇÍÇΩÇ∆Ç´
	if (CheckHitKey(KEY_INPUT_A))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1ïbà»è„Ç…Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
			time -= 1.0f;							//ÇPïbà»è„Ç…Ç»ÇÈÇ∆1ïbÉ}ÉCÉiÉXÇ≥ÇπÇÈ
			animNowIndex += 1;						//ÇPïbÇ…Ç»ÇÈÇ∆îzóÒÇÃóvëfêîÇÇPëùÇ‚ÇµÇƒÇ¢ÇÈ
			if (animNowIndex >= animPatternNum)		//animNowIndexÇ™ÉAÉjÉÅÅ[ÉVÉáÉìÇÃâ°óvëfêîÇ…Ç»ÇÈÇ∆
			{
				animNowIndex -= animPatternNum;		//ÇRanimPatternNumÇanimIndexÇ…É}ÉCÉiÉXÇ≥ÇπÇƒ0Å`Ç…ÇµÇƒÇ¢ÇÈ
			}
		}
		AnimStopLeftFlag =true;
		PlayerPos.x -= 1.0f;
		animDirectionOffset = 8;
	}
	//DÇ™âüÇ≥ÇÍÇΩÇ∆Ç´
	if (CheckHitKey(KEY_INPUT_D))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1ïbà»è„Ç…Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
			time -= 1.0f;							//ÇPïbà»è„Ç…Ç»ÇÈÇ∆1ïbÉ}ÉCÉiÉXÇ≥ÇπÇÈ
			animNowIndex += 1;						//ÇPïbÇ…Ç»ÇÈÇ∆îzóÒÇÃóvëfêîÇÇPëùÇ‚ÇµÇƒÇ¢ÇÈ
			if (animNowIndex >= animPatternNum)		//animNowIndexÇ™ÉAÉjÉÅÅ[ÉVÉáÉìÇÃâ°óvëfêîÇ…Ç»ÇÈÇ∆
			{
				animNowIndex -= animPatternNum;		//ÇRanimPatternNumÇanimIndexÇ…É}ÉCÉiÉXÇ≥ÇπÇƒ0Å`Ç…ÇµÇƒÇ¢ÇÈ
			}
		}
		AnimStopRighttFlag = true;
		PlayerPos.x += 1.0f;
		animDirectionOffset = 40;
	}
	//SÇ™âüÇ≥ÇÍÇΩÇ∆Ç´
	if (CheckHitKey(KEY_INPUT_S))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1ïbà»è„Ç…Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
			time -= 1.0f;							//ÇPïbà»è„Ç…Ç»ÇÈÇ∆1ïbÉ}ÉCÉiÉXÇ≥ÇπÇÈ
			animNowIndex += 1;						//ÇPïbÇ…Ç»ÇÈÇ∆îzóÒÇÃóvëfêîÇÇPëùÇ‚ÇµÇƒÇ¢ÇÈ
			if (animNowIndex >= animPatternNum)		//animNowIndexÇ™ÉAÉjÉÅÅ[ÉVÉáÉìÇÃâ°óvëfêîÇ…Ç»ÇÈÇ∆
			{
				animNowIndex -= animPatternNum;		//ÇRanimPatternNumÇanimIndexÇ…É}ÉCÉiÉXÇ≥ÇπÇƒ0Å`Ç…ÇµÇƒÇ¢ÇÈ
			}
		}
		AnimStopUnderFlag = true;
		PlayerPos.y += 1.0;
		animDirectionOffset = 0;
	
	}
	//WÇ™âüÇ≥ÇÍÇΩÇ∆Ç´
	if (CheckHitKey(KEY_INPUT_W))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1ïbà»è„Ç…Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
			time -= 1.0f;							//ÇPïbà»è„Ç…Ç»ÇÈÇ∆1ïbÉ}ÉCÉiÉXÇ≥ÇπÇÈ
			animNowIndex += 1;						//ÇPïbÇ…Ç»ÇÈÇ∆îzóÒÇÃóvëfêîÇÇPëùÇ‚ÇµÇƒÇ¢ÇÈ
			if (animNowIndex >= animPatternNum)		//animNowIndexÇ™ÉAÉjÉÅÅ[ÉVÉáÉìÇÃâ°óvëfêîÇ…Ç»ÇÈÇ∆
			{
				animNowIndex -= animPatternNum;		//ÇRanimPatternNumÇanimIndexÇ…É}ÉCÉiÉXÇ≥ÇπÇƒ0Å`Ç…ÇµÇƒÇ¢ÇÈ
			}
		}
		PlayerPos.y -= 1.0f;
		animDirectionOffset = 24;
	}

	//if(CheckHitKey(KEY_INPUT_))


	//à⁄ìÆèàóùëOå„ÇÃê¸ï™Ç∆É}ÉbÉvÇ…Ç†ÇÈï«Ç∆ÇÃîªíË
	MapData* MapDataOllPtr = MapData::Instance();
	MapDataOllPtr->CalcVectorSlideOnWallChips(
		prevPos,
		&PlayerPos,
		Vector2DX::vget(static_cast<float>(-hitSizeX / 2), static_cast<float>(-hitSizeY / 2)),
		Vector2DX::vget(static_cast<float>(hitSizeX / 2), static_cast<float>(hitSizeY / 2))
	);
}

void OllPlayerData::PlayerDraw()
{

	DrawExtendGraph(static_cast<int>(PlayerPos.x) + drawOffsetX, static_cast<int>(PlayerPos.y) + drawOffsetY, PlayerPos.x + drawOffsetX + 110, PlayerPos.y + drawOffsetY + 110, charaimg[animDirectionOffset + animNowIndex], TRUE);

	//à⁄ìÆîªíËÇ…ê≥ämÇ»ÉLÉÉÉâÉNÉ^Å[ï`âÊ
	Vector2DX PlayerMinSize = Vector2DX::vget(static_cast<float>(-hitSizeX / 2), static_cast<float>(-hitSizeY / 2));
	Vector2DX PlayerMaxSize = Vector2DX::vget(static_cast<float>(hitSizeX / 2), static_cast<float>(hitSizeY / 2));
	Vector2DX PlayerMinPos = PlayerPos + PlayerMinSize;
	Vector2DX PlayerMaxPos = PlayerPos + PlayerMaxSize;
	DrawExtendGraph(
		static_cast<int>(PlayerMinPos.x), static_cast<int>(PlayerMinPos.y),
		static_cast<int>(PlayerMaxPos.x), static_cast<int>(PlayerMaxPos.y),
		charaimg[animDirectionOffset + animNowIndex], TRUE);
	DrawBox(static_cast<int>(PlayerMinPos.x), static_cast<int>(PlayerMinPos.y),
		static_cast<int>(PlayerMaxPos.x), static_cast<int>(PlayerMaxPos.y),
		GetColor(255, 0, 0), FALSE);
}

void OllPlayerData::PlayerFinalize()
{
	if (charaimg[0] != -1)
	{
		for (int i = 0; i < animPatternNum * animTypeNum; i++)
		{
			DeleteGraph(charaimg[i]);
		}
	}
}
