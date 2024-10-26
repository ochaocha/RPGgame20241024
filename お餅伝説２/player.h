#pragma once
#include "collision.h"
//Emiya 1-3:�����Ɉړ�

class PlayerFunction {
	//Emiya 1-1:�����Ɉړ�
public:
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

	static const int animPatternNum = 8;			// �A�j���[�V�����̃p�^�[����		//Emiya 1-2:1-1�ňړ���G���[���o��̂ŐÓI�ϐ��Ƃ��Ă��܂��܂��傤
	const int animDown = 0;			// ������
	const int animLeft = 1;			// ������
	const int animRight = 2;			// �E����
	const int animUp = 3;			// �����
	static const int animTypeNum = 6;			// �A�j���[�V�����̎��		//Emiya 1-2:1-1�ňړ���G���[���o��̂ŐÓI�ϐ��Ƃ��Ă��܂��܂��傤
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
	void PlayerInit(int displaywidth, int displayhight);
	void Playerappdate();
	void PlayerDraw();
	void PlayerFinalize();

};

static PlayerFunction PlayerFunctionOll;//Emiya 1-5:main.cpp�̃��[�J���ϐ��ɂ��邽�߈ړ����܂��傤

//Emiya 1-4:ScreenCamera�N���X��camera2D.h�̂悤�ȃt�@�C����p�ӂ��Ă�����Ɉړ������Ⴂ�܂��傤

//Emiya 1-5:main.cpp�̃��[�J���ϐ��ɂ��邽�߈ړ����܂��傤

