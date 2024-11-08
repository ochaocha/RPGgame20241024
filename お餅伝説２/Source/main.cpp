#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "fps.h"
#include "Screen.h"
#include "camera2D.h"
#include <Windows.h>
#include "Battle.h"

int displaywidhtX = 0;
int displayhigthY = 0;

 int displaywidth2X = 1920;
 int displayhight2Y = 1080;

 int MeinscreenVertical = 1080;
 int MeinscreenHorizontal = 1920;


 


 int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	 //キャラクターのオブジェクト生成
	CharacterManager::Create();

#if SINGLETON
	//マップデータのオブジェクト生成
	MapData::Create();
	//マップデータのシングルトン生成
	MapData* MapDataOllPtr = MapData::Instance();
#else

	MapData MapDataOll;
#endif
	//バトル画面のインターフェース
	BattleScreenChanger battlescreen;
	
	//バトルのデータ構造を所有
	BattleData battledata;


	//スクロールのカメラを生成
	ScreenCamera camera;
	//FPS制御オブジェクトとしてローカル変数を作成
	FPSControl FPSCtrl;

	// DxLib初期化
	ChangeWindowMode(TRUE);

	//SetGraphMode(displaywidth, displayhight, 16);
	camera.Screen(displaywidhtX, displayhigthY, displaywidth2X,displayhight2Y);

	
	//エラー処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}


	//playerステータスのオブジェクト生成
	
	CharacterStatus obj;
	obj.playerstatusInit();
	CharacterStatus* playerstatus;
	playerstatus = &obj;


	//NPCのオブジェクト生成
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);

	//MapDataOll変数を初期化
	MapDataOllPtr->Engine();//マップの読み込み
	MapDataOllPtr->Init();//マップの初期化
	
	int nowCount, prevCount;

	nowCount = prevCount = GetNowCount();
	// ゲームループ
	//FPS初期化
	FPSCtrl.Initialize();
	//スクリーンの変数
	int Exchange = 0;
	
	//バトルのエンカウント処理
	bool BattleEncounterFlag = false;
	//メインクラスを実態として定義
	MeinScreenChanger Mein;
	battlescreen.Imginitialize();
	//メイン画面の描画入力処理をさせている
	Mein.ScreenInit();
	

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//FPS計測を開始
		FPSCtrl.StartMeasureTime(); 
		// deltaTime計測
		float deltaTime;

		nowCount = GetNowCount();

		deltaTime = (nowCount - prevCount) / 1000.0f;

		switch (Exchange)
		{
		case 0://ゲームスタートシーン

			Mein.Draw(&Exchange);
			break;

		case 1://オプションシーン

			//しっかりとここに行っていることの確認済み

		case 2://メニューシーン

			ClearDrawScreen();		//マップの描画
			//MapEngine();		//マップの動き
			MapDataOllPtr->Update();			//マップの動き

			//プレイヤーの入力処理
			{
				Input Player;

				Player.SetInput
				(
					CheckHitKey(KEY_INPUT_A),
					CheckHitKey(KEY_INPUT_D),
					CheckHitKey(KEY_INPUT_S),
					CheckHitKey(KEY_INPUT_W)
				);
				battledata.InputEncounterCheck
				(
					BattleEncounterFlag,
					&Exchange,
					CheckHitKey(KEY_INPUT_A),
					CheckHitKey(KEY_INPUT_D),
					CheckHitKey(KEY_INPUT_S),
					CheckHitKey(KEY_INPUT_W)

				);
				if (Exchange == 3)
				{
					
					break;
				}
				CharacterManager::Instance()->GetChara(0).CalcInput(Player);
			}

			//NPCの入力処理
			{
				Input Player;
				Player.SetInput(
					1,
					0,
					0,
					0
				);
				CharacterManager::Instance()->GetChara(1).CalcInput(Player);
			}

			CharacterManager::Instance()->Update();		//プレイヤーの更新

			MapDataOllPtr->Draw();
			//画面更新処理
			CharacterManager::Instance()->Draw();			//プレイヤー描画
			
			break;


			//バトルシーン
		case 3:
			//エンカウントの実装を確認しました
	
			ClearDrawScreen();
			//バトル中の背景画像
			battlescreen.Scene();
			//バトル中のplayerを描画
			battlescreen.PlayerDraw();
			//バトル中のEnemyを描画
			battlescreen.EnemyDraw();
			//バトル中のplayerのデータを格納
			battledata.StatusUI(
				playerstatus->CharacterHP,
				playerstatus->CharacterATK,
				playerstatus->CharacterMP,
				playerstatus->CharacterDEF,
				playerstatus->CharacterLUCK,
				playerstatus->CharacterAGI
			);

			
			break;



		};
		

		// ヒットしたか?を画面表示

		FPSCtrl.Draw();		//FPS描画
		ScreenFlip();		//これまで描画した内容をウィンドウに反映する
		FPSCtrl.FPSWait();  //処理時間計測を終了し、これまでの時間と目標FPSとで待機する時間を割り出し待機します
	}
	CharacterManager::Instance()->DeleteChara();
	//WaitKey();//終了時に即時に終了できるように一応コメントアウトします
	DxLib_End();

	return 0;
}

