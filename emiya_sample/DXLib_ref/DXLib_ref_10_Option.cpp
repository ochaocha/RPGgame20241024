#include "DXLib_ref_10_Option.hpp"

#include <fstream>

namespace DXLibRef {
	// シングルトンの実態定義
	const OPTION* SingletonBase<OPTION>::m_Singleton = nullptr;
	const OptionWindowClass* SingletonBase<OptionWindowClass>::m_Singleton = nullptr;
	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	void			OPTION::Load(void) noexcept {
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::GraphicsPreset)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::DirectXVer)).SetEnumParamType(EnumParamType::Else);// DirectXVer
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::bloom)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::shadow)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::vsync)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::FpsLimit)).SetEnumParamType(EnumParamType::Int);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::SE)).SetEnumParamType(EnumParamType::Float);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::BGM)).SetEnumParamType(EnumParamType::Float);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::WindowMode)).SetEnumParamType(EnumParamType::Int);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::ScreenEffect)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::MotionBlur)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::Xsensing)).SetEnumParamType(EnumParamType::Float);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::Ysensing)).SetEnumParamType(EnumParamType::Float);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::ControlType)).SetEnumParamType(EnumParamType::Else);// ControlType

		m_ProjectSettingParams.at(static_cast<size_t>(EnumProjectSettingParam::Distortion)).SetEnumParamType(EnumParamType::Boolean);

		// SetOutApplicationLogValidFlag(FALSE);
		{
			FileStreamDX FileStream;
			//セーブしたデータ
			if (IsFileExist("Save/Setting.txt")) {
				FileStream.Open("Save/Setting.txt");
			}
			//新規データでプロジェクト側にデフォルト値設定がある場合
			else if (IsFileExist("data/Setting.txt")) {
				FileStream.Open("data/Setting.txt");
			}
			//新規データ
			else {
				// デフォ値
				SetParamInt(EnumSaveParam::GraphicsPreset, TRUE);
				SetParamInt(EnumSaveParam::DirectXVer, 1);
				SetParamBoolean(EnumSaveParam::bloom, true);
				SetParamBoolean(EnumSaveParam::shadow, true);
				SetParamBoolean(EnumSaveParam::vsync, true);
				SetParamInt(EnumSaveParam::FpsLimit, 60);
				SetParamFloat(EnumSaveParam::SE, 0.5f);
				SetParamFloat(EnumSaveParam::BGM, 0.5f);
				SetParamBoolean(EnumSaveParam::WindowMode, static_cast<int>(WindowType::Borderless));
				SetParamBoolean(EnumSaveParam::ScreenEffect, true);
				SetParamBoolean(EnumSaveParam::MotionBlur, false);
				SetParamFloat(EnumSaveParam::Xsensing, 0.5f);
				SetParamFloat(EnumSaveParam::Ysensing, 0.5f);
				SetParamInt(EnumSaveParam::ControlType, 2);
				return;

			}
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL == "") {
					continue;
				}
				auto LEFT = FileStreamDX::getleft(ALL);
				auto RIGHT = FileStreamDX::getright(ALL);
				for (size_t loop = 0; loop < static_cast<int>(EnumSaveParam::Max); ++loop) {
					if (LEFT != OptionStr[loop]) {
						continue;
					}
					switch (m_SaveParams.at(loop).GetEnumParamType()) {
					case EnumParamType::Boolean:
						SetParamBoolean((EnumSaveParam)loop, (RIGHT.find("true") != std::string::npos));
						break;
					case EnumParamType::Int:
						if (std::all_of(RIGHT.cbegin(), RIGHT.cend(), isdigit)) {
							SetParamInt((EnumSaveParam)loop, std::stoi(RIGHT));
						}
						else {
							SetParamInt((EnumSaveParam)loop, 0);
						}
						break;
					case EnumParamType::Float:
						SetParamFloat((EnumSaveParam)loop, std::stof(RIGHT));// todo 小数か確認
						break;
					case EnumParamType::Else:
						if (loop == static_cast<size_t>(EnumSaveParam::DirectXVer)) {
							for (size_t i = 0; i < 2; ++i) {
								if (RIGHT == DirectXVerStr[i]) {
									SetParamInt((EnumSaveParam)loop, static_cast<int>(i));
									break;
								}
							}
						}
						else if (loop == static_cast<size_t>(EnumSaveParam::ControlType)) {
							for (size_t i = 1; i < 3; ++i) {
								if (RIGHT == ControlTypeStr[i]) {
									SetParamInt((EnumSaveParam)loop, static_cast<int>(i));
									break;
								}
							}
						}
						break;
					default:
						break;
					}
					break;
				}
			}
		}
		{
			if (!IsFileExist("CommonData/ProjectSetting.txt")) {
				SetParamBoolean(EnumProjectSettingParam::Distortion, true);
				return;
			}
			FileStreamDX FileStream("CommonData/ProjectSetting.txt");
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL == "") {
					continue;
				}
				auto LEFT = FileStreamDX::getleft(ALL);
				auto RIGHT = FileStreamDX::getright(ALL);
				for (size_t loop = 0; loop < static_cast<size_t>(EnumProjectSettingParam::Max); ++loop) {
					if (LEFT != ProjectSettingStr[loop]) {
						continue;
					}
					switch (m_ProjectSettingParams.at(loop).GetEnumParamType()) {
					case EnumParamType::Boolean:
						SetParamBoolean((EnumProjectSettingParam)loop, (RIGHT.find("true") != std::string::npos));
						break;
					case EnumParamType::Int:
						if (std::all_of(RIGHT.cbegin(), RIGHT.cend(), isdigit)) {
							SetParamInt((EnumProjectSettingParam)loop, std::stoi(RIGHT));
						}
						else {
							SetParamInt((EnumProjectSettingParam)loop, 0);
						}
						break;
					case EnumParamType::Float:
						SetParamFloat((EnumProjectSettingParam)loop, std::stof(RIGHT));// todo 小数か確認
						break;
					case EnumParamType::Else:
						break;
					default:
						break;
					}
					break;
				}
			}
		}
		// SetOutApplicationLogValidFlag(TRUE);
	}

	void			OPTION::Save(void) const noexcept {
		std::ofstream outputfile("Save/Setting.txt");

		for (int loop = 0; loop < static_cast<int>(EnumSaveParam::Max); ++loop) {
			switch (m_SaveParams.at(static_cast<size_t>(loop)).GetEnumParamType()) {
			case EnumParamType::Boolean:
				outputfile << std::string(OptionStr[loop]) + "=" + std::string(GetParamBoolean((EnumSaveParam)loop) ? "true" : "false") + "\n";
				break;
			case EnumParamType::Int:
				outputfile << std::string(OptionStr[loop]) + "=" + std::to_string(GetParamInt((EnumSaveParam)loop)) + "\n";
				break;
			case EnumParamType::Float:
				outputfile << std::string(OptionStr[loop]) + "=" + std::to_string(GetParamFloat((EnumSaveParam)loop)) + "\n";
				break;
			case EnumParamType::Else:
				if (loop == static_cast<int>(EnumSaveParam::DirectXVer)) {
					outputfile << std::string(OptionStr[loop]) + "=" + std::string(DirectXVerStr[GetParamInt((EnumSaveParam)loop)]) + "\n";
				}
				else if (loop == static_cast<int>(EnumSaveParam::ControlType)) {
					outputfile << std::string(OptionStr[loop]) + "=" + std::string(ControlTypeStr[GetParamInt((EnumSaveParam)loop)]) + "\n";
				}
				break;
			default:
				break;
			}
		}

		outputfile.close();
	}

	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	// 
	void OptionWindowClass::OptionElementsInfo::Draw(int xpos, int ypos, bool isMine) const noexcept {
		auto* DrawParts = DXDraw::Instance();
		ypos += DrawParts->GetUIY(static_cast<int>(selanim));
		UISystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, UISystem::FontXCenter::LEFT, isMine ? White : Gray50, Black, m_Name);
		m_Draw(xpos + DrawParts->GetUIY(720 - 324), ypos, isMine);
	}
	// 
	void OptionWindowClass::OptionTabsInfo::Update(int* select, bool CanPress) noexcept {
		if ((*select) < 0) {
			return;
		}
		auto* SoundParts = SoundPool::Instance();
		auto* Pad = PadControl::Instance();
		m_Elements.at(static_cast<size_t>(*select)).GetAnyDoing();
		if (CanPress) {
			if (Pad->GetPadsInfo(PADS::MOVE_W).GetKey().trigger()) {
				--(*select);
				if ((*select) < 0) {
					(*select) = static_cast<int>(m_Elements.size()) - 1;
				}
				m_Elements.at(static_cast<size_t>(*select)).selanim = 3.f;
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
			if (Pad->GetPadsInfo(PADS::MOVE_S).GetKey().trigger()) {
				++(*select);
				if ((*select) > static_cast<int>(m_Elements.size()) - 1) {
					(*select) = 0;
				}
				m_Elements.at(static_cast<size_t>(*select)).selanim = -3.f;
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
			if (Pad->GetPadsInfo(PADS::MOVE_A).GetKey().repeat()) {
				m_Elements.at(static_cast<size_t>(*select)).GetLeftPush();
			}
			if (Pad->GetPadsInfo(PADS::MOVE_D).GetKey().repeat()) {
				m_Elements.at(static_cast<size_t>(*select)).GetRightPush();
			}
			if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || Pad->GetMouseClick().trigger()) {
				m_Elements.at(static_cast<size_t>(*select)).GetOKPush();
			}
		}
		for (auto& e : m_Elements) {
			Easing(&e.selanim, 0.f, 0.95f, EasingType::OutExpo);
		}
	}
	void OptionWindowClass::OptionTabsInfo::Draw(int xpos, int ypos, bool isActive, int* TabSel, int* select) noexcept {
		auto* DrawParts = DXDraw::Instance();
		int xp1, yp1;
		// タブ
		{
			xp1 = xpos + (DrawParts->GetUIY(140) + DrawParts->GetUIY(12)) * m_id;
			yp1 = ypos;
			if (UISystem::SetMsgClickBox(xp1, yp1 + DrawParts->GetUIY(5), xp1 + DrawParts->GetUIY(140), yp1 + LineHeight * 2 - DrawParts->GetUIY(5), LineHeight, (isActive ? Gray25 : Gray75), false, true, m_name)) {
				*TabSel = GetID();
				*select = 0;
				auto* SoundParts = SoundPool::Instance();
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
		}
		// 内容
		if (isActive) {
			xp1 = xpos;
			yp1 = ypos + LineHeight * 2;
			for (int i = 0; i < static_cast<int>(m_Elements.size()); ++i) {
				yp1 += (LineHeight + DrawParts->GetUIY(6));

				auto* Pad = PadControl::Instance();
				if (HitPointToRectangle(Pad->GetMS_X(), Pad->GetMS_Y(), xp1, yp1, xp1 + DrawParts->GetUIY(500), yp1 + (LineHeight + DrawParts->GetUIY(6)))) {
					*select = i;
				}
				m_Elements.at(static_cast<size_t>(i)).Draw(xp1, yp1, (*select == i));
			}
		}
	}
	void OptionWindowClass::OptionTabsInfo::DrawInfo(int xpos, int ypos, int select) noexcept {
		UISystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, UISystem::FontXCenter::LEFT, White, Black, m_Elements.at(static_cast<size_t>(select)).GetInfoText().c_str());
	}
	// 
	void OptionWindowClass::SoundTabsInfo::Init_Sub(void) noexcept {
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("BGM", "BGMボリュームを変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::BGM, Clamp(OptionParts->GetParamFloat(EnumSaveParam::BGM) - 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->UpdateVolume();
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::BGM, Clamp(OptionParts->GetParamFloat(EnumSaveParam::BGM) + 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->UpdateVolume();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* DrawParts = DXDraw::Instance();
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				int value = UISystem::UpDownBar(xpos, xpos + DrawParts->GetUIY(200), ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::BGM) * 100.f + 0.5f), 0, 100);
				OptionParts->SetParamFloat(EnumSaveParam::BGM, static_cast<float>(value) / 100.f);
				SoundParts->UpdateVolume();
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("SE", "SEボリュームを変更します",
			[]() {
				auto* SoundParts = SoundPool::Instance();
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::SE, Clamp(OptionParts->GetParamFloat(EnumSaveParam::SE) - 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->UpdateVolume();
			},
			[]() {
				auto* SoundParts = SoundPool::Instance();
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::SE, Clamp(OptionParts->GetParamFloat(EnumSaveParam::SE) + 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->UpdateVolume();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* DrawParts = DXDraw::Instance();
				auto* SoundParts = SoundPool::Instance();
				auto* OptionParts = OPTION::Instance();
				int value = UISystem::UpDownBar(xpos, xpos + DrawParts->GetUIY(200), ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::SE) * 100.f + 0.5f), 0, 100);
				OptionParts->SetParamFloat(EnumSaveParam::SE, static_cast<float>(value) / 100.f);
				SoundParts->UpdateVolume();
			}
		);
	}
	void OptionWindowClass::GraphicTabsInfo::Init_Sub(void) noexcept {
		HDC hdc;
		hdc = GetDC(GetMainWindowHandle());	// デバイスコンテキストの取得
		RefreshRate = GetDeviceCaps(hdc, VREFRESH);	// リフレッシュレートの取得
		ReleaseDC(GetMainWindowHandle(), hdc);	// デバイスコンテキストの解放

		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Graphics Preset", "グラフィック設定を一律設定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::GraphicsPreset);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				if (!OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
					OptionParts->SetParamBoolean(EnumSaveParam::shadow, false);
					OptionParts->SetParamBoolean(EnumSaveParam::bloom, false);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, false);
				}
				else {
					OptionParts->SetParamBoolean(EnumSaveParam::shadow, true);
					OptionParts->SetParamBoolean(EnumSaveParam::bloom, true);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, true);
				}
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::GraphicsPreset);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				if (!OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
					OptionParts->SetParamBoolean(EnumSaveParam::shadow, false);
					OptionParts->SetParamBoolean(EnumSaveParam::bloom, false);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, false);
				}
				else {
					OptionParts->SetParamBoolean(EnumSaveParam::shadow, true);
					OptionParts->SetParamBoolean(EnumSaveParam::bloom, true);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, true);
				}
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto prev = OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset);
				OptionParts->SetParamBoolean(EnumSaveParam::GraphicsPreset, UISystem::CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)));
				if (prev != OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
					if (!OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
						OptionParts->SetParamBoolean(EnumSaveParam::shadow, false);
						OptionParts->SetParamBoolean(EnumSaveParam::bloom, false);
						OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, false);
					}
					else {
						OptionParts->SetParamBoolean(EnumSaveParam::shadow, true);
						OptionParts->SetParamBoolean(EnumSaveParam::bloom, true);
						OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, true);
					}
				}
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Window Mode", "ウィンドウ、ボーダーレス、フルスクリーンモードを選択します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::WindowMode, Clamp(OptionParts->GetParamInt(EnumSaveParam::WindowMode) - 1, static_cast<int>(WindowType::None), static_cast<int>(WindowType::Max)));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				auto* DrawParts = DXDraw::Instance();
				DrawParts->SetWindowOrBorderless();
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::WindowMode, Clamp(OptionParts->GetParamInt(EnumSaveParam::WindowMode) + 1, static_cast<int>(WindowType::None), static_cast<int>(WindowType::Max)));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				auto* DrawParts = DXDraw::Instance();
				DrawParts->SetWindowOrBorderless();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto* DrawParts = DXDraw::Instance();
				auto prev = OptionParts->GetParamInt(EnumSaveParam::WindowMode);
				OptionParts->SetParamInt(EnumSaveParam::WindowMode, UISystem::UpDownBox(xpos, xpos + DrawParts->GetUIY(200), ypos, OptionParts->GetParamInt(EnumSaveParam::WindowMode), static_cast<int>(WindowType::Max)));
				if (prev != OptionParts->GetParamInt(EnumSaveParam::WindowMode)) {
					DrawParts->SetWindowOrBorderless();
				}
				std::string Type;
				switch (static_cast<WindowType>(OptionParts->GetParamInt(EnumSaveParam::WindowMode))) {
				case WindowType::Window:
					Type = "ウィンドウ";
					break;
				case WindowType::Borderless:
					Type = "ボーダーレス";
					break;
				case WindowType::FullScreen:
					Type = "フルスクリーン";
					break;
				case WindowType::Max:
				default:
					break;
				}
				UISystem::SetMsg(xpos + DrawParts->GetUIY(250), ypos + LineHeight / 2,
					LineHeight, UISystem::FontXCenter::RIGHT, White, Black, Type.c_str());
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("V Sync", "垂直同期の有効無効を指定します",
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::vsync);
				if (OptionParts->GetParamBoolean(EnumSaveParam::vsync)) {
					OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
				}
				SetWaitVSyncFlag(OptionParts->GetParamBoolean(EnumSaveParam::vsync));									// 垂直同期
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::vsync);
				if (OptionParts->GetParamBoolean(EnumSaveParam::vsync)) {
					OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
				}
				SetWaitVSyncFlag(OptionParts->GetParamBoolean(EnumSaveParam::vsync));									// 垂直同期
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[this](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto prev = OptionParts->GetParamBoolean(EnumSaveParam::vsync);
				OptionParts->SetParamBoolean(EnumSaveParam::vsync, UISystem::CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::vsync)));
				if (OptionParts->GetParamBoolean(EnumSaveParam::vsync)) {
					OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
				}
				if (prev != OptionParts->GetParamBoolean(EnumSaveParam::vsync)) {
					if (OptionParts->GetParamBoolean(EnumSaveParam::vsync)) {
						OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
					}
					SetWaitVSyncFlag(OptionParts->GetParamBoolean(EnumSaveParam::vsync));									// 垂直同期
				}
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("FPS Limit", "垂直同期を切った際のFPS制限を指定します",
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();

				int value = OptionParts->GetParamInt(EnumSaveParam::FpsLimit);
				bool isHit = false;
				for (int i = 0; i < FrameLimitsNum; ++i) {
					if (FrameLimits[static_cast<size_t>(i)] == value) {
						i--;
						if (i < 0) {
							i = FrameLimitsNum - 1;
						}
						value = FrameLimits[static_cast<size_t>(i)];
						isHit = true;
						break;
					}
				}
				if (!isHit) {
					value = FrameLimits[1];
				}

				OptionParts->SetParamInt(EnumSaveParam::FpsLimit, value);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				int value = OptionParts->GetParamInt(EnumSaveParam::FpsLimit);
				bool isHit = false;
				for (int i = 0; i < FrameLimitsNum; ++i) {
					if (FrameLimits[static_cast<size_t>(i)] == value) {
						++i;
						if (i > FrameLimitsNum - 1) {
							i = 0;
						}
						value = FrameLimits[static_cast<size_t>(i)];
						isHit = true;
						break;
					}
				}
				if (!isHit) {
					value = FrameLimits[1];
				}
				OptionParts->SetParamInt(EnumSaveParam::FpsLimit, value);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[this](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto* DrawParts = DXDraw::Instance();
				int ret = 0;
				// 結果から一番近いやつに指定
				int diff = 10000;
				for (int i = 0; i < FrameLimitsNum; ++i) {
					int tmp = std::abs(FrameLimits[static_cast<size_t>(i)] - OptionParts->GetParamInt(EnumSaveParam::FpsLimit));
					if (diff > tmp) {
						diff = tmp;
						ret = i;
					}
				}
				int value = UISystem::UpDownBox(xpos, xpos + DrawParts->GetUIY(200), ypos, ret, FrameLimitsNum);
				OptionParts->SetParamInt(EnumSaveParam::FpsLimit, FrameLimits[value]);
				UISystem::SetMsg(xpos + DrawParts->GetUIY(250), ypos + LineHeight / 2,
					LineHeight, UISystem::FontXCenter::RIGHT, White, Black, "%d", OptionParts->GetParamInt(EnumSaveParam::FpsLimit));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("DirectX Version", "DirectXのバージョンを変更します(反映は再起動後にされます)",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* OptionWindowParts = OptionWindowClass::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::DirectXVer, 1 - OptionParts->GetParamInt(EnumSaveParam::DirectXVer));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				OptionWindowParts->SetRestart();
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* OptionWindowParts = OptionWindowClass::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::DirectXVer, 1 - OptionParts->GetParamInt(EnumSaveParam::DirectXVer));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				OptionWindowParts->SetRestart();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* DrawParts = DXDraw::Instance();
				auto* OptionWindowParts = OptionWindowClass::Instance();
				auto* OptionParts = OPTION::Instance();
				auto prev = OptionParts->GetParamInt(EnumSaveParam::DirectXVer);
				OptionParts->SetParamInt(EnumSaveParam::DirectXVer, UISystem::CheckBox(xpos, ypos, (OptionParts->GetParamInt(EnumSaveParam::DirectXVer) == 1)) ? 1 : 0);
				if (prev != OptionParts->GetParamInt(EnumSaveParam::DirectXVer)) {
					OptionWindowParts->SetRestart();
				}
				UISystem::SetMsg(xpos + DrawParts->GetUIY(100), ypos + LineHeight / 2,
					LineHeight, UISystem::FontXCenter::MIDDLE, White, Black, DirectXVerStr[OptionParts->GetParamInt(EnumSaveParam::DirectXVer)]);
			}
		);

		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Shadow", "影のクオリティを指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::shadow);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::shadow);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::shadow, UISystem::CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::shadow)));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Bloom Effect", "ブルームエフェクトの有効無効を指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::bloom);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::bloom);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::bloom, UISystem::CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::bloom)));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Screen Effect", "画面エフェクトの有効無効を指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::ScreenEffect);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::ScreenEffect);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, UISystem::CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::ScreenEffect)));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("MotionBlur", "モーションブラーの有効無効を指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::MotionBlur);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::MotionBlur);
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::MotionBlur, UISystem::CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::MotionBlur)));
			}
		);
	}
	void OptionWindowClass::ElseTabsInfo::Init_Sub(void) noexcept {
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("X sensing", "横方向の感度を変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Xsensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Xsensing) - 0.01f, 0.01f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Xsensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Xsensing) + 0.01f, 0.01f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* DrawParts = DXDraw::Instance();
				auto* OptionParts = OPTION::Instance();
				int value = UISystem::UpDownBar(xpos, xpos + DrawParts->GetUIY(200), ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::Xsensing) * 100.f + 0.5f), 10, 100);
				OptionParts->SetParamFloat(EnumSaveParam::Xsensing, static_cast<float>(value) / 100.f);
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Y sensing", "縦方向の感度を変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Ysensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Ysensing) - 0.01f, 0.f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Ysensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Ysensing) + 0.01f, 0.f, 1.f));
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* DrawParts = DXDraw::Instance();
				auto* OptionParts = OPTION::Instance();
				int value = UISystem::UpDownBar(xpos, xpos + DrawParts->GetUIY(200), ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::Ysensing) * 100.f + 0.5f), 10, 100);
				OptionParts->SetParamFloat(EnumSaveParam::Ysensing, static_cast<float>(value) / 100.f);
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("GamePadType", "パッドの入力方式を変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				switch ((ControlType)OptionParts->GetParamInt(EnumSaveParam::ControlType)) {
				case ControlType::PS4:
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::XBox));
					break;
				case ControlType::XBox:
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::PS4));
					break;
				case ControlType::PC:
				default:
					break;
				}
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundPool::Instance();
				switch ((ControlType)OptionParts->GetParamInt(EnumSaveParam::ControlType)) {
				case ControlType::PS4:
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::XBox));
					break;
				case ControlType::XBox:
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::PS4));
					break;
				case ControlType::PC:
				default:
					break;
				}
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* DrawParts = DXDraw::Instance();
				auto* OptionParts = OPTION::Instance();
				if (UISystem::CheckBox(xpos, ypos, (OptionParts->GetParamInt(EnumSaveParam::ControlType) == static_cast<int>(ControlType::PS4)))) {
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::PS4));
				}
				else {
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::XBox));
				}
				ypos -= LineHeight * 1 / 6;
				if (OptionParts->GetParamInt(EnumSaveParam::ControlType) == static_cast<int>(ControlType::XBox)) {
					UISystem::SetMsg(xpos + DrawParts->GetUIY(125), ypos + LineHeight / 3,
						LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, White, Black, "XInput");
				}
				else {
					UISystem::SetMsg(xpos + DrawParts->GetUIY(125), ypos + LineHeight / 3,
						LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, White, Black, "DirectInput");
				}

				if (GetJoypadNum() > 0) {
					switch (GetJoypadType(DX_INPUT_PAD1)) {
					case DX_PADTYPE_OTHER:
					case DX_PADTYPE_DUAL_SHOCK_4:
					case DX_PADTYPE_DUAL_SENSE:
					case DX_PADTYPE_SWITCH_JOY_CON_L:
					case DX_PADTYPE_SWITCH_JOY_CON_R:
					case DX_PADTYPE_SWITCH_PRO_CTRL:
					case DX_PADTYPE_SWITCH_HORI_PAD:
						UISystem::SetMsg(xpos + DrawParts->GetUIY(125), ypos + LineHeight * 3 / 3,
							LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, White, Black, "推奨:DirectInput");
						break;
					case DX_PADTYPE_XBOX_360:
					case DX_PADTYPE_XBOX_ONE:
						UISystem::SetMsg(xpos + DrawParts->GetUIY(125), ypos + LineHeight * 3 / 3,
							LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, White, Black, "推奨:XInput");
						break;
					default:
						break;
					}
				}
			}
		);
	}
	void OptionWindowClass::ControlTabsInfo::KeyDraw(int xpos, int ypos, bool isMine, PADS Sel) noexcept {
		auto* Pad = PadControl::Instance();
		auto* DrawParts = DXDraw::Instance();

		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		KeyGuideParts->DrawButton(xpos + DrawParts->GetUIY(50), ypos + LineHeight / 2, KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(Sel).GetAssign(), Pad->GetControlType()));

		UISystem::SetMsg(xpos + DrawParts->GetUIY(100), ypos + LineHeight / 2,
			LineHeight, UISystem::FontXCenter::LEFT, isMine ? White : Gray25, Black,
			"->");

		if (Pad->GetPadsInfo(Sel).IsEnableSelectReserve()) {
			KeyGuideParts->DrawButton(xpos + DrawParts->GetUIY(150), ypos + LineHeight / 2, KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(Sel).GetReserve(), Pad->GetControlType()));
		}
	}


	void OptionWindowClass::ControlTabsInfo::Init_Sub(void) noexcept {
		auto* Pad = PadControl::Instance();
		const char* KeyInfo = "[マウス必須]カーソルをかざし設定するボタンを押してください";

		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Reset", "[マウス必須]変更をリセットします",
			[]() {},
			[]() {},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool isMine) {
				auto* DrawParts = DXDraw::Instance();
				auto* Pad = PadControl::Instance();
				if (isMine && Pad->GetMouseClick().trigger()) {
					Pad->ResetAssign();
				}
				UISystem::SetMsg(xpos + DrawParts->GetUIY(100), ypos + LineHeight / 2, LineHeight, UISystem::FontXCenter::MIDDLE, isMine ? White : Gray25, Black, "LMB Click");
			}
		);
		if (Pad->GetPadsInfo(PADS::MOVE_W).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("前進/上を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_W)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_W); }
			);
		}
		if (Pad->GetPadsInfo(PADS::MOVE_S).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("後退/下を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_S)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_S); }
			);
		}
		if (Pad->GetPadsInfo(PADS::MOVE_A).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("左移動/左を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_A)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_A); }
			);
		}
		if (Pad->GetPadsInfo(PADS::MOVE_D).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("右移動/右を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_D)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_D); }
			);
		}
		if (Pad->GetPadsInfo(PADS::LEAN_L).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("左タブを選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::LEAN_L)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::LEAN_L); }
			);
		}
		if (Pad->GetPadsInfo(PADS::LEAN_R).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("右タブを選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::LEAN_R)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::LEAN_R); }
			);
		}
		if (Pad->GetPadsInfo(PADS::RELOAD).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("戻る", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::RELOAD)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::RELOAD); }
			);
		}
		if (Pad->GetPadsInfo(PADS::INTERACT).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::INTERACT)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::INTERACT); }
			);
		}
		if (Pad->GetPadsInfo(PADS::JUMP).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("ドッジロール", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::JUMP)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::JUMP); }
			);
		}
		if (Pad->GetPadsInfo(PADS::RUN).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("走る", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::RUN)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::RUN); }
			);
		}
		if (Pad->GetPadsInfo(PADS::WALK).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("ゆっくり歩く", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::WALK)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::WALK); }
			);
		}
		if (Pad->GetPadsInfo(PADS::SHOT).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("射撃", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::SHOT)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::SHOT); }
			);
		}
		if (Pad->GetPadsInfo(PADS::AIM).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Init("注目", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::AIM)) {
						auto* SoundParts = SoundPool::Instance();
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::AIM); }
			);
		}
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Init("Save", "[マウス必須]キーコンフィグをセーブします",
			[]() {},
			[]() {},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool isMine) {
				auto* Pad = PadControl::Instance();
				auto* DrawParts = DXDraw::Instance();
				if (isMine && Pad->GetMouseClick().trigger()) {
					Pad->FlipAssign();
					Pad->Save();
				}
				UISystem::SetMsg(xpos + DrawParts->GetUIY(100), ypos + LineHeight / 2, LineHeight, UISystem::FontXCenter::MIDDLE, isMine ? White : Gray25, Black, "LMB Click");
			}
		);
	}
	// 
	void OptionWindowClass::Init(void) noexcept {
		// 
		m_Tabs.at(0) = std::make_unique<SoundTabsInfo>();
		m_Tabs.at(0)->Init(0, "Sound");
		m_Tabs.at(1) = std::make_unique<GraphicTabsInfo>();
		m_Tabs.at(1)->Init(1, "Graphic");
		m_Tabs.at(2) = std::make_unique<ElseTabsInfo>();
		m_Tabs.at(2)->Init(2, "Else");
		m_Tabs.at(3) = std::make_unique<ControlTabsInfo>();
		m_Tabs.at(3)->Init(3, "Control");
		// 
	}
	void OptionWindowClass::Update(void) noexcept {
		if (m_ActiveSwitch) {
			m_ActiveSwitch = false;
			m_Active = true;
			auto* PopUpParts = UISystem::PopUp::Instance();
			PopUpParts->Add("Option", 720, 720,
				[this](int xmin, int ymin, int, int ymax, bool EndSwitch) {
					auto* DrawParts = DXDraw::Instance();
					auto* OptionParts = OPTION::Instance();
					auto* Pad = PadControl::Instance();
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* SoundParts = SoundPool::Instance();
					int xp1, yp1;


					xp1 = xmin + DrawParts->GetUIY(24);
					yp1 = ymin;
					for (auto& t : m_Tabs) {
						t->Draw(xp1, yp1, m_tabsel == t->GetID(), &m_tabsel, &m_select);
					}
					// ガイド
					xp1 = xmin + DrawParts->GetUIY(24);
					yp1 = ymax - LineHeight * 3 / 2;
					m_Tabs.at(static_cast<size_t>(m_tabsel))->DrawInfo(xp1, yp1, m_select);

					// 
					if (Pad->GetPadsInfo(PADS::LEAN_L).GetKey().trigger() && (m_tabsel != 3)) {
						--m_tabsel;
						if (m_tabsel < 0) {
							m_tabsel = static_cast<int>(m_Tabs.size()) - 1;
						}
						m_select = 0;
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
					if (Pad->GetPadsInfo(PADS::LEAN_R).GetKey().trigger() && (m_tabsel != 3)) {
						++m_tabsel;
						if (m_tabsel > static_cast<int>(m_Tabs.size()) - 1) {
							m_tabsel = 0;
						}
						m_select = 0;
						SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}

					m_Tabs.at(static_cast<size_t>(m_tabsel))->Update(&m_select, (m_tabsel != 3));
					// 
					if (EndSwitch) {
						KeyGuideParts->SetGuideUpdate();
						OptionParts->Save();
						Pad->Save();
						m_tabsel = 0;
						m_select = 0;
					}
				},
				[this]() {m_Active = false; },
				[]() {
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* Pad = PadControl::Instance();

					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(),Pad->GetControlType()), "決定");

					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::LEAN_L).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::LEAN_R).GetAssign(), Pad->GetControlType()), "タブ切替");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_A).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_D).GetAssign(), Pad->GetControlType()), "");
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "選択");
				},
				true
			);
		}
	}
};
