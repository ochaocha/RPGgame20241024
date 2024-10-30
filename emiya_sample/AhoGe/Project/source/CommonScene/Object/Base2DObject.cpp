#include	"Base2DObject.hpp"
#include	"../../MainScene/BackGround/BackGround.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		void Blur2DControl::BlurParts::Update(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			this->m_Time = std::max(this->m_Time - DrawParts->GetDeltaTime(), 0.f);
		}
		void Blur2DControl::AddBlur(float Blur, const Vector2DX& Pos, const Vector2DX& Vec) noexcept {
			auto* DrawParts = DXDraw::Instance();
			int Max = static_cast<int>(std::max(1.f, 300.f / std::max(30.f, DrawParts->GetFps())));
			Vector2DX Goal = Pos - Vec * ((Tile_DispSize * Cam2DControl::Instance()->GetCamHeight()) * DrawParts->GetDeltaTime());
			for (int i = 0; i < Max; i++) {
				this->m_Blur.at(static_cast<size_t>(this->m_BlurNow)).Set(Lerp(Goal, Pos, (static_cast<float>(i) / static_cast<float>(Max))), Blur);
				++this->m_BlurNow %= static_cast<int>(this->m_Blur.size());
			}
		}

		void Base2DObjectKai::UpdateWallHit(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			// •ÇÕ“Ë‰‰ŽZ
			Vector2DX Vec = this->GetVec() * ((Tile_DispSize * Cam2DControl::Instance()->GetCamHeight()) * DrawParts->GetDeltaTime());
			{
				bool IsHit = false;
				int Max = static_cast<int>(std::max(1.f, 60.f / std::max(30.f, DrawParts->GetFps())));
				for (int i = 0; i < Max; i++) {
					Vector2DX PosTmp = this->GetPos() + Vec * (1.f / static_cast<float>(Max));
					IsHit |= BackGround->CheckLinetoMap(this->GetPrevPos(), &PosTmp, GetSize() / 2.f, !m_IsCheckOnlyHitWall);
					SetPos(PosTmp);
					if (IsHit && m_IsCheckOnlyHitWall) { break; }
				}
				if (IsHit) {
					Update_OnHitWall();
				}
			}
		}
	};
};
