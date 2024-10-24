#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include "mobchara.h"
#include "fight.h"
#include "fps.h"

const int displaywidth = 1920;
const int displayhight = 1080;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	
	// DxLib初期化
	ChangeWindowMode(TRUE);
	SetGraphMode(displaywidth, displayhight, 16);
	mobcharaimg();
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	
	PlayerInit(displaywidth,displayhight);
	mapInit();//マップの初期化
	
	MapEngine();
	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// ゲームループ
	//FPS初期化
	FPSInit();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime計測
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;

		PlayeAttackType Playerchoicetern(); //プレイヤーの攻撃パターン構造体


		switch (int Exchange = 1)
		{
		case 0://メインスクリーン

		case 1:
			
			ClearDrawScreen();		//マップの描画
			//MapEngine();
			mapaupdate();			//マップの動き
			
					//

			Playerappdate();		//プレイヤーの更新

			
			mapDraw();	

			
						//画面更新処理
			
			PlayerDraw();			//プレイヤー描画
			
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
	PlayerFinalize();	//プレイヤーの後始末
	WaitKey();
	DxLib_End();
	return 0;
}

//テスト