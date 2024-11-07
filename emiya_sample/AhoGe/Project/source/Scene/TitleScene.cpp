#include	"TitleScene.hpp"
#include	"../sub.hpp"

#include	"../Header.hpp"

#include	"../MainScene/Player/Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		void			TitleScene::Load_Sub(void) noexcept {
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			//�^�C�g���摜�̃��[�h
			this->m_TitleImage.Load("data/UI/Title.png");
			//�e��{�^���̗p��
			ButtonParts->ResetSel();
			ButtonParts->AddStringButton("Let's Go!", 52, true, BaseScreenWidth - 64 - 48, BaseScreenHeight - 84 - 64 * 2, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Reset SaveData", 48, true, BaseScreenWidth - 64, BaseScreenHeight - 84 - 64 * 1, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddIconButton("data/UI/setting.png", true, BaseScreenWidth - 64, 64, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE);
			// BGM�̃��[�h
			SoundParts->Add(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Title), 1, "data/Sound/BGM/title.wav");
		}
		void			TitleScene::Dispose_Load_Sub(void) noexcept {
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			//�^�C�g���摜�̔j��
			this->m_TitleImage.Dispose();
			//�e��{�^���̔j��
			ButtonParts->Dispose();
			// BGM�̔j��
			SoundParts->Delete(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Title));
		}
		//
		void			TitleScene::Initialize_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			//�t�F�[�h�C������ƍŏ��Ɍ��肵�Ă���
			FadeInParts->SetFadeIn();
			//�Q�[���J�n�t���O��܂�
			m_IsStartGame = false;
			//SE�̍Đ�
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
			//�|�[�Y��ʒ��͈ȉ��������X���[����
			if (SceneParts->IsPause()) {
				return true;
			}
			//�}�E�X�̕\�����펞�L����
			Pad->SetMouseMoveEnable(false);
			//�K�C�h�ɕ\������L�[�Ɛ�����u���Ă���(�K�C�h�̍X�V���ނ�Pad���Ńt���O���������ۂɎq�̊֐��ōs���܂�)
			KeyGuideParts->ChangeGuide(
				[]() {
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* Pad = PadControl::Instance();
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_A).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_D).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "�I��");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "����");
				}
			);
			//���炩�̃|�b�v�A�b�v���o�Ă��邩�t�F�[�h���łȂ��ꍇ
			if (!PopUpParts->IsActivePop() && FadeInParts->IsFadeClear()) {
				// �{�^�����͂��`�F�b�N
				ButtonParts->UpdateInput();
				// �ǂꂩ�̃{�^���������ꂽ���̋���
				if (ButtonParts->GetTriggerButton()) {
					switch (ButtonParts->GetSelect()) {
					case 0://�Q�[���J�n
						SetNextSelect(0);						// ���V�[������
						FadeInParts->SetFadeOut();				// �t�F�[�h�A�E�g
						m_IsStartGame = true;					//�Q�[���J�n�t���O�𗧂Ă�
						break;
					case 1://�Z�[�u�f�[�^�̃��Z�b�g(���̃Q�[���̓I�[�g�Z�[�u���s�����ߐV�K�ɗV�ԏꍇ�Z�[�u�f�[�^�̍폜���K�v�ȔF���ł�)
						PopUpParts->Add("Reset Save", 480, 240,
							[&](int xmin, int ymin, int xmax, int ymax, bool) {
								auto* Pad = PadControl::Instance();
								auto* SceneParts = SceneControl::Instance();
								auto* SaveDataParts = SaveDataClass::Instance();
								auto* PopUpParts = UISystem::PopUp::Instance();
								auto* DrawCtrls = UISystem::DrawControl::Instance();
								// �|�b�v�A�b�v�ɕ\��������^�C�g��
								{
									//�w����W�ɑ΂������������ŕ�����\��
									DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
										LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
										xmin + 24, ymin + LineHeight,
										White, Black, "�Z�[�u�f�[�^���폜���܂����H");
								}
								// �|�b�v�A�b�v�ɕ\��������m��{�^��
								{
									//�|�b�v�A�b�v���������t�߂ɃN���b�N�ł���{�^����\��
									int xpp1 = (xmax + xmin) / 2 - 54;
									int ypp1 = ymax - LineHeight * 3;
									int xpp2 = (xmax + xmin) / 2 + 54;
									int ypp2 = ymax - LineHeight * 3 + LineHeight * 2;
									int StringYSizeMax = LineHeight;
									unsigned int defaultcolor = Gray15;
									bool IsRepeat =false;
									bool IsActive = true;
									const std::string& String = "�I��";

									bool MouseOver = IsActive && UISystem::GetMouseOver(xpp1, ypp1, xpp2, ypp2);
									DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
										xpp1, ypp1, xpp2, ypp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : defaultcolor, true);
									bool ret = (MouseOver && (IsRepeat ? Pad->GetMouseClick().repeat() : Pad->GetMouseClick().trigger()));

									DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
										StringYSizeMax, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
										(xpp1 + xpp2) / 2, (ypp1 + ypp2) / 2, White, Black, String);

									//�C���^���N�g�{�^������L�N���b�N�������ꂽ�ꍇ
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
					case 2://�I�v�V�������J��
						OptionWindowParts->SetActive();
						break;
					default:
						break;
					}
					//��������̑I�������Ă���̂őI��SE��炵�܂�
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
			}
			// �{�^���\������̍X�V
			ButtonParts->Update();
			//�t�F�[�h�C������̍X�V
			FadeInParts->Update();
			//�Q�[���J�n�t���O�������A�����t�F�[�h�A�E�g�����������i�K�ŃV�[���I��
			return (m_IsStartGame) ? !FadeInParts->IsFadeAll() : true;
		}
		void			TitleScene::Dispose_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			//BGM�̒�~
			SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Title))->StopAll();
		}
		// 
		void			TitleScene::DrawMain_Sub(void) const noexcept {
			auto* DrawParts = WindowSizeControl::Instance();
			// �^�C�g���w���2D�����͎g�p���Ȃ��̂ŃO���[�œh��Ԃ��Ă���
			DrawBox(0, 0, DrawParts->GetScreenY(BaseScreenWidth), DrawParts->GetScreenY(BaseScreenHeight), Gray75, TRUE);
		}
		void			TitleScene::DrawUI_Base_Sub(void) const noexcept {
			auto* PopUpParts = UISystem::PopUp::Instance();
			auto* ButtonParts = UI::ButtonControl::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			// �^�C�g���摜�̕`���DrawControl�Ɏw��
			DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal, &this->m_TitleImage,
				64, 64, 64 + 369, 64 + 207, true);
			// �o�[�W�����\���̕`���DrawControl�Ɏw��
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
				64 + 369, 64 + 207, White, Black, "Ver 1.0.0");
			// �{�^���\���̕`���DrawControl�Ɏw��
			ButtonParts->Draw();
			// �{�^���K�C�h�̕`���DrawControl�Ɏw��
			if ((ButtonParts->GetSelect() != InvalidID) && !PopUpParts->IsActivePop()) {//�{�^���̑I��悪�L���Ȓl�ɂȂ��Ă��āA���|�b�v�A�b�v�̕\�������Ă��Ȃ���
				//�e�{�^���ɉ������e�L�X�g���w��
				std::string Text = "";
				switch (ButtonParts->GetSelect()) {
				case 0:Text = "�o�q�I"; break;
				case 1:Text = "�Z�[�u�f�[�^�����Z�b�g���܂��B"; break;
				case 2:Text = "�I�v�V�������J���܂�"; break;
				case 3:Text = "�N���W�b�g���J���܂�"; break;
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
