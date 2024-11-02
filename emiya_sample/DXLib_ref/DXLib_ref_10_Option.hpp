#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	enum class EnumSaveParam {
		GraphicsPreset,
		DirectXVer,
		Bloom,
		Shadow,
		VSync,
		FpsLimit,
		SE,
		BGM,
		WindowMode,
		ScreenEffect,
		MotionBlur,
		Xsensing,
		Ysensing,
		ControlType,
		Max,
	};
	static const char* OptionStr[static_cast<int>(EnumSaveParam::Max)] = {
		"GraphicsPreset",
		"DirectXVer",
		"Bloom",
		"Shadow",
		"VSync",
		"FpsLimit",
		"SE",
		"BGM",
		"WindowMode",
		"ScreenEffect",
		"MotionBlur",
		"Xsensing",
		"Ysensing",
		"ControlType",
	};

	enum class EnumProjectSettingParam {
		Distortion,
		Max,
	};
	static const char* ProjectSettingStr[static_cast<int>(EnumProjectSettingParam::Max)] = {
		"Distortion",
	};
	enum class EnumParamType {
		Boolean,
		Int,
		Float,
		Else,
	};
	// 
	static const int DirectXVerID[] = {
		DX_DIRECT3D_9EX,
		DX_DIRECT3D_11,
	};
	static const char* DirectXVerStr[] = {
	"9c",
	"11",
	};

	enum class WindowType :int {
		None = 0,
		Window = None,
		Borderless,
		FullScreen,
		Max,
	};

	struct SaveParams {
	private:
		EnumParamType m_EnumParamType{};
		int valueint{ 0 };
	public:
		void SetEnumParamType(EnumParamType value) noexcept { m_EnumParamType = value; }
		auto GetEnumParamType(void) const noexcept { return m_EnumParamType; }

		void SetBoolean(bool use) noexcept { valueint = use ? 1 : 0; }
		auto GetBoolean(void) const noexcept { return (valueint != 0); }
		void ChangeBoolean(void) noexcept { SetBoolean(GetBoolean() ^ 1); }

		void SetInt(int use) noexcept { valueint = use; }
		auto GetInt(void) const noexcept { return valueint; }

		void SetFloat(float use) noexcept { valueint = static_cast<int>(use * 1000.f); }
		auto GetFloat(void) const noexcept { return static_cast<float>(valueint) / 1000.f; }

	};

	class OPTION : public SingletonBase<OPTION> {
	private:
		friend class SingletonBase<OPTION>;
	private:
		// コンストラクタ
		OPTION(void) noexcept {
			Load();
		}
		OPTION(const OPTION&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		OPTION(OPTION&& o) = delete;
		OPTION& operator=(const OPTION&) = delete;
		OPTION& operator=(OPTION&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	private:
		std::array<SaveParams, static_cast<int>(EnumSaveParam::Max)> m_SaveParams;
		std::array<SaveParams, static_cast<int>(EnumProjectSettingParam::Max)> m_ProjectSettingParams;
	public:
		auto			GetParamBoolean(EnumSaveParam id)const noexcept { return m_SaveParams.at(static_cast<size_t>(id)).GetBoolean(); }
		auto			GetParamInt(EnumSaveParam id)const noexcept { return m_SaveParams.at(static_cast<size_t>(id)).GetInt(); }
		auto			GetParamFloat(EnumSaveParam id)const noexcept { return m_SaveParams.at(static_cast<size_t>(id)).GetFloat(); }
	public:
		void			SetParamBoolean(EnumSaveParam id, bool use) noexcept { m_SaveParams.at(static_cast<size_t>(id)).SetBoolean(use); }
		void			ChangeParamBoolean(EnumSaveParam id) noexcept { m_SaveParams.at(static_cast<size_t>(id)).ChangeBoolean(); }

		void			SetParamInt(EnumSaveParam id, int use) noexcept { m_SaveParams.at(static_cast<size_t>(id)).SetInt(use); }
		void			SetParamFloat(EnumSaveParam id, float use) noexcept { m_SaveParams.at(static_cast<size_t>(id)).SetFloat(use); }
	public:
		auto			GetParamBoolean(EnumProjectSettingParam id)const noexcept { return m_ProjectSettingParams.at(static_cast<size_t>(id)).GetBoolean(); }
		auto			GetParamInt(EnumProjectSettingParam id)const noexcept { return m_ProjectSettingParams.at(static_cast<size_t>(id)).GetInt(); }
		auto			GetParamFloat(EnumProjectSettingParam id)const noexcept { return m_ProjectSettingParams.at(static_cast<size_t>(id)).GetFloat(); }
	public:
		void			SetParamBoolean(EnumProjectSettingParam id, bool use) noexcept { m_ProjectSettingParams.at(static_cast<size_t>(id)).SetBoolean(use); }
		void			SetParamInt(EnumProjectSettingParam id, int use) noexcept { m_ProjectSettingParams.at(static_cast<size_t>(id)).SetInt(use); }
		void			SetParamFloat(EnumProjectSettingParam id, float use) noexcept { m_ProjectSettingParams.at(static_cast<size_t>(id)).SetFloat(use); }
	public:
		void			Load(void) noexcept;
		void			Save(void) const noexcept;
	};

	class OptionWindowClass : public SingletonBase<OptionWindowClass> {
	private:
		friend class SingletonBase<OptionWindowClass>;
	private:
		class OptionElementsInfo {
		private:
			std::string m_Name;
			std::string m_Info;
			std::function<void()> m_LeftPush;
			std::function<void()> m_RightPush;
			std::function<void()> m_OKPush;
			std::function<void()> m_AnyDoing;
			std::function<void(int xpos, int ypos, bool isMine)> m_Draw;
		public:
			float selanim{ 0.f };
		public:
			const auto& GetName(void) const noexcept { return m_Name; }
			const auto& GetInfoText(void) const noexcept { return m_Info; }

			void GetLeftPush(void) const noexcept { m_LeftPush(); }
			void GetRightPush(void) const noexcept { m_RightPush(); }
			void GetOKPush(void) const noexcept { m_OKPush(); }
			void GetAnyDoing(void) const noexcept { m_AnyDoing(); }
		public:
			void Init(const char* name, const char* infoText, std::function<void()> LeftPush, std::function<void()> RightPush, std::function<void()> OKPush,
				std::function<void()> AnyDoing,
				std::function<void(int xpos, int ypos, bool isMine)> draw) noexcept {
				selanim = 0;

				m_Name = name;
				m_Info = infoText;
				m_LeftPush = LeftPush;
				m_RightPush = RightPush;
				m_OKPush = OKPush;
				m_AnyDoing = AnyDoing;
				m_Draw = draw;
			}
			void Draw(int xpos, int ypos, bool isMine) const noexcept;
		};
		class OptionTabsInfo {
		private:
		protected:
			int m_id{ 0 };
			std::string m_name;
			std::vector<OptionElementsInfo> m_Elements;
		protected:
			virtual void Init_Sub(void) noexcept {}
		public:
			const auto& GetID(void) const noexcept { return m_id; }
		public:
			OptionTabsInfo(void) noexcept {}
			virtual ~OptionTabsInfo(void) noexcept {}
		public:
			void Init(int ID, const char* name) noexcept {
				m_id = ID;
				m_name = name;
				Init_Sub();
			}
			void Update(int* select, bool CanPress) noexcept;
			void Draw(int xpos, int ypos, bool isActive, int* TabSel, int* select) noexcept;

			void DrawInfo(int xpos, int ypos, int select) noexcept;
		};

		class SoundTabsInfo :public OptionTabsInfo {
		public:
			SoundTabsInfo(void) noexcept {}
			virtual ~SoundTabsInfo(void) noexcept {}
		protected:
			void Init_Sub(void) noexcept override;
		};
		class GraphicTabsInfo :public OptionTabsInfo {
			static const int	FrameLimitsNum = 10;
			const int	FrameLimits[FrameLimitsNum] = {
				30,
				60,
				75,
				90,
				120,
				144,
				240,
				300,
				360,
				1000,
			};
		private:
			int RefreshRate{ FrameLimits[1] };
		public:
			GraphicTabsInfo(void) noexcept {}
			GraphicTabsInfo(const GraphicTabsInfo&) = delete;
			GraphicTabsInfo(GraphicTabsInfo&& o) = delete;
			GraphicTabsInfo& operator=(const GraphicTabsInfo&) = delete;
			GraphicTabsInfo& operator=(GraphicTabsInfo&& o) = delete;

			virtual ~GraphicTabsInfo(void) noexcept {}
		protected:
			void Init_Sub(void) noexcept override;
		};
		class ControlTabsInfo :public OptionTabsInfo {
		public:
			ControlTabsInfo(void) noexcept {}
			virtual ~ControlTabsInfo(void) noexcept {}
		protected:
			void Init_Sub(void) noexcept override;
		private:
			void KeyDraw(int xpos, int ypos, bool isMine, PADS Sel) noexcept;
		};
		class ElseTabsInfo :public OptionTabsInfo {
		public:
			ElseTabsInfo(void) noexcept {}
			virtual ~ElseTabsInfo(void) noexcept {}
		protected:
			void Init_Sub(void) noexcept override;
		};
	private:
		int m_tabsel{ 0 };
		int m_select{ 0 };
		std::array<std::unique_ptr<OptionTabsInfo>, 4> m_Tabs;
		bool						m_Active{ false };
		bool						m_ActiveSwitch{ false };
		bool						m_RestartSwitch{ false };
	public:
		void SetRestart(void) noexcept { m_RestartSwitch = true; }
		void SetActive(void) noexcept { m_ActiveSwitch = true; }
		auto IsRestartSwitch(void) noexcept {
			if (!m_Active && m_RestartSwitch) {
				m_RestartSwitch = false;
				return true;
			}
			return false;
		}
		const auto& IsActive(void) const noexcept { return m_Active; }
	private:
		// コンストラクタ
		OptionWindowClass(void) noexcept {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		OptionWindowClass(const OptionWindowClass&) = delete;
		OptionWindowClass(OptionWindowClass&& o) = delete;
		OptionWindowClass& operator=(const OptionWindowClass&) = delete;
		OptionWindowClass& operator=(OptionWindowClass&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		void Init(void) noexcept;
		void Update(void)noexcept;
	};
}
