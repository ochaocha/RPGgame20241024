#include "player.h"
#include "DxLib.h"
#include "collision.h"		//Emiya 1-3:1-1�ňړ���G���[���o��̂ł����player.h�Ɉړ����܂��傤


//Emiya 1-1:��������
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

const int animPatternNum = 8;			// �A�j���[�V�����̃p�^�[����		//Emiya 1-2:1-1�ňړ���G���[���o��̂ŐÓI�ϐ��Ƃ��Ă��܂��܂��傤
const int animDown = 0;			// ������
const int animLeft = 1;			// ������
const int animRight = 2;			// �E����
const int animUp = 3;			// �����
const int animTypeNum = 6;			// �A�j���[�V�����̎��		//Emiya 1-2:1-1�ňړ���G���[���o��̂ŐÓI�ϐ��Ƃ��Ă��܂��܂��傤
const int animSizeX = 48;
const int animSizeY = 64;

const int hitSizeX = 40;
const int hitSizeY = 60;

const float animationFPS = 6.0f;		// �b�� 6�R�}�̃A�j���[�V����
const float moveSpeed = 200.0f;		// �ړ����x (�����Ŏw�肵���l���̃s�N�Z��/�b�i��) 
const int animetion = 11;
bool charaPrev;


int drawOffsetX;
int drawOffsetY;
int charaimg[animPatternNum * animTypeNum];

float time = 0.f;
//Emiya 1-1:�����܂ł�player.h�Ɉړ����܂��傤(PlayerFunction�̃����o�ϐ��ɂ��܂�)

void PlayerFunction ::PlayerInit(int displaywidth, int displayhight)
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
	LoadDivGraph("chara/walk.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX);
	drawOffsetY = (hitSizeY - animSizeY);

}

void PlayerFunction ::Playerappdate()
{
	time += 1.0f / 60.f;						//60FPS�͂P�b�Ԃ����炻���time�Ɏ��Ԃ����Ă���

	//0,1,2
	//3,4,5
	//6,7,8
	//9,10,11


	if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1�b�ȏ�ɂȂ�Ȃ��悤�ɂ���
		time -= 1.0f;							//�P�b�ȏ�ɂȂ��1�b�}�C�i�X������
		animNowIndex += 1;						//�P�b�ɂȂ�Ɣz��̗v�f�����P���₵�Ă���
		if (animNowIndex >= animPatternNum)		//animNowIndex���A�j���[�V�����̉��v�f���ɂȂ��
		{
			animNowIndex -= animPatternNum;		//�RanimPatternNum��animIndex�Ƀ}�C�i�X������0�`�ɂ��Ă���
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


 //Emiya 1-4:ScreenCamera�N���X��camera2D.cpp�̂悤�ȃt�@�C����p�ӂ��Ă�����Ɉړ������Ⴂ�܂��傤
 void ScreenCamera::Screen(int displaywidthX, int displayhightY, int displaywidth2X, int displayhight2Y)
 {
	 if (CheckHitKey(KEY_INPUT_D))
	 {
		 displaywidth2X++;
		 displaywidthX++;
	 }
	 if(CheckHitKey(KEY_INPUT_A))
	 {
		 displaywidth2X--;
		 displaywidthX--;
	 }
	 if (CheckHitKey(KEY_INPUT_S))
	 {
		 displayhightY++;
		 displayhight2Y++;
	 }
	 if (CheckHitKey(KEY_INPUT_W))
	 {
		 displayhightY--;
		 displayhight2Y--;
	 }
	 SetGraphMode(displaywidthX+ displaywidth2X, displayhightY+ displayhight2Y, 16);
 }




