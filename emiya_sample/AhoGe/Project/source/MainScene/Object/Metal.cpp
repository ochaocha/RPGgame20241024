#include	"Metal.hpp"

#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		MetalObject::MetalObject(void) noexcept {
			this->m_InputVec.Set(0.f, 0.f);
			SetObjType(static_cast<int>(Object2DType::Metal));
		}
		// 
		void MetalObject::OnHitObject_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* Effect2DParts = Effect2DControl::Instance();
			const auto& Obj = Obj2DParts->GetObj(GetHitUniqueID());
			if (Obj) {
				// �e�ȊO�������������͈ȉ��͒ʂ��Ȃ�
				if (Obj->GetObjType() != static_cast<int>(Object2DType::Rocket) && Obj->GetObjType() != static_cast<int>(Object2DType::Bullet)) { return; }
				//�q�b�g�G�t�F�N�g���o��
				Effect2DParts->SetEffect(Obj->GetPosition(), EffectType::Guard, 1.f);
				// Effect2DParts->Set(Obj->GetPosition(), EffectType::Damage, 0.25f);
				if (Obj->GetObjType() == static_cast<int>(Object2DType::Rocket)) {
					//���P�b�g�����`���[�Ƀq�b�g�����ꍇ��HP��0�ɂ���
					this->m_HitPoint = 0;
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Bomb))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				else {
					//����ȊO�̗͑͂�1���炷����
					this->m_HitPoint--;
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Guard))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				//�̗͂�0�ɂȂ�����
				if (this->m_HitPoint == 0) {
					//��������̃G�t�F�N�g�𔭐�����
					Effect2DParts->SetEffect(GetPosition(), EffectType::WallHit, 2.f);
					//����t���O�𗧂Ă�
					SetDelete();
				}
			}
		}
		// 
		void MetalObject::Initialize_Sub(void) noexcept {
			SetIsHitOtherObject(true);
			SetMass(100000.f);
			m_FootTimer = 0.f;
			for (auto& g : m_MissileID) {
				g = 0;
			}
			m_MissileIDNum = 0;
			this->m_HitPoint = m_MaxHitPoint;
		}
		void MetalObject::Update_Sub(void) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* Cam2D = Cam2DControl::Instance();

			if (GetIsFirstLoop()) {
				m_BasePos = GetPosition();//�s���̊�ƂȂ���W���擾(�������炠�܂藣��Ȃ��悤�ȃ��[�`��)
				m_PosL = GetPosition();
				m_PosR = GetPosition();
			}
			//�ғ��t���O���I�t�Ȃ瓮���Ȃ�
			if (!m_CanMove) {
				SetVec(Vector2DX::zero());
				return;
			}
			//�t���O���I�t�@��̃t���[����SetCanMove���Ă΂�Ȃ��Ƌ@�\��~�ƂȂ�
			m_CanMove = false;

			//�^�[�Q�b�g�L�������擾
			auto& TargetCharacter = PlayerMngr->GetPlayer((PlayerID)0)->GetChara();
			if (!TargetCharacter) {
				//�^�[�Q�b�g�����Ȃ��ꍇ��~
				SetVec(Vector2DX::zero());
				return;
			}
			//�^�[�Q�b�g�Ƃ̋������擾
			float Length = (GetPosition() - TargetCharacter->GetPosition()).magnitude();

			//�^�[�Q�b�g�Ɍ����ĉ�]�ڕW����������
			float TargetRad = DX_PI_F - GetRadVec(TargetCharacter->GetPosition() - GetPosition());

			//�ԑ̂�ڕW�����ɉ�]
			this->m_Rad += GetRadRad2Rad(this->m_Rad, TargetRad) * 0.6f * DXLib_ref::Instance()->GetDeltaTime();
			{
				// �ړ����x
				float SpeedLimit = 0.5f;

				//�ԍ����ɉ�����WASD���͂����z�ōs��
				bool WKey = false;
				bool SKey = false;
				bool AKey = false;
				bool DKey = false;
				//���͈̔͂ɓ��낤�Ƃ���悤�ɑ���
				{
					Vector2DX LastFindPos = TargetCharacter->GetPosition();
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
				// ���܂蒆������O���ƌ��h���������̂ň��l�ӌ����ƒ����ɖ߂�
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
				// ��L���zWASD����Ɉړ����Z
				if (DKey && !AKey) {
					this->m_InputVec.x = std::clamp(this->m_InputVec.x + 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else if (AKey && !DKey) {
					this->m_InputVec.x = std::clamp(this->m_InputVec.x - 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else {
					Easing(&this->m_InputVec.x, 0.f, 0.95f, EasingType::OutExpo);
				}
				if (WKey && !SKey) {
					this->m_InputVec.y = std::clamp(this->m_InputVec.y + 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else if (SKey && !WKey) {
					this->m_InputVec.y = std::clamp(this->m_InputVec.y - 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else {
					Easing(&this->m_InputVec.y, 0.f, 0.95f, EasingType::OutExpo);
				}
				float Speed = GetMax(std::abs(this->m_InputVec.x), std::abs(this->m_InputVec.y));
				// �ǔ�����s���ƕǕt�߂ŏd�������o�Ȃ��̂ŕǂƂ͔�����s��Ȃ�
				SetVec(this->m_InputVec.normalized() * Speed * 5.f);
			}
			{
				m_FootTimer += DXLib_ref::Instance()->GetDeltaTime();
				if (m_FootTimer > 2.f) {
					m_FootTimer -= 2.f;//2�b��1����
				}

				//0.6~0.8�b�ڂł͍������グ��
				if (0.6f < m_FootTimer && m_FootTimer < 0.8f) {
					//TargetRad�ɍ����̍��W�A��]���񂹂�
					this->m_RadL += GetRadRad2Rad(this->m_RadL, TargetRad) * 10.f * DXLib_ref::Instance()->GetDeltaTime();
					Easing(&m_PosR, GetPosition() + GetVec() * 1.f, 0.5f, EasingType::OutExpo);
					//�r���グ������𗧂Ă�
					m_FootUp = true;
				}
				else {
					//�r�����낵���Ƃ��ĐU����ǉ�
					if (m_FootUp) {
						m_FootUp = false;
						if (Length < 30.f) {
							Cam2D->SetCamShake(0.1f, 20.f * 5.f / GetMax(Length, 1.f));
						}
					}
				}
				//1.6~1.8�b�ڂł͉E�����グ��
				if (1.6f < m_FootTimer && m_FootTimer < 1.8f) {
					//TargetRad�ɉE���̍��W�A��]���񂹂�
					this->m_RadR += GetRadRad2Rad(this->m_RadR, TargetRad) * 10.f * DXLib_ref::Instance()->GetDeltaTime();
					Easing(&m_PosL, GetPosition() + GetVec() * 1.f, 0.5f, EasingType::OutExpo);
					//�r���グ������𗧂Ă�
					m_FootUp = true;
				}
				else {
					//�r�����낵���Ƃ��ĐU����ǉ�
					if (m_FootUp) {
						m_FootUp = false;
						if (Length < 30.f) {
							Cam2D->SetCamShake(0.1f, 20.f * 5.f / GetMax(Length, 1.f));
						}
					}
				}
			}
			// �e
			if (this->m_ShotCoolTime == 0.f) {
				//10~20m�͈͓̔��Ń����_���Ȓl���T��1/10(10%)�Ȃ�
				if (10.f < Length && Length < 20.f && (GetRand(10) < 1)) {
					//�e�𔭎˂���
					AddBullet(GetPlayerID(), GetPosition(), DX_PI_F - TargetRad, GetSize() / 2 + 0.75f, 3.f, 0.5f);
					//0.5�b�̃N�[���_�E����ݒ�
					this->m_ShotCoolTime = 0.5f;
				}
				//20m�ȍ~�͈̔͂Ń����_���Ȓl���T��1/10(10%)�Ȃ�
				else if (20.f < Length && (GetRand(10) < 1)) {
					{
						//�e�𔭎˂���
						const auto Obj = AddBullet(GetPlayerID(), GetPosition(), DX_PI_F - TargetRad + DX_PI_F / 6, GetSize() / 2.f + 0.75f, 1.5f * 5.f, 0.5f);
						//�U���ΏۂƂ��Đݒ�
						SetMissileID(Obj->GetUniqueID());
					}
					{
						//�e�𔭎˂���
						const auto Obj = AddBullet(GetPlayerID(), GetPosition(), DX_PI_F - TargetRad - DX_PI_F / 6, GetSize() / 2.f + 0.75f, 1.5f * 5.f, 0.5f);
						//�U���ΏۂƂ��Đݒ�
						SetMissileID(Obj->GetUniqueID());
					}
					//5�b�̃N�[���_�E����ݒ�
					this->m_ShotCoolTime = 5.f;
				}
			}
			else {
				//�e���������̃N�[���_�E��
				this->m_ShotCoolTime = GetMax(this->m_ShotCoolTime - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
			}
			//�~�T�C���Ƃ��ēo�^���ꂽ�I�u�W�F�N�g���^�[�Q�b�g�Ɍ����ď������U�����܂�
			for (auto& g : m_MissileID) {
				const auto& Obj = Obj2DParts->GetObj(g);
				if (Obj) {
					Vector2DX Vec; Vec = (TargetCharacter->GetPosition() - Obj->GetPosition()).normalized();
					Vector2DX Vec_R; Vec_R = Obj->GetVec().normalized();
					if (Vector2DX::Dot(Vec, Vec_R) > 0.f) {//�^�[�Q�b�g�Ɍ����Ă���ꍇ�̂ݗU������
						Easing(&Vec_R, Vec, 0.975f, EasingType::OutExpo);
						Obj->SetVec(Vec_R.normalized() * Obj->GetVec().magnitude());
					}
				}
			}
			// �ǏՓˉ��Z�����̈ړ�����
			SetPosition(this->GetPosition() + this->GetVec() * DXLib_ref::Instance()->GetDeltaTime());
		}
		// �e
		void MetalObject::DrawShadow_Sub(void) noexcept {
			auto* BackGround = BackGroundClassBase::Instance();
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(GetSize() / 2.f));
			Vector2DX DispPosA;
			Vector2DX DispPosB;
			// �E�r
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosL + GetVectorRotated(Vector2DX::vget(-1.2f, -1.2f) * (GetSize() / 2.f), m_RadR) + BackGround->GetAmbientLightVec() * 0.25f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.8f, -0.2f) * (GetSize() / 2.f), m_RadR) + BackGround->GetAmbientLightVec() * 0.25f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
			// �E�r
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosR + GetVectorRotated(Vector2DX::vget(1.2f, -1.2f) * (GetSize() / 2.f), m_RadL) + BackGround->GetAmbientLightVec() * 0.25f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(0.8f, -0.2f) * (GetSize() / 2.f), m_RadL) + BackGround->GetAmbientLightVec() * 0.25f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
			// �g��
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + BackGround->GetAmbientLightVec() * 0.25f);
			DrawCircle(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(Radius), Black);
			// �E��
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, -2.5f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, 0.5f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.15f) * 2);
			// ����
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, 0.35f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, -0.35f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
		}
		// �`��
		void MetalObject::Draw_Sub(void) noexcept {
			int Radius = Cam2DControl::GetTileToDispSize(GetSize() / 2.f);//��ƂȂ�T�C�Y
			Vector2DX DispPosA;
			Vector2DX DispPosB;
			// �E�r
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosL + GetVectorRotated(Vector2DX::vget(-1.2f, -1.2f) * (GetSize() / 2.f), m_RadL));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.8f, -0.2f) * (GetSize() / 2.f), m_RadL));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(128, 0, 0), Radius / 2);
			// ���r
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosR + GetVectorRotated(Vector2DX::vget(1.2f, -1.2f) * (GetSize() / 2.f), m_RadR));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(0.8f, -0.2f) * (GetSize() / 2.f), m_RadR));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(128, 0, 0), Radius / 2);
			// �g��
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition());
			DrawCircle(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), Radius, GetColor(192, 0, 0));
			// �E��
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, -2.5f) * (GetSize() / 2.f), this->m_Rad));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, 0.5f) * (GetSize() / 2.f), this->m_Rad));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(255, 0, 0), Radius * 3 / 10);
			// ����
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, 0.35f) * (GetSize() / 2.f), this->m_Rad));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, -0.35f) * (GetSize() / 2.f), this->m_Rad));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(255, 0, 0), Radius / 2);
		}
	}
}
