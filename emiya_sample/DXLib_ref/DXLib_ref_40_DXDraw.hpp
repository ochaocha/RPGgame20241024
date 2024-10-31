#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// DXLIBの開始、終了、ウィンドウサイズ制御、垂直同期制御を行います
	// --------------------------------------------------------------------------------------------------
	class DXDraw : public SingletonBase<DXDraw> {
	private:
		friend class SingletonBase<DXDraw>;
	private:
		// 
		int							m_DispXSize{ deskx };
		int							m_DispYSize{ desky };
		// 
		int							m_DispXSize_Win{ deskx };
		int							m_DispYSize_Win{ desky };
		// 
		int							m_DispXSize_Border{ deskx };
		int							m_DispYSize_Border{ desky };
		// 
		int							m_ScreenXSize{ deskx };
		int							m_ScreenYSize{ desky };

		float						m_DeltaTime{ 0.f };
		// 
		LONGLONG					m_StartTime{ 0 };
	public:// ゲッター
		// UI以外のスクリーン空間
		const auto		GetScreenX(int value) const noexcept { return value * this->m_ScreenXSize / BaseScreenWidth; }
		const auto		GetScreenY(int value) const noexcept { return value * this->m_ScreenYSize / BaseScreenHeight; }
		// UI以外のスクリーン空間(1920,1080の場合)
		const auto& GetScreenXMax(void) const noexcept { return this->m_ScreenXSize; }
		const auto& GetScreenYMax(void) const noexcept { return this->m_ScreenYSize; }
		// UI用
		const auto		GetUIX(int value) const noexcept { return value * this->m_DispXSize / BaseScreenWidth; }
		const auto		GetUIY(int value) const noexcept { return value * this->m_DispYSize / BaseScreenHeight; }
		// UI用(1920,1080の場合)
		const auto& GetUIXMax(void) const noexcept { return this->m_DispXSize; }
		const auto& GetUIYMax(void) const noexcept { return this->m_DispYSize; }
		// UI用(1920,1080の場合)
		const auto GetSizeXMax(void) const noexcept { return GetMax(this->m_DispXSize_Border, m_DispXSize_Win); }
		const auto GetSizeYMax(void) const noexcept { return GetMax(this->m_DispYSize_Border, m_DispYSize_Win); }
		// UIに対応したマウス座標を取得
		void			GetMousePosition(int* MouseX, int* MouseY) const noexcept;
		// FPS値の取得
		const auto& GetDeltaTime(void) const noexcept { return m_DeltaTime; }
		const auto		GetFps(void) const noexcept { return 1.f / m_DeltaTime; }
	public:
		// 設定に応じてウィンドウサイズと設定を切り替え
		void			SetWindowOrBorderless(void) noexcept;
	private:
		// ウィンドウサイズを反映する部分を抜き出した関数
		void			UpdateWindowSize(void) const noexcept {
			// DPI設定
			int DPI = 96;
			GetMonitorDpi(NULL, &DPI);
			SetWindowSize(this->m_DispXSize * DPI / 96, this->m_DispYSize * DPI / 96);
		}
	private:
		// コンストラクタ
		DXDraw(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		DXDraw(const DXDraw&) = delete;
		DXDraw(DXDraw&& o) = delete;
		DXDraw& operator=(const DXDraw&) = delete;
		DXDraw& operator=(DXDraw&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		// ループの最初に通す
		void			StartCount(void) noexcept;
		// 表画面に反映し、垂直同期または一定のFPSまで待機する
		bool			Screen_Flip(void) const noexcept;
		// 
		void			DXLibEnd(void) noexcept;
	};
};
