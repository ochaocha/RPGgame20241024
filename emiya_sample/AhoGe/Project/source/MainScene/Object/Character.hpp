#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		enum class GunType {
			None,
			Handgun,
			Rifle,
			Rocket,
		};

		class CharacterObject : public Object2DManager::Base2DObject {
		private:
			PlayerID					m_PlayerID{ 0 };
			Vector2DX					m_InputVec{};

			float						m_Speed{ 1.f };

			float						m_Alpha{ 1.f };

			float						m_Radian{ 0.f };
			float						m_DodgeCoolTime{ 0.f };

			float						m_ShotCoolTime{ 0.f };

			float						m_RunFootPer{ 0.f };

			HitPoint					m_HitPoint{ 0 };
			const HitPoint				m_MaxHitPoint{ 3 };

			GunType						m_GunType{ GunType::None };
			Blur2DControl				m_Blur;
		public:
			CharacterObject(void) noexcept;
			CharacterObject(const CharacterObject&) = delete;
			CharacterObject(CharacterObject&& o) = delete;
			CharacterObject& operator=(const CharacterObject&) = delete;
			CharacterObject& operator=(CharacterObject&& o) = delete;

			virtual ~CharacterObject(void) noexcept{}
		private:
			void		UpdateInputVector(const InputControl& MyInput) noexcept;
		public:
			void		UpdateInput(const InputControl& MyInput) noexcept;
			void		SetPlayerID(PlayerID value) noexcept { this->m_PlayerID = value; }
			void		SetGunType(GunType value) noexcept { this->m_GunType = value; }
		public:
			auto CanLookPlayer0(void) const noexcept { return this->m_Alpha > 0.5f; }
			const auto& GetViewRad(void) const noexcept { return this->m_Radian; }
			const auto& GetSpeed(void) const noexcept { return this->m_Speed; }
			const auto& GetPlayerID(void) const noexcept { return this->m_PlayerID; }
			const auto& GetHitPoint(void) const noexcept { return this->m_HitPoint; }
			const auto& GetHitPointMax(void) const noexcept { return m_MaxHitPoint; }
		public:
			void Update_OnHitObject(void) noexcept override;
		public:
			void Init_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override{}
		};
	}
}
