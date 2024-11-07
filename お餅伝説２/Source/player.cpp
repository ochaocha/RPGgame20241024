#include "DxLib.h"
#include "player.h"
#include "map.h"

const CharacterManager* CharacterManager::s_Singleton = nullptr;
void OllCharacterData::PlayerInit(int displaywidth, int displayhight)
{
	PlayerPos.x = float(displaywidth / 2);
	PlayerPos.y = float(displayhight / 2);
	animNowType;
	animNowIndex = 0;
	animDirectionOffset = 0;

	//initRect(playerHit,hitSizeX,hitSize)
	LoadDivGraph("Data/chara/walk.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX);
	drawOffsetY = (hitSizeY - animSizeY);

}

void OllCharacterData::CalcInput(const Input& input)
{
	MyInput = input;

}

void OllCharacterData::Update()
{
	//移動前のプレイヤー座標を保存
	Vector2DX prevPos = PlayerPos;
	
	PlayerVelocity.x = 0;
	PlayerVelocity.y = 0;

	//Aが押されたとき
	if (MyInput.IsMoveLeft())
	{	
		PlayerVelocity.x = -1;
	}
	//Dが押されたとき
	if (MyInput.IsMoveRight())
	{
		PlayerVelocity.x = 1;
	}
	//Sが押されたとき
	if (MyInput.IsMoveDown())
	{
		PlayerVelocity.y = 1;
	}
	//Wが押されたとき
	if (MyInput.IsMoveUp())
	{
		PlayerVelocity.y = -1;
	}

	if (PlayerVelocity.magnitude() > 0.f) {
		//うごいてる
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

#if FALSE
		//アニメーション指定(例1)　縦横対応
		if (PlayerVelocity.x > 0.f && PlayerVelocity.y == 0.f) {
			//右
			animDirectionOffset = 40;
		}
		if (PlayerVelocity.x == 0.f && PlayerVelocity.y < 0.f) {
			//上
			animDirectionOffset = 24;
		}
		if (PlayerVelocity.x < 0.f && PlayerVelocity.y == 0.f) {
			//左
			animDirectionOffset = 8;
		}
		if (PlayerVelocity.x == 0.f && PlayerVelocity.y > 0.f) {
			//下
			animDirectionOffset = 0;
		}
#else
		//アニメーション指定(例2) 角度を求めてそこから
		float radian = std::atan2f(PlayerVelocity.x, PlayerVelocity.y);//-DX_PI_F~DX_PI_Fの範囲で入力角度を得る
		float degree = radian * 180.f / DX_PI_F;//角度をDegreeに変換

		//下
		if (-150.f > degree || degree > 150.f) {
			animDirectionOffset = 8 * 3;
		}
		//右下
		if (90.f <= degree && degree <= 150.f) {
			animDirectionOffset = 8 * 4;
		}
		//右上
		if (30.f <= degree && degree <= 90.f) {
			animDirectionOffset = 8 * 5;
		}
		//上
		if (-30.f <= degree && degree <= 30.f) {
			animDirectionOffset = 8 * 0;
		}
		//左上
		if (-90.f <= degree && degree <= -30.f) {
			animDirectionOffset = 8 * 1;
		}
		//左下
		if (-150.f <= degree && degree <= -90.f) {
			animDirectionOffset = 8 * 2;
		}
#endif
	}
	else {
		//止まっている
	}



	PlayerPos += PlayerVelocity;


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

void OllCharacterData::PlayerDraw()
{

	DrawExtendGraph(static_cast<int>(PlayerPos.x) + drawOffsetX, static_cast<int>(PlayerPos.y) + drawOffsetY, static_cast<int>(PlayerPos.x) + drawOffsetX + 110, static_cast<int>(PlayerPos.y)+ drawOffsetY + 110, charaimg[animDirectionOffset + animNowIndex], TRUE);

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

void OllCharacterData::PlayerFinalize()
{
	if (charaimg[0] != -1)
	{
		for (int i = 0; i < animPatternNum * animTypeNum; i++)
		{
			DeleteGraph(charaimg[i]);
		}
	}
}
