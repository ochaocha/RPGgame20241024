#pragma once

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*ビルド設定																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#define DEBUG (TRUE)/*デバッグ表示を入れるか否か*/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*pragma																																	*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma warning(disable:4505)	/*使用されていない関数を破棄する旨の警告を取るために追加*/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include																																	*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// 共通
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <D3D11.h>
#include <cmath>

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*const																																		*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
constexpr float		FrameRate{ 60.f };								/*処理の基準になるフレームレート*/
constexpr float		GravityRate{ -9.8f };							/*重力加速度*/
constexpr int		InvalidID{ -1 };								/*共通の無効値*/
constexpr int		BaseScreenWidth{ 1920 };						/*UI描画などの基準となる解像度*/
constexpr int		BaseScreenHeight{ 1080 };						/*UI描画などの基準となる解像度*/

// DPIを反映するデスクトップサイズ
const int deskx{ static_cast<int>(GetSystemMetrics(SM_CXSCREEN)) };
const int desky{ static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) };

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include DXLIB																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// DXLIB
#include "DxLib.h"
// 追加
#include "DXLib_ref_00_Util.hpp"
#include "DXLib_ref_01_Vector.hpp"
#include "DXLib_ref_02_SoundHandle.hpp"
#include "DXLib_ref_03_GraphHandle.hpp"
#include "DXLib_ref_08_Etc.hpp"
#include "DXLib_ref_09_KeyControl.hpp"
#include "DXLib_ref_10_Option.hpp"
#include "DXLib_ref_11_FileControl.hpp"
#include "DXLib_ref_12_Draw2DSystem.hpp"
#include "DXLib_ref_20_Debug.hpp"
#include "DXLib_ref_40_DXDraw.hpp"
#include "DXLib_ref_100_PostPass.hpp"
#include "DXLib_ref_101_sequence.hpp"
#include "DXLib_ref_102_ObjectManager.hpp"

namespace DXLibRef {
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLibRefのメインループなど全般的な管理を行います																							*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class DXLib_ref : public SingletonBase<DXLib_ref> {
	private:
		friend class SingletonBase<DXLib_ref>;
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
			// ポーズ中かどうかのフラグ
			bool						m_IsPauseActive{ false };
		public:
			void	UpdatePause(void) noexcept;
			void	DrawPause(void) const noexcept;
		public:
			// ポーズの有効無効を取得
			const auto		IsPause(void) const noexcept { return m_IsPauseActive; }
			void			ChangePause(bool value) noexcept;
		};
	private:
		// FPS表示用クラスの実体
		FPSDrawer					m_FPSDrawer;
		// ポーズ画面表示用クラスの実体
		PauseDrawer					m_PauseDrawer;
		// 現在のシーンが終了したフラグ
		bool						m_IsEndScene{ false };
		// ゲーム終了フラグ
		bool						m_IsEndGame{ false };
		// 初回起動フラグ
		bool						m_IsFirstBoot{ false };
		// 終了ポップアップが開いているかのフラグ
		bool						m_IsExitSelect{ false };
		// 再起動ポップアップが開いているかのフラグ
		bool						m_IsRestartSelect{ false };
	private:
		// 初回画面
		void			InitFirstBootSetting(void) noexcept;
		void			UpdateFirstBootSetting(void) noexcept;
		void			DrawFirstBootSetting(void) const noexcept;
		// 通常画面
		void			InitMainLoop(void) noexcept;
		void			UpdateMainLoop(void) noexcept;
		void			DrawMainLoop(void) const noexcept;
		void			ExitMainLoop(void) noexcept;
	public:
		// ポーズの有効無効を取得
		const auto		IsPause(void) const noexcept { return m_PauseDrawer.IsPause(); }
		void			ChangePause(bool value) noexcept { m_PauseDrawer.ChangePause(value); }
		// 終了、再起動フラグが立っているか外から確認できます
		const auto&		IsExit(void) const noexcept { return m_IsExitSelect; }
		const auto&		IsRestart(void) const noexcept { return m_IsRestartSelect; }
	private:
		// コンストラクタ
		DXLib_ref(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		DXLib_ref(const DXLib_ref&) = delete;
		DXLib_ref(DXLib_ref&& o) = delete;
		DXLib_ref& operator=(const DXLib_ref&) = delete;
		DXLib_ref& operator=(DXLib_ref&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		// ロジック開始時に通る
		bool			StartLogic(void) noexcept;
		// メインループやシーン遷移管理を行う
		bool			MainLogic(void) noexcept;
	};
};
