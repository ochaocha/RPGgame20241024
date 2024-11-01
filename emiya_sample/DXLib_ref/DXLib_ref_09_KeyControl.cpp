#include "DXLib_ref.h"
// #include "DXLib_ref_09_KeyControl.hpp"

#include <fstream>

namespace DXLibRef {
	// �V���O���g���̎��Ԓ�`
	const PadControl* SingletonBase<PadControl>::m_Singleton = nullptr;
	// ���͏����Ɋ�Â��čX�V
	void			switchs::Update(bool key) noexcept {
		auto* DrawParts = DXDraw::Instance();
		//�������ƋL�^
		m_press = key;
		//��������J�E���g�A�b�v�A��������J�E���g�_�E��
		m_presscount = static_cast<int8_t>(Clamp(static_cast<int>(m_presscount) + (m_press ? 1 : -1), 0, 2));
		//���s�[�g����
		m_repeat = trigger();//�������u�Ԃ��A��������...
		//�����Ă����莞�Ԍ�A���Ԋu��true��
		if (m_press) {
			m_repeatcount -= DrawParts->GetDeltaTime();
			if (m_repeatcount <= 0.f) {
				m_repeatcount += m_RepeatTime;
				m_repeat = true;
			}
		}
		else {
			//�������牟���Ă���̃J�E���g�����Z�b�g
			m_repeatcount = m_RepeatWaitTime;
		}
	}
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	void PadControl::Load(void) noexcept {
		FileStreamDX FileStream;
		if (IsFileExist(GetSavePath())) {
			FileStream.Open(GetSavePath());
		}
		else {
			FileStream.Open(GetBasePath());
		}
		while (true) {
			if (FileStream.ComeEof()) { break; }
			auto ALL = FileStream.SeekLineAndGetStr();
			if (ALL == "") {
				continue;
			}
			auto LEFT = FileStreamDX::getleft(ALL);
			auto RIGHT = FileStreamDX::getright(ALL);
			for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
				if (LEFT == PADSStr[i]) {
					m_PadsInfo.at(i).SetAssign(GetStrtoID(RIGHT.c_str()));
					break;
				}
			}
		}
	}
	void PadControl::Save(void) const noexcept {
		std::ofstream outputfile(GetSavePath());
		for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
			PADS Pads = static_cast<PADS>(i);
			outputfile << (std::string)PADSStr[i] + "=" + GetIDtoStr(GetPadsInfo(Pads).GetAssign()) + "\n";
		}
		outputfile.close();
	}
	void PadControl::Update(void) noexcept {
		auto* SceneParts = SceneControl::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* OptionParts = OPTION::Instance();
		// �R���g���[���^�C�v����
		{
			//��{��PC
			ControlType NextControlType = ControlType::PC;
			//�Q�[���p�b�h�����m������
			if (GetJoypadNum() > 0) {
				//�R���g���[���^�C�v�ɍ��킹���ݒ���s��
				NextControlType = (ControlType)OptionParts->GetParamInt(EnumSaveParam::ControlType);
			}
			//�ȑO�̐ݒ�ƈقȂ�ꍇ��
			if (m_ControlType != NextControlType) {
				Save();									//���̃R���g���[���p�b�h�̐ݒ��ۑ�
				m_ControlType = NextControlType;		//�ύX
				Load();									//���̃R���g���[���p�b�h�̏������[�h
				UISystem::KeyGuide::Instance()->SetGuideUpdate();	//�K�C�h�\�����A�b�v�f�[�g
			}
		}
		// �^�C�v�ɍ��킹������
		switch (m_ControlType) {
		case ControlType::XBox:
		{
			//�}�E�X���W���擾���Ă���
			DrawParts->GetMousePosition(&MouseX, &MouseY);
			SetMouseDispFlag(TRUE);

			XINPUT_STATE input;
			GetJoypadXInputState(DX_INPUT_PAD1, &input);
			// ���X�e�B�b�N
			{
				int LS_X = input.ThumbLX;
				int LS_Y = -input.ThumbLY;
				if (GetPadsInfo(PADS::MOVE_STICK).GetAssign() == 0xF002) {
					LS_X = input.ThumbRX;
					LS_Y = -input.ThumbRY;
				}
				//XInput��-1000~1000�ɂȂ�悤�ɕϊ�
				LS_X = 1000 * LS_X / 32768;
				LS_Y = 1000 * LS_Y / 32768;
				//�ϊ��ɂ��덷������������̂Ńf�b�h�]�[�����w��
				if (abs(LS_X) < 10) { LS_X = 0; }
				if (abs(LS_Y) < 10) { LS_Y = 0; }

				float deg = rad2deg(atan2f(static_cast<float>(LS_X), static_cast<float>(-LS_Y)));
				bool w_key = false;
				bool s_key = false;
				bool a_key = false;
				bool d_key = false;
				if (!(LS_X == 0 && LS_Y == 0)) {
					w_key = (-50.f <= deg && deg <= 50.f);
					a_key = (-140.f <= deg && deg <= -40.f);
					s_key = (130.f <= deg || deg <= -130.f);
					d_key = (40.f <= deg && deg <= 140.f);
				}
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_W)).Update(w_key);
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_S)).Update(s_key);
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_A)).Update(a_key);
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_D)).Update(d_key);
			}
			// �E�X�e�B�b�N
			{
				int RS_X = input.ThumbRX;
				int RS_Y = -input.ThumbRY;
				if (GetPadsInfo(PADS::DIR_STICK).GetAssign() == 0xF001) {
					RS_X = input.ThumbLX;
					RS_Y = -input.ThumbLY;
				}
				RS_X = 1000 * RS_X / 32768;
				RS_Y = 1000 * RS_Y / 32768;

				if (!SceneParts->IsPause()) {
					Look_XradAdd = Clamp(static_cast<float>(RS_X) / 100.f * 4.f * OptionParts->GetParamFloat(EnumSaveParam::Xsensing), -180.f, 180.f);
					Look_YradAdd = Clamp(-static_cast<float>(RS_Y) / 100.f * 4.f * OptionParts->GetParamFloat(EnumSaveParam::Ysensing), -180.f, 180.f);
				}
				else {
					Look_XradAdd = 0.f;
					Look_YradAdd = 0.f;
				}
			}
		}
		break;
		case ControlType::PS4:
		{
			//�}�E�X���W���擾���Ă���
			DrawParts->GetMousePosition(&MouseX, &MouseY);
			SetMouseDispFlag(TRUE);
			DINPUT_JOYSTATE input;
			GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
			// ���X�e�B�b�N
			{
				int LS_X = input.X;
				int LS_Y = input.Y;
				if (GetPadsInfo(PADS::MOVE_STICK).GetAssign() == 0xF002) {
					LS_X = input.Z;
					LS_Y = input.Rz;
				}

				float deg = rad2deg(atan2f(static_cast<float>(LS_X), static_cast<float>(-LS_Y)));
				bool w_key = false;
				bool s_key = false;
				bool a_key = false;
				bool d_key = false;
				if (!(LS_X == 0 && LS_Y == 0)) {
					w_key = (-50.f <= deg && deg <= 50.f);
					a_key = (-140.f <= deg && deg <= -40.f);
					s_key = (130.f <= deg || deg <= -130.f);
					d_key = (40.f <= deg && deg <= 140.f);
				}
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_W)).Update(w_key);
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_S)).Update(s_key);
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_A)).Update(a_key);
				m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_D)).Update(d_key);
			}
			// �E�X�e�B�b�N
			{
				int RS_X = input.Z;
				int RS_Y = input.Rz;
				if (GetPadsInfo(PADS::DIR_STICK).GetAssign() == 0xF001) {
					RS_X = input.X;
					RS_Y = input.Y;
				}
				if (!SceneParts->IsPause()) {
					Look_XradAdd = static_cast<float>(RS_X) / 100.f * 4.f * OptionParts->GetParamFloat(EnumSaveParam::Xsensing);
					Look_YradAdd = -static_cast<float>(RS_Y) / 100.f * 4.f * OptionParts->GetParamFloat(EnumSaveParam::Ysensing);
				}
				else {
					Look_XradAdd = 0.f;
					Look_YradAdd = 0.f;
				}
			}
		}
		break;
		case ControlType::PC:
		{
			int PrevX = MouseX;
			int PrevY = MouseY;
			//�}�E�X���W���擾���Ă���
			DrawParts->GetMousePosition(&MouseX, &MouseY);
			// ���_�ړ��͑O�t���[������̈ړ���
			Look_XradAdd = static_cast<float>(MouseX - PrevX) * 2.f * OptionParts->GetParamFloat(EnumSaveParam::Xsensing);
			Look_YradAdd = -static_cast<float>(MouseY - PrevY) * 2.f * OptionParts->GetParamFloat(EnumSaveParam::Ysensing);
			//�}�E�X�ɂ�����(FPS�ATPS)�̏ꍇ
			if (m_MouseMoveEnable) {
				//�őO�ʂŃ|�[�Y���łȂ��ꍇ
				if ((GetMainWindowHandle() == GetForegroundWindow()) && !SceneParts->IsPause()) {
					//�ړ������Z�b�g
					SetMousePoint(DrawParts->GetUIXMax() / 2, DrawParts->GetUIYMax() / 2);
					//�}�E�X��\�����Ȃ�
					SetMouseDispFlag(FALSE);
				}
				else {
					//�}�E�X��\��
					SetMouseDispFlag(TRUE);
					// ���_�ړ��͂Ȃ��Ƃ��ď㏑��
					Look_XradAdd = 0.f;
					Look_YradAdd = 0.f;
				}
			}
			else {
				//�}�E�X��\��
				SetMouseDispFlag(TRUE);
			}
			//�ړ�
			m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_W)).Update(GetButtonPress(GetPadsInfo(PADS::MOVE_W).GetAssign()));
			m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_A)).Update(GetButtonPress(GetPadsInfo(PADS::MOVE_A).GetAssign()));
			m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_S)).Update(GetButtonPress(GetPadsInfo(PADS::MOVE_S).GetAssign()));
			m_PadsInfo.at(static_cast<size_t>(PADS::MOVE_D)).Update(GetButtonPress(GetPadsInfo(PADS::MOVE_D).GetAssign()));
		}
		break;
		default:
			break;
		}
		// �{�^��
		for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
			PADS Pads = static_cast<PADS>(i);
			if (
				Pads == PADS::MOVE_W ||
				Pads == PADS::MOVE_A ||
				Pads == PADS::MOVE_S ||
				Pads == PADS::MOVE_D
				) {
				continue;
			}
			m_PadsInfo.at(i).Update(GetButtonPress(GetPadsInfo(Pads).GetAssign()));
		}
		// ���̑��������
		{
			MouseClick.Update(GetWindowActiveFlag() ? ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) : false);
			if (GetWindowActiveFlag()) {
				MouseWheelRot = GetMouseWheelRotVol();
			}
			else {
				GetMouseWheelRotVol();//�O��Ƃ̍���������d�g�݂Ȃ̂ŃA�N�e�B�u�łȂ��ꍇ���ʂ��Ă͂����悤��
				MouseWheelRot = 0;
			}
		}
	}
};
