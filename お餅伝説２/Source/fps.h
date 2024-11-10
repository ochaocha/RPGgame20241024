#pragma once

#include "DXlib.h"
#include <array>

//定数定義(defineでもいいのですが、記法が古いので非推奨です。)
constexpr float TargetFPS = 60.f;					//設定するFPS
constexpr int RecordFPSCount = 60;					//FPSの数値を記録する総数
constexpr auto MicroSecond = 1000.0f * 1000.0f;		//1マイクロ秒
constexpr auto MillSecond = 1000.0f;				//1ミリ秒

/// <summary>
/// FPS制御クラス
/// FPS = 画面の更新を1秒に何回行うか(FramePerSecond)
/// </summary>
class FPSControl {
private:								//外に出さないメンバ変数
	LONGLONG		m_FirstTakeTime = 0;				//計測開始時の時間をマイクロ秒単位で保存
														//平均FPSを計算するための変数
	std::array<float, RecordFPSCount> m_OldFPS{};		//記録するFPS値(RecordFPSCountフレーム前まで記録します)
	int				m_NowRecordPoint{};					//m_OldFPSのどの位置に現在のFPSを記録するか
	float			m_AverageFPS = 0.0f;				//平均FPS値

public:									//外に出すメンバ関数のプロトタイプ宣言
	VOID Initialize(VOID);				//メインループ前に行う内容:各メンバ変数の初期化など			
	VOID StartMeasureTime(VOID);		//処理時間計測の開始				
	VOID Draw(VOID);					//FPS描画			
	VOID FPSWait(VOID);					//処理時間計測を終了し、これまでの時間と目標FPSとで待機する時間を割り出し待機します
private:								//外に出さないメンバ関数のプロトタイプ宣言
	VOID InitializeRecord();			//平均FPSを計算する処理の初期化
	VOID RecordNowFPS(float nowFPS);	//現在のFPS値を記録し、これまでの値から平均FPS値を計算
};
