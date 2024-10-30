#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// �Ή�����Q�[���p�b�h�A�L�[�A�T�C��
	// --------------------------------------------------------------------------------------------------
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
	// �E�B���h�E�A�N�e�B�u�`�F�b�N�t���L�[����
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	static int CheckHitKeyWithCheck(int KeyCode) noexcept {
		if (GetWindowActiveFlag()) {
			return CheckHitKey(KeyCode);
		}
		else {
			return 0;
		}
	}
	static int GetMouseInputWithCheck(void) noexcept {
		if (GetWindowActiveFlag()) {
			return GetMouseInput();
		}
		else {
			return 0;
		}
	}
	static int GetMouseWheelRotVolWithCheck(int CounterReset = TRUE) noexcept {
		if (GetWindowActiveFlag()) {
			return GetMouseWheelRotVol(CounterReset);
		}
		else {
			GetMouseWheelRotVol(CounterReset);
			return 0;
		}
	}

	// �L�[��������
	class switchs {
		bool		m_on{ false };// �I���I�t����
		bool		m_press{ false };// �I���I�t����
		bool		m_repeat{ false };// �I���I�t����
		int8_t		m_presscount{ 0 };// �v�b�V������
		float		m_repeatcount{ FrameRate / 2.f };// �v�b�V������
	public:
		switchs(void) noexcept {
			Set(false);
			m_presscount = 0;
			m_repeatcount = FrameRate / 2.f;
			m_press = false;
		}
		~switchs(void) noexcept {}
		// �g�p�O�̗p��
		void			Set(bool on) noexcept { m_on = on; }
		// �X�V
		void			Execute(bool key) noexcept;
		// �I���I�t�̎擾
		bool			on(void) const noexcept { return m_on; }
		// �������u��
		bool			trigger(void) const noexcept { return m_press && (m_presscount == 1); }
		// �����Ă����
		bool			press(void) const noexcept { return m_press; }
		// �����Ă����(30F+2F��)
		bool			repeat(void) const noexcept { return m_repeat; }
		// �������u��
		bool			release_trigger(void) const noexcept { return (!m_press) && (m_presscount == 1); }
		// �����Ă����
		bool			release(void) const noexcept { return !m_press; }
	};
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------

	class PadControl : public SingletonBase<PadControl> {
	private:
		friend class SingletonBase<PadControl>;
	private:
		class KeyGuideGraphs {
		public:
			int xsize{ 0 }, ysize{ 0 };
			GraphHandle* pGuideImg{ nullptr };
			std::string GuideString;
		public:
			KeyGuideGraphs(void) noexcept {}
			KeyGuideGraphs(const KeyGuideGraphs&) = delete;
			KeyGuideGraphs(KeyGuideGraphs&& o) = delete;
			KeyGuideGraphs& operator=(const KeyGuideGraphs&) = delete;
			KeyGuideGraphs& operator=(KeyGuideGraphs&& o) = delete;

			~KeyGuideGraphs(void) noexcept {}
		public:
			void AddGuideXBox(GraphHandle* pGuide, const std::string& GuideStr) noexcept;
			void AddGuideDS4(GraphHandle* pGuide, const std::string& GuideStr) noexcept;
			void AddGuidePC(GraphHandle* pGuide, const std::string& GuideStr) noexcept;
			void Reset(void) noexcept {
				pGuideImg = nullptr;
				GuideString = "";
			}
			int GetDrawSize(void) const noexcept;
			int Draw(int x, int y) const noexcept;
		};

		class PadsInfo {
		public:
			switchs m_Key;
			int		m_assign = 0;
			int		m_reserve = 0;
			bool	m_IsUse{ true };
		};
	private:
		// �{�^��
		std::array<PadsInfo, static_cast<int>(PADS::MAX)> m_PadsInfo;
		// �E�X�e�B�b�N
		float Look_XradAdd{ 0.f };
		float Look_YradAdd{ 0.f };
		// 
		int MouseX{ 0 };
		int MouseY{ 0 };
		switchs MouseClick;
		switchs KeyEsc;
		switchs KeyBSorDel;
		int MouseWheelRot{ 0 };
		// 
		std::array<switchs, 26>	m_AtoZKey;
		std::array<switchs, 10>	m_NumKey;
		// 
		GraphHandle GuideBase;
		std::vector<GraphHandle> GuideRect;
		// �K�C�h���̃R���g���[��
		bool		m_IsUpdate{ true };
		bool		m_MouseMoveEnable{ true };
		ControlType	m_ControlType{ ControlType::PC };
	private:
		std::vector<std::unique_ptr<KeyGuideGraphs>>	Key;
	public:
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
	private:
		int			GetStrtoID(const char* Str) const noexcept {
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
	private:
		// �R���X�g���N�^
		PadControl(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		PadControl(const PadControl&) = delete;
		PadControl(PadControl&& o) = delete;
		PadControl& operator=(const PadControl&) = delete;
		PadControl& operator=(PadControl&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		const auto& GetLS_X(void) const noexcept { return Look_XradAdd; }
		const auto& GetLS_Y(void) const noexcept { return Look_YradAdd; }

		const auto& GetMS_X(void) const noexcept { return MouseX; }
		const auto& GetMS_Y(void) const noexcept { return MouseY; }
		const auto& GetMouseClick(void) const noexcept { return MouseClick; }
		const auto& GetEsc(void) const noexcept { return KeyEsc; }
		const auto& GetBSorDel(void) const noexcept { return KeyBSorDel; }
		const auto& GetMouseWheelRot(void) const noexcept { return MouseWheelRot; }

		const auto& IsUseButton(PADS select) const noexcept { return m_PadsInfo.at(static_cast<size_t>(select)).m_IsUse; }
		const auto& GetKey(PADS select) const noexcept { return m_PadsInfo.at(static_cast<size_t>(select)).m_Key; }
		const auto& GetAtoZKey(char AtoZ) const noexcept {
			if ('A' <= AtoZ && AtoZ <= 'Z') {
				return this->m_AtoZKey.at(static_cast<size_t>(AtoZ - 'A'));
			}
			if ('a' <= AtoZ && AtoZ <= 'z') {
				return this->m_AtoZKey.at(static_cast<size_t>(AtoZ - 'a'));
			}
			return this->m_AtoZKey.at(0);
		}
		const auto& GetNumKey(int Num) const noexcept {
			if (0 <= Num && Num <= 9) {
				return this->m_NumKey.at(Num);
			}
			return this->m_NumKey.at(0);
		}
		const auto& GetKeyassign(PADS select) const noexcept { return m_PadsInfo.at(static_cast<size_t>(select)).m_assign; }
		const auto GetKeyStr(PADS select) const noexcept { return GetIDtoStr(m_PadsInfo.at(static_cast<size_t>(select)).m_assign); }
		const auto& GetKeyReserve(PADS select) const noexcept { return m_PadsInfo.at(static_cast<size_t>(select)).m_reserve; }

		void SetIsUseButton(PADS select, bool value) noexcept { m_PadsInfo.at(static_cast<size_t>(select)).m_IsUse = value; }
		void SetKeyReserve(PADS select, int assign) noexcept { m_PadsInfo.at(static_cast<size_t>(select)).m_reserve = assign; }

		bool GetButtonPress(int ID) noexcept;
		void GetPushAnySwitch(PADS select) noexcept;

		bool GetPushAnySwitchLocal(PADS select, int ID) noexcept;
	public:
		void SetMouseMoveEnable(bool value) noexcept { m_MouseMoveEnable = value; }
		void SetGuideUpdate(void) noexcept { m_IsUpdate = true; }

		void Save(void) noexcept;
		void Load(void) noexcept;

		void ResetAssign(void) noexcept;
		void FlipAssign(void) noexcept;

		void ChangeGuide(std::function<void()>Guide_Pad) noexcept;

		void AddGuide(PADS select, const std::string& GuideStr, ControlType FixedControlType = ControlType::Max) noexcept {
			Key.emplace_back(std::make_unique<KeyGuideGraphs>());
			switch ((FixedControlType != ControlType::Max) ? m_ControlType : FixedControlType) {
			case ControlType::XBox:
				for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
					if (XBoxID[i] == m_PadsInfo.at(static_cast<size_t>(select)).m_assign) {
						Key.back()->AddGuideXBox(&GuideRect.at(i + KeyNum + XBoxNum), GuideStr);
						return;
					}
				}
				break;
			case ControlType::PS4:
				for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
					if (DS4ID[i] == m_PadsInfo.at(static_cast<size_t>(select)).m_assign) {
						Key.back()->AddGuideDS4(&GuideRect.at(i + KeyNum), GuideStr);
						return;
					}
				}
				break;
			case ControlType::PC:
				for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
					if (KeyID[i] == m_PadsInfo.at(static_cast<size_t>(select)).m_assign) {
						Key.back()->AddGuidePC(&GuideRect.at(i), GuideStr);
						return;
					}
				}
				break;
			default:
				break;
			}
			Key.back()->AddGuidePC(nullptr, GuideStr);
		}
		void Reset(void) noexcept {
			for (auto& k : Key) {
				k->Reset();
				k.reset();
			}
			Key.clear();
		}
	public:
		void Update(void) noexcept;
		void Draw(void) const noexcept;
	};

	// ����
	class InputControl {
	private:
		float			m_yRad{ 0.f };
		unsigned long long	m_Flags{ 0 };
	public:
		InputControl(void) noexcept {}
		InputControl(const InputControl& tgt) noexcept {
			this->m_yRad = tgt.m_yRad;
			this->m_Flags = tgt.m_Flags;
		}
		InputControl(InputControl&& tgt) noexcept {
			this->m_yRad = tgt.m_yRad;
			this->m_Flags = tgt.m_Flags;
		}
		// InputControl& operator=(const InputControl&) = delete;
		// InputControl& operator=(InputControl&& o) = delete;

		virtual ~InputControl(void) noexcept {}
	public:
		void			ResetAllInput(void) noexcept {
			this->m_yRad = 0.f;
			this->m_Flags = 0;
		}
		void			SetyRad(float yRad) noexcept { this->m_yRad = yRad; }
		void			SetInputPADS(PADS select, bool value) noexcept {
			if (value) { this->m_Flags |= ((unsigned long long)1 << (0 + static_cast<int>(select))); }
			else{ this->m_Flags &= ~((unsigned long long)1 << (0 + static_cast<int>(select))); }
		}
	public:
		const auto& GetyRad(void) const noexcept { return this->m_yRad; }
		auto		GetPADSPress(PADS select) const noexcept { return (this->m_Flags & ((unsigned long long)1 << (0 + static_cast<int>(select)))) != 0; }
	};
};
