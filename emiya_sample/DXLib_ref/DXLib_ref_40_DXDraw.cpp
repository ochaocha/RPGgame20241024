#include "DXLib_ref_40_DXDraw.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const DXDraw* SingletonBase<DXDraw>::m_Singleton = nullptr;
	// コンストラクタ
	DXDraw::DXDraw(void) noexcept {
		auto* OptionParts = OPTION::Instance();
		int DispXSize = deskx;
		int DispYSize = desky;
		{
			// 解像度指定
			if (desky >= (deskx * BaseScreenHeight / BaseScreenWidth)) {// 4:3
				DispXSize = deskx;
				DispYSize = (deskx * BaseScreenHeight / BaseScreenWidth);
			}
			else {// 16:9より横長
				DispXSize = (desky * BaseScreenWidth / BaseScreenHeight);
				DispYSize = desky;
			}
		}
		int DXVer = DirectXVerID[OptionParts->GetParamInt(EnumSaveParam::DirectXVer)];
		SetOutApplicationLogValidFlag(TRUE);						// log
		SetMainWindowText("Loading...");							// タイトル
		ChangeWindowMode(TRUE);										// 窓表示
		SetUseDirect3DVersion(DXVer);								// directX ver
		SetUseDirectInputFlag(TRUE);								// 
		SetDirectInputMouseMode(TRUE);								// 
		{
			// DPI設定
			int DPI = 96;
			GetMonitorDpi(NULL, &DPI);
			SetGraphMode(DispXSize * DPI / 96, DispYSize * DPI / 96, 32);		// 解像度
		}
		SetWindowSizeChangeEnableFlag(FALSE, FALSE);				// ウインドウサイズを手動不可、ウインドウサイズに合わせて拡大もしないようにする
		SetWaitVSyncFlag((DXVer == DX_DIRECT3D_11) && OptionParts->GetParamBoolean(EnumSaveParam::VSync));		// 垂直同期
		SetZBufferBitDepth(32);										// デフォのZバッファ精度を32bitに
		DxLib_Init();												// 初期化
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);			// 画面モード変更時( とウインドウモード変更時 )にリセットを走らせない
		SetUsePixelLighting(TRUE);									// ピクセルライティングの使用
		if (GetUseDirect3DVersion() != DXVer) {
			MessageBox(NULL, "DirectXのバージョンが適用していません。古いバージョンで動作しています", "", MB_OK);
		}
		SetSysCommandOffFlag(TRUE);									// 
		SetAlwaysRunFlag(TRUE);										// background
		SetUseZBuffer3D(TRUE);										// zbufuse
		SetWriteZBuffer3D(TRUE);									// zbufwrite
		// MV1SetLoadModelPhysicsWorldGravity(GravityRate);			// 重力
		// ウィンドウモードの際の画面サイズ
		this->m_DispXSize_Win = DispXSize - 72 * 16 / 9;
		this->m_DispYSize_Win = DispYSize - 72;
		// ボーダーレス、フルスクリーンの際の画面サイズ
		this->m_DispXSize_Border = DispXSize;
		this->m_DispYSize_Border = DispYSize;
		// UI側での画面サイズ
		this->m_ScreenXSize = BaseScreenWidth * GetMin(DispXSize, BaseScreenWidth) / this->m_DispXSize_Win;
		this->m_ScreenYSize = BaseScreenHeight * GetMin(DispYSize, BaseScreenHeight) / this->m_DispYSize_Win;
		// 上記をもとにウィンドウサイズを設定
		SetWindowOrBorderless();
	}
	// 設定に応じてウィンドウサイズと設定を切り替え
	void			DXDraw::SetWindowOrBorderless(void) noexcept {
		auto* OptionParts = OPTION::Instance();
		switch (static_cast<WindowType>(OptionParts->GetParamInt(EnumSaveParam::WindowMode))) {
		case WindowType::Window:
			this->m_DispXSize = this->m_DispXSize_Win;
			this->m_DispYSize = this->m_DispYSize_Win;
			SetWindowStyleMode(0);
			SetWindowPosition(0, 0);
			UpdateWindowSize();
			ChangeWindowMode(TRUE);
			break;
		case WindowType::Borderless:
			this->m_DispXSize = this->m_DispXSize_Border;
			this->m_DispYSize = this->m_DispYSize_Border;
			SetWindowStyleMode(2);
			SetWindowPosition(0, 0);
			UpdateWindowSize();
			ChangeWindowMode(TRUE);
			break;
		case WindowType::FullScreen:
			this->m_DispXSize = this->m_DispXSize_Border;
			this->m_DispYSize = this->m_DispYSize_Border;
			SetWindowStyleMode(2);
			SetWindowPosition(0, 0);
			UpdateWindowSize();
			SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
			SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
			ChangeWindowMode(FALSE);
			break;
		case WindowType::Max:
		default:
			break;
		}
	}
	// UIに対応したマウス座標を取得
	void			DXDraw::GetMousePosition(int* MouseX, int* MouseY) const noexcept {
		auto y_UIMs = [this](int p1) {
			auto* OptionParts = OPTION::Instance();
			if (OptionParts->GetParamInt(EnumSaveParam::WindowMode) == static_cast<int>(WindowType::Window)) {
				return (int(p1) * m_DispYSize_Border / desky);
			}
			return (int(p1) * m_DispYSize / desky);
			};
		int mx = 0, my = 0;
		GetMousePoint(&mx, &my);
		*MouseX = y_UIMs(mx);
		*MouseY = y_UIMs(my);
	}
	// ループの最初に通す
	void			DXDraw::StartCount(void) noexcept {
		m_DeltaTime = static_cast<float>(GetNowHiPerformanceCount() - m_StartTime) / 1000000.f;
		m_StartTime = GetNowHiPerformanceCount();
	}
	// 表画面に反映し、垂直同期または一定のFPSまで待機する
	bool			DXDraw::Screen_Flip(void) const noexcept {
		auto* OptionParts = OPTION::Instance();
		ScreenFlip();
		if (!OptionParts->GetParamBoolean(EnumSaveParam::VSync)) {
			// 4msだけスリープ
			while ((GetNowHiPerformanceCount() - m_StartTime) < static_cast<LONGLONG>(1000 * (1000 / OptionParts->GetParamInt(EnumSaveParam::FpsLimit) - 4))) {
				if (ProcessMessage() != 0) {
					return false;
				}
				SleepThread(1);	// 1msecスリープする
			}
			while ((GetNowHiPerformanceCount() - m_StartTime) < static_cast<LONGLONG>(1000 * 1000 / OptionParts->GetParamInt(EnumSaveParam::FpsLimit))) {
			}
		}
		else {
			if (GetUseDirect3DVersion() != DX_DIRECT3D_11) {
				WaitVSync(1);
			}
		}
		return true;
	}
	void DXDraw::DXLibEnd(void) noexcept {
		DxLib_End();
	}
};
