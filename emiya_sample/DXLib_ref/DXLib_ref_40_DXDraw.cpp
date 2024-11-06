#include "DXLib_ref_40_DXDraw.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const WindowSizeControl* SingletonBase<WindowSizeControl>::m_Singleton = nullptr;
	// コンストラクタ
	WindowSizeControl::WindowSizeControl(void) noexcept {
		// 解像度指定(*96/DPIして画面上のサイズに合わせたもの)
		int DispXSize = static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
		int DispYSize = static_cast<int>(GetSystemMetrics(SM_CYSCREEN));
		if (DispYSize >= (DispXSize * BaseScreenHeight / BaseScreenWidth)) {// 縦長
			DispYSize = (DispXSize * BaseScreenHeight / BaseScreenWidth);
		}
		else {// 横長
			DispXSize = (DispYSize * BaseScreenWidth / BaseScreenHeight);
		}
		// ウィンドウモードの際の画面サイズ(縦が72小さくなるように)
		this->m_DispXSize_Win = DispXSize - 72 * BaseScreenWidth / BaseScreenHeight;
		this->m_DispYSize_Win = DispYSize - 72;
		// ボーダーレス、フルスクリーンの際の画面サイズ
		this->m_DispXSize_Border = DispXSize;
		this->m_DispYSize_Border = DispYSize;
	}
	// 設定に応じてウィンドウサイズと設定を切り替え
	void			WindowSizeControl::SetWindowOrBorderless(void) noexcept {
		auto* OptionParts = OPTION::Instance();
		switch (static_cast<WindowType>(OptionParts->GetParamInt(EnumSaveParam::WindowMode))) {
		case WindowType::Window:
			this->m_DispXSize = this->m_DispXSize_Win;
			this->m_DispYSize = this->m_DispYSize_Win;
			SetWindowStyleMode(0);
			ChangeWindowMode(TRUE);
			break;
		case WindowType::Borderless:
			this->m_DispXSize = this->m_DispXSize_Border;
			this->m_DispYSize = this->m_DispYSize_Border;
			SetWindowStyleMode(2);
			ChangeWindowMode(TRUE);
			break;
		case WindowType::FullScreen:
			this->m_DispXSize = this->m_DispXSize_Border;
			this->m_DispYSize = this->m_DispYSize_Border;
			SetWindowStyleMode(2);
			SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
			SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
			ChangeWindowMode(FALSE);
			break;
		case WindowType::Max:
		default:
			break;
		}
		// DPI込々で実際のウィンドウサイズとしたもの
		int DPI = 96;
		GetMonitorDpi(NULL, &DPI);
		SetWindowSize(this->m_DispXSize * DPI / 96, this->m_DispYSize * DPI / 96);
		SetWindowPosition(0, 0);
	}
};
