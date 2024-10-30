#include	"Character.hpp"
#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

#include	"Bullet.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		CharacterObject::CharacterObject(void) noexcept {
			this->m_InputVec.Set(0.f, 0.f);
			SetObjType(Object2DType::Human);
			SetSize(Get2DSize(1.f));
		}
		CharacterObject::~CharacterObject(void) noexcept {}
		void CharacterObject::UpdateInputVector(const InputControl& MyInput) noexcept {
			auto* DrawParts = DXDraw::Instance();
			// 移動入力を取得
			const bool WKey = MyInput.GetPADSPress(PADS::MOVE_W) && !MyInput.GetPADSPress(PADS::MOVE_S);
			const bool SKey = MyInput.GetPADSPress(PADS::MOVE_S) && !MyInput.GetPADSPress(PADS::MOVE_W);
			const bool AKey = MyInput.GetPADSPress(PADS::MOVE_A) && !MyInput.GetPADSPress(PADS::MOVE_D);
			const bool DKey = MyInput.GetPADSPress(PADS::MOVE_D) && !MyInput.GetPADSPress(PADS::MOVE_A);
			// 通常移動速度の最大値(1秒間に進むタイルの枚数)
			float SpeedLimit = 0.75f;
			if (!m_IsPlayableCharacter) { SpeedLimit = 0.5f; }
			if (MyInput.GetPADSPress(PADS::RUN)) { SpeedLimit = 0.95f; }
			if (MyInput.GetPADSPress(PADS::WALK)) { SpeedLimit = 0.25f; }
			// 移動演算
			if (DKey) { this->m_InputVec.x = std::clamp(this->m_InputVec.x + 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else if (AKey) { this->m_InputVec.x = std::clamp(this->m_InputVec.x - 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else { Easing(&this->m_InputVec.x, 0.f, 0.95f, EasingType::OutExpo); }
			if (WKey) { this->m_InputVec.y = std::clamp(this->m_InputVec.y + 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else if (SKey) { this->m_InputVec.y = std::clamp(this->m_InputVec.y - 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else { Easing(&this->m_InputVec.y, 0.f, 0.95f, EasingType::OutExpo); }
			// 速度は入力方向のうち大きいほうを使用
			this->m_Speed = std::max(std::abs(this->m_InputVec.x), std::abs(this->m_InputVec.y));
			// ローリング(移動押下中にスペース)
			if (this->m_DodgeCoolTime == 0.f) {
				if ((DKey || AKey || WKey || SKey) && MyInput.GetPADSPress(PADS::JUMP)) {
					this->m_DodgeCoolTime = 1.f;
				}
			}
			else {
				this->m_DodgeCoolTime = std::max(this->m_DodgeCoolTime - DrawParts->GetDeltaTime(), 0.f);
				if (this->m_DodgeCoolTime > 0.75f) {
					this->m_Speed = 2.f;
				}
				else {
					this->m_Speed = 0.375f;
				}
			}
			//上記の結果をまとめ、Vectorに渡す
			SetVec(this->m_InputVec.normalized() * this->m_Speed);
		}
		// 
		void		CharacterObject::UpdateInput(const InputControl& MyInput) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* SoundParts = SoundPool::Instance();

			// 移動速度
			UpdateInputVector(MyInput);
			// ブラー
			if (this->m_DodgeCoolTime > 0.75f) {
				m_Blur.AddBlur(0.3f, this->GetPos(), this->GetVec());
			}
			else if (MyInput.GetPADSPress(PADS::RUN)) {
				m_Blur.AddBlur(0.1f, this->GetPos(), this->GetVec());
			}
			// 狙い
			{
				Vector2DX Vec; Vec.Set(std::sin(this->m_Rad), std::cos(this->m_Rad));
				Vector2DX vec_a; vec_a.Set(std::sin(MyInput.GetyRad()), std::cos(MyInput.GetyRad()));
				float cost = Vector2DX::Cross(vec_a, Vec);
				float sint = sqrtf(std::abs(1.f - cost * cost));
				this->m_Rad += (std::atan2f(cost, sint)) * 10.f * DrawParts->GetDeltaTime();

				if (this->m_Rad < 0.f) { this->m_Rad += DX_PI_F * 2.f; }
				if (this->m_Rad > DX_PI_F * 2.f) { this->m_Rad -= DX_PI_F * 2.f; }
			}
			// 弾
			if (this->m_ShotCoolTime == 0.f) {
				if (MyInput.GetPADSPress(PADS::SHOT) && (m_GunType != GunType::None)) {
					auto* Obj2DParts = Object2DManager::Instance();
					const auto& Obj = std::make_shared<BulletObject>();
					Obj2DParts->AddObject(Obj);
					Obj->SetShootPlayer(this->m_PlayerID);
					Vector2DX Vec; Vec.Set(std::sin(this->m_Rad), std::cos(this->m_Rad));
					switch (m_GunType) {
					case GunType::Handgun:
						this->m_ShotCoolTime = 1.f;
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot1))->Play(DX_PLAYTYPE_BACK, TRUE);
						Obj->SetPos(GetPos() + Vec * Get2DSize(1.2f));
						Obj->SetVec(Vec * 3.f);
						Obj->SetSize(Get2DSize(0.5f));
						break;
					case GunType::Rifle:
						this->m_ShotCoolTime = 0.1f;
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot2))->Play(DX_PLAYTYPE_BACK, TRUE);
						Obj->SetPos(GetPos() + Vec * Get2DSize(1.2f));
						Obj->SetVec(Vec * 3.f);
						Obj->SetSize(Get2DSize(0.5f));
						break;
					case GunType::Rocket:
						this->m_ShotCoolTime = 5.f;
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot3))->Play(DX_PLAYTYPE_BACK, TRUE);
						Obj->SetObjType(Object2DType::Rocket);// 特別にロケットとして登録
						Obj->SetPos(GetPos() + Vec * Get2DSize(2.5f));
						Obj->SetVec(Vec * 0.5f);
						Obj->SetSize(Get2DSize(3.f));
						break;
					default:
						break;
					}
				}
			}
			else {
				this->m_ShotCoolTime = std::max(this->m_ShotCoolTime - DrawParts->GetDeltaTime(), 0.f);
			}
		}
		void CharacterObject::DrawHPBer() noexcept {
			auto* DrawParts = DXDraw::Instance();
			Vector2DX DispPos;
			Convert2DtoDisp(this->GetPos(), &DispPos);
			int xmin = DrawParts->GetScreenY(-50);
			int ymin = DrawParts->GetScreenY(-50);
			int xmax = DrawParts->GetScreenY(50);

			int xper = xmin + (xmax - xmin) * this->GetHitPoint() / this->GetHitPointMax();
			DrawLine(static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xmax), static_cast<int>(DispPos.y + ymin), Gray75, 10);
			DrawLine(static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xper), static_cast<int>(DispPos.y + ymin), Green, 10);
		}
		void CharacterObject::Update_OnHitObject(void) noexcept {
			auto* Obj2DParts = Object2DManager::Instance();
			const auto& Obj = Obj2DParts->GetObj(GetHitUniqueID());
			if (Obj) {
				std::shared_ptr<Base2DObjectKai>& o = (std::shared_ptr<Base2DObjectKai>&)(Obj);
				// 弾以外が当たった時は以下は通さない
				if (o->GetObjType() != Object2DType::Rocket && o->GetObjType() != Object2DType::Bullet) { return; }

				Effect2DControl::Instance()->Set(o->GetPos(), EffectType::Damage, 0.25f);
				this->m_HitPoint--;
				if (this->m_HitPoint == 0) {
					SetDelete();
				}
				auto* SoundParts = SoundPool::Instance();
				SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Hit))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
		}
		// 
		void CharacterObject::Init_Sub(void) noexcept {
			SetIsHitOtherObject(true);
			this->m_RunFootPer = 0.f;
			this->m_HitPoint = 3;
		}
		void CharacterObject::Update_Sub(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* SoundParts = SoundPool::Instance();
			auto* Cam2D = Cam2DControl::Instance();

			if (!m_IsPlayableCharacter) {
				auto& Chara = PlayerMngr->GetPlayer(0)->GetChara();
				if (Chara) {
					float ViewLimit = Get2DSize(40.f);
					if ((Chara->GetPos() - GetPos()).sqrMagnitude() < ViewLimit * ViewLimit) {
						Easing(&this->m_Alpha, BackGround->CheckHideShadow(Chara->GetPos(), GetPos(), GetSize() / 2.f), 0.5f, EasingType::OutExpo);
					}
					else {
						Easing(&this->m_Alpha, 0.f, 0.5f, EasingType::OutExpo);
					}
				}
			}
			else {
				this->m_Alpha = 1.f;
			}
			// サウンド
			{
				//速度*4回/秒足音を鳴らすものとしてカウンターを増減速します
				this->m_RunFootPer += this->m_Speed * 4.f * DrawParts->GetDeltaTime();
				if (this->m_RunFootPer > 1.f) {
					this->m_RunFootPer -= 1.f;
					float Len = Get2DSize(30.f);
					if ((Cam2D->GetCamPos() - GetPos()).sqrMagnitude() < Len * Len) {
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::RunFoot))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
			// 
			UpdateWallHit();

			m_Blur.Update();
		}
		void CharacterObject::DrawShadow_Sub(void) noexcept {
			float Radius = static_cast<float>(GetDispSize(Get2DSizetoTile(GetSize() / 2.f)));
			if (!Is2DPositionInDisp(GetPos(), static_cast<int>(Radius))) { return; }
			auto* BackGround = BackGroundClassBase::Instance();
			if (this->m_Alpha > 1.f / 255.f) {
				Vector2DX DispPos;
				Convert2DtoDisp(GetPos(), &DispPos);

				DispPos += BackGround->GetAmbientLightVec() * 0.25f * Cam2DControl::Instance()->GetCamHeight();
				DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius), Black);
			}
		}
		void CharacterObject::Draw_Sub(void) noexcept {
			float Radius = static_cast<float>(GetDispSize(Get2DSizetoTile(GetSize() / 2.f)));
			if (!Is2DPositionInDisp(GetPos(), static_cast<int>(Radius))) { return; }
			// ブラー
			for (auto& b : m_Blur.GetBlur()) {
				if (b.IsActive()) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(16.f * this->m_Alpha * b.GetPer()), 0, 255));
					Vector2DX DispPos;
					Convert2DtoDisp(b.GetPos(), &DispPos);
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius * std::pow(b.GetPer(), 0.5f)), (m_IsPlayableCharacter) ? GetColor(37, 68, 141) : GetColor(92, 84, 50));
				}
			}
			// 本体
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_Alpha), 0, 255));
			Vector2DX DispPos;
			Convert2DtoDisp(GetPos(), &DispPos);
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius), (m_IsPlayableCharacter) ? GetColor(37, 68, 141) : GetColor(92, 84, 50));

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		void CharacterObject::Dispose_Sub(void) noexcept {}
	};
};
