#include "DXLib_ref.h"

namespace DXLibRef {
	// シングルトンの実態定義
	const DXLib_ref* SingletonBase<DXLib_ref>::m_Singleton = nullptr;
	// FPS表示
	void DXLib_ref::FPSDrawer::InitFPSCounter(void) noexcept {
		// 各々の数値を初期化
		for (auto& f : FPSAvgs) {
			f = FrameRate;
		}
		m_FPSAvgCount = 0;
	}
	void DXLib_ref::FPSDrawer::UpdateFPSCounter(void) noexcept {
		auto* DrawParts = DXDraw::Instance();
		// m_FPSAvgCountの番号に対して今のフレームレートを保存
		FPSAvgs.at(m_FPSAvgCount) = DrawParts->GetFps();
		// 保存する場所をずらす
		++m_FPSAvgCount %= FPSAvgs.size();
		// 保存している過去のFPS値の平均をとる
		m_FPSAvg = 0.f;
		for (auto& f : FPSAvgs) {
			m_FPSAvg += f;
		}
		m_FPSAvg = m_FPSAvg / static_cast<float>(FPSAvgs.size());
	}
	void DXLib_ref::FPSDrawer::DrawFPSCounter(void) const noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto* OptionParts = OPTION::Instance();
		// FPSの平均値が設定していた上限値に対して高いなら緑、低いなら黄色赤色と変化させる
		auto color = White;
		if (m_FPSAvg > static_cast<float>(OptionParts->GetParamInt(EnumSaveParam::FpsLimit) - 2)) {
			color = Green;// 十分にFPSが出ている
		}
		else if (m_FPSAvg > static_cast<float>(OptionParts->GetParamInt(EnumSaveParam::FpsLimit) - 10)) {
			color = Yellow;// 十分にFPSが出ていない
		}
		else {
			color = Red;// まったくFPSが出ていない
		}
		// FPS値の表示
		WindowSystem::SetMsg(DrawParts->GetUIXMax() - DrawParts->GetUIY(8), DrawParts->GetUIY(8) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::RIGHT, color, Black, "%5.2f FPS", m_FPSAvg);
		// ドローコール(DirectXに何回描画指示を送ったか)の表示
		WindowSystem::SetMsg(DrawParts->GetUIXMax() - DrawParts->GetUIY(8), DrawParts->GetUIY(8 + 20) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::RIGHT, White, Black, "%d Drawcall", GetDrawCallCount());
	}
	// ポーズ画面
	void DXLib_ref::PauseDrawer::UpdatePause(void) noexcept {
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* Pad = PadControl::Instance();
		// ポーズ入力によるオンオフ
		if (!OptionWindowParts->IsActive() && Pad->GetKey(PADS::INVENTORY).trigger()) {
			ChangePause(!m_IsPauseActive);
		}
		// ポーズ画面では点滅の演算を行う
		if (m_IsPauseActive) {
			auto* DrawParts = DXDraw::Instance();
			// 1秒経ったら0秒にリセットする
			m_PauseFlashCount += DrawParts->GetDeltaTime();
			if (m_PauseFlashCount > 1.f) {
				m_PauseFlashCount -= 1.f;
			}
		}
	}
	void DXLib_ref::PauseDrawer::DrawPause(void) const noexcept {
		// ポーズ画面に入っていない場合はスルーする
		if (!m_IsPauseActive) { return; }
		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();
		// 半透明の暗幕
		DrawCtrls->SetAlpha(WindowSystem::DrawLayer::Normal, 128);
		DrawCtrls->SetDrawBox(WindowSystem::DrawLayer::Normal, 0, 0, DrawParts->GetUIXMax(), DrawParts->GetUIYMax(), Black, TRUE);
		DrawCtrls->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
		// カウントが0.5秒以上であれば Pause の文字を表示
		if (m_PauseFlashCount > 0.5f) {
			WindowSystem::SetMsg(DrawParts->GetUIY(16), DrawParts->GetUIY(16) + DrawParts->GetUIY(36) / 2, DrawParts->GetUIY(36), FontSystem::FontXCenter::LEFT, Green, Black, "Pause");
		}
	}
	void DXLib_ref::PauseDrawer::ChangePause(bool value) noexcept {
		if (m_IsPauseActive != value) {
			m_IsPauseActive = value;
			auto* Pad = PadControl::Instance();
			Pad->SetGuideUpdate();
		}
	}
	// 初回設定画面の初期化
	void DXLib_ref::InitFirstBootSetting(void) noexcept {
		auto* OptionWindowParts = OptionWindowClass::Instance();
		// タイトル切り替え
		SetMainWindowText("FirstBoot Option");
		// オプション画面を開く
		OptionWindowParts->SetActive();
	}
	// 初回設定画面の更新
	void DXLib_ref::UpdateFirstBootSetting(void) noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();
		auto* Pad = PadControl::Instance();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* PopUpParts = PopUp::Instance();
		// ループ初回の更新
		DrawParts->StartCount();
		// UI描画リストをクリア
		DrawCtrls->ClearList();
		// ウィンドウサイズを更新
		int xBase = DrawParts->GetUIY(1366);
		int yBase = DrawParts->GetUIY(768);
		SetWindowPosition((deskx - xBase) / 2, (desky - yBase) / 2);
		SetWindowSize(xBase, yBase);
		// 操作のアップデート
		Pad->Update();
		// オプション画面のアップデート
		OptionWindowParts->Update();
		// ポップアップのアップデート
		PopUpParts->Update();
	}
	// 初回設定画面の描画
	void DXLib_ref::DrawFirstBootSetting(void) const noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto* PopUpParts = PopUp::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();
		// 裏画面をクリアして描画先に設定
		GraphHandle::SetDraw_Screen(static_cast<int>(DX_SCREEN_BACK), true);
		{
			int xBase = DrawParts->GetUIY(1366);
			// int yBase = DrawParts->GetUIY(768);
			int Width = DrawParts->GetUIY(720);
			int Height = DrawParts->GetUIY(720);
			int Edge = DrawParts->GetUIY(16);

			PopUpParts->Draw(Width / 2 + Edge, Height / 2 + Edge);

			WindowSystem::SetMsg(Edge + Edge, Height + Edge + Edge, DrawParts->GetUIY(12), FontSystem::FontXCenter::LEFT, Green, Black, "初期設定を行います。適切なゲームパッドの入力方式/グラフィックを確認してください。");

			int xp = Width + Edge + Edge + Edge;
			int yp = Height;
			if (WindowSystem::SetMsgClickBox(xp, yp, xBase - Edge + Edge, yp + Edge + Edge, LineHeight, Green, false, true, "Start Game!")) {
				PopUpParts->EndAll();
			}
			// 結果を描画
			DrawCtrls->Draw();
		}
	}
	// シーンのループ開始前に行う処理
	void DXLib_ref::InitMainLoop(void) noexcept {
		auto* SceneParts = SceneControl::Instance();
		auto* Pad = PadControl::Instance();
		// シーンのロード
		SceneParts->GetNowScene()->Load();
		// シーンの初回処理
		SceneParts->GetNowScene()->Set();
		// キーガイドに設定しているものをリセット
		Pad->Reset();
		// シーン開始時にキーガイド表示の更新を行うフラグを立てる
		Pad->SetGuideUpdate();
		// FPS表示の初期化
		m_FPSDrawer.InitFPSCounter();
	}
	// シーンの演算更新を行う処理
	void DXLib_ref::UpdateMainLoop(void) noexcept {
		auto* SceneParts = SceneControl::Instance();
		auto* Pad = PadControl::Instance();
		auto* PopUpParts = PopUp::Instance();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* PostPassParts = PostPassEffect::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();
		// ループ初回の更新
		DrawParts->StartCount();
		// UI描画リストをクリア
		DrawCtrls->ClearList();
		// 終了キーを押し、尚且つ終了ウィンドウが出ていない場合
		if (Pad->GetEsc().trigger() && !m_IsExitSelect) {
			// 終了ウィンドウフラグを立てて
			m_IsExitSelect = true;
			// 終了ポップアップを表示
			PopUpParts->Add("Exit", 480, 240,
				[this](int xmin, int ymin, int xmax, int ymax, bool) {
					// ポップアップ内で描画するもの
					auto* DrawParts = DXDraw::Instance();
					int xp1, yp1;
					// タイトル
					{
						xp1 = xmin + DrawParts->GetUIY(24);
						yp1 = ymin + LineHeight;

						WindowSystem::SetMsg(xp1, yp1 + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, "ゲームを終了しますか？");
					}
					// 終了するボタン
					{
						xp1 = (xmax + xmin) / 2 - DrawParts->GetUIY(54);
						yp1 = ymax - LineHeight * 3;

						auto* Pad = PadControl::Instance();
						bool ret = WindowSystem::SetMsgClickBox(xp1, yp1, xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2, LineHeight, Gray15, false, true, "終了");
						if (Pad->GetKey(PADS::INTERACT).trigger() || ret) {
							// 終了フラグを立てる
							this->m_IsEndGame = true;
						}
					}
				},
				[this]() {
					// ポップアップが閉じるボタン等で終了した際に行うもの
					m_IsExitSelect = false;
				},
				[]() {/*ガイド表示に追加するもの。ここでは特に表示しない*/},
				true
			);
		}
		// 再起動フラグが立ち、尚且つ再起動ポップアップが出ていない場合
		if (OptionWindowParts->IsRestartSwitch() && !m_IsRestartSelect) {
			m_IsRestartSelect = true;
			PopUpParts->Add("Restart", 480, 240,
				[this](int xmin, int ymin, int xmax, int ymax, bool) {
					// ポップアップ内で描画するもの
					auto* DrawParts = DXDraw::Instance();
					int xp1, yp1;
					// タイトル
					{
						xp1 = xmin + DrawParts->GetUIY(24);
						yp1 = ymin + LineHeight;

						WindowSystem::SetMsg(xp1, yp1 + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, "ゲームを再起動しますか？");
					}
					// 終了するボタン
					{
						xp1 = (xmax + xmin) / 2 - DrawParts->GetUIY(54);
						yp1 = ymax - LineHeight * 3;

						auto* Pad = PadControl::Instance();
						bool ret = WindowSystem::SetMsgClickBox(xp1, yp1, xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2, LineHeight, Gray15, false, true, "再起動");
						if (Pad->GetKey(PADS::INTERACT).trigger() || ret) {
							// 終了フラグを立てる
							this->m_IsEndGame = true;
							// 別プロセスとして自身を起動しておく
							StartMe();
						}
					}
				},
				[this]() {
					// ポップアップが閉じるボタン等で終了した際に行うもの
					m_IsRestartSelect = false;
				},
				[]() {/*ガイド表示に追加するもの。ここでは特に表示しない*/},
				true
			);
		}
		// 操作のアップデート
		Pad->Update();
		// シーンの更新
		m_IsEndScene = !SceneParts->GetNowScene()->Update();
		// オプション画面のアップデート
		OptionWindowParts->Update();
		// カメラシェイクの更新
		CameraShake::Instance()->Update();
		// ポストプロセス処理の更新
		PostPassParts->Update();
		// ポップアップのアップデート
		PopUpParts->Update();
		// ポーズ画面の更新
		m_PauseDrawer.UpdatePause();
		// FPS表示機能の更新
		m_FPSDrawer.UpdateFPSCounter();
	}
	// シーンの描画を行う処理
	void DXLib_ref::DrawMainLoop(void) const noexcept {
		auto* SceneParts = SceneControl::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* Pad = PadControl::Instance();
		auto* PopUpParts = PopUp::Instance();
		auto* PostPassParts = PostPassEffect::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// ポストプロセス用のバッファー画面に反映
		PostPassParts->GetBufferScreen().SetDraw_Screen();
		{
			// ワールド空間の描画
			SceneParts->GetNowScene()->MainDraw();
		}
		// ポストプロセス
		PostPassParts->DrawPostProcess();
		// 裏画面をクリアして描画先に設定
		GraphHandle::SetDraw_Screen(static_cast<int>(DX_SCREEN_BACK), true);
		{
			// バイリニアフィルターをかけてポストプロセス処理後のバッファー画面を描画
			int Prev = GetDrawMode();
			// SetDrawMode(DX_DRAWMODE_NEAREST);
			SetDrawMode(DX_DRAWMODE_BILINEAR);
			PostPassParts->GetBufferScreen().DrawExtendGraph(0, 0, DrawParts->GetUIXMax(), DrawParts->GetUIYMax(), false);
			SetDrawMode(Prev);

			// UI描画設定
			SceneParts->GetNowScene()->DrawUI_Base();
			// ポーズ描画を設定
			m_PauseDrawer.DrawPause();
			// FPS表示描画を設定
			m_FPSDrawer.DrawFPSCounter();
			// キーガイド描画を設定
			Pad->Draw();
			// ポップアップ描画を画面中央をセンターとして設定
			PopUpParts->Draw(DrawParts->GetUIXMax() / 2, DrawParts->GetUIYMax() / 2);
			// ポーズ画面の前などに描画するUIの描画設定
			SceneParts->GetNowScene()->DrawUI_In();
#if DEBUG
			// デバッグ画面描画を設定
			DebugParts->DebugWindow(DrawParts->GetUIXMax() - DrawParts->GetUIY(350), DrawParts->GetUIY(150));
#endif // DEBUG
			// 結果を描画
			DrawCtrls->Draw();
		}
	}
	// シーンの終わりに通る処理
	void DXLib_ref::ExitMainLoop(void) noexcept {
		auto* SceneParts = SceneControl::Instance();
		// 次のシーンへ移行
		SceneParts->NextScene();
	}
	// コンストラクタ
	DXLib_ref::DXLib_ref(void) noexcept {
		// オプションデータ制御クラスの生成
		OPTION::Create();
		// セーブデータ制御クラスの生成
		SaveDataClass::Create();
		auto* SaveDataParts = SaveDataClass::Instance();
		// セーブデータのロード
		m_IsFirstBoot = !SaveDataParts->Load();
		// ロードするデータがない場合初回と判定
		if (m_IsFirstBoot) {
			// 初回データ作成のためセーブ
			SaveDataParts->Save();
		}
		// ウィンドウ等制御クラスの生成
		DXDraw::Create();
		// デバッグ制御クラスの生成
#if DEBUG
		DebugClass::Create();
#endif // DEBUG
		// サウンド制御クラスの生成
		SoundPool::Create();
		// フォント制御クラスの生成
		FontSystem::FontPool::Create();
		// キー、パッド、キーガイド制御クラスの生成
		PadControl::Create();
		// オプション画面制御クラスの生成
		OptionWindowClass::Create();
		// オブジェクト制御クラスの生成
		Object2DManager::Create();
		// ポップアップ制御クラスの生成
		PopUp::Create();
		// UI画面制御クラスの生成
		WindowSystem::DrawControl::Create();
		// カメラシェイク機能制御クラスの生成
		CameraShake::Create();
		// ポストプロセス制御の生成
		PostPassEffect::Create();
		// シーン遷移制御の生成
		SceneControl::Create();
		// UI用の共通SEを読み込み
		auto* SoundParts = SoundPool::Instance();
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select), 2, "CommonData/Sound/UI/cursor.wav");
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_CANCEL), 1, "CommonData/Sound/UI/cancel.wav");
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_OK), 1, "CommonData/Sound/UI/ok.wav");
		SoundParts->Add(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_NG), 1, "CommonData/Sound/UI/ng.wav");
		auto* OptionWindowParts = OptionWindowClass::Instance();
		// オプション画面の初期化
		OptionWindowParts->Init();
	}
	// ロジック開始時に通る
	bool DXLib_ref::StartLogic(void) noexcept {
		// 初回であれば初回設定を開いて、終了次第再起動する
		if (m_IsFirstBoot) {
			auto* DrawParts = DXDraw::Instance();
			auto* PopUpParts = PopUp::Instance();
			auto* OptionParts = OPTION::Instance();
			// 初回設定画面の初期化
			InitFirstBootSetting();
			// ループ開始
			while (true) {
				// プロセスからのメッセージが届かない場合異常事態として強制終了
				if (!(ProcessMessage() == 0)) { break; }
				// 初回設定画面の更新
				UpdateFirstBootSetting();
				// 初回設定画面の描画
				DrawFirstBootSetting();
				// 表画面に反映し、垂直同期または一定のFPSまで待機する
				DrawParts->Screen_Flip();
				// ポップアップが閉じた場合ループを抜ける(終了)
				if (!PopUpParts->IsActivePop()) {
					break;
				}
			}
			// 終わったらセーブ
			OptionParts->Save();
			// 別プロセスとして自身を起動しておく
			StartMe();
			// 終了次第ゲームを終了する
			DrawParts->DXLibEnd();
			return false;
		}
		return true;
	}
	// メインループやシーン遷移管理を行う
	bool DXLib_ref::MainLogic(void) noexcept {
		auto* DrawParts = DXDraw::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// 一連のシーンループを開始
		while (true) {
			InitMainLoop();
			// メインループ開始
			while (true) {
				// プロセスからのメッセージが届かない場合異常事態としてすぐに強制終了
				if (!(ProcessMessage() == 0)) {
					m_IsEndScene = true;
					break;
				}
#if DEBUG
				// DXLIBのメッセージ表示をクリア
				clsDx();
				// デバッグの計測開始位置をここに指定
				DebugParts->SetStartPoint();
#endif // DEBUG
				// シーンの演算更新を行う処理
				UpdateMainLoop();
#if DEBUG
				// ポイントを追加
				DebugParts->SetPoint("-----DrawStart-----");
#endif // DEBUG
				// シーンの描画を行う処理
				DrawMainLoop();
#if DEBUG
				// デバッグ計測の終了点を設定
				DebugParts->SetEndPoint();
#endif // DEBUG
				// 表画面に反映し、垂直同期または一定のFPSまで待機する
				DrawParts->Screen_Flip();
				// シーン/ゲームの終了判定が立っているのでループを抜ける
				if (m_IsEndScene || m_IsEndGame) {
					break;
				}
				// 終了フラグは立っていないのでメインループの最初に戻る
			}
			// シーンの終わりに通る処理
			ExitMainLoop();
			// 終了フラグが立った場合即終了
			if (m_IsEndGame) {
				break;
			}
			// 終了フラグは立っていないのでシーンループの最初に戻る
		}
		// DXLIBを終了させる
		DrawParts->DXLibEnd();
		return true;
	}
};
