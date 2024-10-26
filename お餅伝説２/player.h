#pragma once
#include "collision.h"
//Emiya 1-3:ここに移動

class PlayerFunction {
	//Emiya 1-1:ここに移動
	float px, py;								//プレイヤーの初期座標
	int animNowType;							//プレイヤーの配列の縦要素数
	int animNowIndex;							//プレイヤーの現在のアニメーション

	int animDirectionOffset;					//アニメーションの方向を変えるための変数

	bool charaMove = false;						//プレイヤーが動いているかどうか

	static const int animPatternNum = 8;			// アニメーションのパターン数		//Emiya 1-2:1-1で移動後エラーが出るので静的変数としてしまいましょう

	static const int animTypeNum = 6;			// アニメーションの種類		//Emiya 1-2:1-1で移動後エラーが出るので静的変数としてしまいましょう
	const int animSizeX = 48;					//プレイヤーのアニメーションサイズ
	const int animSizeY = 64;

	const int hitSizeX = 40;					
	const int hitSizeY = 60;



	int drawOffsetX;								//プレイヤーの大きさを変える変数
	int drawOffsetY;					

	int charaimg[animPatternNum * animTypeNum];		//キャラクターのアニメーション要素数

	float time = 0.f;								
public:
	/// @brief プレイヤーの読み込み
	/// @param displaywidth 
	/// @param displayhight 
	void PlayerInit(int displaywidth, int displayhight);
	/// @brief プレイヤーの移動
	void Playerappdate();
	/// @brief プレイヤ―アニメーションを描画
	void PlayerDraw();
	/// @brief プレイヤーの後始末
	void PlayerFinalize();

};

//Emiya 1-4:ScreenCameraクラスはcamera2D.hのようなファイルを用意してそちらに移動しちゃいましょう

//Emiya 1-5:main.cppのローカル変数にするため移動しましょう

