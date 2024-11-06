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
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::Bloom)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::Shadow)).SetEnumParamType(EnumParamType::Boolean);
		m_SaveParams.at(static_cast<size_t>(EnumSaveParam::VSync)).SetEnumParamType(EnumParamType::Boolean);
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
				SetParamBoolean(EnumSaveParam::Bloom, true);
				SetParamBoolean(EnumSaveParam::Shadow, true);
				SetParamBoolean(EnumSaveParam::VSync, true);
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
			if (!IsFileExist("data/ProjectSetting.txt")) {
				SetParamBoolean(EnumProjectSettingParam::Distortion, true);
				return;
			}
			FileStreamDX FileStream("data/ProjectSetting.txt");
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

	// オンオフできるボタン
	static bool CheckBox(int xp1, int yp1, bool switchturn) noexcept {
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		int EdgeSize = 2;
		int xp3 = xp1 + EdgeSize;
		int yp3 = yp1 + EdgeSize;
		int xp4 = xp1 + LineHeight * 2 - EdgeSize;
		int yp4 = yp1 + LineHeight - EdgeSize;

		auto* Pad = PadControl::Instance();

		bool MouseOver = UISystem::GetMouseOver(xp3, yp3, xp4, yp4);
		if (MouseOver && Pad->GetMouseClick().trigger()) {
			switchturn ^= 1;
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
		}
		unsigned int color = Gray25;
		int Edge = 5;
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
			xp3 + Edge, yp3 + Edge, xp4 - Edge, yp4 - Edge, Black, true);
		xp4 = xp1 + LineHeight * (switchturn ? 1 : 0) - EdgeSize;
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
			xp3 + Edge, yp3 + Edge, xp4 + Edge, yp4 - Edge, Gray50, true);
		xp3 = xp1 + LineHeight * (switchturn ? 1 : 0) + EdgeSize;
		xp4 = xp1 + LineHeight * (switchturn ? 2 : 1) - EdgeSize;
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
			xp3, yp3, xp4, yp4, color, true);
		return switchturn;
	}
	// スライダー
	static int UpDownBar(int xmin, int xmax, int yp, int value, int valueMin, int valueMax) noexcept {
		int xpmin = xmin + 1;
		int xpmax = xmax - 1;

		auto* Pad = PadControl::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();

		bool MouseOver = UISystem::GetMouseOver(xpmin - 5, yp, xpmin + (xpmax - xpmin) + 5, yp + LineHeight);
		if (MouseOver && Pad->GetMouseClick().trigger()) {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
		}
		if (MouseOver && Pad->GetMouseClick().press()) {
			value = Clamp(((valueMax - valueMin) * (Pad->GetMS_X() - xpmin) / (xpmax - xpmin)) + valueMin, valueMin, valueMax);
		}

		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
			xpmin, yp, xpmin + (xpmax - xpmin), yp + LineHeight, DarkGreen, true);
		DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
			xpmin, yp, xpmin + (xpmax - xpmin) * Clamp(value - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), yp + LineHeight,
			MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : Green, true);

		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::TOP,
			xmin + (xmax - xmin) / 2, yp, White, Black, "%03d", value);
		return value;
	}
	// 0~valueMaxの選択制ボタン集
	static int UpDownBox(int xmin, int xmax, int yp, int value, int valueMax) noexcept {
		auto* DrawCtrls = UISystem::DrawControl::Instance();

		int width = LineHeight;
		int r = LineHeight / 3;
		int xps = (xmax + xmin) / 2;
		int yps = yp + LineHeight / 2;
		for (int loop = 0; loop < valueMax; ++loop) {
			int xp1 = xps + loop * width - width * (valueMax - 1) / 2;

			auto* Pad = PadControl::Instance();
			bool MouseOver = UISystem::GetMouseOver(xp1 - r, yps - r, xp1 + r, yps + r);
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
				xp1 - r, yps - r, xp1 + r, yps + r, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : (value == loop) ? Green : DarkGreen, true);
			bool ret = (MouseOver && Pad->GetMouseClick().trigger());

			if (ret) {
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);

				value = loop;
			}
		}
		return value;
	}

	// --------------------------------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------------------------------
	// 
	void OptionWindowClass::OptionElementsInfo::Draw(int xpos, int ypos, bool isMine) const noexcept {
		auto* DrawCtrls = UISystem::DrawControl::Instance();

		ypos += static_cast<int>(selanim);
		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
			xpos, ypos,isMine ? White : Gray50, Black, m_Name);
		m_Draw(xpos + (720 - 324), ypos, isMine);
	}
	// 
	void OptionWindowClass::OptionTabsInfo::Update(int* select, bool CanPress) noexcept {
		if ((*select) < 0) {
			return;
		}
		auto* SoundParts = SoundSystem::SoundPool::Instance();
		auto* Pad = PadControl::Instance();
		m_Elements.at(static_cast<size_t>(*select)).GetAnyDoing();
		if (CanPress) {
			if (Pad->GetPadsInfo(PADS::MOVE_W).GetKey().trigger()) {
				--(*select);
				if ((*select) < 0) {
					(*select) = static_cast<int>(m_Elements.size()) - 1;
				}
				m_Elements.at(static_cast<size_t>(*select)).selanim = 3.f;
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
			if (Pad->GetPadsInfo(PADS::MOVE_S).GetKey().trigger()) {
				++(*select);
				if ((*select) > static_cast<int>(m_Elements.size()) - 1) {
					(*select) = 0;
				}
				m_Elements.at(static_cast<size_t>(*select)).selanim = -3.f;
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
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
		// タブ
		{
			auto* Pad = PadControl::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();

			int xp1 = xpos + (140 + 12) * m_id;
			int yp1 = ypos + 5;
			int xp2 = xpos + (140 + 12) * m_id + 140;
			int yp2 = ypos + LineHeight * 2 - 5;
			bool MouseOver = UISystem::GetMouseOver(xp1, yp1, xp2, yp2);
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
				xp1, yp1, xp2, yp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : (isActive ? Gray25 : Gray75), true);
			bool ret = (MouseOver && (Pad->GetMouseClick().trigger()));

			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
				(xp1 + xp2) / 2, (yp1 + yp2) / 2, White, Black, m_name);
			if (ret) {
				*TabSel = GetID();
				*select = 0;
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
		}
		// 内容
		if (isActive) {
			int yp1 = ypos + LineHeight * 2;
			for (int i = 0; i < static_cast<int>(m_Elements.size()); ++i) {
				yp1 += (LineHeight + 6);

				if (UISystem::GetMouseOver(xpos, yp1, xpos + 500, yp1 + (LineHeight + 6))) {
					*select = i;
				}
				m_Elements.at(static_cast<size_t>(i)).Draw(xpos, yp1, (*select == i));
			}
		}
	}
	void OptionWindowClass::OptionTabsInfo::DrawInfo(int xpos, int ypos, int select) noexcept {
		auto* DrawCtrls = UISystem::DrawControl::Instance();

		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
			xpos, ypos,White, Black, m_Elements.at(static_cast<size_t>(select)).GetInfoText().c_str());
	}
	// 
	void OptionWindowClass::SoundTabsInfo::Initialize_Sub(void) noexcept {
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("BGM", "BGMボリュームを変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::BGM, Clamp(OptionParts->GetParamFloat(EnumSaveParam::BGM) - 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->FlipVolume();
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::BGM, Clamp(OptionParts->GetParamFloat(EnumSaveParam::BGM) + 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->FlipVolume();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				int value = UpDownBar(xpos, xpos + 200, ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::BGM) * 100.f + 0.5f), 0, 100);
				OptionParts->SetParamFloat(EnumSaveParam::BGM, static_cast<float>(value) / 100.f);
				SoundParts->FlipVolume();
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("SE", "SEボリュームを変更します",
			[]() {
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::SE, Clamp(OptionParts->GetParamFloat(EnumSaveParam::SE) - 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->FlipVolume();
			},
			[]() {
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::SE, Clamp(OptionParts->GetParamFloat(EnumSaveParam::SE) + 0.1f, 0.f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				SoundParts->FlipVolume();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				auto* OptionParts = OPTION::Instance();
				int value = UpDownBar(xpos, xpos + 200, ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::SE) * 100.f + 0.5f), 0, 100);
				OptionParts->SetParamFloat(EnumSaveParam::SE, static_cast<float>(value) / 100.f);
				SoundParts->FlipVolume();
			}
		);
	}
	void OptionWindowClass::GraphicTabsInfo::Initialize_Sub(void) noexcept {
		HDC hdc;
		hdc = GetDC(GetMainWindowHandle());	// デバイスコンテキストの取得
		RefreshRate = GetDeviceCaps(hdc, VREFRESH);	// リフレッシュレートの取得
		ReleaseDC(GetMainWindowHandle(), hdc);	// デバイスコンテキストの解放

		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Graphics Preset", "グラフィック設定を一律設定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::GraphicsPreset);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				if (!OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
					OptionParts->SetParamBoolean(EnumSaveParam::Shadow, false);
					OptionParts->SetParamBoolean(EnumSaveParam::Bloom, false);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, false);
				}
				else {
					OptionParts->SetParamBoolean(EnumSaveParam::Shadow, true);
					OptionParts->SetParamBoolean(EnumSaveParam::Bloom, true);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, true);
				}
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::GraphicsPreset);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				if (!OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
					OptionParts->SetParamBoolean(EnumSaveParam::Shadow, false);
					OptionParts->SetParamBoolean(EnumSaveParam::Bloom, false);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, false);
				}
				else {
					OptionParts->SetParamBoolean(EnumSaveParam::Shadow, true);
					OptionParts->SetParamBoolean(EnumSaveParam::Bloom, true);
					OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, true);
				}
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto prev = OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset);
				OptionParts->SetParamBoolean(EnumSaveParam::GraphicsPreset, CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)));
				if (prev != OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
					if (!OptionParts->GetParamBoolean(EnumSaveParam::GraphicsPreset)) {
						OptionParts->SetParamBoolean(EnumSaveParam::Shadow, false);
						OptionParts->SetParamBoolean(EnumSaveParam::Bloom, false);
						OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, false);
					}
					else {
						OptionParts->SetParamBoolean(EnumSaveParam::Shadow, true);
						OptionParts->SetParamBoolean(EnumSaveParam::Bloom, true);
						OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, true);
					}
				}
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Window Mode", "ウィンドウ、ボーダーレス、フルスクリーンモードを選択します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::WindowMode, Clamp(OptionParts->GetParamInt(EnumSaveParam::WindowMode) - 1, static_cast<int>(WindowType::None), static_cast<int>(WindowType::Max)));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				auto* WindowSizeParts = WindowSizeControl::Instance();
				WindowSizeParts->SetWindowOrBorderless();
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::WindowMode, Clamp(OptionParts->GetParamInt(EnumSaveParam::WindowMode) + 1, static_cast<int>(WindowType::None), static_cast<int>(WindowType::Max)));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				auto* WindowSizeParts = WindowSizeControl::Instance();
				WindowSizeParts->SetWindowOrBorderless();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto* DrawCtrls = UISystem::DrawControl::Instance();
				auto* WindowSizeParts = WindowSizeControl::Instance();
				auto prev = OptionParts->GetParamInt(EnumSaveParam::WindowMode);
				OptionParts->SetParamInt(EnumSaveParam::WindowMode, UpDownBox(xpos, xpos + 200, ypos, OptionParts->GetParamInt(EnumSaveParam::WindowMode), static_cast<int>(WindowType::Max)));
				if (prev != OptionParts->GetParamInt(EnumSaveParam::WindowMode)) {
					WindowSizeParts->SetWindowOrBorderless();
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
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
					xpos + 250, ypos,
					White, Black, Type.c_str());
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("V Sync", "垂直同期の有効無効を指定します",
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::VSync);
				if (OptionParts->GetParamBoolean(EnumSaveParam::VSync)) {
					OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
				}
				DXLib_ref::Instance()->SetWaitVSync();
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::VSync);
				if (OptionParts->GetParamBoolean(EnumSaveParam::VSync)) {
					OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
				}
				DXLib_ref::Instance()->SetWaitVSync();
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[this](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto prev = OptionParts->GetParamBoolean(EnumSaveParam::VSync);
				OptionParts->SetParamBoolean(EnumSaveParam::VSync, CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::VSync)));
				if (OptionParts->GetParamBoolean(EnumSaveParam::VSync)) {
					OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
				}
				if (prev != OptionParts->GetParamBoolean(EnumSaveParam::VSync)) {
					if (OptionParts->GetParamBoolean(EnumSaveParam::VSync)) {
						OptionParts->SetParamInt(EnumSaveParam::FpsLimit, RefreshRate);
					}
					DXLib_ref::Instance()->SetWaitVSync();
				}
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("FPS Limit", "垂直同期を切った際のFPS制限を指定します",
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();

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
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[this]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
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
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[this](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto* DrawCtrls = UISystem::DrawControl::Instance();
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
				int value = UpDownBox(xpos, xpos + 200, ypos, ret, FrameLimitsNum);
				OptionParts->SetParamInt(EnumSaveParam::FpsLimit, FrameLimits[value]);
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::TOP,
					xpos + 250, ypos,
					White, Black, "%d", OptionParts->GetParamInt(EnumSaveParam::FpsLimit));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("DirectX Version", "DirectXのバージョンを変更します(反映は再起動後にされます)",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* OptionWindowParts = OptionWindowClass::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::DirectXVer, 1 - OptionParts->GetParamInt(EnumSaveParam::DirectXVer));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				OptionWindowParts->SetRestart();
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* OptionWindowParts = OptionWindowClass::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamInt(EnumSaveParam::DirectXVer, 1 - OptionParts->GetParamInt(EnumSaveParam::DirectXVer));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				OptionWindowParts->SetRestart();
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionWindowParts = OptionWindowClass::Instance();
				auto* OptionParts = OPTION::Instance();
				auto* DrawCtrls = UISystem::DrawControl::Instance();
				auto prev = OptionParts->GetParamInt(EnumSaveParam::DirectXVer);
				OptionParts->SetParamInt(EnumSaveParam::DirectXVer, CheckBox(xpos, ypos, (OptionParts->GetParamInt(EnumSaveParam::DirectXVer) == 1)) ? 1 : 0);
				if (prev != OptionParts->GetParamInt(EnumSaveParam::DirectXVer)) {
					OptionWindowParts->SetRestart();
				}
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::TOP,
					xpos + 100, ypos,
					White, Black, DirectXVerStr[OptionParts->GetParamInt(EnumSaveParam::DirectXVer)]);
			}
		);

		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Shadow", "影のクオリティを指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::Shadow);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::Shadow);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::Shadow, CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::Shadow)));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Bloom Effect", "ブルームエフェクトの有効無効を指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::Bloom);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::Bloom);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::Bloom, CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::Bloom)));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Screen Effect", "画面エフェクトの有効無効を指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::ScreenEffect);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::ScreenEffect);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::ScreenEffect, CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::ScreenEffect)));
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("MotionBlur", "モーションブラーの有効無効を指定します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::MotionBlur);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->ChangeParamBoolean(EnumSaveParam::MotionBlur);
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				OptionParts->SetParamBoolean(EnumSaveParam::MotionBlur, CheckBox(xpos, ypos, OptionParts->GetParamBoolean(EnumSaveParam::MotionBlur)));
			}
		);
	}
	void OptionWindowClass::ElseTabsInfo::Initialize_Sub(void) noexcept {
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("X sensing", "横方向の感度を変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Xsensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Xsensing) - 0.01f, 0.01f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Xsensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Xsensing) + 0.01f, 0.01f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				int value = UpDownBar(xpos, xpos + 200, ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::Xsensing) * 100.f + 0.5f), 10, 100);
				OptionParts->SetParamFloat(EnumSaveParam::Xsensing, static_cast<float>(value) / 100.f);
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Y sensing", "縦方向の感度を変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Ysensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Ysensing) - 0.01f, 0.f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
				OptionParts->SetParamFloat(EnumSaveParam::Ysensing, Clamp(OptionParts->GetParamFloat(EnumSaveParam::Ysensing) + 0.01f, 0.f, 1.f));
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				int value = UpDownBar(xpos, xpos + 200, ypos, static_cast<int>(OptionParts->GetParamFloat(EnumSaveParam::Ysensing) * 100.f + 0.5f), 10, 100);
				OptionParts->SetParamFloat(EnumSaveParam::Ysensing, static_cast<float>(value) / 100.f);
			}
		);
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("GamePadType", "パッドの入力方式を変更します",
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
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
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {
				auto* OptionParts = OPTION::Instance();
				auto* SoundParts = SoundSystem::SoundPool::Instance();
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
				SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool) {
				auto* OptionParts = OPTION::Instance();
				auto* DrawCtrls = UISystem::DrawControl::Instance();
				if (CheckBox(xpos, ypos, (OptionParts->GetParamInt(EnumSaveParam::ControlType) == static_cast<int>(ControlType::PS4)))) {
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::PS4));
				}
				else {
					OptionParts->SetParamInt(EnumSaveParam::ControlType, static_cast<int>(ControlType::XBox));
				}
				ypos -= LineHeight * 1 / 6;
				if (OptionParts->GetParamInt(EnumSaveParam::ControlType) == static_cast<int>(ControlType::XBox)) {
					DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
						LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
						xpos + 125, ypos + LineHeight / 3,
						White, Black, "XInput");
				}
				else {
					DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
						LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
						xpos + 125, ypos + LineHeight / 3,
						White, Black, "DirectInput");
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
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
							xpos + 125, ypos + LineHeight * 3 / 3,
							White, Black, "推奨:DirectInput");
						break;
					case DX_PADTYPE_XBOX_360:
					case DX_PADTYPE_XBOX_ONE:
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							LineHeight * 2 / 3, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
							xpos + 125, ypos + LineHeight * 3 / 3,
							White, Black, "推奨:XInput");
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
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		auto* KeyGuideParts = UISystem::KeyGuide::Instance();
		KeyGuideParts->DrawButton(xpos + 50, ypos + LineHeight / 2, KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(Sel).GetAssign(), Pad->GetControlType()));

		DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
			LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
			xpos + 100, ypos,
			isMine ? White : Gray25, Black,
			"->");

		if (Pad->GetPadsInfo(Sel).IsEnableSelectReserve()) {
			KeyGuideParts->DrawButton(xpos + 150, ypos + LineHeight / 2, KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(Sel).GetReserve(), Pad->GetControlType()));
		}
	}


	void OptionWindowClass::ControlTabsInfo::Initialize_Sub(void) noexcept {
		auto* Pad = PadControl::Instance();
		const char* KeyInfo = "[マウス必須]カーソルをかざし設定するボタンを押してください";

		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Reset", "[マウス必須]変更をリセットします",
			[]() {},
			[]() {},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool isMine) {
				auto* Pad = PadControl::Instance();
				auto* DrawCtrls = UISystem::DrawControl::Instance();
				if (isMine && Pad->GetMouseClick().trigger()) {
					Pad->ResetAssign();
				}
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::TOP,
					xpos + 100, ypos, isMine ? White : Gray25, Black, "LMB Click");
			}
		);
		if (Pad->GetPadsInfo(PADS::MOVE_W).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("前進/上を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_W)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_W); }
			);
		}
		if (Pad->GetPadsInfo(PADS::MOVE_S).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("後退/下を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_S)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_S); }
			);
		}
		if (Pad->GetPadsInfo(PADS::MOVE_A).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("左移動/左を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_A)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_A); }
			);
		}
		if (Pad->GetPadsInfo(PADS::MOVE_D).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("右移動/右を選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::MOVE_D)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::MOVE_D); }
			);
		}
		if (Pad->GetPadsInfo(PADS::LEAN_L).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("左タブを選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::LEAN_L)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::LEAN_L); }
			);
		}
		if (Pad->GetPadsInfo(PADS::LEAN_R).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("右タブを選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::LEAN_R)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::LEAN_R); }
			);
		}
		if (Pad->GetPadsInfo(PADS::RELOAD).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("戻る", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::RELOAD)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::RELOAD); }
			);
		}
		if (Pad->GetPadsInfo(PADS::INTERACT).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("選択", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::INTERACT)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::INTERACT); }
			);
		}
		if (Pad->GetPadsInfo(PADS::JUMP).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("ドッジロール", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::JUMP)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::JUMP); }
			);
		}
		if (Pad->GetPadsInfo(PADS::RUN).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("走る", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::RUN)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::RUN); }
			);
		}
		if (Pad->GetPadsInfo(PADS::WALK).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("ゆっくり歩く", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::WALK)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::WALK); }
			);
		}
		if (Pad->GetPadsInfo(PADS::SHOT).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("射撃", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::SHOT)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::SHOT); }
			);
		}
		if (Pad->GetPadsInfo(PADS::AIM).IsUse()) {
			this->m_Elements.resize(this->m_Elements.size() + 1);
			this->m_Elements.back().Initialize("注目", KeyInfo,
				[]() {},
				[]() {},
				[]() {},
				[]() {
					auto* Pad = PadControl::Instance();
					if (Pad->ChangeConfig(PADS::AIM)) {
						auto* SoundParts = SoundSystem::SoundPool::Instance();
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
				},
				[this](int xpos, int ypos, bool isMine) { KeyDraw(xpos, ypos, isMine, PADS::AIM); }
			);
		}
		this->m_Elements.resize(this->m_Elements.size() + 1);
		this->m_Elements.back().Initialize("Save", "[マウス必須]キーコンフィグをセーブします",
			[]() {},
			[]() {},
			[]() {},
			[]() {},
			[](int xpos, int ypos, bool isMine) {
				auto* Pad = PadControl::Instance();
				auto* DrawCtrls = UISystem::DrawControl::Instance();
				if (isMine && Pad->GetMouseClick().trigger()) {
					Pad->FlipAssign();
					Pad->Save();
				}
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::TOP,
					xpos + 100, ypos, isMine ? White : Gray25, Black, "LMB Click");
			}
		);
	}
	// 
	void OptionWindowClass::Initialize(void) noexcept {
		// 
		m_Tabs.at(0) = std::make_unique<SoundTabsInfo>();
		m_Tabs.at(0)->Initialize(0, "Sound");
		m_Tabs.at(1) = std::make_unique<GraphicTabsInfo>();
		m_Tabs.at(1)->Initialize(1, "Graphic");
		m_Tabs.at(2) = std::make_unique<ElseTabsInfo>();
		m_Tabs.at(2)->Initialize(2, "Else");
		m_Tabs.at(3) = std::make_unique<ControlTabsInfo>();
		m_Tabs.at(3)->Initialize(3, "Control");
		// 
	}
	void OptionWindowClass::Update(void) noexcept {
		if (m_ActiveSwitch) {
			m_ActiveSwitch = false;
			m_Active = true;
			auto* PopUpParts = UISystem::PopUp::Instance();
			PopUpParts->Add("Option", 720, 720,
				[this](int xmin, int ymin, int, int ymax, bool EndSwitch) {
					auto* OptionParts = OPTION::Instance();
					auto* Pad = PadControl::Instance();
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* SoundParts = SoundSystem::SoundPool::Instance();

					for (auto& t : m_Tabs) {
						t->Draw(xmin + 24, ymin, m_tabsel == t->GetID(), &m_tabsel, &m_select);
					}
					// ガイド
					m_Tabs.at(static_cast<size_t>(m_tabsel))->DrawInfo(xmin + 24, ymax - LineHeight * 3 / 2, m_select);

					// 
					if (Pad->GetPadsInfo(PADS::LEAN_L).GetKey().trigger() && (m_tabsel != 3)) {
						--m_tabsel;
						if (m_tabsel < 0) {
							m_tabsel = static_cast<int>(m_Tabs.size()) - 1;
						}
						m_select = 0;
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}
					if (Pad->GetPadsInfo(PADS::LEAN_R).GetKey().trigger() && (m_tabsel != 3)) {
						++m_tabsel;
						if (m_tabsel > static_cast<int>(m_Tabs.size()) - 1) {
							m_tabsel = 0;
						}
						m_select = 0;
						SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
					}

					m_Tabs.at(static_cast<size_t>(m_tabsel))->Update(&m_select, (m_tabsel != 3));
					// 
					if (EndSwitch) {
						KeyGuideParts->SetGuideFlip();
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
