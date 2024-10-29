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
	time += 1.0f / 60.f;						//60FPSは１秒間だからそれをtimeに時間を入れている

	//0,1,2
	//3,4,5
	//6,7,8
	//9,10,11



	//移動前のプレイヤー座標を保存
	Vector2DX prevPos = PlayerPos;


	//Aが押されたとき
	if (CheckHitKey(KEY_INPUT_A))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1秒以上にならないようにする
			time -= 1.0f;							//１秒以上になると1秒マイナスさせる
			animNowIndex += 1;						//１秒になると配列の要素数を１増やしている
			if (animNowIndex >= animPatternNum)		//animNowIndexがアニメーションの横要素数になると
			{
				animNowIndex -= animPatternNum;		//３animPatternNumをanimIndexにマイナスさせて0〜にしている
			}
		}
		AnimStopLeftFlag =true;
		PlayerPos.x -= 1.0f;
		animDirectionOffset = 8;
	}
	//Dが押されたとき
	if (CheckHitKey(KEY_INPUT_D))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1秒以上にならないようにする
			time -= 1.0f;							//１秒以上になると1秒マイナスさせる
			animNowIndex += 1;						//１秒になると配列の要素数を１増やしている
			if (animNowIndex >= animPatternNum)		//animNowIndexがアニメーションの横要素数になると
			{
				animNowIndex -= animPatternNum;		//３animPatternNumをanimIndexにマイナスさせて0〜にしている
			}
		}
		AnimStopRighttFlag = true;
		PlayerPos.x += 1.0f;
		animDirectionOffset = 40;
	}
	//Sが押されたとき
	if (CheckHitKey(KEY_INPUT_S))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1秒以上にならないようにする
			time -= 1.0f;							//１秒以上になると1秒マイナスさせる
			animNowIndex += 1;						//１秒になると配列の要素数を１増やしている
			if (animNowIndex >= animPatternNum)		//animNowIndexがアニメーションの横要素数になると
			{
				animNowIndex -= animPatternNum;		//３animPatternNumをanimIndexにマイナスさせて0〜にしている
			}
		}
		AnimStopUnderFlag = true;
		PlayerPos.y += 1.0;
		animDirectionOffset = 0;
	
	}
	//Wが押されたとき
	if (CheckHitKey(KEY_INPUT_W))
	{

		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1秒以上にならないようにする
			time -= 1.0f;							//１秒以上になると1秒マイナスさせる
			animNowIndex += 1;						//１秒になると配列の要素数を１増やしている
			if (animNowIndex >= animPatternNum)		//animNowIndexがアニメーションの横要素数になると
			{
				animNowIndex -= animPatternNum;		//３animPatternNumをanimIndexにマイナスさせて0〜にしている
			}
		}
		PlayerPos.y -= 1.0f;
		animDirectionOffset = 24;
	}

	//if(CheckHitKey(KEY_INPUT_))


	//移動処理前後の線分とマップにある壁との判定
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

	//移動判定に正確なキャラクター描画
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
