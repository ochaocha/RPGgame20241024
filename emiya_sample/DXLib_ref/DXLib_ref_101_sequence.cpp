#include "DXLib_ref_101_sequence.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const SceneControl* SingletonBase<SceneControl>::m_Singleton = nullptr;

	// FPS表示
	void SceneControl::FPSDrawer::Initialize(void) noexcept {
		// 各々の数値を初期化
		for (auto& f : FPSAvgs) {
			f = FrameRate;
		}
		m_FPSAvgCount = 0;
	}
	void SceneControl::FPSDrawer::Update(void) noexcept {
		// m_FPSAvgCountの番号に対して今のフレームレートを保存
		FPSAvgs.at(m_FPSAvgCount) = DXLib_ref::Instance()->GetFps();
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
		auto* DrawCtrls = UISystem::DrawControl::Instance();
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
		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
			BaseScreenWidth - 8, 8, color, Black, "%5.2f FPS", m_FPSAvg);
		// ドローコール(DirectXに何回描画指示を送ったか)の表示
		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
			BaseScreenWidth - 8, 8 + 20, White, Black, "%d Drawcall", GetDrawCallCount());
	}
	// ポーズ画面
	void SceneControl::PauseDrawer::Update(void) noexcept {
		// ポーズ画面では点滅の演算を行う
		m_PauseFlashCount += DXLib_ref::Instance()->GetDeltaTime();
		// 1秒経ったら0秒にリセットする
		if (m_PauseFlashCount > 1.f) {
			m_PauseFlashCount -= 1.f;
		}
	}
	void SceneControl::PauseDrawer::DrawPause(void) const noexcept {
		// ポーズ画面に入っていない場合はスルーする
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		// 半透明の暗幕
		DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 128);
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, 0, BaseScreenWidth, BaseScreenHeight, Black, TRUE);
		DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
		// カウントが0.5秒以上であれば Pause の文字を表示
		if (m_PauseFlashCount > 0.5f) {
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				36, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
				16, 16, Green, Black, "Pause");
		}
	}

	void SceneControl::ChangePause(bool value) noexcept {
		auto* PopUpParts = UISystem::PopUp::Instance();
		if (m_IsPauseActive != value) {
			m_IsPauseActive = value;
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			KeyGuideParts->SetGuideFlip();
		}
		//ポップアップをすべて削除とする
		PopUpParts->EndAll();
	}

	// シーンのループ開始前に行う処理
	void SceneControl::Initialize(void) noexcept {
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		// シーンのロード
		this->m_NowScenesPtr->Load();
		// シーンの初回処理
		this->m_NowScenesPtr->Initialize();
		// キーガイドに設定しているものをリセット
		KeyGuideParts->Dispose();
		// シーン開始時にキーガイド表示の更新を行うフラグを立てる
		KeyGuideParts->SetGuideFlip();
		// FPS表示の初期化
		m_FPSDrawer.Initialize();
	}
	// シーンの演算更新を行う処理
	void SceneControl::Update(void) noexcept {
		auto* Pad = PadControl::Instance();
		auto* PopUpParts = UISystem::PopUp::Instance();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* PostPassParts = PostPass::PostPassEffect::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
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
					auto* DrawCtrls = UISystem::DrawControl::Instance();
					auto* Pad = PadControl::Instance();
					// タイトル
					{
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
							xmin + 24, ymin + LineHeight,White, Black, "ゲームを終了しますか？");
					}
					// 終了するボタン
					{
						int xp1 = (xmax + xmin) / 2 - 54;
						int yp1 = ymax - LineHeight * 3;
						int xp2 = (xmax + xmin) / 2 + 54;
						int yp2 = ymax - LineHeight * 3 + LineHeight * 2;

						bool MouseOver = UISystem::GetMouseOver(xp1, yp1, xp2, yp2);
						DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
							xp1, yp1, xp2, yp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : Gray15, true);
						bool ret = (MouseOver && (Pad->GetMouseClick().trigger()));

						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
							(xp1 + xp2) / 2, (yp1 + yp2) / 2, White, Black, "終了");

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
					auto* DrawCtrls = UISystem::DrawControl::Instance();
					auto* Pad = PadControl::Instance();
					// タイトル
					{
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
							xmin + 24, ymin + LineHeight, White, Black, "ゲームを再起動しますか？");
					}
					// 終了するボタン
					{
						int xp1 = (xmax + xmin) / 2 - 54;
						int yp1 = ymax - LineHeight * 3;
						int xp2 = (xmax + xmin) / 2 + 54;
						int yp2 = ymax - LineHeight * 3 + LineHeight * 2;

						bool MouseOver = UISystem::GetMouseOver(xp1, yp1, xp2, yp2);
						DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
							xp1, yp1, xp2, yp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : Gray15, true);
						bool ret = (MouseOver && (Pad->GetMouseClick().trigger()));

						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
							(xp1 + xp2) / 2, (yp1 + yp2) / 2, White, Black, "再起動");

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
		if (IsPause()) {
			m_PauseDrawer.Update();
		}
		// ポーズ入力によるオンオフ
		if (Pad->GetPadsInfo(PADS::INVENTORY).GetKey().trigger()) {
			ChangePause(!IsPause());
		}
		// FPS表示機能の更新
		m_FPSDrawer.Update();
	}
	// シーンの描画を行う処理
	void SceneControl::DrawMainLoop(void) const noexcept {
		auto* WindowSizeParts = WindowSizeControl::Instance();
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		auto* PopUpParts = UISystem::PopUp::Instance();
		auto* PostPassParts = PostPass::PostPassEffect::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// ポストプロセス用のバッファー画面に反映
		PostPassParts->GetBufferScreen().SetDraw_Screen();
		{
			// ワールド空間の描画
			this->m_NowScenesPtr->DrawMain();
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
			PostPassParts->GetBufferScreen().DrawExtendGraph(0, 0, WindowSizeParts->GetUIY(BaseScreenWidth), WindowSizeParts->GetUIY(BaseScreenHeight), false);
			SetDrawMode(Prev);

			// UI描画設定
			this->m_NowScenesPtr->DrawUI_Base();
			// ポーズ描画を設定
			if (IsPause()) {
				m_PauseDrawer.DrawPause();
			}
			// FPS表示描画を設定
			m_FPSDrawer.DrawFPSCounter();
			// キーガイド描画を設定
			KeyGuideParts->Draw();
			// ポップアップ描画を画面中央をセンターとして設定
			PopUpParts->Draw(BaseScreenWidth / 2, BaseScreenHeight / 2);
			// ポーズ画面の前などに描画するUIの描画設定
			this->m_NowScenesPtr->DrawUI_In();
#if DEBUG
			// デバッグ画面描画を設定
			DebugParts->DebugWindow(BaseScreenWidth - 350, 150);
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
