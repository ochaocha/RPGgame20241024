#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"
#include	"../../CommonScene/Object/Base2DObject.hpp"
#include	"Bullet.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		class MetalObject : public Base2DObjectKai {
		private:
			PlayerID					m_PlayerID{ 0 };
			PlayerID					m_HitPlayerID{ 0 };
			float						m_Speed{ 1.f };
			float						m_Rad{ 0.f };
			float						m_Rad_R{ 0.f };
			float						m_RadL{ 0.f };
			float						m_RadR{ 0.f };

			float						m_Foot{ 0.f };
			bool						m_FootDown{ false };
			Vector2DX					m_BasePos{};
			Vector2DX					m_PosL{};
			Vector2DX					m_PosR{};

			Vector2DX					m_InputVec{};

			float						m_ShotCoolTime{ 0.f };

			std::array<int, 8>			m_MissileID{};
			int							m_MissileIDNum{};

			bool						m_CanMove{ false };

			HitPoint					m_HitPoint{ 0 };
			HitPoint					m_MaxHitPoint{ 1000 };
		public:
			MetalObject(void) noexcept;
			MetalObject(const MetalObject&) = delete;
			MetalObject(MetalObject&& o) = delete;
			MetalObject& operator=(const MetalObject&) = delete;
			MetalObject& operator=(MetalObject&& o) = delete;

			virtual ~MetalObject(void) noexcept;
		public:
			void		SetPlayerID(PlayerID value) noexcept { this->m_PlayerID = value; }
			void		SetCanMove(bool value) noexcept { this->m_CanMove = value; }

			const auto& GetHitPoint(void) const noexcept { return this->m_HitPoint; }
			auto GetHitPointMax(void) const noexcept { return m_MaxHitPoint; }
		public:
			void DrawHPBer() noexcept;
			std::shared_ptr<BulletObject>	AddBullet(PlayerID shootID, const Vector2DX& Pos, float Radian, float Offset, float Speed, float Size) noexcept {
				auto* Obj2DParts = Object2DManager::Instance();
				const auto& Obj = std::make_shared<BulletObject>();
				Obj2DParts->AddObject(Obj);
				Obj->SetShootPlayer(shootID);
				Vector2DX Vec; Vec.Set(std::sin(Radian), std::cos(Radian));
				Obj->SetPosition(Pos + Vec * Offset);
				Obj->SetVec(Vec * Speed);
				Obj->SetSize(Size);
				return Obj;
			}

		public:
			void Update_OnHitObject(void) noexcept override;
			void Update_OnHitWall(void) noexcept override {}
		public:
			void Init_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override;
		};
	};
};
