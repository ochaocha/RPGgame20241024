#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//
		// --------------------------------------------------------------------------------------------------
		class BulletObject : public Object2DManager::Base2DObject {
		private:
			PlayerID					m_ShotPlayerID{ 0 };
			float						m_Rad{ 0.f };
			Blur2DControl				m_Blur;
		public:
			//�R���X�g���N�^
			BulletObject(void) noexcept;
			BulletObject(const BulletObject&) = delete;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			BulletObject(BulletObject&& o) = delete;
			BulletObject& operator=(const BulletObject&) = delete;
			BulletObject& operator=(BulletObject&& o) = delete;
			//�f�X�g���N�^ �p���I�u�W�F�N�g�ł���׃f�X�g���N�^��virtual���w��
			virtual ~BulletObject(void) noexcept {}
		public:
			void			SetShootPlayer(PlayerID value) noexcept { this->m_ShotPlayerID = value; }
		public:
			void OnHitObject_Sub(void) noexcept override;
		public:
			void Initialize_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override;
		};
	}
}
