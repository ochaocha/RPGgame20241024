#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// 1シーンぶんの基底クラス
	// --------------------------------------------------------------------------------------------------
	class TEMPSCENE {
	private:
		bool			m_IsFirstLoop{ true };			// 初回チェック
		bool			m_IsLoading{ false };			// 既に自分のシーンがLoadを通ったかどうか(DisposeLoadを通るまでは再Loadさせないようにする)
		std::array<std::shared_ptr<TEMPSCENE>, 10> Next_ptr{ nullptr };// 次に進むシーンのリスト
		size_t			m_Next_Select{ 0 };				// 次に進むシーンのリストのうちどれに進むか
	public:
		// 次に進むシーンをリストに追加追加
		void		SetNextSceneList(int index, const std::shared_ptr<TEMPSCENE>& Next_scenes_ptr_t) noexcept { Next_ptr.at(static_cast<std::size_t>(index)) = Next_scenes_ptr_t; }
		// 次に進むシーンを示す
		auto&		Get_Next(void) noexcept { return Next_ptr.at(this->m_Next_Select); }
		// リストのうちどれに進むか決定
		void		SetNextSelect(size_t value) noexcept { this->m_Next_Select = value; }
	public:// ゲッター
		// Update_Subが初めて呼び出されたかどうか
		const auto& GetIsFirstLoop(void) const noexcept { return m_IsFirstLoop; }
	public:
		// コンストラクタ
		TEMPSCENE(void) noexcept {}
		TEMPSCENE(const TEMPSCENE&) = delete;
		TEMPSCENE(TEMPSCENE&& o) = delete;
		TEMPSCENE& operator=(const TEMPSCENE&) = delete;
		TEMPSCENE& operator=(TEMPSCENE&& o) = delete;
		// デストラクタ
		virtual ~TEMPSCENE(void) noexcept {}
	public:// メイン更新
		void Load(void) noexcept {
			if (!m_IsLoading) {
				m_IsLoading = true;
				Load_Sub();
			}
		}
		void Set(void) noexcept {
			this->m_Next_Select = 0;
			Set_Sub();
			m_IsFirstLoop = true;
		}
		bool Update(void) noexcept {
			auto ans = Update_Sub();
			m_IsFirstLoop = false;
			return ans;
		}
		// 
		void MainDraw(void) const noexcept { MainDraw_Sub(); }
		void DrawUI_Base(void) const noexcept { DrawUI_Base_Sub(); }
		void DrawUI_In(void) const noexcept { DrawUI_In_Sub(); }
		// 描画
		void Dispose(void) noexcept { Dispose_Sub(); }
		void Dispose_Load(void) noexcept {
			if (m_IsLoading) {
				m_IsLoading = false;
				Dispose_Load_Sub();
			}
		}
	protected:// 継承物 継承先のシーンではこれらをoverrideさせる
		virtual void Load_Sub(void) noexcept {}
		virtual void Set_Sub(void) noexcept {}
		virtual bool Update_Sub(void) noexcept { return true; }

		virtual void MainDraw_Sub(void) const noexcept {}
		virtual void DrawUI_Base_Sub(void) const noexcept {}
		virtual void DrawUI_In_Sub(void) const noexcept {}

		virtual void Dispose_Sub(void) noexcept {}
		virtual void Dispose_Load_Sub(void) noexcept {}
	};
	// --------------------------------------------------------------------------------------------------
	// シーンを取りまとめるクラス
	// --------------------------------------------------------------------------------------------------
	class SceneControl : public SingletonBase<SceneControl> {
	private:
		friend class SingletonBase<SceneControl>;
	private:
		// FPS表示用クラス
		class FPSDrawer {
			std::array<float, 60>		FPSAvgs{};
			size_t						m_FPSAvgCount{ 0 };
			float						m_FPSAvg{ 0.f };
		public:
			// FPS表示
			void	InitFPSCounter(void) noexcept;
			void	UpdateFPSCounter(void) noexcept;
			void	DrawFPSCounter(void) const noexcept;
		};
		// ポーズ画面表示用クラス
		class PauseDrawer {
			float						m_PauseFlashCount{ 0.f };
		public:
			void	UpdatePause(void) noexcept;
			void	DrawPause(void) const noexcept;
		};
	private:
		bool						m_IsEndScene{ false };		// 現在のシーンが終了したフラグ
		bool						m_IsEndGame{ false };		// ゲーム終了フラグ
		bool						m_IsExitSelect{ false };		// 終了ポップアップが開いているかのフラグ
		bool						m_IsRestartSelect{ false };		// 再起動ポップアップが開いているかのフラグ

		FPSDrawer					m_FPSDrawer;		// FPS表示用クラスの実体
		PauseDrawer					m_PauseDrawer;		// ポーズ画面表示用クラスの実体
		std::shared_ptr<TEMPSCENE>	m_NowScenesPtr;		// 今行っているシーン
		// ポーズ中かどうかのフラグ
		bool						m_IsPauseActive{ false };
	public:
		const auto		IsEndScene(void) const noexcept { return m_IsEndScene || m_IsEndGame; }
		const auto		IsEndGame(void) const noexcept { return m_IsEndGame; }
		// ポーズの有効無効を取得
		const auto		IsPause(void) const noexcept { return m_IsPauseActive; }
		void			ChangePause(bool value) noexcept;
		// 終了、再起動フラグが立っているか外から確認できます
		const auto&		IsExit(void) const noexcept { return m_IsExitSelect; }
		const auto&		IsRestart(void) const noexcept { return m_IsRestartSelect; }
	private:
		// コンストラクタ
		SceneControl(void) noexcept {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		SceneControl(const SceneControl&) = delete;
		SceneControl(SceneControl&& o) = delete;
		SceneControl& operator=(const SceneControl&) = delete;
		SceneControl& operator=(SceneControl&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		//最初に進むシーンを設定
		void			SetFirstScene(const std::shared_ptr<TEMPSCENE>& ptr) noexcept { m_NowScenesPtr = ptr; }
		void			InitMainLoop(void) noexcept;
		void			UpdateMainLoop(void) noexcept;
		void			DrawMainLoop(void) const noexcept;
		void			ExitMainLoop(void) noexcept;
	};
};
