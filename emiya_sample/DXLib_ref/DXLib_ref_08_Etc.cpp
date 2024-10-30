#include "DXLib_ref_08_Etc.hpp"

namespace DXLibRef {
	// �V���O���g���̎��Ԓ�`
	const CameraShake* SingletonBase<CameraShake>::m_Singleton = nullptr;
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
	// 
	void CameraShake::Update(void) noexcept {
		auto* DrawParts = DXDraw::Instance();
		if (this->m_ShakeTotalTime > 0.f) {
			//���͈̔͂Ń����_���Ȓn�_���擾���A���̕����ɖ��t���[��m_CamShake�𓮂���
			auto RandRange = this->m_ShakeTime / this->m_ShakeTotalTime * this->m_ShakePower;
			//2��⊮���s�����ƂŎ��R�ȗh������o
			Easing(&this->m_CamShake1, Vector2DX::vget(GetRandf(RandRange), GetRandf(RandRange)), 0.8f, EasingType::OutExpo);
			Easing(&this->m_CamShake2, this->m_CamShake1, 0.8f, EasingType::OutExpo);
			//���Ԍo�߂Ŏ�܂�悤�ɂ��邽�ߎ��Ԃ��v�����Č��炵�܂�
			this->m_ShakeTime = GetMax(this->m_ShakeTime - DrawParts->GetDeltaTime(), 0.f);
		}
	}
};
