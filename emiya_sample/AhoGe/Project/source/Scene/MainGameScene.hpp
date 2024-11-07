#pragma once
#include	"../Header.hpp"
#include	"../CommonScene/UI/CommonUIControl.hpp"
#include	"../MainScene/MainSceneSub.hpp"
#include	"../MainScene/Player/CPU.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//
		// --------------------------------------------------------------------------------------------------
		class MainGameScene : public TEMPSCENE {
		private:
			PauseMenuControl			m_PauseMenuControl;
			MapNameDrawControl			m_MapNameDrawControl;
			CutSceneControl				m_CutSceneControl;

			GraphHandle					m_ViewHandle;
			GraphHandle					m_Watch;
			GraphHandle					m_Caution;
			GraphHandle					m_Alert;
			GraphHandle					m_Goal;

			Vector2DX					m_GoalPos;
			float						m_StartTime{ 0.f };

			bool						m_IsGoodEnd{ false };// シーン全体の終了フラグ

			bool						m_IsBadEnd{ false };// シーン全体の終了フラグ

			bool						m_IsEndUpdate{ false };// 次シーンに行くフラグ
			bool						m_IsEnd{ false };// シーン全体の終了フラグ
			bool						m_IsPlayable{ false };// 操作可能
			// 
			Vector2DX					m_CamAddPos;
			// 
			std::string					m_MapName;
			int							m_EntryID{};
			int							m_CutSceneID{ InvalidID };// カットシーンID

			int							m_BossUniqueID{};
			int							m_WinCutSceneID{};// カットシーンID
			// 
			std::pair<int, int>			m_PrevXY{};
			// 
			bool						m_IsNormalBGM{ false };
			float						m_NormalBGM{ 0.f };
			bool						m_IsCautionBGM{ false };
			float						m_CautionBGM{ 0.f };
			bool						m_IsAlertBGM{ false };
			float						m_AlertBGM{ 0.f };
		private:
			void			DrawCharaUI_Back(PlayerID value) const noexcept;
			void			SetSceneEnd(void) noexcept;
		public:
			//コンストラクタ
			MainGameScene(void) noexcept { }
			MainGameScene(const MainGameScene&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			MainGameScene(MainGameScene&& o) = delete;
			MainGameScene& operator=(const MainGameScene&) = delete;
			MainGameScene& operator=(MainGameScene&& o) = delete;
			//デストラクタ 継承オブジェクトである為デストラクタにvirtualを指定
			virtual ~MainGameScene(void) noexcept {}
		public:
			//このシーンで永続して使用するリソースの読み込み、破棄をおこなうもの
			void			Load_Sub(void) noexcept override;
			void			Dispose_Load_Sub(void) noexcept override;
			// 描画以外の初期化、更新、破棄に関するもの
			void			Initialize_Sub(void) noexcept override;
			bool			Update_Sub(void) noexcept override;
			void			Dispose_Sub(void) noexcept override;
			// 2D空間表示
			void			DrawMain_Sub(void) const noexcept override;
			// UI表示
			void			DrawUI_Base_Sub(void) const noexcept override;
			void			DrawUI_In_Sub(void) const noexcept override;
		};
	}
}
