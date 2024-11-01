#include "DXLib_ref_101_sequence.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const SceneControl* SingletonBase<SceneControl>::m_Singleton = nullptr;

	// FPS表示
	void SceneControl::FPSDrawer::InitFPSCounter(void) noexcept {
		// 各々の数値を初期化
		for (auto& f : FPSAvgs) {
			f = FrameRate;
		}
		m_FPSAvgCount = 0;
	}
	void SceneControl::FPSDrawer::UpdateFPSCounter(void) noexcept {
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
	void SceneControl::FPSDrawer::DrawFPSCounter(void) const noexcept {
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
		UISystem::SetMsg(DrawParts->GetUIXMax() - DrawParts->GetUIY(8), DrawParts->GetUIY(8) + LineHeight / 2, LineHeight, UISystem::FontXCenter::RIGHT, color, Black, "%5.2f FPS", m_FPSAvg);
		// ドローコール(DirectXに何回描画指示を送ったか)の表示
		UISystem::SetMsg(DrawParts->GetUIXMax() - DrawParts->GetUIY(8), DrawParts->GetUIY(8 + 20) + LineHeight / 2, LineHeight, UISystem::FontXCenter::RIGHT, White, Black, "%d Drawcall", GetDrawCallCount());
	}
	// ポーズ画面
	void SceneControl::PauseDrawer::UpdatePause(void) noexcept {
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* Pad = PadControl::Instance();
		// ポーズ入力によるオンオフ
		if (!OptionWindowParts->IsActive() && Pad->GetPadsInfo(PADS::INVENTORY).GetKey().trigger()) {
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
	void SceneControl::PauseDrawer::DrawPause(void) const noexcept {
		// ポーズ画面に入っていない場合はスルーする
		if (!m_IsPauseActive) { return; }
		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		// 半透明の暗幕
		DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 128);
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, 0, DrawParts->GetUIXMax(), DrawParts->GetUIYMax(), Black, TRUE);
		DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
		// カウントが0.5秒以上であれば Pause の文字を表示
		if (m_PauseFlashCount > 0.5f) {
			UISystem::SetMsg(DrawParts->GetUIY(16), DrawParts->GetUIY(16) + DrawParts->GetUIY(36) / 2, DrawParts->GetUIY(36), UISystem::FontXCenter::LEFT, Green, Black, "Pause");
		}
	}
	void SceneControl::PauseDrawer::ChangePause(bool value) noexcept {
		if (m_IsPauseActive != value) {
			m_IsPauseActive = value;
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			KeyGuideParts->SetGuideUpdate();
		}
	}

	// シーンのループ開始前に行う処理
	void SceneControl::InitMainLoop(void) noexcept {
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		// シーンのロード
		this->m_NowScenesPtr->Load();
		// シーンの初回処理
		this->m_NowScenesPtr->Set();
		// キーガイドに設定しているものをリセット
		KeyGuideParts->Reset();
		// シーン開始時にキーガイド表示の更新を行うフラグを立てる
		KeyGuideParts->SetGuideUpdate();
		// FPS表示の初期化
		m_FPSDrawer.InitFPSCounter();
	}
	// シーンの演算更新を行う処理
	void SceneControl::UpdateMainLoop(void) noexcept {
		auto* Pad = PadControl::Instance();
		auto* PopUpParts = UISystem::PopUp::Instance();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* PostPassParts = PostPassEffect::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		// ループ初回の更新
		DrawParts->StartCount();
		// UI描画リストをクリア
		DrawCtrls->ClearList();
		// 終了キーを押し、尚且つ終了ウィンドウが出ていない場合
		if (Pad->GetPadsInfo(PADS::Escape).GetKey().trigger() && !m_IsExitSelect) {
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

						UISystem::SetMsg(xp1, yp1 + LineHeight / 2, LineHeight, UISystem::FontXCenter::LEFT, White, Black, "ゲームを終了しますか？");
					}
					// 終了するボタン
					{
						xp1 = (xmax + xmin) / 2 - DrawParts->GetUIY(54);
						yp1 = ymax - LineHeight * 3;

						auto* Pad = PadControl::Instance();
						bool ret = UISystem::SetMsgClickBox(xp1, yp1, xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2, LineHeight, Gray15, false, true, "終了");
						if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || ret) {
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

						UISystem::SetMsg(xp1, yp1 + LineHeight / 2, LineHeight, UISystem::FontXCenter::LEFT, White, Black, "ゲームを再起動しますか？");
					}
					// 終了するボタン
					{
						xp1 = (xmax + xmin) / 2 - DrawParts->GetUIY(54);
						yp1 = ymax - LineHeight * 3;

						auto* Pad = PadControl::Instance();
						bool ret = UISystem::SetMsgClickBox(xp1, yp1, xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2, LineHeight, Gray15, false, true, "再起動");
						if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || ret) {
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
		m_IsEndScene = !this->m_NowScenesPtr->Update();
		// オプション画面のアップデート
		OptionWindowParts->Update();
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
	void SceneControl::DrawMainLoop(void) const noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		auto* PopUpParts = UISystem::PopUp::Instance();
		auto* PostPassParts = PostPassEffect::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// ポストプロセス用のバッファー画面に反映
		PostPassParts->GetBufferScreen().SetDraw_Screen();
		{
			// ワールド空間の描画
			this->m_NowScenesPtr->MainDraw();
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
			this->m_NowScenesPtr->DrawUI_Base();
			// ポーズ描画を設定
			m_PauseDrawer.DrawPause();
			// FPS表示描画を設定
			m_FPSDrawer.DrawFPSCounter();
			// キーガイド描画を設定
			KeyGuideParts->Draw();
			// ポップアップ描画を画面中央をセンターとして設定
			PopUpParts->Draw(DrawParts->GetUIXMax() / 2, DrawParts->GetUIYMax() / 2);
			// ポーズ画面の前などに描画するUIの描画設定
			this->m_NowScenesPtr->DrawUI_In();
#if DEBUG
			// デバッグ画面描画を設定
			DebugParts->DebugWindow(DrawParts->GetUIXMax() - DrawParts->GetUIY(350), DrawParts->GetUIY(150));
#endif // DEBUG
			// 結果を描画
			DrawCtrls->Draw();
		}
	}
	// シーンの終わりに通る処理
	void SceneControl::ExitMainLoop(void) noexcept {
		this->m_NowScenesPtr->Dispose();										// 今のシーンからの解放
		if (this->m_NowScenesPtr != this->m_NowScenesPtr->Get_Next()) {	// 今のシーンと次のシーンとが別のシーンなら
			this->m_NowScenesPtr->Dispose_Load();								// ロードしていたデータを破棄
		}
		this->m_NowScenesPtr = this->m_NowScenesPtr->Get_Next();	// 次のシーンへ遷移
	}
};
