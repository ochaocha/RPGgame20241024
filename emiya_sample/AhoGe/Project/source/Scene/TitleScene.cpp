#include	"TitleScene.hpp"
#include	"../sub.hpp"

#include	"../Header.hpp"

#include	"../MainScene/Player/Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		void			TitleScene::Load_Sub(void) noexcept {
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			//タイトル画像のロード
			this->m_TitleImage.Load("data/UI/Title.png");
			//各種ボタンの用意
			ButtonParts->ResetSel();
			ButtonParts->AddStringButton("Let's Go!", 52, true, BaseScreenWidth - 64 - 48, BaseScreenHeight - 84 - 64 * 2, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Reset SaveData", 48, true, BaseScreenWidth - 64, BaseScreenHeight - 84 - 64 * 1, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddIconButton("data/UI/setting.png", true, BaseScreenWidth - 64, 64, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE);
			// BGMのロード
			SoundParts->Add(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Title), 1, "data/Sound/BGM/title.wav");
		}
		void			TitleScene::Dispose_Load_Sub(void) noexcept {
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			//タイトル画像の破棄
			this->m_TitleImage.Dispose();
			//各種ボタンの破棄
			ButtonParts->Dispose();
			// BGMの破棄
			SoundParts->Delete(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Title));
		}
		//
		void			TitleScene::Initialize_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			//フェードインすると最初に決定しておく
			FadeInParts->SetFadeIn();
			//ゲーム開始フラグを折る
			m_IsStartGame = false;
			//SEの再生
			SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Title))->Play(DX_PLAYTYPE_LOOP, TRUE);
		}
		bool			TitleScene::Update_Sub(void) noexcept {
			auto* Pad = PadControl::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* PopUpParts = UISystem::PopUp::Instance();
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* SceneParts = SceneControl::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			auto* OptionWindowParts = OptionWindowClass::Instance();
			//ポーズ画面中は以下処理をスルーする
			if (SceneParts->IsPause()) {
				return true;
			}
			//マウスの表示を常時有効化
			Pad->SetMouseMoveEnable(false);
			//ガイドに表示するキーと説明を置いておく(ガイドの更新辞退はPad側でフラグが立った際に子の関数で行われます)
			KeyGuideParts->ChangeGuide(
				[]() {
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* Pad = PadControl::Instance();
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_A).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_D).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "選択");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "決定");
				}
			);
			//何らかのポップアップが出ているかフェード中でない場合
			if (!PopUpParts->IsActivePop() && FadeInParts->IsFadeClear()) {
				// ボタン入力をチェック
				ButtonParts->UpdateInput();
				// どれかのボタンが押された時の挙動
				if (ButtonParts->GetTriggerButton()) {
					switch (ButtonParts->GetSelect()) {
					case 0://ゲーム開始
						SetNextSelect(0);						// 次シーン決定
						FadeInParts->SetFadeOut();				// フェードアウト
						m_IsStartGame = true;					//ゲーム開始フラグを立てる
						break;
					case 1://セーブデータのリセット(このゲームはオートセーブを行うため新規に遊ぶ場合セーブデータの削除が必要な認識です)
						PopUpParts->Add("Reset Save", 480, 240,
							[&](int xmin, int ymin, int xmax, int ymax, bool) {
								auto* Pad = PadControl::Instance();
								auto* SceneParts = SceneControl::Instance();
								auto* SaveDataParts = SaveDataClass::Instance();
								auto* PopUpParts = UISystem::PopUp::Instance();
								auto* DrawCtrls = UISystem::DrawControl::Instance();
								// ポップアップに表示させるタイトル
								{
									//指定座標に対し左中央揃えで文字を表示
									DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
										LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
										xmin + 24, ymin + LineHeight,
										White, Black, "セーブデータを削除しますか？");
								}
								// ポップアップに表示させる確定ボタン
								{
									//ポップアップ下限中央付近にクリックできるボタンを表示
									int xpp1 = (xmax + xmin) / 2 - 54;
									int ypp1 = ymax - LineHeight * 3;
									int xpp2 = (xmax + xmin) / 2 + 54;
									int ypp2 = ymax - LineHeight * 3 + LineHeight * 2;
									int StringYSizeMax = LineHeight;
									unsigned int defaultcolor = Gray15;
									bool IsRepeat =false;
									bool IsActive = true;
									const std::string& String = "終了";

									bool MouseOver = IsActive && UISystem::GetMouseOver(xpp1, ypp1, xpp2, ypp2);
									DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
										xpp1, ypp1, xpp2, ypp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : defaultcolor, true);
									bool ret = (MouseOver && (IsRepeat ? Pad->GetMouseClick().repeat() : Pad->GetMouseClick().trigger()));

									DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
										StringYSizeMax, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
										(xpp1 + xpp2) / 2, (ypp1 + ypp2) / 2, White, Black, String);

									//インタラクトボタンか上記クリックが押された場合
									if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || ret) {
										SaveDataParts->Dispose();
										SaveDataParts->Save();
										SceneParts->ChangePause(false);
										PopUpParts->EndAll();
									}
								}
							},
							[&]() {},
							[&]() {},
							true
						);
						break;
					case 2://オプションを開く
						OptionWindowParts->SetActive();
						break;
					default:
						break;
					}
					//何かしらの選択をしているので選択SEを鳴らします
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
			}
			// ボタン表示周りの更新
			ButtonParts->Update();
			//フェードイン周りの更新
			FadeInParts->Update();
			//ゲーム開始フラグが立ち、尚且つフェードアウトが完了した段階でシーン終了
			return (m_IsStartGame) ? !FadeInParts->IsFadeAll() : true;
		}
		void			TitleScene::Dispose_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			//BGMの停止
			SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Title))->StopAll();
		}
		// 
		void			TitleScene::DrawMain_Sub(void) const noexcept {
			auto* DrawParts = WindowSizeControl::Instance();
			// タイトル背後の2D部分は使用しないのでグレーで塗りつぶしておく
			DrawBox(0, 0, DrawParts->GetScreenY(BaseScreenWidth), DrawParts->GetScreenY(BaseScreenHeight), Gray75, TRUE);
		}
		void			TitleScene::DrawUI_Base_Sub(void) const noexcept {
			auto* PopUpParts = UISystem::PopUp::Instance();
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			// タイトル画像の描画をDrawControlに指示
			DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal, &this->m_TitleImage,
				64, 64, 64 + 369, 64 + 207, true);
			// バージョン表示の描画をDrawControlに指示
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
				64 + 369, 64 + 207, White, Black, "Ver 1.0.0");
			// ボタン表示の描画をDrawControlに指示
			ButtonParts->Draw();
			// ボタンガイドの描画をDrawControlに指示
			if ((ButtonParts->GetSelect() != InvalidID) && !PopUpParts->IsActivePop()) {//ボタンの選択先が有効な値になっていて、かつポップアップの表示がついていないか
				//各ボタンに応じたテキストを指定
				std::string Text = "";
				switch (ButtonParts->GetSelect()) {
				case 0:Text = "出航！"; break;
				case 1:Text = "セーブデータをリセットします。"; break;
				case 2:Text = "オプションを開きます"; break;
				case 3:Text = "クレジットを開きます"; break;
				}
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::BOTTOM,
					32, BaseScreenHeight - 32 - 32, White, Black, Text.c_str());
			}
			// 
			FadeInParts->DrawFade();
		}
	}
}
