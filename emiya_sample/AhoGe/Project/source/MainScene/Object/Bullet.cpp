#include	"Bullet.hpp"
#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		BulletObject::BulletObject(void) noexcept {
			SetObjType(static_cast<int>(Object2DType::Bullet));
		}
		// 
		void BulletObject::OnHitObject_Sub(void) noexcept {
			SetDelete();
		}
		// 
		void BulletObject::Initialize_Sub(void) noexcept {
			SetIsHitOtherObject(true);
			SetMass(0.01f);
		}
		void BulletObject::Update_Sub(void) noexcept {
			// �u���[
			m_Blur.AddBlur(0.5f, GetPosition(), this->GetVec());
			m_Blur.Update();
			// �ړ�����
			auto* BackGround = BackGroundClassBase::Instance();
			auto* Effect2DParts = Effect2DControl::Instance();
			// �ړ���̍��W���擾
			Vector2DX PosTmp = this->GetPosition() + this->GetVec() * DXLib_ref::Instance()->GetDeltaTime();
			// �ǏՓˉ��Z(���������炻�̏�ŏ��łƂ���)
			if (BackGround->CheckLinetoMap(this->GetPrevPos(), &PosTmp, GetSize() / 2.f, false)) {
				// ���g�������t���O�𗧂Ă�
				SetDelete();
				// �q�b�g�ʒu�Ƀq�b�g�G�t�F�N�g��ǉ�
				Effect2DParts->SetEffect(PosTmp, EffectType::WallHit, 0.5f);
			}
			SetPosition(PosTmp);
		}
		void BulletObject::DrawShadow_Sub(void) noexcept {
			auto* BackGround = BackGroundClassBase::Instance();

			float Radius = GetSize() / 2.f;
			int R = Cam2DControl::GetTileToDispSize(Radius);
			Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(GetPosition() + BackGround->GetAmbientLightVec() * 0.25f);
			// �͈͊O
			if (!IsOnScreen(DispPos, R)) {
				return;
			}
			// �e
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), R, Black);
		}
		void BulletObject::Draw_Sub(void) noexcept {
			float Radius = GetSize() / 2.f;
			int R = Cam2DControl::GetTileToDispSize(Radius);
			Vector2DX Pos = Cam2DControl::ConvertTiletoDisp(GetPosition());
			// �͈͊O
			if (!IsOnScreen(Pos, R)) {
				return;
			}
			// �u���[
			for (auto& b : m_Blur.GetBlur()) {
				if (b.IsActive()) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(16.f * b.GetPer()), 0, 255));
					Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(b.GetPos());
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
						static_cast<int>(static_cast<float>(R) * std::pow(b.GetPer(), 0.5f)),
						(this->m_ShotPlayerID == PlayerCharacter) ? Yellow : Green);
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// �{��
			DrawCircle(static_cast<int>(Pos.x), static_cast<int>(Pos.y), R, (this->m_ShotPlayerID == PlayerCharacter) ? Yellow : Green);
		}
		void BulletObject::Dispose_Sub(void) noexcept {}
	}
}
