#include	"Bullet.hpp"
#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		BulletObject::BulletObject(void) noexcept {
			SetObjType(Object2DType::Bullet);
		}
		// 
		void BulletObject::Update_OnHitObject(void) noexcept {
			SetDelete();
		}
		void BulletObject::Update_OnHitWall(void) noexcept {
			SetDelete();
			Effect2DControl::Instance()->SetEffect(GetPosition(), EffectType::WallHit, 0.5f);
		}
		// 
		void BulletObject::Init_Sub(void) noexcept {
			SetIsHitOtherObject(true);
			SetIsCheckOnlyHitWall(true);
			SetMass(0.01f);
		}
		void BulletObject::Update_Sub(void) noexcept {
			// ブラー
			m_Blur.AddBlur(0.5f, GetPosition(), this->GetVec());
			// 
			UpdateWallHit();

			m_Blur.Update();
		}
		void BulletObject::DrawShadow_Sub(void) noexcept {
			float Radius = GetSize() / 2.f;
			if (!Cam2DControl::Is2DPositionInDisp(GetPosition(), Radius)) { return; }
			// 影
			auto* BackGround = BackGroundClassBase::Instance();
			Vector2DX DispPos;
			Cam2DControl::Convert2DtoDisp(GetPosition(), &DispPos);

			DispPos += BackGround->GetAmbientLightVec() * 0.25f* Cam2DControl::Instance()->GetCamHeight();
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), Cam2DControl::GetTileToDispSize(Radius), Black);
		}
		void BulletObject::Draw_Sub(void) noexcept {
			float Radius = GetSize() / 2.f;
			if (!Cam2DControl::Is2DPositionInDisp(GetPosition(), Radius)) { return; }
			// ブラー
			for (auto& b : m_Blur.GetBlur()) {
				if (b.IsActive()) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(16.f * b.GetPer()), 0, 255));
					Vector2DX DispPos;
					Cam2DControl::Convert2DtoDisp(b.GetPos(), &DispPos);
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
						static_cast<int>(static_cast<float>(Cam2DControl::GetTileToDispSize(Radius)) * std::pow(b.GetPer(), 0.5f)),
						(this->m_ShotPlayerID == 0) ? Yellow : Green);
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// 本体
			Vector2DX DispPos;
			Cam2DControl::Convert2DtoDisp(GetPosition(), &DispPos);
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), Cam2DControl::GetTileToDispSize(Radius), (this->m_ShotPlayerID == 0) ? Yellow : Green);
		}
		void BulletObject::Dispose_Sub(void) noexcept {}
	};
};
