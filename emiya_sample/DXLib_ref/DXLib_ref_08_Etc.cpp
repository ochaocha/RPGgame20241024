#include "DXLib_ref_08_Etc.hpp"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	float GetEasingRatio(EasingType EasingType, float ratio) noexcept {
		switch (EasingType) {
		case EasingType::OutExpo:
			// 線形補完にて指数関数で0~1になるような値を出力
			return (1.f - std::powf(ratio, FrameRate * DXLib_ref::Instance()->GetDeltaTime()));
		default:
			return 1.f;
		}
	}
	//定積分で1Fあたりの速度を得る
	float EasingMove(float* NowSpeed, float TargetSpeed, float ratio) noexcept {
		//速度を計算
		float Answer = TargetSpeed * DXLib_ref::Instance()->GetDeltaTime() + (*NowSpeed - TargetSpeed) / (FrameRate * std::log(ratio)) * (-GetEasingRatio(EasingType::OutExpo, ratio));
		//目標速度に向けて現在の速度を修正しておく
		Easing(NowSpeed, TargetSpeed, ratio, EasingType::OutExpo);
		return Answer;
	}
};
