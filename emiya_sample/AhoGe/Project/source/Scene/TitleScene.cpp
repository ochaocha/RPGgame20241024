#include	"TitleScene.hpp"
#include	"../sub.hpp"

#include	"../Header.hpp"

#include	"../MainScene/Player/Player.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		void			TitleScene::Set_Sub(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			// 
			m_FadeControl.SetFadeIn();
			this->m_TitleImage.Load("data/UI/Title.png");
			// 
			ButtonParts->ResetSel();
			ButtonParts->AddStringButton("Let's Go!", 52, true, 1920 - 64 - 48, 1080 - 84 - 64 * 2, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Reset SaveData", 48, true, 1920 - 64, 1080 - 84 - 64 * 1, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddIconButton("data/UI/setting.png", true, 1920 - 64, 64, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE);
			// BGM
			SoundParts->Add(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Title), 1, "data/Sound/BGM/title.wav");
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Title))->Play(DX_PLAYTYPE_LOOP, TRUE);
			//
			m_IsStartGame = false;
		}
		bool			TitleScene::Update_Sub(void) noexcept {
			auto* Pad = PadControl::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* SoundParts = SoundPool::Instance();
			auto* PopUpParts = UISystem::PopUp::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			auto* SceneParts = SceneControl::Instance();
			if (SceneParts->IsPause()) {
				return true;
			}
			Pad->SetMouseMoveEnable(false);
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
			if (!PopUpParts->IsActivePop() && m_FadeControl.IsFadeClear()) {
				ButtonParts->UpdateInput();
				// 選択時の挙動
				if (ButtonParts->GetTriggerButton()) {
					switch (ButtonParts->GetSelect()) {
					case 0:
						SetNextSelect(0);						// 次シーン決定
						m_FadeControl.SetFadeOut();				// フェードアウト
						m_IsStartGame = true;
						break;
					case 1:
						PopUpParts->Add("Reset Save", DrawParts->GetUIY(480), DrawParts->GetUIY(240),
							[&](int xmin, int ymin, int xmax, int ymax, bool) {
								auto* Pad = PadControl::Instance();
								auto* DrawParts = DXDraw::Instance();
								auto* SceneParts = SceneControl::Instance();
								auto* SaveDataParts = SaveDataClass::Instance();
								auto* PopUpParts = UISystem::PopUp::Instance();
								int xp1, yp1;
								// タイトル
								{
									xp1 = xmin + DrawParts->GetUIY(24);
									yp1 = ymin + LineHeight;

									UISystem::SetMsg(xp1, yp1 + LineHeight / 2, LineHeight, UISystem::FontXCenter::LEFT, White, Black, "セーブデータを削除しますか？");
								}
								// 
								{
									xp1 = (xmax + xmin) / 2 - DrawParts->GetUIY(54);
									yp1 = ymax - LineHeight * 3;

									bool ret = UISystem::SetMsgClickBox(xp1, yp1, xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2, LineHeight, Gray15, false, true, "終了");
									if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || ret) {
										SaveDataParts->Reset();
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
					case 2:
						OptionWindowClass::Instance()->SetActive();
						break;
					default:
						break;
					}
					SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
			}
			// 
			ButtonParts->Update();
			m_FadeControl.UpdateFade();
			// 
			return (m_IsStartGame) ? m_FadeControl.IsFadeAll() : true;
		}
		void			TitleScene::Dispose_Sub(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			// 
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Title))->StopAll();
			SoundParts->Delete(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Title));
			// 
			ButtonParts->Dispose();
			// 
			this->m_TitleImage.Dispose();
		}
		// 
		void			TitleScene::MainDraw_Sub(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			// 背景
			DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), Gray75, TRUE);
			// 描画
		}
		void			TitleScene::DrawUI_Base_Sub(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* PopUpParts = UISystem::PopUp::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			// タイトル画像
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal,&this->m_TitleImage,
				DrawParts->GetUIY(64), DrawParts->GetUIY(64), DrawParts->GetUIY(64 + 369), DrawParts->GetUIY(64 + 207), true);
			// 
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(18), UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
				DrawParts->GetUIY(64 + 369), DrawParts->GetUIY(64 + 207), White, Black, "Ver 1.0.1");
			// 
			ButtonParts->Draw();
			// 
			if ((ButtonParts->GetSelect() != InvalidID) && !PopUpParts->IsActivePop()) {
				std::string Text = "";
				switch (ButtonParts->GetSelect()) {
				case 0:Text = "出航！"; break;
				case 1:Text = "セーブデータをリセットします。"; break;
				case 2:Text = "オプションを開きます"; break;
				case 3:Text = "クレジットを開きます"; break;
				}
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(18), UISystem::FontXCenter::LEFT, UISystem::FontYCenter::BOTTOM,
					DrawParts->GetUIY(32), DrawParts->GetUIY(1080 - 32 - 32), White, Black, Text.c_str());
			}
			//
			m_FadeControl.DrawFade();
		}
	};
};
