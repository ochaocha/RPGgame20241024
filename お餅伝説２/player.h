#pragma once
#include "collision.h"
//Emiya 1-3:ここに移動

class PlayerFunction {
	//Emiya 1-1:ここに移動
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

	static const int animPatternNum = 8;			// アニメーションのパターン数		//Emiya 1-2:1-1で移動後エラーが出るので静的変数としてしまいましょう
	const int animDown = 0;			// 下方向
	const int animLeft = 1;			// 左方向
	const int animRight = 2;			// 右方向
	const int animUp = 3;			// 上方向
	static const int animTypeNum = 6;			// アニメーションの種類		//Emiya 1-2:1-1で移動後エラーが出るので静的変数としてしまいましょう
	const int animSizeX = 48;
	const int animSizeY = 64;

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
	void PlayerInit(int displaywidth, int displayhight);
	void Playerappdate();
	void PlayerDraw();
	void PlayerFinalize();

};

static PlayerFunction PlayerFunctionOll;//Emiya 1-5:main.cppのローカル変数にするため移動しましょう

//Emiya 1-4:ScreenCameraクラスはcamera2D.hのようなファイルを用意してそちらに移動しちゃいましょう

//Emiya 1-5:main.cppのローカル変数にするため移動しましょう

