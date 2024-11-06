#include "DXLib_ref_101_sequence.hpp"

namespace DXLibRef {
	// �V���O���g���̎��Ԓ�`
	const SceneControl* SingletonBase<SceneControl>::m_Singleton = nullptr;

	// FPS�\��
	void SceneControl::FPSDrawer::Initialize(void) noexcept {
		// �e�X�̐��l��������
		for (auto& f : FPSAvgs) {
			f = FrameRate;
		}
		m_FPSAvgCount = 0;
	}
	void SceneControl::FPSDrawer::Update(void) noexcept {
		// m_FPSAvgCount�̔ԍ��ɑ΂��č��̃t���[�����[�g��ۑ�
		FPSAvgs.at(m_FPSAvgCount) = DXLib_ref::Instance()->GetFps();
		// �ۑ�����ꏊ�����炷
		++m_FPSAvgCount %= FPSAvgs.size();
		// �ۑ����Ă���ߋ���FPS�l�̕��ς��Ƃ�
		m_FPSAvg = 0.f;
		for (auto& f : FPSAvgs) {
			m_FPSAvg += f;
		}
		m_FPSAvg = m_FPSAvg / static_cast<float>(FPSAvgs.size());
	}
	void SceneControl::FPSDrawer::DrawFPSCounter(void) const noexcept {
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		auto* OptionParts = OPTION::Instance();
		// FPS�̕��ϒl���ݒ肵�Ă�������l�ɑ΂��č����Ȃ�΁A�Ⴂ�Ȃ物�F�ԐF�ƕω�������
		auto color = White;
		if (m_FPSAvg > static_cast<float>(OptionParts->GetParamInt(EnumSaveParam::FpsLimit) - 2)) {
			color = Green;// �\����FPS���o�Ă���
		}
		else if (m_FPSAvg > static_cast<float>(OptionParts->GetParamInt(EnumSaveParam::FpsLimit) - 10)) {
			color = Yellow;// �\����FPS���o�Ă��Ȃ�
		}
		else {
			color = Red;// �܂�����FPS���o�Ă��Ȃ�
		}
		// FPS�l�̕\��
		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
			BaseScreenWidth - 8, 8, color, Black, "%5.2f FPS", m_FPSAvg);
		// �h���[�R�[��(DirectX�ɉ���`��w���𑗂�����)�̕\��
		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
			BaseScreenWidth - 8, 8 + 20, White, Black, "%d Drawcall", GetDrawCallCount());
	}
	// �|�[�Y���
	void SceneControl::PauseDrawer::Update(void) noexcept {
		// �|�[�Y��ʂł͓_�ł̉��Z���s��
		m_PauseFlashCount += DXLib_ref::Instance()->GetDeltaTime();
		// 1�b�o������0�b�Ƀ��Z�b�g����
		if (m_PauseFlashCount > 1.f) {
			m_PauseFlashCount -= 1.f;
		}
	}
	void SceneControl::PauseDrawer::DrawPause(void) const noexcept {
		// �|�[�Y��ʂɓ����Ă��Ȃ��ꍇ�̓X���[����
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		// �������̈Ö�
		DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 128);
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, 0, BaseScreenWidth, BaseScreenHeight, Black, TRUE);
		DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
		// �J�E���g��0.5�b�ȏ�ł���� Pause �̕�����\��
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
		//�|�b�v�A�b�v�����ׂč폜�Ƃ���
		PopUpParts->EndAll();
	}

	// �V�[���̃��[�v�J�n�O�ɍs������
	void SceneControl::Initialize(void) noexcept {
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		// �V�[���̃��[�h
		this->m_NowScenesPtr->Load();
		// �V�[���̏��񏈗�
		this->m_NowScenesPtr->Initialize();
		// �L�[�K�C�h�ɐݒ肵�Ă�����̂����Z�b�g
		KeyGuideParts->Dispose();
		// �V�[���J�n���ɃL�[�K�C�h�\���̍X�V���s���t���O�𗧂Ă�
		KeyGuideParts->SetGuideFlip();
		// FPS�\���̏�����
		m_FPSDrawer.Initialize();
	}
	// �V�[���̉��Z�X�V���s������
	void SceneControl::Update(void) noexcept {
		auto* Pad = PadControl::Instance();
		auto* PopUpParts = UISystem::PopUp::Instance();
		auto* OptionWindowParts = OptionWindowClass::Instance();
		auto* PostPassParts = PostPass::PostPassEffect::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		// UI�`�惊�X�g���N���A
		DrawCtrls->ClearList();
		// �I���L�[�������A�����I���E�B���h�E���o�Ă��Ȃ��ꍇ
		if (Pad->GetPadsInfo(PADS::Escape).GetKey().trigger() && !m_IsExitSelect) {
			// �I���E�B���h�E�t���O�𗧂Ă�
			m_IsExitSelect = true;
			// �I���|�b�v�A�b�v��\��
			PopUpParts->Add("Exit", 480, 240,
				[this](int xmin, int ymin, int xmax, int ymax, bool) {
					// �|�b�v�A�b�v���ŕ`�悷�����
					auto* DrawCtrls = UISystem::DrawControl::Instance();
					auto* Pad = PadControl::Instance();
					// �^�C�g��
					{
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
							xmin + 24, ymin + LineHeight,White, Black, "�Q�[�����I�����܂����H");
					}
					// �I������{�^��
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
							(xp1 + xp2) / 2, (yp1 + yp2) / 2, White, Black, "�I��");

						if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || ret) {
							// �I���t���O�𗧂Ă�
							this->m_IsEndGame = true;
						}
					}
				},
				[this]() {
					// �|�b�v�A�b�v������{�^�����ŏI�������ۂɍs������
					m_IsExitSelect = false;
				},
				[]() {/*�K�C�h�\���ɒǉ�������́B�����ł͓��ɕ\�����Ȃ�*/},
				true
			);
		}
		// �ċN���t���O�������A�����ċN���|�b�v�A�b�v���o�Ă��Ȃ��ꍇ
		if (OptionWindowParts->IsRestartSwitch() && !m_IsRestartSelect) {
			m_IsRestartSelect = true;
			PopUpParts->Add("Restart", 480, 240,
				[this](int xmin, int ymin, int xmax, int ymax, bool) {
					// �|�b�v�A�b�v���ŕ`�悷�����
					auto* DrawCtrls = UISystem::DrawControl::Instance();
					auto* Pad = PadControl::Instance();
					// �^�C�g��
					{
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
							xmin + 24, ymin + LineHeight, White, Black, "�Q�[�����ċN�����܂����H");
					}
					// �I������{�^��
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
							(xp1 + xp2) / 2, (yp1 + yp2) / 2, White, Black, "�ċN��");

						if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || ret) {
							// �I���t���O�𗧂Ă�
							this->m_IsEndGame = true;
							// �ʃv���Z�X�Ƃ��Ď��g���N�����Ă���
							StartMe();
						}
					}
				},
				[this]() {
					// �|�b�v�A�b�v������{�^�����ŏI�������ۂɍs������
					m_IsRestartSelect = false;
				},
				[]() {/*�K�C�h�\���ɒǉ�������́B�����ł͓��ɕ\�����Ȃ�*/},
				true
			);
		}
		// ����̃A�b�v�f�[�g
		Pad->Update();
		// �V�[���̍X�V
		m_IsEndScene = !this->m_NowScenesPtr->Update();
		// �I�v�V������ʂ̃A�b�v�f�[�g
		OptionWindowParts->Update();
		// �|�X�g�v���Z�X�����̍X�V
		PostPassParts->Update();
		// �|�b�v�A�b�v�̃A�b�v�f�[�g
		PopUpParts->Update();
		// �|�[�Y��ʂ̍X�V
		if (IsPause()) {
			m_PauseDrawer.Update();
		}
		// �|�[�Y���͂ɂ��I���I�t
		if (Pad->GetPadsInfo(PADS::INVENTORY).GetKey().trigger()) {
			ChangePause(!IsPause());
		}
		// FPS�\���@�\�̍X�V
		m_FPSDrawer.Update();
	}
	// �V�[���̕`����s������
	void SceneControl::DrawMainLoop(void) const noexcept {
		auto* WindowSizeParts = WindowSizeControl::Instance();
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		auto* PopUpParts = UISystem::PopUp::Instance();
		auto* PostPassParts = PostPass::PostPassEffect::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
#if DEBUG
		auto* DebugParts = DebugClass::Instance();
#endif // DEBUG
		// �|�X�g�v���Z�X�p�̃o�b�t�@�[��ʂɔ��f
		PostPassParts->GetBufferScreen().SetDraw_Screen();
		{
			// ���[���h��Ԃ̕`��
			this->m_NowScenesPtr->DrawMain();
		}
		// �|�X�g�v���Z�X
		PostPassParts->DrawPostProcess();
		// ����ʂ��N���A���ĕ`���ɐݒ�
		GraphHandle::SetDraw_Screen(static_cast<int>(DX_SCREEN_BACK), true);
		{
			// �o�C���j�A�t�B���^�[�������ă|�X�g�v���Z�X������̃o�b�t�@�[��ʂ�`��
			int Prev = GetDrawMode();
			// SetDrawMode(DX_DRAWMODE_NEAREST);
			SetDrawMode(DX_DRAWMODE_BILINEAR);
			PostPassParts->GetBufferScreen().DrawExtendGraph(0, 0, WindowSizeParts->GetUIY(BaseScreenWidth), WindowSizeParts->GetUIY(BaseScreenHeight), false);
			SetDrawMode(Prev);

			// UI�`��ݒ�
			this->m_NowScenesPtr->DrawUI_Base();
			// �|�[�Y�`���ݒ�
			if (IsPause()) {
				m_PauseDrawer.DrawPause();
			}
			// FPS�\���`���ݒ�
			m_FPSDrawer.DrawFPSCounter();
			// �L�[�K�C�h�`���ݒ�
			KeyGuideParts->Draw();
			// �|�b�v�A�b�v�`�����ʒ������Z���^�[�Ƃ��Đݒ�
			PopUpParts->Draw(BaseScreenWidth / 2, BaseScreenHeight / 2);
			// �|�[�Y��ʂ̑O�Ȃǂɕ`�悷��UI�̕`��ݒ�
			this->m_NowScenesPtr->DrawUI_In();
#if DEBUG
			// �f�o�b�O��ʕ`���ݒ�
			DebugParts->DebugWindow(BaseScreenWidth - 350, 150);
#endif // DEBUG
			// ���ʂ�`��
			DrawCtrls->Draw();
		}
	}
	// �V�[���̏I���ɒʂ鏈��
	void SceneControl::ExitMainLoop(void) noexcept {
		this->m_NowScenesPtr->Dispose();										// ���̃V�[������̉��
		if (this->m_NowScenesPtr != this->m_NowScenesPtr->Get_Next()) {	// ���̃V�[���Ǝ��̃V�[���Ƃ��ʂ̃V�[���Ȃ�
			this->m_NowScenesPtr->Dispose_Load();								// ���[�h���Ă����f�[�^��j��
		}
		this->m_NowScenesPtr = this->m_NowScenesPtr->Get_Next();	// ���̃V�[���֑J��
	}
};
