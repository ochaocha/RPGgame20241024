#include	"Metal.hpp"
#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		MetalObject::MetalObject(void) noexcept {
			this->m_InputVec.Set(0.f, 0.f);
			SetObjType(static_cast<int>(Object2DType::Metal));
		}
		MetalObject::~MetalObject(void) noexcept {}
		void MetalObject::DrawHPBer() noexcept {
			auto* DrawParts = DXDraw::Instance();
			int R = Cam2DControl::GetTileToDispSize(1.f);
			Vector2DX DispPos;
			Cam2DControl::ConvertTiletoDisp(GetPosition(), &DispPos);
			// îÕàÕäO
			if (!HitPointToRectangle(
				static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
				-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
				return;
			}
			int xmin = DrawParts->GetScreenY(-50);
			int ymin = DrawParts->GetScreenY(-50);
			int xmax = DrawParts->GetScreenY(50);

			int xper = xmin + (xmax - xmin) * this->GetHitPoint() / this->GetHitPointMax();
			DrawLine(static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xmax), static_cast<int>(DispPos.y + ymin), Gray75, 10);
			DrawLine(static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xper), static_cast<int>(DispPos.y + ymin), Green, 10);
		}
		// 
		void MetalObject::Update_OnHitObject(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			const auto& Obj = Obj2DParts->GetObj(GetHitUniqueID());
			if (Obj) {
				std::shared_ptr<Base2DObject>& o = (std::shared_ptr<Base2DObject>&)(Obj);
				// íeà»äOÇ™ìñÇΩÇ¡ÇΩéûÇÕà»â∫ÇÕí Ç≥Ç»Ç¢
				if (o->GetObjType() != static_cast<int>(Object2DType::Rocket) && o->GetObjType() != static_cast<int>(Object2DType::Bullet)) { return; }

				Effect2DControl::Instance()->SetEffect(o->GetPosition(), EffectType::Guard, 1.f);
				// Effect2DControl::Instance()->Set(o->GetPosition(), EffectType::Damage, 0.25f);
				if (o->GetObjType() == static_cast<int>(Object2DType::Rocket)) {
					this->m_HitPoint = 0;
					SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Bomb))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				else {
					this->m_HitPoint--;
					SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Guard))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				if (this->m_HitPoint == 0) {
					Effect2DControl::Instance()->SetEffect(GetPosition(), EffectType::WallHit, 2.f);
					SetDelete();
				}
			}
		}
		// 
		void MetalObject::Init_Sub(void) noexcept {
			SetIsHitOtherObject(true);
			SetMass(100000.f);
			m_Foot = 0.f;
			for (auto& g : m_MissileID) {
				g = 0;
			}
			m_MissileIDNum = 0;
			this->m_HitPoint = m_MaxHitPoint;
		}
		void MetalObject::Update_Sub(void) noexcept {
			if (GetIsFirstLoop()) {
				m_BasePos = GetPosition();
				m_PosL = GetPosition();
				m_PosR = GetPosition();
			}
			if (!m_CanMove) {
				SetVec(Vector2DX::zero());
				return;
			}
			auto* DrawParts = DXDraw::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();

			auto& Chara = PlayerMngr->GetPlayer((PlayerID)0)->GetChara();
			if (!Chara) {
				SetVec(Vector2DX::zero());
				return;
			}
			float Length = (GetPosition() - Chara->GetPosition()).magnitude();

			this->m_Rad = DX_PI_F - GetRadVec(Chara->GetPosition() - GetPosition());
			{
				this->m_Rad_R += GetRadRad2Rad(this->m_Rad_R, this->m_Rad) * 0.6f * DrawParts->GetDeltaTime();
				if (this->m_Rad_R < 0.f) { this->m_Rad_R += DX_PI_F * 2.f; }
				if (this->m_Rad_R > DX_PI_F * 2.f) { this->m_Rad_R -= DX_PI_F * 2.f; }
			}
			{
				// à⁄ìÆë¨ìx
				float SpeedLimit = 0.5f;

				bool WKey = false;
				bool SKey = false;
				bool AKey = false;
				bool DKey = false;

				{
					Vector2DX LastFindPos = Chara->GetPosition();
					float Len = 0.1f;
					Vector2DX Vec = LastFindPos - GetPosition();
					if (Length > 17.5f) {
						WKey = (Vec.y > Len / 3.f);
						SKey = (Vec.y < -Len / 3.f);
						AKey = (Vec.x < -Len / 3.f);
						DKey = (Vec.x > Len / 3.f);
					}
					if (Length < 12.5f) {
						WKey = (Vec.y < -Len / 3.f);
						SKey = (Vec.y > Len / 3.f);
						AKey = (Vec.x > Len / 3.f);
						DKey = (Vec.x < -Len / 3.f);
					}
				}
				// äOÇ©ÇÁñﬂÇÈ
				Vector2DX Vec = m_BasePos - GetPosition();
				if (Vec.y < -20) {
					WKey = false;
					SKey = true;
				}
				if (Vec.y > 20) {
					WKey = true;
					SKey = false;
				}
				if (Vec.x < -20) {
					AKey = true;
					DKey = false;
				}
				if (Vec.x > 20) {
					AKey = false;
					DKey = true;
				}
				this->m_Speed = 0.f;
				// à⁄ìÆââéZ
				if (DKey && !AKey) {
					this->m_InputVec.x = std::clamp(this->m_InputVec.x + 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else if (AKey && !DKey) {
					this->m_InputVec.x = std::clamp(this->m_InputVec.x - 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else {
					Easing(&this->m_InputVec.x, 0.f, 0.95f, EasingType::OutExpo);
				}
				if (WKey && !SKey) {
					this->m_InputVec.y = std::clamp(this->m_InputVec.y + 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else if (SKey && !WKey) {
					this->m_InputVec.y = std::clamp(this->m_InputVec.y - 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else {
					Easing(&this->m_InputVec.y, 0.f, 0.95f, EasingType::OutExpo);
				}
				this->m_Speed = std::max(std::abs(this->m_InputVec.x), std::abs(this->m_InputVec.y));
				// 
				SetVec(this->m_InputVec.normalized() * this->m_Speed * 5.f);
			}
			{
				m_Foot += DrawParts->GetDeltaTime() / 2.f;
				if (m_Foot > 1.f) {
					m_Foot -= 1.f;
				}

				if (0.25f + 0.1f < m_Foot && m_Foot < 0.5f - 0.1f) {
					{
						this->m_RadL += GetRadRad2Rad(this->m_RadL, this->m_Rad) * 10.f * DrawParts->GetDeltaTime();
						if (this->m_RadL < 0.f) { this->m_RadL += DX_PI_F * 2.f; }
						if (this->m_RadL > DX_PI_F * 2.f) { this->m_RadL -= DX_PI_F * 2.f; }
					}
					Easing(&m_PosR, GetPosition() + GetVec() * 1.f, 0.5f, EasingType::OutExpo);
					m_FootDown = true;
				}
				else {
					if (m_FootDown) {
						if (Length < 30.f) {
							Cam2DControl::Instance()->SetCamShake(0.1f, 20.f * 5.f / std::max(Length, 1.f));
						}
					}
					m_FootDown = false;
				}
				if (0.75f + 0.1f < m_Foot && m_Foot < 1.f - 0.1f) {
					{
						this->m_RadR += GetRadRad2Rad(this->m_RadR, this->m_Rad) * 10.f * DrawParts->GetDeltaTime();
						if (this->m_RadR < 0.f) { this->m_RadR += DX_PI_F * 2.f; }
						if (this->m_RadR > DX_PI_F * 2.f) { this->m_RadR -= DX_PI_F * 2.f; }
					}
					Easing(&m_PosL, GetPosition() + GetVec() * 1.f, 0.5f, EasingType::OutExpo);
					m_FootDown = true;
				}
				else {
					if (m_FootDown) {
						if (Length < 30.f) {
							Cam2DControl::Instance()->SetCamShake(0.1f, 20.f * 5.f / std::max(Length, 1.f));
						}
					}
					m_FootDown = false;
				}
			}
			// 
			{
				// íe
				if (this->m_ShotCoolTime == 0.f) {
					if (10.f < Length && Length < 20.f && (GetRand(10) < 1)) {
						const auto Obj = AddBullet(this->m_PlayerID,
							GetPosition(), DX_PI_F - this->m_Rad, GetSize() / 2 + 0.75f, 3.f, 0.5f);
						this->m_ShotCoolTime = 0.5f;
					}
					else if (20.f < Length && (GetRand(10) < 1)) {
						{
							const auto Obj = AddBullet(this->m_PlayerID,
								GetPosition(), DX_PI_F - this->m_Rad + DX_PI_F / 6, GetSize() / 2.f + 0.75f, 1.5f * 5.f, 0.5f);

							m_MissileID.at(m_MissileIDNum) = Obj->GetUniqueID();
							++m_MissileIDNum %= static_cast<int>(m_MissileID.size());
						}
						{
							const auto Obj = AddBullet(this->m_PlayerID,
								GetPosition(), DX_PI_F - this->m_Rad - DX_PI_F / 6, GetSize() / 2.f + 0.75f, 1.5f * 5.f, 0.5f);

							m_MissileID.at(m_MissileIDNum) = Obj->GetUniqueID();
							++m_MissileIDNum %= static_cast<int>(m_MissileID.size());
						}
						this->m_ShotCoolTime = 5.f;
					}
				}
				else {
					this->m_ShotCoolTime = std::max(this->m_ShotCoolTime - DrawParts->GetDeltaTime(), 0.f);
				}
				for (auto& g : m_MissileID) {
					const auto& Obj = Obj2DParts->GetObj(g);
					if (Obj) {
						std::shared_ptr<Base2DObject>& o = (std::shared_ptr<Base2DObject>&)(Obj);
						Vector2DX Vec; Vec = (Chara->GetPosition() - o->GetPosition()).normalized();
						Vector2DX Vec_R; Vec_R = o->GetVec().normalized();
						if (Vector2DX::Dot(Vec, Vec_R) > 0.f) {
							Easing(&Vec_R, Vec, 0.975f, EasingType::OutExpo);
							o->SetVec(Vec_R.normalized() * o->GetVec().magnitude());
						}
					}
				}

			}
			// ï«è’ìÀââéZî≤Ç´ÇÃà⁄ìÆèàóù
			Vector2DX Vec = this->GetVec() * DrawParts->GetDeltaTime();
			SetPosition(this->GetPosition() + Vec);
		}

		void MetalObject::DrawShadow_Sub(void) noexcept {
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(GetSize() / 2.f));
			// âe
			auto* BackGround = BackGroundClassBase::Instance();
			Vector2DX DispPos;
			Vector2DX DispPosB;
			Vector2DX AddPos;

			// ÉAÉtÉBÉìïœä∑Ç…ÇÊÇÈâÒì]
			auto ConvAddPos = [&](Vector2DX PosT, float Rotate) {
				float Sin = std::sinf(Rotate);
				float Cos = std::cosf(Rotate);
				PosT *= GetSize() / 2.f;
				return Vector2DX::vget(PosT.x * Cos - PosT.y * Sin, PosT.x * Sin + PosT.y * Cos);
				};
			// ãr
			AddPos.Set((1.2f), (-1.2f));
			Cam2DControl::ConvertTiletoDisp(m_PosR + ConvAddPos(AddPos, m_RadR) + BackGround->GetAmbientLightVec() * 0.25f, &DispPos);
			DispPosB = DispPos;

			AddPos.Set((0.8f), (-0.2f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_RadR) + BackGround->GetAmbientLightVec() * 0.25f, &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);

			AddPos.Set(-(1.2f), (-1.2f));
			Cam2DControl::ConvertTiletoDisp(m_PosL + ConvAddPos(AddPos, m_RadL) + BackGround->GetAmbientLightVec() * 0.25f, &DispPos);
			DispPosB = DispPos;

			AddPos.Set(-(0.8f), (-0.2f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_RadL) + BackGround->GetAmbientLightVec() * 0.25f, &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
			// êgëÃ
			Cam2DControl::ConvertTiletoDisp(GetPosition() + BackGround->GetAmbientLightVec() * 0.25f, &DispPos);
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius), Black);
			// âEéË
			AddPos.Set(-(0.9f), (-2.5f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R) + BackGround->GetAmbientLightVec() * 0.35f, &DispPos);
			DispPosB = DispPos;

			AddPos.Set(-(0.9f), (0.5f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R) + BackGround->GetAmbientLightVec() * 0.35f, &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.15f) * 2);
			// ç∂éË
			AddPos.Set(-(-1.2f), (0.35f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R) + BackGround->GetAmbientLightVec() * 0.35f, &DispPos);
			DispPosB = DispPos;

			AddPos.Set(-(-1.2f), (-0.35f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R) + BackGround->GetAmbientLightVec() * 0.35f, &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
		}
		void MetalObject::Draw_Sub(void) noexcept {
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(GetSize() / 2.f));
			Vector2DX DispPos;
			Vector2DX DispPosB;
			Vector2DX AddPos;

			// ÉAÉtÉBÉìïœä∑Ç…ÇÊÇÈâÒì]
			auto ConvAddPos = [&](Vector2DX PosT, float Rotate) {
				float Sin = std::sinf(Rotate);
				float Cos = std::cosf(Rotate);
				PosT *= GetSize() / 2.f;
				return Vector2DX::vget(PosT.x * Cos - PosT.y * Sin, PosT.x * Sin + PosT.y * Cos);
				};
			// ãr
			AddPos.Set((1.2f), (-1.2f));
			Cam2DControl::ConvertTiletoDisp(m_PosR + ConvAddPos(AddPos, m_RadR), &DispPos);
			DispPosB = DispPos;

			AddPos.Set((0.8f), (-0.2f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_RadR), &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(128, 0, 0), static_cast<int>(Radius * 0.25f) * 2);

			AddPos.Set(-(1.2f), (-1.2f));
			Cam2DControl::ConvertTiletoDisp(m_PosL + ConvAddPos(AddPos, m_RadL), &DispPos);
			DispPosB = DispPos;

			AddPos.Set(-(0.8f), (-0.2f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_RadL), &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(128, 0, 0), static_cast<int>(Radius * 0.25f) * 2);
			// êgëÃ
			Cam2DControl::ConvertTiletoDisp(GetPosition(), &DispPos);
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius), GetColor(192, 0, 0));
			// âEéË
			AddPos.Set(-(0.9f), (-2.5f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R), &DispPos);
			DispPosB = DispPos;

			AddPos.Set(-(0.9f), (0.5f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R), &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(255, 0, 0), static_cast<int>(Radius * 0.15f) * 2);
			// ç∂éË
			AddPos.Set(-(-1.2f), (0.35f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R), &DispPos);
			DispPosB = DispPos;

			AddPos.Set(-(-1.2f), (-0.35f));
			Cam2DControl::ConvertTiletoDisp(GetPosition() + ConvAddPos(AddPos, m_Rad_R), &DispPos);
			DrawLine(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(255, 0, 0), static_cast<int>(Radius * 0.25f) * 2);
		}
		void MetalObject::Dispose_Sub(void) noexcept {}
	};
};
