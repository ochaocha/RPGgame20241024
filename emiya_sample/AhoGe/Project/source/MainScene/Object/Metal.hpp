#pragma once

#include	"../../Header.hpp"
#include	"../../sub.hpp"
#include	"Bullet.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//���^���M�A���@����I�u�W�F�N�g(�Ǝ���AI������)
		// --------------------------------------------------------------------------------------------------
		class MetalObject : public Object2DManager::Base2DObject {
		private:
			PlayerID					m_PlayerID{ 0 };		//�v���C���[ID(���ˎ���ID�����)

			float						m_Rad{ 0.f };			//���̂̉�]�p�x
			float						m_RadL{ 0.f };			//�����̉�]�p�x
			Vector2DX					m_PosL{};				//�����̍��W
			float						m_RadR{ 0.f };			//�E���̉�]�p�x
			Vector2DX					m_PosR{};				//�E���̍��W

			float						m_FootTimer{ 0.f };		//�r�̉�]�T�C�N���p�^�C�}�[
			bool						m_FootUp{ false };		//�r���グ��/�������t���O

			Vector2DX					m_BasePos{};			//AI�̍s���͈͂̊�ɂȂ���W
			Vector2DX					m_InputVec{};			//AI�̓��̓x�N�g��

			float						m_ShotCoolTime{ 0.f };	//���˂���̃N�[���_�E���^�C�}�[
			std::array<int, 8>			m_MissileID{};			//�U���I�u�W�F�N�g�̕ۑ����X�g(8�ȏ�͕ۑ����܂���)
			int							m_MissileIDNum{};		//�U���I�u�W�F�N�gID�̎w���

			bool						m_CanMove{ false };		//�ғ����邩�̃t���O

			HitPoint					m_HitPoint{ 0 };		//���݂̗̑�
			const HitPoint				m_MaxHitPoint{ 1000 };	//�ő�̗�
		public:
			//�R���X�g���N�^
			MetalObject(void) noexcept;
			MetalObject(const MetalObject&) = delete;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			MetalObject(MetalObject&& o) = delete;
			MetalObject& operator=(const MetalObject&) = delete;
			MetalObject& operator=(MetalObject&& o) = delete;
			//�f�X�g���N�^ �p���I�u�W�F�N�g�ł���׃f�X�g���N�^��virtual���w��
			virtual ~MetalObject(void) noexcept {}
		public:
			void		SetPlayerID(PlayerID value) noexcept { this->m_PlayerID = value; }
			void		SetCanMove(void) noexcept { m_CanMove = true; }//���ꂪ�Ă΂�����͉ғ����܂�

			const auto& GetPlayerID(void) const noexcept { return this->m_PlayerID; }
			const auto& GetHitPoint(void) const noexcept { return this->m_HitPoint; }
			const auto& GetHitPointMax(void) const noexcept { return m_MaxHitPoint; }
		private:
			//�~�T�C���Ƃ��ėU�����s��ID��o�^
			void		SetMissileID(int value) noexcept {
				m_MissileID.at(m_MissileIDNum) = value;
				++m_MissileIDNum %= static_cast<int>(m_MissileID.size());
			}
			//�e�𔭎�
			std::shared_ptr<BulletObject>	AddBullet(PlayerID shootID, const Vector2DX& Pos, float Radian, float Offset, float Speed, float Size) noexcept {
				auto* Obj2DParts = Object2DManager::Instance();
				const auto& Obj = std::make_shared<BulletObject>();
				Obj2DParts->AddObject(Obj);
				Obj->SetShootPlayer(shootID);
				Vector2DX Vec = GetVecByRad(DX_PI_F - Radian);
				Obj->SetPosition(Pos + Vec * Offset);
				Obj->SetVec(Vec * Speed);
				Obj->SetSize(Size);
				return Obj;
			}
		protected:
			void Update_OnHitObject(void) noexcept override;
		protected:
			void Init_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override {}
		};
	}
}
