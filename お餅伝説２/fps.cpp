#include "fps.h"

//グローバル変数
FPS fps;

//初期化
VOID FPSInit(VOID)
{
	//Windowsが起動してから現在までの時間を得る
	fps.FirstTakeTime = GetNowHiPerformanceCount();
	

	//他の値も初期化
	fps.NowTakeTime = fps.FirstTakeTime;
	fps.OldTakeTime = fps.FirstTakeTime;
	fps.Deltatime = 0.000001f;
	fps.Average = 60.0f;

	return;
}

VOID FPSCheck(VOID)
{
	//現在の時刻
	fps.NowTakeTime = GetNowHiPerformanceCount();

	//デルタタイムを計算
	fps.Deltatime = (fps.NowTakeTime - fps.OldTakeTime) / MicroSecond;

	//現在の時刻を保存
	fps.OldTakeTime = fps.NowTakeTime;

	//現在がMAXフレーム目なら
	if (fps.FrameCount == GameFPS)
	{
		//1フレーム目～MAXフレーム目までの合計時間を計算
		float TotalFrameTime = fps.NowTakeTime - fps.FirstTakeTime;

		//合計時間を理想のFPS値で割って平均値を計算
		float CalcAverage = TotalFrameTime / GameFPS;

		//１秒あたりのフレーム数に変換
		fps.Average = MicroSecond / CalcAverage;
		
		//1フレーム目の時刻を取得
		fps.FirstTakeTime = GetNowHiPerformanceCount();

		//フレーム数を１に戻す
		fps.FrameCount = 1;

	}
	else
	{
		//フレーム数をカウントアップ
		fps.FrameCount;

	}
	return;
}

VOID FPSDraw(VOID)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS：%3.1f", fps.Average);
	return;
}

VOID FPSWait(VOID)
{
	int wait = 0;
	wait =
		MicroSecond / GameFPS * fps.FrameCount
		- (fps.NowTakeTime - fps.FirstTakeTime);

	wait /= MillSecond;
	
	if (wait > 0 && wait <= waitTimeMill)
	{
		WaitTimer(wait);
	
	}

	return;

}