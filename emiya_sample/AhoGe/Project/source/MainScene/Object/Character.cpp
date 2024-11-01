#include	"Character.hpp"
#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

#include	"Bullet.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		CharacterObject::CharacterObject(void) noexcept {
			this->m_InputVec.Set(0.f, 0.f);
			SetObjType(Object2DType::Human);
			SetSize(1.f);
		}
		CharacterObject::~CharacterObject(void) noexcept {}
		void CharacterObject::UpdateInputVector(const InputControl& MyInput) noexcept {
			auto* DrawParts = DXDraw::Instance();
			// �ړ����͂��擾
			const bool WKey = MyInput.GetPADSPress(PADS::MOVE_W) && !MyInput.GetPADSPress(PADS::MOVE_S);
			const bool SKey = MyInput.GetPADSPress(PADS::MOVE_S) && !MyInput.GetPADSPress(PADS::MOVE_W);
			const bool AKey = MyInput.GetPADSPress(PADS::MOVE_A) && !MyInput.GetPADSPress(PADS::MOVE_D);
			const bool DKey = MyInput.GetPADSPress(PADS::MOVE_D) && !MyInput.GetPADSPress(PADS::MOVE_A);
			// �ʏ�ړ����x�̍ő�l(1�b�Ԃɐi�ރ^�C���̖���)
			float SpeedLimit = 0.75f;
			if (!m_IsPlayableCharacter) { SpeedLimit = 0.5f; }
			if (MyInput.GetPADSPress(PADS::RUN)) { SpeedLimit = 0.95f; }
			if (MyInput.GetPADSPress(PADS::WALK)) { SpeedLimit = 0.25f; }
			// �ړ����Z
			if (DKey) { this->m_InputVec.x = std::clamp(this->m_InputVec.x + 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else if (AKey) { this->m_InputVec.x = std::clamp(this->m_InputVec.x - 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else { Easing(&this->m_InputVec.x, 0.f, 0.95f, EasingType::OutExpo); }
			if (WKey) { this->m_InputVec.y = std::clamp(this->m_InputVec.y + 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else if (SKey) { this->m_InputVec.y = std::clamp(this->m_InputVec.y - 4.f * DrawParts->GetDeltaTime(), -SpeedLimit, SpeedLimit); }
			else { Easing(&this->m_InputVec.y, 0.f, 0.95f, EasingType::OutExpo); }
			// ���x�͓��͕����̂����傫���ق����g�p
			this->m_Speed = std::max(std::abs(this->m_InputVec.x), std::abs(this->m_InputVec.y));
			// ���[�����O(�ړ��������ɃX�y�[�X)
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
			//��L�̌��ʂ��܂Ƃ߁AVector�ɓn��
			SetVec(this->m_InputVec.normalized() * this->m_Speed * 5.f);
		}
		// 
		void		CharacterObject::UpdateInput(const InputControl& MyInput) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* SoundParts = SoundPool::Instance();

			// �ړ����x
			UpdateInputVector(MyInput);
			// �u���[
			if (this->m_DodgeCoolTime > 0.75f) {
				m_Blur.AddBlur(0.3f, GetPosition(), this->GetVec());
			}
			else if (MyInput.GetPADSPress(PADS::RUN)) {
				m_Blur.AddBlur(0.1f, GetPosition(), this->GetVec());
			}
			// �_��
			{
				Vector2DX Vec; Vec.Set(std::sin(this->m_Rad), std::cos(this->m_Rad));
				Vector2DX vec_a; vec_a.Set(std::sin(MyInput.GetyRad()), std::cos(MyInput.GetyRad()));
				float cost = Vector2DX::Cross(vec_a, Vec);
				float sint = sqrtf(std::abs(1.f - cost * cost));
				this->m_Rad += (std::atan2f(cost, sint)) * 10.f * DrawParts->GetDeltaTime();

				if (this->m_Rad < 0.f) { this->m_Rad += DX_PI_F * 2.f; }
				if (this->m_Rad > DX_PI_F * 2.f) { this->m_Rad -= DX_PI_F * 2.f; }
			}
			// �e
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
						Obj->SetPosition(GetPosition() + Vec * 1.2f);
						Obj->SetVec(Vec * 3.f * 5.f);
						Obj->SetSize(0.5f);
						break;
					case GunType::Rifle:
						this->m_ShotCoolTime = 0.1f;
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot2))->Play(DX_PLAYTYPE_BACK, TRUE);
						Obj->SetPosition(GetPosition() + Vec * 1.2f);
						Obj->SetVec(Vec * 3.f * 5.f);
						Obj->SetSize(0.5f);
						break;
					case GunType::Rocket:
						this->m_ShotCoolTime = 5.f;
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot3))->Play(DX_PLAYTYPE_BACK, TRUE);
						Obj->SetObjType(Object2DType::Rocket);// ���ʂɃ��P�b�g�Ƃ��ēo�^
						Obj->SetPosition(GetPosition() + Vec * 2.5f);
						Obj->SetVec(Vec * 0.5f * 5.f);
						Obj->SetSize(3.f);
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
			Cam2DControl::Convert2DtoDisp(GetPosition(), &DispPos);
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
				// �e�ȊO�������������͈ȉ��͒ʂ��Ȃ�
				if (o->GetObjType() != Object2DType::Rocket && o->GetObjType() != Object2DType::Bullet) { return; }

				Effect2DControl::Instance()->SetEffect(o->GetPosition(), EffectType::Damage, 0.25f);
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
					float ViewLimit = 40.f;
					if ((Chara->GetPosition() - GetPosition()).sqrMagnitude() < ViewLimit * ViewLimit) {
						Easing(&this->m_Alpha, BackGround->CheckHideShadow(Chara->GetPosition(), GetPosition(), GetSize() / 2.f / Cam2DControl::GetTileToDispSize(1.f)), 0.5f, EasingType::OutExpo);
					}
					else {
						Easing(&this->m_Alpha, 0.f, 0.5f, EasingType::OutExpo);
					}
				}
			}
			else {
				this->m_Alpha = 1.f;
			}
			// �T�E���h
			{
				//���x*4��/�b������炷���̂Ƃ��ăJ�E���^�[�𑝌������܂�
				this->m_RunFootPer += this->m_Speed * 4.f * DrawParts->GetDeltaTime();
				if (this->m_RunFootPer > 1.f) {
					this->m_RunFootPer -= 1.f;
					float Len = 30.f;
					if ((Cam2D->GetCamPos() - GetPosition()).sqrMagnitude() < Len * Len) {
						SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::RunFoot))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
			// 
			UpdateWallHit();

			m_Blur.Update();
		}
		void CharacterObject::DrawShadow_Sub(void) noexcept {
			float Radius = static_cast<float>(GetSize() / 2.f);
			if (!Cam2DControl::Is2DPositionInDisp(GetPosition(), Radius)) { return; }
			auto* BackGround = BackGroundClassBase::Instance();
			if (this->m_Alpha > 1.f / 255.f) {
				Vector2DX DispPos;
				Cam2DControl::Convert2DtoDisp(GetPosition(), &DispPos);

				DispPos += BackGround->GetAmbientLightVec() * 0.25f * Cam2DControl::Instance()->GetCamHeight();
				DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Cam2DControl::GetTileToDispSize(Radius)), Black);
			}
		}
		void CharacterObject::Draw_Sub(void) noexcept {
			float Radius = static_cast<float>(GetSize() / 2.f);
			if (!Cam2DControl::Is2DPositionInDisp(GetPosition(), Radius)) { return; }
			// �u���[
			for (auto& b : m_Blur.GetBlur()) {
				if (b.IsActive()) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(16.f * this->m_Alpha * b.GetPer()), 0, 255));
					Vector2DX DispPos;
					Cam2DControl::Convert2DtoDisp(b.GetPos(), &DispPos);
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Cam2DControl::GetTileToDispSize(Radius) * std::pow(b.GetPer(), 0.5f)), (m_IsPlayableCharacter) ? GetColor(37, 68, 141) : GetColor(92, 84, 50));
				}
			}
			// �{��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_Alpha), 0, 255));
			Vector2DX DispPos;
			Cam2DControl::Convert2DtoDisp(GetPosition(), &DispPos);
			DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Cam2DControl::GetTileToDispSize(Radius)), (m_IsPlayableCharacter) ? GetColor(37, 68, 141) : GetColor(92, 84, 50));

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		void CharacterObject::Dispose_Sub(void) noexcept {}
	};
};
