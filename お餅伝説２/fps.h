#pragma once

#include "DXlib.h"
//マクロ定義
#define GameFPS    60   //設定するFPS

#define MicroSecond       1000000.0f       //1マイクロ秒」
#define MillSecond        1000.0f          //1ミリ秒
#define waitTimeMill       3000			   //最大待ち時間（ミリ）

typedef struct _FPS
{
	LONGLONG FirstTakeTime = 0;	           //デルタタイム（経過時間）		
	LONGLONG NowTakeTime= 0;	           //現在の計測時間
	LONGLONG OldTakeTime = 0;	           //以前の計測時間
	
	float Deltatime = 0.000001f;			//デルタタイム（経過時間）
	int FrameCount = 1;						//現在のフレーム数（1フレーム目～MAXフレームめまで）
	float Average = 0.0f;					//平均FPS値

}FPS;

//外部グローバル変数
extern FPS fps;

//外部プロトタイプ宣言
extern  VOID FPSInit(VOID);					//FPS初期値			
extern  VOID FPSCheck(VOID);				//FPS計画				
extern  VOID FPSDraw(VOID);					//FPS描画			
extern  VOID FPSWait(VOID);					//FPS待機			
