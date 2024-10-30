#include "DXLib_ref_08_Etc.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const CameraShake* SingletonBase<CameraShake>::m_Singleton = nullptr;
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	float GetEasingRatio(EasingType EasingType, float ratio) noexcept {
		auto* DrawParts = DXDraw::Instance();
		switch (EasingType) {
		case EasingType::OutExpo:
			// 線形補完にて指数関数で0~1になるような値を出力
			return (1.f - std::powf(ratio, FrameRate * DrawParts->GetDeltaTime()));
		default:
			return 1.f;
		}
	}
	//定積分で1Fあたりの速度を得る
	float EasingMove(float* NowSpeed, float TargetSpeed, float ratio) noexcept {
		auto* DrawParts = DXDraw::Instance();
		//速度を計算
		float answer = TargetSpeed * DrawParts->GetDeltaTime() + (*NowSpeed - TargetSpeed) / (FrameRate * std::log(ratio)) * (-GetEasingRatio(EasingType::OutExpo, ratio));
		//目標速度に向けて現在の速度を修正しておく
		Easing(NowSpeed, TargetSpeed, ratio, EasingType::OutExpo);
		return answer;
	}
	// 
	void CameraShake::Update(void) noexcept {
		auto* DrawParts = DXDraw::Instance();
		if (this->m_ShakeTotalTime > 0.f) {
			//この範囲でランダムな地点を取得し、その方向に毎フレームm_CamShakeを動かす
			auto RandRange = this->m_ShakeTime / this->m_ShakeTotalTime * this->m_ShakePower;
			//2回補完を行うことで自然な揺れを演出
			Easing(&this->m_CamShake1, Vector2DX::vget(GetRandf(RandRange), GetRandf(RandRange)), 0.8f, EasingType::OutExpo);
			Easing(&this->m_CamShake2, this->m_CamShake1, 0.8f, EasingType::OutExpo);
			//時間経過で弱まるようにするため時間を計測して減らします
			this->m_ShakeTime = GetMax(this->m_ShakeTime - DrawParts->GetDeltaTime(), 0.f);
		}
	}
};
