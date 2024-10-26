#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include "mobchara.h"
#include "fight.h"
#include "fps.h"
#include"MainScreen.h"
#include "Camera.h"


int displaywidhtX = 0;
int displayhigthY = 0;

 int displaywidth2X = 1920;
 int displayhight2Y = 1080;

 int MeinscreenVertical = 1080;
 int MeinscreenHorizontal = 1920;
 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//Emiya 1-5:移動先　staticである必要がなくなるので外しましょう
	

	// DxLib初期化
	ChangeWindowMode(TRUE);

	//スクリーンがあった場所
	//SetGraphMode(displaywidth, displayhight, 16);
	camera.Screen(displaywidhtX, displayhigthY, displaywidth2X,displayhight2Y);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	
	

	PlayerFunctionOll.PlayerInit(displaywidth2X, displayhight2Y);

	mapInit();//マップの初期化
	
	MapEngine();
	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// ゲームループ
	//FPS初期化
	FPSInit();
	int Exchange = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime計測
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;

		PlayeAttackType Playerchoicetern(); //プレイヤーの攻撃パターン構造体
		

		switch (Exchange)
		{
		case 0://メインスクリーン
			
			Mein.MeinDrawScreen(&Exchange);

			break;
			
		case 1:
			
			ClearDrawScreen();		//マップの描画
			//MapEngine();
			mapaupdate();			//マップの動き
			
			

			PlayerFunctionOll.Playerappdate();		//プレイヤーの更新

			
			mapDraw();	

			
						//画面更新処理
			
			PlayerFunctionOll.PlayerDraw();			//プレイヤー描画
			

			break;

		}

		// ヒットしたか?を画面表示
		ScreenFlip();   //スクリーンの動きを滑らかにする処理
		{
			FPSCheck(); //FPS計測
			FPSDraw();  //FPS描画
			FPSWait();  //FPS待機
		}
	
	}
	PlayerFunctionOll.PlayerFinalize();	//プレイヤーの後始末
	WaitKey();
	DxLib_End();
	return 0;
}

