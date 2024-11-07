#pragma once

#include	"../sub.hpp"

#include	"../CommonScene/UI/CommonUIControl.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//
		// --------------------------------------------------------------------------------------------------
		class PauseMenuControl {
			bool			m_IsRetire{ false };
		public:
			const auto& IsRetire(void) const noexcept { return this->m_IsRetire; }
		public:
			void			Load(void) noexcept;
			void			Set(void) noexcept;
			void			Update(void) noexcept;
			void			Draw(void) const noexcept;
			void			DisposeLoad(void) noexcept;
		};
		// 
		class MapNameDrawControl {
			float						m_MapDrawTime{ 0.f };
			float						m_MapDrawPer{ 0.f };
			int							m_MapTextID{ 0 };
		public:
			void			Set(void) noexcept;
			void			Update(void) noexcept;
			void			Draw(void) const noexcept;
		};
		// 
		enum class CutSceneType {
			MsgBox,
			ViewPoint,
			WaitMilSec,
			CG,
			End,
			Max,
		};
		static const char* g_CutSceneStr[static_cast<int>(CutSceneType::Max)] = {
			"MsgBox",
			"ViewPoint",
			"WaitMilSec",
			"CG",
			"End",
		};
		class CutSceneControl {
			struct CutSceneData {
				CutSceneType m_Type{};
				int m_NameID{};
				int m_MsgID{};
				int m_PointX{};
				int m_PointY{};
				int m_WatiMS{};
				int m_CGSel{};
			};
		private:
			bool						m_IsCutScene{ false };// カットシーン中フラグ
			float						m_CutSceneAlpha{ 0.f };

			std::vector<CutSceneData>	m_CutSceneData{};
			int							m_CutSceneSeek{ 0 };

			std::array<std::string, 10>	m_MsgString{};
			bool						m_IsMsgBox{ false };// カットシーン中フラグ
			float						m_MsgBoxAlpha{ 0.f };
			float						m_MsgBoxSeek{ 0.f };
			int							m_PrevMsgBoxSeek{};
			int							m_PointX{};
			int							m_PointY{};
			float						m_WaitMS{ 0.f };
			int							m_CGSel{ InvalidID };
			float						m_CGFade{ 0.f };
			GraphHandle					m_CGGraph;
		public:
			const auto& IsCutScene(void) const noexcept { return this->m_IsCutScene; }
			const auto& GetAddViewPointX(void) const noexcept { return this->m_PointX; }
			const auto& GetAddViewPointY(void) const noexcept { return this->m_PointY; }
		public:
			void			StartCutScene(int ID) noexcept;
		public:
			void			Set(void) noexcept;
			void			Update(void) noexcept;
			void			DrawCut(void) const noexcept;
		};
	}
}
