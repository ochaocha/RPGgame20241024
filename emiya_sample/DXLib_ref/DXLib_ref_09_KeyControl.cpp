#include "DXLib_ref.h"
// #include "DXLib_ref_09_KeyControl.hpp"

#include <fstream>

namespace DXLibRef {
	// �V���O���g���̎��Ԓ�`
	const PadControl* SingletonBase<PadControl>::m_Singleton = nullptr;
	// 
	void			switchs::Execute(bool key) noexcept {
		auto* DrawParts = DXDraw::Instance();
		m_press = key;
		if (m_press) {
			m_presscount = static_cast<int8_t>(Clamp(static_cast<int>(m_presscount) + 1, 0, 2));

			m_repeat = trigger();
			m_repeatcount -= FrameRate * DrawParts->GetDeltaTime();
			if (m_repeatcount <= 0.f) {
				m_repeatcount += 2.f;
				m_repeat = true;
			}
		}
		else {
			m_presscount = static_cast<int8_t>(Clamp(static_cast<int>(m_presscount) - 1, 0, 2));

			m_repeat = false;
			m_repeatcount = FrameRate / 2.f;
		}
		if (trigger()) {
			m_on ^= 1;
		}
	}
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	void PadControl::KeyGuideGraphs::AddGuideXBox(GraphHandle* pGuide, const std::string& GuideStr) noexcept {
		auto* DrawParts = DXDraw::Instance();
		if (pGuide) {
			pGuideImg = pGuide;
			pGuideImg->GetSize(&xsize, &ysize);
			xsize = xsize * DrawParts->GetUIY(24) / ysize;
			ysize = ysize * DrawParts->GetUIY(24) / ysize;
		}
		else {
			xsize = 0;
			ysize = 0;
		}
		GuideString = GuideStr;
	}
	void PadControl::KeyGuideGraphs::AddGuideDS4(GraphHandle* pGuide, const std::string& GuideStr) noexcept {
		auto* DrawParts = DXDraw::Instance();
		if (pGuide) {
			pGuideImg = pGuide;
			pGuideImg->GetSize(&xsize, &ysize);
			xsize = xsize * DrawParts->GetUIY(24) / ysize;
			ysize = ysize * DrawParts->GetUIY(24) / ysize;
		}
		else {
			xsize = 0;
			ysize = 0;
		}
		GuideString = GuideStr;
	}
	void PadControl::KeyGuideGraphs::AddGuidePC(GraphHandle* pGuide, const std::string& GuideStr) noexcept {
		auto* DrawParts = DXDraw::Instance();
		if (pGuide) {
			pGuideImg = pGuide;
			pGuideImg->GetSize(&xsize, &ysize);
			if (ysize == 0) {
				ysize = 1;
			}
			xsize = xsize * DrawParts->GetUIY(21) / ysize;
			ysize = ysize * DrawParts->GetUIY(21) / ysize;
		}
		else {
			xsize = 0;
			ysize = 0;
		}
		GuideString = GuideStr;
	}
	int PadControl::KeyGuideGraphs::GetDrawSize(void) const noexcept {
		auto* DrawParts = DXDraw::Instance();

		int ofs = 0;
		if (xsize > 0) {
			ofs += static_cast<int>(xsize) + DrawParts->GetUIY(3);
		}
		if (GuideString != "") {
			ofs += WindowSystem::GetMsgLen(LineHeight, GuideString) + DrawParts->GetUIY(12);
		}
		return ofs;
	}

	int PadControl::KeyGuideGraphs::Draw(int x, int y) const noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();

