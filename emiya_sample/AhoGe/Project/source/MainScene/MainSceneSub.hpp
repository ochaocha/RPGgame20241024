#pragma once

#include	"../sub.hpp"

#include	"../CommonScene/UI/CommonUIControl.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		//
		class PauseMenuControl {
			bool			m_IsRetire{ false };
		public:
			const auto& IsRetire(void) const noexcept { return this->m_IsRetire; }
		public:
			void			LoadPause(void) noexcept;
			void			SetPause(void) noexcept;
			void			UpdatePause(void) noexcept;
			void			DrawPause(void) const noexcept;
			void			DisposePause(void) noexcept;
		};
		//
		class InGameUIControl {
			GraphHandle					m_ViewHandle;
			GraphHandle					m_Watch;
			GraphHandle					m_Caution;
			GraphHandle					m_Alert;
			GraphHandle					m_Goal;

			float						m_MapDrawTime{ 0.f };
			float						m_MapDrawPer{ 0.f };
			int							m_MapTextID{ 0 };

			Vector2DX					m_GoalPos;
			float						m_StartTime{ 0.f };
		private:
			void			DrawCharaUI_Back(PlayerID value) noexcept;
			void			DrawCharaUI_Front(PlayerID value) const noexcept;
		public:
			void			SetMap(int value, const Vector2DX& GoalPos) noexcept {
				this->m_MapTextID = value;
				this->m_GoalPos = GoalPos;
			}
			void			SetStartTime(float value) noexcept { this->m_StartTime = value; }
		public:
			InGameUIControl(void) {}
			InGameUIControl(const InGameUIControl&) = delete;
			InGameUIControl(InGameUIControl&& o) = delete;
			InGameUIControl& operator=(const InGameUIControl&) = delete;
			InGameUIControl& operator=(InGameUIControl&& o) = delete;
		public:
			void			LoadUI(void) noexcept;
			void			SetUI(void) noexcept;
			void			UpdateUI(void) noexcept;
			void			Dispose_LoadUI(void) noexcept;
			void			SetupWatchScreen(void) noexcept;
			void			DrawUI_Back(void) const noexcept;
			void			DrawUI_Front(void) const noexcept;
			void			DrawUI_MapName(void) const noexcept;
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
			void			SetCut(void) noexcept;
			void			UpdateCut(void) noexcept;
			void			DrawCut(void) const noexcept;
		};
	};
};
