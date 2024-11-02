#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

#include	"../BackGround/BackGround.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		class AIControl {
		private:
			class Impl;
			Impl* m_Param;
			const Impl* GetParam(void) const noexcept { return this->m_Param; }
			Impl* GetParam(void) noexcept { return this->m_Param; }
		public:
			AIControl(void) noexcept;
			~AIControl(void) noexcept;
		public:
			const InputControl& GetAIInput(void) const noexcept;
			const float& GetInputRad(void) const noexcept;
			const float		GetGraphAlpha(void) const noexcept;
			bool			IsCaution(void) const noexcept;
			bool			IsAlert(void) const noexcept;
		public:
			void			SetCharacter(PlayerID MyCharaID, PlayerID TargetCharaID) noexcept;
		public:
			void			Init(void) noexcept;
			void			Update(void) noexcept;
			void			Draw(void) noexcept;
			void			Dispose(void) noexcept;
		};
	};
};
