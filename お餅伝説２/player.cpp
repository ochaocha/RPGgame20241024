#include "player.h"
#include "DxLib.h"

#include "map.h"

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
	time += 1.0f / 60.f;						//60FPSは１秒間だからそれをtimeに時間を入れている

	//0,1,2
	//3,4,5
	//6,7,8
	//9,10,11


	if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1秒以上にならないようにする
		time -= 1.0f;							//１秒以上になると1秒マイナスさせる
		animNowIndex += 1;						//１秒になると配列の要素数を１増やしている
		if (animNowIndex >= animPatternNum)		//animNowIndexがアニメーションの横要素数になると
		{
			animNowIndex -= animPatternNum;		//３animPatternNumをanimIndexにマイナスさせて0～にしている
		}
	}

	//移動前のプレイヤー座標を保存
	float prevXpos = px;
	float prevYpos = py;

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

	//移動処理前後の線分とマップにある壁との判定
	MapData* MapDataOllPtr = MapData::Instance();
	MapDataOllPtr->CalcVectorSlideOnWallChips(
		prevXpos, prevYpos,
		&px, &py,
		static_cast<float>(-hitSizeX / 2), static_cast<float>(hitSizeX / 2),
		static_cast<float>(-hitSizeY / 2), static_cast<float>(hitSizeY / 2)
	);
}

void PlayerFunction::PlayerDraw()
 {

	DrawExtendGraph(static_cast<int>(px) + drawOffsetX, static_cast<int>(py) + drawOffsetY,px+drawOffsetX+110,py+drawOffsetY+110,charaimg[animDirectionOffset + animNowIndex], TRUE);

	//移動判定に正確なキャラクター描画
	DrawExtendGraph(static_cast<int>(px) - hitSizeX / 2, static_cast<int>(py) - hitSizeY / 2, px + hitSizeX / 2, py + hitSizeY / 2, charaimg[animDirectionOffset + animNowIndex], TRUE);
	DrawBox(static_cast<int>(px - hitSizeX / 2), static_cast<int>(py - hitSizeY / 2), static_cast<int>(px + hitSizeX / 2), static_cast<int>(py + hitSizeY / 2), GetColor(255, 0, 0), FALSE);
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
