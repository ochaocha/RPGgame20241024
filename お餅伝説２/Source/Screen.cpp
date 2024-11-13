#include "Dxlib.h"
#include "Screen.h"


constexpr int  graphX = 600, graphY = 600, graph2X = 1320, graph2Y = 1000;


/// @brief エフェクトの初期化
void MeinScreenChanger::ScreenInit()
{
	EfectNowIndex = 0;
	LoadDivGraph("Data/Free/startscreenfile/233.png",StartEfectNum,StartEfectXNum,StartEfectYNum, StartEfectXSize, StartEfectYSize, EfectImg);

}




/// @briefスタート画面の関数
/// @param change 
/// @param MeinscreenVertical 
/// @param MeinscreenHorizontal 
void MeinScreenChanger::Draw(int* screenchange)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////音量設定
	//////////////////////////////////////////////////////////////////////////////////////////
	ChangeVolumeSoundMem(50, StartSound);	
	ChangeVolumeSoundMem(100, StartDesition);	
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////色設定と画像設定
	//////////////////////////////////////////////////////////////////////////////////////////
	
	
	//スタートの文字の色
	unsigned int StatrString = GetColor(ColorHandle, ColorHandle, ColorHandle);
	//オプションの文字の色
	unsigned int GreenString= GetColor(0, ColorHandle, 0);
	//スタート画面の背景画像ハンドル
	int startHandle = LoadGraph("Data/img/pipo-bg004b.png");
	//スタート画面の枠画像ハンドル
	int Hnadle = LoadGraph("Data/img/R.png", TRUE);

	//マウスポイントの取得
	GetMousePoint(&mauseX, &mauseY);

	//フォントサイズの変更
	SetFontSize(34);

	//スタート画面背景の描画
	DrawExtendGraph(0, 0,1920,1080,startHandle, TRUE);
	//スタート画面の枠を描画
	DrawExtendGraph(graphX, graphY, graph2X, graph2Y, Hnadle, TRUE);

	DrawString(graphX + 285, graphY + 100, "スタート", StatrString);

	DrawString(graphX + 270, graphY + 150, "オプション", StatrString);

	

		//スタートの文字の範囲に入った時のif文
		if (mauseX >= graphX+285 && mauseX <= graphX + 420 && mauseY >= graphY+100&& mauseY <= graphY+ 134)
		{
			//範囲に入って音が出力されていない時
			if (ClickStartSound == false)
			{
				//範囲の外に出るまで音を鳴らさないようにする
				ClickStartSound = true;
				//sound再生
				PlaySoundMem(StartSound, DX_PLAYTYPE_BACK);
			}
			
			time += 1.0f / 3.f;

			//範囲に行ったときにエフェクトの出現
			if (time > 1.0f)
			{
				time -= 1.0f;
				EfectNowIndex += 1;
				if (EfectNowIndex >= StartEfectXNum)
				{
					EfectNowIndex -= StartEfectXNum;
				}
			}
			DrawExtendGraph(graphX + 220, graphY + 100, graphX + 480, graphY + 160, EfectImg[EfectNowIndex], TRUE);
			//スタートの範囲に入るとエフェクトが出力される
			
			StatrString = GetColor(ColorHandleChange, ColorHandle, ColorHandleChange);
			DrawString(graphX + 285, graphY + 100, "スタート", StatrString);
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{//エフェクトのフレーム数
				
				PlaySoundMem(StartDesition, DX_PLAYTYPE_NORMAL, TRUE);
				*screenchange = 2;
			}
		}
		else
		{
			ClickStartSound = false;
		}
	

		if (mauseX >= graphX + 270 && mauseX <= graphX + 436 && mauseY >= graphY + 150 && mauseY <= graphY + 184)
		{
			StatrString = GetColor(ColorHandleChange, ColorHandle, ColorHandleChange);
			DrawString(graphX + 270, graphY + 150, "オプション", StatrString);
			if (ClickOptionSound == false)
			{
				ClickOptionSound = true;
				PlaySoundMem(StartSound, DX_PLAYTYPE_BACK);
			}


			GreenString = GetColor(ColorHandleChange, ColorHandleChange, ColorHandleChange);
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{
				*screenchange = 1;
			}
		}
		else
		{

			ClickOptionSound =false;
		}

		
	
}


void MeinScreenChanger::Music()
{
	if (CheckSoundMem(MainScreenBgm)==0)
	{
		ChangeVolumeSoundMem(82, MainScreenBgm);
		PlaySoundMem(MainScreenBgm, DX_PLAYTYPE_LOOP, TRUE);
	}
}

