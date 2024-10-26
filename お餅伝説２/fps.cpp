#include "fps.h"

/// <summary>
/// メインループ前に行う内容:各メンバ変数の初期化など
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::Initialize(VOID)
{
	InitializeRecord();																	//FPS値の記録を初期化
	return;//※VOIDが戻り値の場合returnは書かなくてもok
}

/// <summary>
/// 処理時間計測の開始
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::StartMeasureTime(VOID)
{
	this->m_FirstTakeTime = GetNowHiPerformanceCount();									//Windowsが起動してから現在までの時間をマイクロ秒で得る
	return;
}

/// <summary>
/// 平均FPSを描画
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::Draw(VOID)
{
	//左上に描画
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS：%3.1f", this->m_AverageFPS);	//%3.1f->小数点込みで3文字以上、小数点1桁まで表示
	return;
}

/// <summary>
/// 処理時間計測を終了し、これまでの時間と目標FPSとで待機する時間を割り出し待機します
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::FPSWait(VOID)
{
	LONGLONG NowTakeTime = GetNowHiPerformanceCount();									//Windowsが起動してから現在までの時間をマイクロ秒で得る
	float Deltatime = (NowTakeTime - this->m_FirstTakeTime) / MicroSecond;				//計測開始と終了までの差分時間（デルタタイム）を計算して秒数として保存
	if (Deltatime > 0.0f)																//0で割ってはいけないためそれを確認
	{
		RecordNowFPS(1.0f / Deltatime);													//平均FPSの記録 現在のFPS値は 1秒/1フレームにかかった秒数 で求められます
	}

	float WaitSecond = (1.0f / TargetFPS - Deltatime);									//目標FPSを出すのにかかる時間-デルタタイム=待機する必要がある秒数
	if (0.f < WaitSecond)																//待機時間がある場合
	{
		WaitTimer(static_cast<int>(WaitSecond * MillSecond));							//ミリ秒単位でWaitSecond秒ぶん待機
	}
	return;
}

/// <summary>
/// 平均FPSを計算する処理の初期化
/// </summary>
/// <returns></returns>
VOID FPSControl::InitializeRecord()
{
	for (auto& RecordFPS : this->m_OldFPS)												//リストを全て範囲forで走査
	{
		RecordFPS = TargetFPS;															//計測値をすべてTargetFPSで初期化
	}
	this->m_NowRecordPoint = 0;															//m_NowRecordPointで指定する値を初期化
	this->m_AverageFPS = TargetFPS;														//平均値もTargetFPSになるはずなのでそちらで初期化
	return;
}

/// <summary>
/// 現在のFPS値を記録し、これまでの値から平均FPS値を計算
/// </summary>
/// <param name="nowFPS">保存するFPS値</param>
/// <returns></returns>
VOID FPSControl::RecordNowFPS(float nowFPS)
{
	this->m_OldFPS[this->m_NowRecordPoint] = nowFPS;									//現在のFPSをm_NowRecordPointの位置に記録
	this->m_NowRecordPoint++;															//m_NowRecordPointで指定する値を移動
	if (this->m_NowRecordPoint >= RecordFPSCount) {
		this->m_NowRecordPoint = 0;														//記録する位置がthis->m_OldFPSの範囲を超えてしまったら位置をリセット
	}

	float TotalFPS = 0.0f;
	for (auto& RecordFPS : this->m_OldFPS)												//リストを全て範囲forで走査
	{
		TotalFPS += RecordFPS;															//平均値を求めるため、これまで記録していたFPSを合算
	}
	this->m_AverageFPS = TotalFPS / static_cast<float>(RecordFPSCount);					//データの総数で割り、平均を求める
	return;
}
