#include "player.h"
#include "DxLib.h"
#include "collision.h"



float px, py;
float vx, vy;
float animeTimer;
int animNowType;
int animNowIndex;

int animDirectionOffset;

int animNowPattern;

bool charaMove = false;

sHitRect playerHit;
sHitRect playerFootCollider;
sHitRect playerHeadCollider;

const int animPatternNum = 3;			// アニメーションのパターン数
const int animDown = 0;			// 下方向
const int animLeft = 1;			// 左方向
const int animRight = 2;			// 右方向
const int animUp = 3;			// 上方向
const int animTypeNum = 4;			// アニメーションの種類
const int animSizeX = 32;
const int animSizeY = 32;

const int hitSizeX = 40;
const int hitSizeY = 60;

const float animationFPS = 6.0f;		// 秒間 6コマのアニメーション
const float moveSpeed = 200.0f;		// 移動速度 (ここで指定した値分のピクセル/秒進む) 
const int animetion = 11;
bool charaPrev;


int drawOffsetX;
int drawOffsetY;
int charaimg[animPatternNum * animTypeNum];

float time = 0.f;

void PlayerInit(int displaywidth, int displayhight)
{
	px = float(displaywidth / 2);
	py = float(displayhight / 2);
	vx = 0.0f;
	vy = 0.0f;
	animeTimer = 0.0f;
	animNowType = animDown;
	animNowPattern = 0;
	animNowIndex = 0;
	animDirectionOffset = 0;

	//initRect(playerHit,hitSizeX,hitSize)
	LoadDivGraph("chara/pipo-charachip003b.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX) / 2;
	drawOffsetY = (hitSizeY - animSizeY);

}

void Playerappdate()
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


	if (CheckHitKey(KEY_INPUT_A))
	{
		px--;
		charaMove = true;
		animDirectionOffset = 3;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		px++;
		charaMove = true;
		animDirectionOffset = 6;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		py++;
		charaMove = true;
		animDirectionOffset = 0;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		py--;
		charaMove = true;
		animDirectionOffset = 9;
	}
	if (CheckHitKey(KEY_INPUT_W) == 0 && CheckHitKey(KEY_INPUT_S) == 0 && CheckHitKey(KEY_INPUT_D) == 0 && CheckHitKey(KEY_INPUT_A) == 0)
	{
		charaMove = false;
	}

}

void PlayerDraw() 
 {

	 DrawGraph(static_cast<int>(px) + drawOffsetX, static_cast<int>(py) + drawOffsetY, charaimg[animDirectionOffset + animNowIndex], TRUE);


 }

 void PlayerFinalize()
 {
	 if (charaimg[0] != -1)
	 {
		 for (int i = 0; i < animPatternNum * animTypeNum; i++)
		 {
			 DeleteGraph(charaimg[i]);
		 }
	 }
 }