		int ofs = 0;
		if (xsize > 0) {
			DrawCtrls->SetDrawExtendGraph(WindowSystem::DrawLayer::Normal,
				pGuideImg, x + ofs, y, x + ofs + static_cast<int>(xsize), y + static_cast<int>(ysize), true);
			ofs += static_cast<int>(xsize) + DrawParts->GetUIY(3);
		}
		WindowSystem::SetMsg(x + ofs, y + DrawParts->GetUIY(24) / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, GuideString);
		return GetDrawSize();
	}

	PadControl::PadControl(void) noexcept {
		m_IsUpdate = true;
		m_MouseMoveEnable = false;
		for (auto& k : this->m_AtoZKey) {
			k.Set(false);
		}
		for (auto& k : this->m_NumKey) {
			k.Set(false);
		}
		GuideBase.Load("CommonData/key/OutputFont.png");
		{
			FileStreamDX FileStream("CommonData/key/OutputFont.psf");
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL == "") {
					continue;
				}
				auto LEFT = FileStreamDX::getleft(ALL);
				auto RIGHT = FileStreamDX::getright(ALL);

				std::vector<int> Args;
				while (true) {
					auto div = RIGHT.find(",");
					if (div != std::string::npos) {
						Args.emplace_back(std::stoi(RIGHT.substr(0, div)));
						RIGHT = RIGHT.substr(div + 1);
					}
					else {
						Args.emplace_back(std::stoi(RIGHT));
						break;
					}
				}
				GuideRect.resize(GuideRect.size() + 1);
				GuideRect.back().DerivationGraph(Args.at(0), Args.at(1), Args.at(2), Args.at(3), GuideBase);
			}
		}
		Load();
	}
	bool PadControl::GetButtonPress(int ID) noexcept {
		switch (m_ControlType) {
		case ControlType::XBox:
		{
			XINPUT_STATE input;
			GetJoypadXInputState(DX_INPUT_PAD1, &input);
			// �{�^��
			if (ID >= 0xF100) {
				switch (ID) {
				case 0xF100:
					return input.LeftTrigger > 0;
				case 0xF200:
					return input.RightTrigger > 0;
				default:
					break;
				}
			}
			else if (0 <= ID && ID < 16) {
				return (input.Buttons[ID] != 0);
			}
		}
		break;
		case ControlType::PS4:
		{
			DINPUT_JOYSTATE input;
			GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
			// �{�^��
			if (ID >= 0xF010) {
				// �\���L�[
				float deg = static_cast<float>(input.POV[0]) / 100.f;
				bool w_key = false;
				bool s_key = false;
				bool a_key = false;
				bool d_key = false;
				if (input.POV[0] != 0xffffffff) {
					w_key = (310.f <= deg || deg <= 50.f);
					s_key = (130.f <= deg && deg <= 230.f);
					a_key = (220.f <= deg && deg <= 320.f);
					d_key = (40.f <= deg && deg <= 140.f);
				}
				switch (ID) {
				case 0xF010:
					return a_key;
				case 0xF020:
					return w_key;
				case 0xF040:
					return d_key;
				case 0xF080:
					return s_key;
				default:
					break;
				}
			}
			else if (0 <= ID && ID < 32) {
				return (input.Buttons[ID] != 0);
			}
		}
		break;
		case ControlType::PC:
			// �{�^��
			if (0 <= ID) {
				if ((ID & 0xF00) != 0) {
					return ((GetMouseInputWithCheck() & (ID & 0xFF)) != 0);
				}
				else {
					return (CheckHitKeyWithCheck(ID) != 0);
				}
			}
			break;
		default:
			break;
		}
		return false;
	}
	bool PadControl::GetPushAnySwitchLocal(PADS select, int ID) noexcept {
		auto* SoundParts = SoundPool::Instance();
		if (GetButtonPress(ID)) {
			auto& P = m_PadsInfo.at(static_cast<size_t>(select));
			if (P.m_assign != ID) {
			}
			else if (P.m_Key.trigger()) {
				ID = (P.m_reserve == InvalidID) ? P.m_assign : InvalidID;
			}
			else {
				return true;
			}

			// ���ɃA�T�C���ς̂��̂��������ꍇ�����𖳌���
			for (size_t p = 0; p < static_cast<size_t>(PADS::MAX); ++p) {
				auto& P2 = m_PadsInfo.at(p);
				if ((select != (PADS)p) && (P2.m_reserve == ID || P2.m_assign == ID)) {
					P2.m_reserve = InvalidID;
					break;
				}
			}
			if (P.m_reserve != ID) {
				P.m_reserve = ID;
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
			return true;
		}
		return false;
	}
	void PadControl::GetPushAnySwitch(PADS select) noexcept {
		for (size_t p = 0; p < static_cast<size_t>(PADS::MAX); ++p) {
			if (p == static_cast<size_t>(select)) {
				continue;
			}
			auto& P = m_PadsInfo.at(p);
			if (GetPushAnySwitchLocal(select, P.m_reserve)) {
				break;
			}
		}
		for (size_t p = 0; p < static_cast<size_t>(PADS::MAX); ++p) {
			if (p == static_cast<size_t>(select)) {
				continue;
			}
			auto& P = m_PadsInfo.at(p);
			if (GetPushAnySwitchLocal(select, P.m_assign)) {
				break;
			}
		}
		switch (m_ControlType) {
		case ControlType::XBox:
			for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
				if (GetPushAnySwitchLocal(select, XBoxID[i])) {
					break;
				}
			}
			break;
		case ControlType::PS4:
			for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
				if (GetPushAnySwitchLocal(select, DS4ID[i])) {
					break;
				}
			}
			break;
		case ControlType::PC:
			for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
				if (GetPushAnySwitchLocal(select, KeyID[i])) {
					break;
				}
			}
			break;
		default:
			break;
		}
	}
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
					this->m_PadsInfo.at(i).m_assign = GetStrtoID(RIGHT.c_str());
					this->m_PadsInfo.at(i).m_reserve = this->m_PadsInfo.at(i).m_assign;
					break;
				}
			}
		}
	}
	void PadControl::Save(void) noexcept {
		std::ofstream outputfile(GetSavePath());
		for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
			outputfile << (std::string)PADSStr[i] + "=" + GetIDtoStr(this->m_PadsInfo[i].m_assign) + "\n";
		}
		outputfile.close();
	}
	void PadControl::ResetAssign(void) noexcept {
		for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
			this->m_PadsInfo.at(i).m_reserve = this->m_PadsInfo.at(i).m_assign;
		}
	}
	void PadControl::FlipAssign(void) noexcept {
		for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
			this->m_PadsInfo.at(i).m_assign = this->m_PadsInfo.at(i).m_reserve;
		}
	}
	void PadControl::ChangeGuide(std::function<void()>Guide_Pad) noexcept {
		if (m_IsUpdate) {
			m_IsUpdate = false;
			Reset();
			// 
			AddGuide(PADS::Escape, "�I��", ControlType::PC);
			AddGuide(PADS::INVENTORY, "�|�[�Y");
			// 
			Guide_Pad();
		}
	}
	void PadControl::Update(void) noexcept {
		auto* DXParts = DXLib_ref::Instance();
		auto* DrawParts = DXDraw::Instance();
		// �R���g���[���^�C�v����
		{
			ControlType NextControlType = ControlType::PC;
			if (GetJoypadNum() > 0) {
				auto* OptionParts = OPTION::Instance();
				NextControlType = (ControlType)OptionParts->GetParamInt(EnumSaveParam::ControlType);
			}
			if (m_ControlType != NextControlType) {
				Save();
				m_ControlType = NextControlType;
				m_IsUpdate = true;
				Load();
				if (GetJoypadNum() > 0) {
					// �p�b�h�ڑ��ŊJ�n���܂�
				}
			}
		}
		// �^�C�v�ɍ��킹������
		switch (m_ControlType) {
		case ControlType::XBox:
		{
			XINPUT_STATE input;
			GetJoypadXInputState(DX_INPUT_PAD1, &input);
			// ���X�e�B�b�N
			{
				int LS_X = input.ThumbLX;
				int LS_Y = -input.ThumbLY;
				if (this->m_PadsInfo[static_cast<int>(PADS::MOVE_STICK)].m_assign == 0xF002) {
					LS_X = input.ThumbRX;
					LS_Y = -input.ThumbRY;
				}
				LS_X = 1000 * LS_X / 32768;
				LS_Y = 1000 * LS_Y / 32768;

				if (abs(LS_X) < 10) {
					LS_X = 0;
				}
				if (abs(LS_Y) < 10) {
					LS_Y = 0;
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
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_W)].m_Key.Execute(w_key);
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_S)].m_Key.Execute(s_key);
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_A)].m_Key.Execute(a_key);
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_D)].m_Key.Execute(d_key);
			}
			// �E�X�e�B�b�N
			{
				DrawParts->GetMousePosition(&MouseX, &MouseY);
				MouseClick.Execute((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0);
				SetMouseDispFlag(TRUE);
				int RS_X = input.ThumbRX;
				int RS_Y = -input.ThumbRY;
				if (this->m_PadsInfo[static_cast<int>(PADS::DIR_STICK)].m_assign == 0xF001) {
					RS_X = input.ThumbLX;
					RS_Y = -input.ThumbLY;
				}
				RS_X = 1000 * RS_X / 32768;
				RS_Y = 1000 * RS_Y / 32768;

				if (!DXParts->IsPause()) {
					auto* OptionParts = OPTION::Instance();
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
			DINPUT_JOYSTATE input;
			GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
			// ���X�e�B�b�N
			{
				int LS_X = input.X;
				int LS_Y = input.Y;
				if (this->m_PadsInfo[static_cast<int>(PADS::MOVE_STICK)].m_assign == 0xF002) {
					LS_X = input.Z;
					LS_Y = input.Rz;
				}

				float deg = rad2deg(atan2f(static_cast<float>(LS_X), static_cast<float>(-LS_Y)));
				bool w_key = false;
				bool s_key = false;
				bool a_key = false;
				bool d_key = false;
				if (!(input.X == 0 && input.Y == 0)) {
					w_key = (-50.f <= deg && deg <= 50.f);
					a_key = (-140.f <= deg && deg <= -40.f);
					s_key = (130.f <= deg || deg <= -130.f);
					d_key = (40.f <= deg && deg <= 140.f);
				}
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_W)].m_Key.Execute(w_key);
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_S)].m_Key.Execute(s_key);
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_A)].m_Key.Execute(a_key);
				this->m_PadsInfo[static_cast<int>(PADS::MOVE_D)].m_Key.Execute(d_key);
			}
			// �E�X�e�B�b�N
			{
				DrawParts->GetMousePosition(&MouseX, &MouseY);
				MouseClick.Execute((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0);
				SetMouseDispFlag(TRUE);
				int RS_X = input.Z;
				int RS_Y = input.Rz;
				if (this->m_PadsInfo[static_cast<int>(PADS::DIR_STICK)].m_assign == 0xF001) {
					RS_X = input.X;
					RS_Y = input.Y;
				}

				auto* OptionParts = OPTION::Instance();
				Look_XradAdd = static_cast<float>(RS_X) / 100.f * 4.f * OptionParts->GetParamFloat(EnumSaveParam::Xsensing);
				Look_YradAdd = -static_cast<float>(RS_Y) / 100.f * 4.f * OptionParts->GetParamFloat(EnumSaveParam::Ysensing);
			}
		}
		break;
		case ControlType::PC:
			// �E�X�e�B�b�N
		{
			DrawParts->GetMousePosition(&MouseX, &MouseY);
			MouseClick.Execute((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0);

			if (m_MouseMoveEnable) {
				if (GetMainWindowHandle() != GetForegroundWindow()) {// ����ʂ��őO�ł͂Ȃ����
					SetMouseDispFlag(TRUE);
				}
				else {
					if (!DXParts->IsPause()) {
						SetMousePoint(DrawParts->GetUIXMax() / 2, DrawParts->GetUIYMax() / 2);
						SetMouseDispFlag(FALSE);
					}
					else {
						SetMouseDispFlag(TRUE);
					}
				}
			}
			else {
				SetMouseDispFlag(TRUE);
			}
			auto* OptionParts = OPTION::Instance();
			Look_XradAdd = static_cast<float>(MouseX - DrawParts->GetUIXMax() / 2) * 2.f * OptionParts->GetParamFloat(EnumSaveParam::Xsensing);
			Look_YradAdd = -static_cast<float>(MouseY - DrawParts->GetUIYMax() / 2) * 2.f * OptionParts->GetParamFloat(EnumSaveParam::Ysensing);
		}
		break;
		default:
			break;
		}
		// �{�^��
		for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
			if (m_ControlType != ControlType::PC) {
				switch ((PADS)i) {
				case PADS::MOVE_W:
				case PADS::MOVE_A:
				case PADS::MOVE_S:
				case PADS::MOVE_D:
					continue;
					break;
				case PADS::MOVE_STICK:
				case PADS::DIR_UP:
				case PADS::DIR_DOWN:
				case PADS::DIR_LEFT:
				case PADS::DIR_RIGHT:
				case PADS::DIR_STICK:
				case PADS::LEAN_L:
				case PADS::LEAN_R:
				case PADS::RELOAD:
				case PADS::INTERACT:
				case PADS::JUMP:
				case PADS::INVENTORY:
				case PADS::RUN:
				case PADS::WALK:
				case PADS::SHOT:
				case PADS::AIM:
				case PADS::Escape:
				case PADS::MAX:
				default:
					break;
				}
			}
			this->m_PadsInfo.at(i).m_Key.Execute(GetButtonPress(this->m_PadsInfo.at(i).m_assign));
		}
		// 
		{
			int KEYS[26] = {
				KEY_INPUT_A,
				KEY_INPUT_B,
				KEY_INPUT_C,
				KEY_INPUT_D,
				KEY_INPUT_E,
				KEY_INPUT_F,
				KEY_INPUT_G,
				KEY_INPUT_H,
				KEY_INPUT_I,
				KEY_INPUT_J,
				KEY_INPUT_K,
				KEY_INPUT_L,
				KEY_INPUT_M,
				KEY_INPUT_N,
				KEY_INPUT_O,
				KEY_INPUT_P,
				KEY_INPUT_Q,
				KEY_INPUT_R,
				KEY_INPUT_S,
				KEY_INPUT_T,
				KEY_INPUT_U,
				KEY_INPUT_V,
				KEY_INPUT_W,
				KEY_INPUT_X,
				KEY_INPUT_Y,
				KEY_INPUT_Z,
			};
			for (int loop = 0, loopMax = static_cast<int>(this->m_AtoZKey.size()); loop < loopMax; ++loop) {
				m_AtoZKey[loop].Execute(CheckHitKeyWithCheck(KEYS[loop]) != 0);
			}
			int NUMKEYS[10] = {
				KEY_INPUT_0,
				KEY_INPUT_1,
				KEY_INPUT_2,
				KEY_INPUT_3,
				KEY_INPUT_4,
				KEY_INPUT_5,
				KEY_INPUT_6,
				KEY_INPUT_7,
				KEY_INPUT_8,
				KEY_INPUT_9,
			};
			for (int loop = 0, loopMax = static_cast<int>(this->m_NumKey.size()); loop < loopMax; ++loop) {
				m_NumKey[loop].Execute(CheckHitKeyWithCheck(NUMKEYS[loop]) != 0);
			}
		}
		// 
		KeyEsc.Execute(CheckHitKeyWithCheck(KEY_INPUT_ESCAPE) != 0);
		KeyBSorDel.Execute((CheckHitKeyWithCheck(KEY_INPUT_BACK) != 0) || (CheckHitKeyWithCheck(KEY_INPUT_DELETE) != 0));
		MouseWheelRot = GetMouseWheelRotVolWithCheck();
	}
	void PadControl::Draw(void) const noexcept {
		auto* DrawParts = DXDraw::Instance();
		int xp = 0;
		int y = DrawParts->GetUIYMax() - DrawParts->GetUIY(21 + 16);
		for (const auto& k : Key) {
			xp += k->Draw(DrawParts->GetUIY(32) + xp, y);
			if (xp > DrawParts->GetUIXMax() / 2) {
				xp = 0;
				y -= DrawParts->GetUIY(28);
			}
		}
	}
};
