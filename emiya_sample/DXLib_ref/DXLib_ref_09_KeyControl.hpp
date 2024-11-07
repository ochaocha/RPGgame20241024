#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// PadControl�ɂĎg�p����A�e�p�b�h�ɑΉ�����Q�[���p�b�h�A�L�[�A�T�C��
	// --------------------------------------------------------------------------------------------------
	//�Ή�����R���g���[���^�C�v�̈ꗗ
	enum class ControlType :int {
		XBox,
		PS4,
		PC,
		Max,
	};
	static const char* ControlTypeStr[] = {
		"XBox",
		"PS4",
		"PC",
	};
	//��t����A�T�C���̈ꗗ
	enum class PADS :int {
		MOVE_W,
		MOVE_A,
		MOVE_S,
		MOVE_D,
		MOVE_STICK,
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_STICK,
		LEAN_L,
		LEAN_R,
		RELOAD,
		INTERACT,
		JUMP,
		INVENTORY,
		RUN,
		WALK,
		SHOT,
		AIM,
		Escape,

		MAX,
	};
	static const char* PADSStr[static_cast<int>(PADS::MAX)] = {
		"MOVE_W",
		"MOVE_A",
		"MOVE_S",
		"MOVE_D",
		"MOVE_STICK",
		"DIR_UP",
		"DIR_DOWN",
		"DIR_LEFT",
		"DIR_RIGHT",
		"DIR_STICK",
		"LEAN_L",
		"LEAN_R",
		"RELOAD",
		"INTERACT",
		"JUMP",
		"INVENTORY",
		"RUN",
		"WALK",
		"SHOT",
		"AIM",
		"ESCAPE",
	};
	//�L�[�{�[�h�̓��͈ꗗ
	static const size_t KeyNum = 109 + 3;
	static const int KeyID[KeyNum] = {
		KEY_INPUT_BACK,
		KEY_INPUT_TAB,
		KEY_INPUT_RETURN,

		KEY_INPUT_LSHIFT,
		KEY_INPUT_RSHIFT,
		KEY_INPUT_LCONTROL,
		KEY_INPUT_RCONTROL,
		KEY_INPUT_ESCAPE,
		KEY_INPUT_SPACE,
		KEY_INPUT_PGUP,
		KEY_INPUT_PGDN,
		KEY_INPUT_END,
		KEY_INPUT_HOME,
		KEY_INPUT_LEFT,
		KEY_INPUT_UP,
		KEY_INPUT_RIGHT,
		KEY_INPUT_DOWN,
		KEY_INPUT_INSERT,
		KEY_INPUT_DELETE,

		KEY_INPUT_MINUS,
		KEY_INPUT_YEN,
		KEY_INPUT_PREVTRACK,
		KEY_INPUT_PERIOD,
		KEY_INPUT_SLASH,
		KEY_INPUT_LALT,
		KEY_INPUT_RALT,
		KEY_INPUT_SCROLL,
		KEY_INPUT_SEMICOLON,
		KEY_INPUT_COLON,
		KEY_INPUT_LBRACKET,
		KEY_INPUT_RBRACKET,
		KEY_INPUT_AT,
		KEY_INPUT_BACKSLASH,
		KEY_INPUT_COMMA,
		KEY_INPUT_KANJI,
		KEY_INPUT_CONVERT,
		KEY_INPUT_NOCONVERT,
		KEY_INPUT_KANA,
		KEY_INPUT_APPS,
		KEY_INPUT_CAPSLOCK,
		KEY_INPUT_SYSRQ,
		KEY_INPUT_PAUSE,
		KEY_INPUT_LWIN,
		KEY_INPUT_RWIN,

		KEY_INPUT_NUMLOCK,
		KEY_INPUT_NUMPAD0,
		KEY_INPUT_NUMPAD1,
		KEY_INPUT_NUMPAD2,
		KEY_INPUT_NUMPAD3,
		KEY_INPUT_NUMPAD4,
		KEY_INPUT_NUMPAD5,
		KEY_INPUT_NUMPAD6,
		KEY_INPUT_NUMPAD7,
		KEY_INPUT_NUMPAD8,
		KEY_INPUT_NUMPAD9,
		KEY_INPUT_MULTIPLY,
		KEY_INPUT_ADD,
		KEY_INPUT_SUBTRACT,
		KEY_INPUT_DECIMAL,
		KEY_INPUT_DIVIDE,
		KEY_INPUT_NUMPADENTER,

		KEY_INPUT_F1,
		KEY_INPUT_F2,
		KEY_INPUT_F3,
		KEY_INPUT_F4,
		KEY_INPUT_F5,
		KEY_INPUT_F6,
		KEY_INPUT_F7,
		KEY_INPUT_F8,
		KEY_INPUT_F9,
		KEY_INPUT_F10,
		KEY_INPUT_F11,
		KEY_INPUT_F12,

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

		KEY_INPUT_0 ,
		KEY_INPUT_1,
		KEY_INPUT_2,
		KEY_INPUT_3,
		KEY_INPUT_4,
		KEY_INPUT_5,
		KEY_INPUT_6,
		KEY_INPUT_7,
		KEY_INPUT_8,
		KEY_INPUT_9,

		MOUSE_INPUT_LEFT | 0xF00,
		MOUSE_INPUT_RIGHT | 0xF00,
		MOUSE_INPUT_MIDDLE | 0xF00,
	};
	static const char* KeyStr[KeyNum] = {
		"BACK",
		"TAB",
		"RETURN",

		"LSHIFT",
		"RSHIFT",
		"LCONTROL",
		"RCONTROL",
		"ESCAPE",
		"SPACE",
		"PGUP",
		"PGDN",
		"END",
		"HOME",
		"LEFT",
		"UP",
		"RIGHT",
		"DOWN",
		"INSERT",
		"DELETE",

		"MINUS",
		"YEN",
		"PREVTRACK",
		"PERIOD",
		"SLASH",
		"LALT",
		"RALT",
		"SCROLL",
		"SEMICOLON",
		"COLON",
		"LBRACKET",
		"RBRACKET",
		"AT",
		"BACKSLASH",
		"COMMA",
		"KANJI",
		"CONVERT",
		"NOCONVERT",
		"KANA",
		"APPS",
		"CAPSLOCK",
		"SYSRQ",
		"PAUSE",
		"LWIN",
		"RWIN",

		"NUMLOCK",
		"NUMPAD0",
		"NUMPAD1",
		"NUMPAD2",
		"NUMPAD3",
		"NUMPAD4",
		"NUMPAD5",
		"NUMPAD6",
		"NUMPAD7",
		"NUMPAD8",
		"NUMPAD9",
		"MULTIPLY",
		"ADD",
		"SUBTRACT",
		"DECIMAL",
		"DIVIDE",
		"NUMPADENTER",

		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",

		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",

		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",

		"LMOUSE",
		"RMOUSE",
		"WMOUSE",
	};
	static const char* KeyGuidePath[KeyNum] = {
		"BackSpace",
		"Tab",
		"Enter",

		"Shift",
		"Shift",
		"Ctrl",
		"Ctrl",
		"ESC",
		"none",
		"PageUp",
		"PageDown",
		"End",
		"Home",
		"hidari",
		"ue",
		"migi",
		"sita",
		"Ins",
		"Del",

		"mainasu",
		"en",
		"quotation",
		"period",
		"slash",
		"Alt",
		"Alt",
		"ScrollLock",
		"semikoron",
		"coron",
		"kakko3",
		"kakko4",
		"atto",
		"backslash",
		"comma",
		"kanji",
		"Henkan",
		"Muhenkan",
		"kana",
		"F",
		"CapsLock",
		"PrintScreen",
		"Pause",
		"W",
		"W",

		"NumLock",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"asutarisku",
		"purasu",
		"mainasu",
		"period",
		"slash",
		"Enter",

		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",

		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",

		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",

		"LM",
		"RM",
		"MM",
	};
	//PS4�̓��͈ꗗ(DirectInput)
	static const size_t DS4Num = 12 - 1 + 2 + 4;
	static const int DS4ID[DS4Num] = {
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		9,
		10,
		11,
		0xF001,
		0xF002,
		0xF010,
		0xF020,
		0xF040,
		0xF080,
	};
	static const char* DS4Str[DS4Num] = {
		"SQUARE",
		"NG",
		"OK",
		"TRIANGLE",
		"L1",
		"R1",
		"L2",
		"R2",
		"OPTION",
		"L3",
		"R3",
		"LSTICK",
		"RSTICK",
		"LEFT",
		"UP",
		"RIGHT",
		"DOWN",
	};
	static const char* DS4GuidePath[DS4Num] = {
		"square",
		"ng",
		"ok",
		"triangle",
		"L1",
		"R1",
		"L2",
		"R2",
		"option",
		"L_stick",
		"R_stick",
		"L_stick",
		"R_stick",
		"left",
		"up",
		"right",
		"down",
	};
	//XBox�̓��͈ꗗ(XInput)
	static const size_t XBoxNum = 12 - 1 + 2 + 4;
	static const int XBoxID[XBoxNum] = {
		14,
		12,
		13,
		15,

		8,
		9,

		0xF100,
		0xF200,

		4,

		6,
		7,

		0xF001,
		0xF002,
		2,
		0,
		3,
		1,
	};
	static const char* XBoxStr[XBoxNum] = {
		"SQUARE",
		"NG",
		"OK",
		"TRIANGLE",

		"L1",
		"R1",

		"L2",
		"R2",

		"OPTION",

		"L3",
		"R3",

		"LSTICK",
		"RSTICK",
		"LEFT",
		"UP",
		"RIGHT",
		"DOWN",
	};
	static const char* XBoxGuidePath[XBoxNum] = {
		"square",
		"ng",
		"ok",
		"triangle",
		"L1",
		"R1",
		"L2",
		"R2",
		"option",
		"L_stick",
		"R_stick",
		"L_stick",
		"R_stick",
		"left",
		"up",
		"right",
		"down",
	};
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// �L�[��������(�������u�Ԃ⃊�s�[�g�A�������u�ԂȂǂɂ��Ή�)
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class switchs {
		const float m_RepeatWaitTime = 0.5f;			//���s�[�g����1�`2�ŊԂ̎��ԊԊu
		const float m_RepeatTime = 0.04f;				//���s�[�g����2�ňȍ~�̔���Ԋu
	private:
		bool		m_press{ false };					// �������������ǂ����̔��蔻��
		int8_t		m_presscount{ 0 };					// �����������t���[�������L�����Ă����@��������2�܂ŃJ�E���g�A�b�v�A��������0�܂ŃJ�E���g�_�E��
		bool		m_repeat{ false };					// �����������ۂ̌J��Ԃ�����
		float		m_repeatcount{ m_RepeatWaitTime };	// �J��Ԃ�����Ɏg���^�C�}�[
	public:
		//�R���X�g���N�^
		switchs(void) noexcept {
			m_presscount = 0;
			m_repeatcount = m_RepeatWaitTime;
			m_press = false;
		}
		//�f�X�g���N�^
		~switchs(void) noexcept {}
	public:
		// �X�V
		void			Update(bool key) noexcept;
		// �������u��
		bool			trigger(void) const noexcept { return m_press && (m_presscount == 1); }//�����Ă��āA�����J�E���g��1�̃t���[��
		// �����Ă����
		bool			press(void) const noexcept { return m_press; }
		// �����Ă���ԃ��s�[�g
		bool			repeat(void) const noexcept { return m_repeat; }
		// �������u��
		bool			release_trigger(void) const noexcept { return (!m_press) && (m_presscount == 1); }//�����Ă��āA�����J�E���g��1�̃t���[��
		// �����Ă����
		bool			release(void) const noexcept { return !m_press; }
	};
	// --------------------------------------------------------------------------------------------------
	// �L�[�R���t�B�O�����������e�L�[�A�p�b�h����
	// --------------------------------------------------------------------------------------------------
	class PadControl : public SingletonBase<PadControl> {
	private:
		friend class SingletonBase<PadControl>;
	private:
		class PadsInfo {
			switchs m_Key;
			int		m_assign = 0;
			int		m_reserve = 0;
			bool	m_IsUse{ true };
		public:
			void SetAssign(int select) noexcept {
				this->m_assign = select;
				this->m_reserve = this->m_assign;
			}
			void SetReserve(int select) noexcept { this->m_reserve = select; }
			void ResetAssign(void) noexcept { this->m_reserve = this->m_assign; }
			void FlipAssign(void) noexcept { this->m_assign = this->m_reserve; }
			void Update(bool Press) noexcept { this->m_Key.Update(Press); }
		public:
			const auto& GetReserve(void) const noexcept { return this->m_reserve; }
			const auto& GetAssign(void) const noexcept { return this->m_assign; }
			const auto& IsUse(void) const noexcept { return this->m_IsUse; }
			const auto& GetKey(void) const noexcept { return this->m_Key; }
			const auto IsEnableSelectReserve() const noexcept { return this->m_reserve != InvalidID; }
		};
	private:
		std::array<PadsInfo, static_cast<int>(PADS::MAX)>		m_PadsInfo;						// �{�^�����͂�ێ�
		float													Look_XradAdd{ 0.f };			// �E�X�e�B�b�N���͂�ێ�
		float													Look_YradAdd{ 0.f };			// �E�X�e�B�b�N���͂�ێ�
		int														MouseX{ 0 };					//�}�E�X��DXLIB����̒l��ێ����Ă���
		int														MouseY{ 0 };					//�}�E�X��DXLIB����̒l��ێ����Ă���
		switchs													MouseClick;						//���N���b�N
		int														MouseWheelRot{ 0 };				//�}�E�X�z�C�[���̉�]�ʂ�ێ����Ă���
		// 
		bool													m_MouseMoveEnable{ false };		//FPS�Ȃǂ̃}�E�X��\�����Ȃ�������@��p���邩�ǂ���
		ControlType												m_ControlType{ ControlType::PC };//���݂̃R���g���[���^�C�v
	private:
		// �R���X�g���N�^
		PadControl(void) noexcept { Load(); }
		PadControl(const PadControl&) = delete;		// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		PadControl(PadControl&& o) = delete;
		PadControl& operator=(const PadControl&) = delete;
		PadControl& operator=(PadControl&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		//���F�����Ă���R���g���[���^�C�v�𓾂�
		const auto& GetControlType(void) const noexcept { return m_ControlType; }
		//���_�ړ��ɑ���������͂𓾂�
		const auto& GetLS_X(void) const noexcept { return Look_XradAdd; }
		const auto& GetLS_Y(void) const noexcept { return Look_YradAdd; }
		//�}�E�X�̈ʒu����͂�Ԃ�(UI�̃N���b�N�Ȃǐ�p)
		const auto& GetMS_X(void) const noexcept { return MouseX; }
		const auto& GetMS_Y(void) const noexcept { return MouseY; }
		const auto& GetMouseClick(void) const noexcept { return MouseClick; }
		const auto& GetMouseWheelRot(void) const noexcept { return MouseWheelRot; }
		//�e�L�[�R���t�B�O�ɑΉ��������͂�����
		const auto& GetPadsInfo(PADS select) const noexcept { return m_PadsInfo.at(static_cast<size_t>(select)); }
	private:
		//�Z�[�u�f�[�^������ꏊ���߂�p�X
		const char* GetSavePath(void) const noexcept {
			switch (m_ControlType) {
			case ControlType::XBox:
				return "Save/KeyConfig_XBox.txt";
			case ControlType::PS4:
				return "Save/KeyConfig_DS4.txt";
			case ControlType::PC:
				return "Save/KeyConfig_PC.txt";
			default:
				break;
			}
			return "NONE";
		};
		const char* GetBasePath(void) const noexcept {
			switch (m_ControlType) {
			case ControlType::XBox:
				return "data/KeyConfigBase_XBox.txt";
			case ControlType::PS4:
				return "data/KeyConfigBase_DS4.txt";
			case ControlType::PC:
				return "data/KeyConfigBase_PC.txt";
			default:
				break;
			}
			return "NONE";
		};
		//��������KEY_INPUT_BACK�Ȃǂ�ID���擾
		const auto GetStrtoID(const char* Str) const noexcept {
			switch (m_ControlType) {
			case ControlType::XBox:
				for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
					if (strcmpDx(XBoxStr[i], Str) == 0) {
						return XBoxID[i];
					}
				}
				break;
			case ControlType::PS4:
				for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
					if (strcmpDx(DS4Str[i], Str) == 0) {
						return DS4ID[i];
					}
				}
				break;
			case ControlType::PC:
				for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
					if (strcmpDx(KeyStr[i], Str) == 0) {
						return KeyID[i];
					}
				}
				break;
			default:
				break;
			}
			return InvalidID;
		}
		//KEY_INPUT_BACK�Ȃǂ�ID���當�����擾
		const std::string	GetIDtoStr(int ID) const noexcept {
			switch (m_ControlType) {
			case ControlType::XBox:
				for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
					if (XBoxID[i] == ID) {
						return XBoxStr[i];
					}
				}
				break;
			case ControlType::PS4:
				for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
					if (DS4ID[i] == ID) {
						return DS4Str[i];
					}
				}
				break;
			case ControlType::PC:
				for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
					if (KeyID[i] == ID) {
						return KeyStr[i];
					}
				}
				break;
			default:
				break;
			}
			return "NONE";
		};
		bool GetButtonPress(int ID) noexcept {
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
						if (GetWindowActiveFlag()) {
							return ((GetMouseInput() & (ID & 0xFF)) != 0);
						}
						else {
							return 0;
						}
					}
					else {
						if (GetWindowActiveFlag()) {
							return (CheckHitKey(ID) != 0);
						}
						else {
							return 0;
						}
					}
				}
				break;
			default:
				break;
			}
			return false;
		}
		//�L�[�R���t�B�O�̊e�{�^���̐ݒ�
		void ChangeConfigOnce(PADS select, int SetID) noexcept {
			// ���ɓK�p�ς̂��̂��������ꍇ�����𖳌������Ă��
			for (size_t p = 0; p < static_cast<size_t>(PADS::MAX); ++p) {
				if (m_PadsInfo.at(p).GetReserve() == SetID) {
					m_PadsInfo.at(p).SetReserve(InvalidID);
					break;
				}
			}
			//�ݒ�
			m_PadsInfo.at(static_cast<size_t>(select)).SetReserve(SetID);
		}
	public:
		//FPS�Ȃǂ̃}�E�X��\�����Ȃ�������@��p���邩�ǂ����w��
		void SetMouseMoveEnable(bool value) noexcept { m_MouseMoveEnable = value; }
		//�L�[�R���t�B�O�Ƃ���Reserve�Ɏb��l�����鏈��
		//�����ʂ�ԁA�����L�[�������Ǝ��g�̃L�[���O���B�L�[�A�T�C�����O��Ă��邩�Ⴄ�L�[�������Ƃ��̃L�[��ݒ肷��
		bool ChangeConfig(PADS select) noexcept {
			auto Prev = GetPadsInfo(select).GetReserve();

			//�ύX�O�Ɠ����L�[�������Ă�
			if (GetPadsInfo(select).GetKey().press()) {
				if (GetPadsInfo(select).GetKey().trigger()) {//�������u�Ԃ���
					// SetID��select�Őݒ肵�Ă���{�^����������������=> SetID==Assign
					// Reserve�ɉ��������Ă������ɁA�����Ă��Ȃ�������Assign�Ƃ���
					m_PadsInfo.at(static_cast<size_t>(select)).SetReserve(GetPadsInfo(select).IsEnableSelectReserve() ? InvalidID : GetPadsInfo(select).GetAssign());
					//���ƈႤ�ݒ�ɂȂ��Ă�����true
					return true;
				}
				//���������Ă���ꍇ�͈ȉ��̏�����ʂ��Ȃ�
				return false;
			}
			//������Ă����炻�̃L�[�ɐݒ肷��
			switch (m_ControlType) {
			case ControlType::XBox:
				for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
					if (GetButtonPress(XBoxID[i])) {
						ChangeConfigOnce(select, XBoxID[i]);
						break;
					}
				}
				break;
			case ControlType::PS4:
				for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
					if (GetButtonPress(DS4ID[i])) {
						ChangeConfigOnce(select, DS4ID[i]);
						break;
					}
				}
				break;
			case ControlType::PC:
				for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
					if (GetButtonPress(KeyID[i])) {
						ChangeConfigOnce(select, KeyID[i]);
						break;
					}
				}
				break;
			default:
				break;
			}
			//���ƈႤ�ݒ�ɂȂ��Ă�����true
			return (Prev != GetPadsInfo(select).GetReserve());
		}
		//�K�p���Ă��Ȃ��ύX�̃��Z�b�g
		void ResetAssign(void) noexcept {
			for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
				this->m_PadsInfo.at(i).ResetAssign();
			}
		}
		//�K�p���Ă��Ȃ��ύX��K�p
		void FlipAssign(void) noexcept {
			for (size_t i = 0; i < static_cast<size_t>(PADS::MAX); ++i) {
				this->m_PadsInfo.at(i).FlipAssign();
			}
		}
	public:
		//����̃t�@�C�����獡�ݒ肳��Ă���L�[�R���t�B�O��ǂݎ��
		void Load(void) noexcept;
		//���ݒ肵�Ă����Ԃ��t�@�C���ɕۑ�
		void Save(void) const noexcept;
	public:
		//���͏����ƃR���g���[���^�C�v�̍X�V
		void Update(void) noexcept;
	};
};
