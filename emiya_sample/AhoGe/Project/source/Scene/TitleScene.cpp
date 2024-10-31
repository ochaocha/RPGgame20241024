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
			this->m_GameFadeIn = 1.f;
			this->m_GameStart = 0.f;
			this->m_TitleImage.Load("data/UI/Title.png");
			// 
			ButtonParts->ResetSel();
			// 
			ButtonParts->AddStringButton(
				"Let's Go!", 52, true,
				1920 - 64 - 48, 1080 - 84 - 64 * 2, FontSystem::FontXCenter::RIGHT, FontSystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton(
				"Reset SaveData", 48, true,
				1920 - 64, 1080 - 84 - 64 * 1, FontSystem::FontXCenter::RIGHT, FontSystem::FontYCenter::BOTTOM);
			ButtonParts->AddIconButton(
				"data/UI/setting.png", true,
				1920 - 96 - 64, 64, FontSystem::FontXCenter::MIDDLE, FontSystem::FontYCenter::MIDDLE);
			ButtonParts->AddIconButton(
				"data/UI/credit.png", true,
				1920 - 64, 64, FontSystem::FontXCenter::MIDDLE, FontSystem::FontYCenter::MIDDLE);
			// クレジット
			this->m_CreditControl = std::make_unique<CreditControl>();
			this->m_CreditControl->Init();
			// 
			// BGM
			SoundParts->Add(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Title), 1, "data/Sound/BGM/title.wav");
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Title))->Play(DX_PLAYTYPE_LOOP, TRUE);
			//
			m_CloseResetSave = false;
		}
		bool			TitleScene::Update_Sub(void) noexcept {
			auto* Pad = PadControl::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* SoundParts = SoundPool::Instance();
			auto* PopUpParts = PopUp::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			auto* DXLib_refParts = DXLib_ref::Instance();
			if (DXLib_refParts->IsPause()) {
				return true;
			}
			Pad->SetMouseMoveEnable(false);
			Pad->ChangeGuide(
				[]() {
					auto* Pad = PadControl::Instance();
					Pad->AddGuide(PADS::MOVE_W, "");
					Pad->AddGuide(PADS::MOVE_S, "");
					Pad->AddGuide(PADS::MOVE_A, "");
					Pad->AddGuide(PADS::MOVE_D, "");
					Pad->AddGuide(PADS::MOVE_STICK, "選択");
					Pad->AddGuide(PADS::INTERACT, "決定");
				}
			);
			if (!PopUpParts->IsActivePop() && (this->m_GameFadeIn == 0.f) && (this->m_GameStart == 0.f)) {
				ButtonParts->UpdateInput();
				// 選択時の挙動
				if (ButtonParts->GetTriggerButton()) {
					switch (ButtonParts->GetSelect()) {
					case 0:
						this->m_GameStart += 0.0001f;
						break;
					case 1:
						PopUpParts->Add("Reset Save", DrawParts->GetUIY(480), DrawParts->GetUIY(240),
							[&](int xmin, int ymin, int xmax, int ymax, bool) {
								auto* DrawParts = DXDraw::Instance();
								auto* DXLib_refParts = DXLib_ref::Instance();
								int xp1, yp1;
								// タイトル
								{
									xp1 = xmin + DrawParts->GetUIY(24);
									yp1 = ymin + LineHeight;

									WindowSystem::SetMsg(xp1, yp1 + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, "セーブデータを削除しますか？");
								}
								// 
								{
									xp1 = (xmax + xmin) / 2 - DrawParts->GetUIY(54);
									yp1 = ymax - LineHeight * 3;

									auto* Pad = PadControl::Instance();
									bool ret = WindowSystem::SetMsgClickBox(xp1, yp1, xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2, LineHeight, Gray15, false, true, "終了");
									if (Pad->GetKey(PADS::INTERACT).trigger() || ret) {
										auto* SaveDataParts = SaveDataClass::Instance();
										SaveDataParts->Reset();
										SaveDataParts->Save();
										DXLib_refParts->ChangePause(false);
										m_CloseResetSave = true;
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
					case 3:
						PopUpParts->Add("Credit", DrawParts->GetUIY(720), DrawParts->GetUIY(840),
							[&](int xmin, int ymin, int xmax, int, bool) {
								this->m_CreditControl->Draw(xmin, ymin, xmax);
							},
							[]() {},
							[]() {},
							true
						);
						break;
					default:
						break;
					}
					SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if (m_CloseResetSave && PopUpParts->IsActivePop()) {
				m_CloseResetSave = false;
				PopUpParts->EndAll();
			}
			// 
			ButtonParts->Update();
			// 
			this->m_GameFadeIn = std::max(this->m_GameFadeIn - DrawParts->GetDeltaTime() / 0.5f, 0.f);
			if (this->m_GameStart != 0.f) { this->m_GameStart += DrawParts->GetDeltaTime() / 0.5f; }
			return (this->m_GameStart <= 1.f);
		}
		void			TitleScene::Dispose_Sub(void) noexcept {
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* SoundParts = SoundPool::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			// 
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Title))->StopAll();
			SoundParts->Delete(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Title));
			// 
			this->m_CreditControl->Dispose();
			this->m_CreditControl.reset();
			// 
			ButtonParts->Dispose();
			// 
			this->m_TitleImage.Dispose();
			// セーブ
			SaveDataParts->Save();
			// 次シーン決定
			SetNextSelect(ButtonParts->GetSelect());
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
			auto* Fonts = FontSystem::FontPool::Instance();
			auto* PopUpParts = PopUp::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			// 
			this->m_TitleImage.DrawExtendGraph(DrawParts->GetUIY(64), DrawParts->GetUIY(64), DrawParts->GetUIY(64 + 369), DrawParts->GetUIY(64 + 207), true);
			// 
			Fonts->Get(FontSystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(18), 3)->DrawString(InvalidID, FontSystem::FontXCenter::RIGHT, FontSystem::FontYCenter::TOP,
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
				Fonts->Get(FontSystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(18), 3)->DrawString(InvalidID, FontSystem::FontXCenter::LEFT, FontSystem::FontYCenter::BOTTOM,
					DrawParts->GetUIY(32), DrawParts->GetUIY(1080 - 32 - 32), White, Black, Text.c_str());
			}
			// 
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * std::clamp(this->m_GameFadeIn, 0.f, 1.f)), 0, 255));
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), Black, TRUE);

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * std::clamp(this->m_GameStart, 0.f, 1.f)), 0, 255));
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, TRUE);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	};
};
