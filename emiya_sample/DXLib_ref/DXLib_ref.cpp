#include "DXLib_ref.h"

namespace DXLibRef {
	// シングルトンの実態定義
	const DXLib_ref* SingletonBase<DXLib_ref>::m_Singleton = nullptr;
	// コンストラクタ
	DXLib_ref::DXLib_ref(void) noexcept {
		// オプションデータ制御クラスの生成
		OPTION::Create();
		// ウィンドウ等制御クラスの生成
		WindowSizeControl::Create();
		// セーブデータ制御クラスの生成
		SaveDataClass::Create();
		auto* SaveDataParts = SaveDataClass::Instance();
		if (!SaveDataParts->Load()) {		// セーブデータのロード
			SaveDataParts->Save();			// ロードするデータがない場合初回データ作成のためセーブ
		}
		auto* WindowSizeParts = WindowSizeControl::Instance();
		auto* OptionParts = OPTION::Instance();

		SetUseDirect3DVersion(DirectXVerID[OptionParts->GetParamInt(EnumSaveParam::DirectXVer)]);	// directX ver
		SetOutApplicationLogValidFlag(TRUE);						// log
		SetMainWindowText("Loading...");							// タイトル
		ChangeWindowMode(TRUE);										// 窓表示
		SetUseDirectInputFlag(TRUE);								// 
		SetDirectInputMouseMode(TRUE);								// 
		SetWindowSizeChangeEnableFlag(FALSE, FALSE);				// ウインドウサイズを手動不可、ウインドウサイズに合わせて拡大もしないようにする
		{
			// DPI込々で実際のウィンドウサイズとしたもの
			int DPI = 96;
			GetMonitorDpi(NULL, &DPI);
			SetGraphMode(WindowSizeParts->GetSizeXMax() * DPI / 96, WindowSizeParts->GetSizeYMax() * DPI / 96, 32);		// 解像度
		}

		SetWaitVSync();// 垂直同期

		SetZBufferBitDepth(32);										// デフォのZバッファ精度を32bitに
		DxLib_Init();												// 初期化
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);			// 画面モード変更時( とウインドウモード変更時 )にリセットを走らせない
		SetUsePixelLighting(TRUE);									// ピクセルライティングの使用
		SetSysCommandOffFlag(TRUE);									// 
		SetAlwaysRunFlag(TRUE);										// background
		SetUseZBuffer3D(TRUE);										// zbufuse
		SetWriteZBuffer3D(TRUE);									// zbufwrite
		if (GetUseDirect3DVersion() != DirectXVerID[OptionParts->GetParamInt(EnumSaveParam::DirectXVer)]) {
			MessageBox(NULL, "DirectXのバージョンが対応していません。古いバージョンで動作しています", "", MB_OK);
		}
		WindowSizeParts->SetWindowOrBorderless();		// 上記をもとにウィンドウサイズを設定
		// デバッグ制御クラスの生成
#if DEBUG
		DebugClass::Create();
#endif // DEBUG

		// フォント制御クラスの生成
		UISystem::FontPool::Create();
		// キーガイド制御クラスの生成
		UISystem::KeyGuide::Create();
		// ポップアップ制御クラスの生成
		UISystem::PopUp::Create();
		// UI画面制御クラスの生成
		UISystem::DrawControl::Create();

		// サウンド制御クラスの生成
		SoundSystem::SoundPool::Create();
		// UI用の共通SEを読み込み
		auto* SoundParts = SoundSystem::SoundPool::Instance();
		SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select), 2, "data/Sound/UI/cursor.wav");
		SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_CANCEL), 1, "data/Sound/UI/cancel.wav");
		SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_OK), 1, "data/Sound/UI/ok.wav");
		SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_NG), 1, "data/Sound/UI/ng.wav");


		// キー、パッド制御クラスの生成
		PadControl::Create();
		// オブジェクト制御クラスの生成
		Object2DManager::Create();
		// ポストプロセス制御の生成
		PostPass::PostPassEffect::Create();
		// シーン遷移制御の生成
		SceneControl::Create();
		// オプション画面制御クラスの生成
		OptionWindowClass::Create();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		// オプション画面の初期化
		OptionWindowParts->Initialize();
	}
	//
	void			DXLib_ref::SetWaitVSync(void) noexcept {
		// 垂直同期
		auto* OptionParts = OPTION::Instance();
		if (DirectXVerID[OptionParts->GetParamInt(EnumSaveParam::DirectXVer)] == DX_DIRECT3D_11) {
			SetWaitVSyncFlag(OptionParts->GetParamBoolean(EnumSaveParam::VSync));//DirectX11ではあとからの変更が効く
		}
		else {
			SetWaitVSyncFlag(FALSE);//DirectX9では後からの変更が効かない
		}
	}
	// ループの最初に通す
	void			DXLib_ref::StartCount(void) noexcept {
		m_DeltaTime = static_cast<float>(GetNowHiPerformanceCount() - m_StartTime) / 1000000.f;
		m_StartTime = GetNowHiPerformanceCount();
	}
	// 表画面に反映し、垂直同期または一定のFPSまで待機する
	bool			DXLib_ref::WaitCount(void) const noexcept {
		auto* OptionParts = OPTION::Instance();
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
	// メインループやシーン遷移管理を行う
	bool			DXLib_ref::MainLogic(void) noexcept {
		auto* SceneParts = SceneControl::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// 一連のシーンループを開始
		while (true) {
			//シーン開始
			SceneParts->Initialize();
			// メインループ開始
			while (true) {
				// プロセスからのメッセージが届かない場合異常事態としてすぐに強制終了
				if (!(ProcessMessage() == 0)) {
					return false;
				}
				// ループ初回の更新
				StartCount();
#if DEBUG
				// DXLIBのメッセージ表示をクリア
				clsDx();
				// デバッグの計測開始位置をここに指定
				DebugParts->SetStartPoint();
#endif // DEBUG
				// シーンの演算更新を行う処理
				SceneParts->Update();
#if DEBUG
				// ポイントを追加
				DebugParts->SetPoint("-----DrawStart-----");
#endif // DEBUG
				// シーンの描画を行う処理
				SceneParts->DrawMainLoop();
#if DEBUG
				// デバッグ計測の終了点を設定
				DebugParts->SetEndPoint();
#endif // DEBUG
				ScreenFlip();	// 表画面に反映
				WaitCount();	// 垂直同期または一定のFPSまで待機する
				// シーン/ゲームの終了判定が立っているのでループを抜ける
				if (SceneParts->IsEndScene()) {
					break;
				}
				// 終了フラグは立っていないのでメインループの最初に戻る
			}
			// シーンの終わりに通る処理
			SceneParts->ExitMainLoop();
			// 終了フラグが立った場合即終了
			if (SceneParts->IsEndGame()) {
				break;
			}
			// 終了フラグは立っていないのでシーンループの最初に戻る
		}
		DxLib_End();		// DXLIBを終了させる
		return true;
	}
}
