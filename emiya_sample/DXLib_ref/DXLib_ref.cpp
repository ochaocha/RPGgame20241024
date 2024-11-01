#include "DXLib_ref.h"

namespace DXLibRef {
	// シングルトンの実態定義
	const DXLib_ref* SingletonBase<DXLib_ref>::m_Singleton = nullptr;
	// コンストラクタ
	DXLib_ref::DXLib_ref(void) noexcept {
		// オプションデータ制御クラスの生成
		OPTION::Create();
		// セーブデータ制御クラスの生成
		SaveDataClass::Create();
		auto* SaveDataParts = SaveDataClass::Instance();
		if (!SaveDataParts->Load()) {		// セーブデータのロード
			SaveDataParts->Save();			// ロードするデータがない場合初回データ作成のためセーブ
		}
		// ウィンドウ等制御クラスの生成
		DXDraw::Create();
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
		SoundPool::Create();
		// UI用の共通SEを読み込み
		auto* SoundParts = SoundPool::Instance();
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select), 2, "CommonData/Sound/UI/cursor.wav");
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_CANCEL), 1, "CommonData/Sound/UI/cancel.wav");
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_OK), 1, "CommonData/Sound/UI/ok.wav");
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_NG), 1, "CommonData/Sound/UI/ng.wav");


		// キー、パッド制御クラスの生成
		PadControl::Create();
		// オブジェクト制御クラスの生成
		Object2DManager::Create();
		// ポストプロセス制御の生成
		PostPassEffect::Create();
		// シーン遷移制御の生成
		SceneControl::Create();
		// オプション画面制御クラスの生成
		OptionWindowClass::Create();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		// オプション画面の初期化
		OptionWindowParts->Init();
	}
	// メインループやシーン遷移管理を行う
	bool DXLib_ref::MainLogic(void) noexcept {
		auto* SceneParts = SceneControl::Instance();
		auto* DrawParts = DXDraw::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// 一連のシーンループを開始
		while (true) {
			//シーン開始
			SceneParts->InitMainLoop();
			// メインループ開始
			while (true) {
				// プロセスからのメッセージが届かない場合異常事態としてすぐに強制終了
				if (!(ProcessMessage() == 0)) {
					return false;
				}
#if DEBUG
				// DXLIBのメッセージ表示をクリア
				clsDx();
				// デバッグの計測開始位置をここに指定
				DebugParts->SetStartPoint();
#endif // DEBUG
				// シーンの演算更新を行う処理
				SceneParts->UpdateMainLoop();
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
				// 表画面に反映し、垂直同期または一定のFPSまで待機する
				DrawParts->Screen_Flip();
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
		// DXLIBを終了させる
		DrawParts->DXLibEnd();
		return true;
	}
};
