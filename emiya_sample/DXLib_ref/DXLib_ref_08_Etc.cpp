#include "DXLib_ref_08_Etc.hpp"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	float GetEasingRatio(EasingType EasingType, float ratio) noexcept {
		auto* DrawParts = DXDraw::Instance();
		switch (EasingType) {
		case EasingType::OutExpo:
			// ���`�⊮�ɂĎw���֐���0~1�ɂȂ�悤�Ȓl���o��
			return (1.f - std::powf(ratio, FrameRate * DrawParts->GetDeltaTime()));
		default:
			return 1.f;
		}
	}
	//��ϕ���1F������̑��x�𓾂�
	float EasingMove(float* NowSpeed, float TargetSpeed, float ratio) noexcept {
		auto* DrawParts = DXDraw::Instance();
		//���x���v�Z
		float answer = TargetSpeed * DrawParts->GetDeltaTime() + (*NowSpeed - TargetSpeed) / (FrameRate * std::log(ratio)) * (-GetEasingRatio(EasingType::OutExpo, ratio));
		//�ڕW���x�Ɍ����Č��݂̑��x���C�����Ă���
		Easing(NowSpeed, TargetSpeed, ratio, EasingType::OutExpo);
		return answer;
	}
};
