#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include "mobchara.h"
#include "fight.h"
#include "fps.h"
#include"MainScreen.h"
#include "camera2D.h"


int displaywidhtX = 0;
int displayhigthY = 0;

 int displaywidth2X = 1920;
 int displayhight2Y = 1080;

 int MeinscreenVertical = 1080;
 int MeinscreenHorizontal = 1920;
 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	PlayerFunction PlayerFunctionOll;
	
	MapData MapDataOll;

	ScreenCamera camera;
	//FPS制御オブジェクトとしてローカル変数を作成
	FPSControl FPSCtrl;

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

	MapDataOll.mapInit();//マップの初期化
	
	MapDataOll.MapEngine();
	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// ゲームループ
	//FPS初期化
	FPSCtrl.Initialize();
	int Exchange = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		FPSCtrl.StartMeasureTime(); //FPS計測を開始

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
			//MapEngine();		//マップの動き
			
			

			PlayerFunctionOll.Playerappdate();		//プレイヤーの更新

			
			MapDataOll.mapDraw();

			
						//画面更新処理
			
			PlayerFunctionOll.PlayerDraw();			//プレイヤー描画
			

			break;

		}

		// ヒットしたか?を画面表示

		FPSCtrl.Draw();		//FPS描画
		ScreenFlip();		//これまで描画した内容をウィンドウに反映する
		FPSCtrl.FPSWait();  //処理時間計測を終了し、これまでの時間と目標FPSとで待機する時間を割り出し待機します
	}
	PlayerFunctionOll.PlayerFinalize();	//プレイヤーの後始末
	//WaitKey();//終了時に即時に終了できるように一応コメントアウトします
	DxLib_End();
	return 0;
}

