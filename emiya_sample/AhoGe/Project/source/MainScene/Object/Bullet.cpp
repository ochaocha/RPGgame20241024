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
			auto* DrawParts = DXDraw::Instance();
			auto* BackGround = BackGroundClassBase::Instance();

			float Radius = GetSize() / 2.f;
			int R = Cam2DControl::GetTileToDispSize(Radius);
			Vector2DX DispPos;
			Cam2DControl::ConvertTiletoDisp(GetPosition() + BackGround->GetAmbientLightVec() * 0.25f, &DispPos);
			// 範囲外
			if (!HitPointToRectangle(
				static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
				-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
				return;
			}
			// 影
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), R, Black);
		}
		void BulletObject::Draw_Sub(void) noexcept {
			float Radius = GetSize() / 2.f;
			auto* DrawParts = DXDraw::Instance();
			int R = Cam2DControl::GetTileToDispSize(Radius);
			Vector2DX Pos;
			Cam2DControl::ConvertTiletoDisp(GetPosition(), &Pos);
			// 範囲外
			if (!HitPointToRectangle(
				static_cast<int>(Pos.x), static_cast<int>(Pos.y),
				-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
				return;
			}
			// ブラー
			for (auto& b : m_Blur.GetBlur()) {
				if (b.IsActive()) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(16.f * b.GetPer()), 0, 255));
					Vector2DX DispPos;
					Cam2DControl::ConvertTiletoDisp(b.GetPos(), &DispPos);
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
						static_cast<int>(static_cast<float>(R) * std::pow(b.GetPer(), 0.5f)),
						(this->m_ShotPlayerID == 0) ? Yellow : Green);
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// 本体
			DrawCircle(static_cast<int>(Pos.x), static_cast<int>(Pos.y), R, (this->m_ShotPlayerID == 0) ? Yellow : Green);
		}
		void BulletObject::Dispose_Sub(void) noexcept {}
	};
};
