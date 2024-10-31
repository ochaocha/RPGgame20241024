#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"
#include	"../../CommonScene/Object/Base2DObject.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		class BulletObject : public Base2DObjectKai {
		private:
			PlayerID					m_ShotPlayerID{ 0 };
			PlayerID					m_HitPlayerID{ 0 };
			float						m_Rad{ 0.f };
			Blur2DControl				m_Blur;
		public:
			BulletObject(void) noexcept;
			BulletObject(const BulletObject&) = delete;
			BulletObject(BulletObject&& o) = delete;
			BulletObject& operator=(const BulletObject&) = delete;
			BulletObject& operator=(BulletObject&& o) = delete;

			virtual ~BulletObject(void) noexcept {}
		public:
			void		SetShootPlayer(PlayerID value) noexcept { this->m_ShotPlayerID = value; }
		public:
			void Update_OnHitObject(void) noexcept override;
			void Update_OnHitWall(void) noexcept override;
		public:
			void Init_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override;
		};
	};
};
