#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// DXLIBの開始、終了、ウィンドウサイズ制御、垂直同期制御を行います
	// --------------------------------------------------------------------------------------------------
	class WindowSizeControl : public SingletonBase<WindowSizeControl> {
	private:
		friend class SingletonBase<WindowSizeControl>;
	private:
		// 
		int							m_DispXSize{ BaseScreenWidth };
		int							m_DispYSize{ BaseScreenHeight };
		// 
		int							m_DispXSize_Win{ BaseScreenWidth };
		int							m_DispYSize_Win{ BaseScreenHeight };
		// 
		int							m_DispXSize_Border{ BaseScreenWidth };
		int							m_DispYSize_Border{ BaseScreenHeight };
	private:
		// コンストラクタ
		WindowSizeControl(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		WindowSizeControl(const WindowSizeControl&) = delete;
		WindowSizeControl(WindowSizeControl&& o) = delete;
		WindowSizeControl& operator=(const WindowSizeControl&) = delete;
		WindowSizeControl& operator=(WindowSizeControl&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:// ゲッター
		// 大きいほうのスクリーンサイズを得る(スクリーンバッファ用)
		const auto	GetSizeXMax(void) const noexcept { return GetMax(this->m_DispXSize_Border, m_DispXSize_Win); }
		const auto	GetSizeYMax(void) const noexcept { return GetMax(this->m_DispYSize_Border, m_DispYSize_Win); }
		// UI以外のスクリーン空間
		const auto	GetScreenX(int value) const noexcept { return value * GetSizeXMax() / this->m_DispXSize_Win; }
		const auto	GetScreenY(int value) const noexcept { return value * GetSizeYMax() / this->m_DispYSize_Win; }
		// UI用
		const auto	GetUIX(int value) const noexcept { return value * this->m_DispXSize / BaseScreenWidth; }
		const auto	GetUIY(int value) const noexcept { return value * this->m_DispYSize / BaseScreenHeight; }
	public:
		// 設定に応じてウィンドウサイズと設定を切り替え
		void		SetWindowOrBorderless(void) noexcept;
	};
};
