#include "DXLib_ref_12_Draw2DSystem.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const UISystem::FontPool* SingletonBase<UISystem::FontPool>::m_Singleton = nullptr;
	const UISystem::DrawControl* SingletonBase<UISystem::DrawControl>::m_Singleton = nullptr;
	const UISystem::PopUp* SingletonBase<UISystem::PopUp>::m_Singleton = nullptr;
	const UISystem::KeyGuide* SingletonBase<UISystem::KeyGuide>::m_Singleton = nullptr;
	// 
	namespace UISystem {
		bool GetMouseOver(int xp1, int yp1, int xp2, int yp2) {
			auto* Pad = PadControl::Instance();
			return HitPointToRectangle(Pad->GetMS_X(), Pad->GetMS_Y(), xp1, yp1, xp2, yp2);
		}
		//
		void FontPool::Fonthave::Set(FontType type, int fontSize, int edgeSize) noexcept {
			this->m_Type = type;
			this->m_EdgeSize = edgeSize;
			this->m_CustomSize = fontSize;
			if (fontSize <= 16) {
				this->m_commonsize = 16;
			}
			else if (fontSize <= 24) {
				this->m_commonsize = 24;
			}
			else {
				this->m_commonsize = 32;
			}

			std::string Str;

			switch (this->m_Type) {
			case FontType::MS_Gothic:
				Str = "data/Font/MSG_";
				break;
			case FontType::DIZ_UD_Gothic:
				Str = "data/Font/BIZUDG_";
				break;
			default:
				break;
			}
			Str += std::to_string(this->m_commonsize);
			Str += ".dft";
			this->m_scaleType = DX_DRAWMODE_BILINEAR;
			this->m_Handle.Load(Str, this->m_EdgeSize);
		}
		//
		void DrawData::Output() const noexcept {
			auto* WindowSizeParts = WindowSizeControl::Instance();
			switch (m_type) {
			case DrawType::Alpha:
				if (this->m_intParam[0] < 255) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->m_intParam[0]);
				}
				else {
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->m_intParam[0]);
				}
				break;
			case DrawType::Add:
				SetDrawBlendMode(DX_BLENDMODE_ADD, this->m_intParam[0]);
				break;
			case DrawType::Bright:
				SetDrawBright(this->m_intParam[0], this->m_intParam[1], this->m_intParam[2]);
				break;
			case DrawType::Box:
				DxLib::DrawBox(
					WindowSizeParts->GetUIY(m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]),
					WindowSizeParts->GetUIY(this->m_intParam[2]), WindowSizeParts->GetUIY(this->m_intParam[3]),
					this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE);
				break;
			case DrawType::Quadrangle:
				DxLib::DrawQuadrangle(
					WindowSizeParts->GetUIY(this->m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]),
					WindowSizeParts->GetUIY(this->m_intParam[2]), WindowSizeParts->GetUIY(this->m_intParam[3]),
					WindowSizeParts->GetUIY(this->m_intParam[4]), WindowSizeParts->GetUIY(this->m_intParam[5]),
					WindowSizeParts->GetUIY(this->m_intParam[6]), WindowSizeParts->GetUIY(this->m_intParam[7]),
					this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE);
				break;
			case DrawType::Circle:
				DxLib::DrawCircle(
					WindowSizeParts->GetUIY(m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]), WindowSizeParts->GetUIY(this->m_intParam[2]),
					this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE,
					(this->m_intParam[3] >= 2) ? WindowSizeParts->GetUIY(this->m_intParam[3]) : this->m_intParam[3]);
				break;
			case DrawType::Line:
				DxLib::DrawLine(WindowSizeParts->GetUIY(m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]), WindowSizeParts->GetUIY(this->m_intParam[2]), WindowSizeParts->GetUIY(this->m_intParam[3]), this->m_UintParam[0],
					(this->m_intParam[4]>=2) ? WindowSizeParts->GetUIY(this->m_intParam[4]) : this->m_intParam[4]);
				break;
			case DrawType::String:
				UISystem::FontPool::Instance()->Get((UISystem::FontPool::FontType)m_intParam[0],
					WindowSizeParts->GetUIY(this->m_intParam[1]), 3)->DrawString(
					InvalidID,
					(UISystem::FontXCenter)m_intParam[2], (UISystem::FontYCenter)m_intParam[3],
					WindowSizeParts->GetUIY(m_intParam[4]), WindowSizeParts->GetUIY(this->m_intParam[5]),
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::StringAutoFit:
				UISystem::FontPool::Instance()->Get((UISystem::FontPool::FontType)m_intParam[0],
					WindowSizeParts->GetUIY(this->m_intParam[1]), 3)->DrawStringAutoFit(
					WindowSizeParts->GetUIY(m_intParam[2]), WindowSizeParts->GetUIY(m_intParam[3]),
					WindowSizeParts->GetUIY(m_intParam[4]), WindowSizeParts->GetUIY(this->m_intParam[5]),
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::RotaGraph:
				if (m_GraphHandleParam.at(0)) {
					float Scale = (float)(WindowSizeParts->GetUIY((int)(this->m_floatParam[0] * 100))) / 100.f;

					if (Scale < 0.95f && 1.05f < Scale) {
						auto prev = GetDrawMode();
						SetDrawMode(DX_DRAWMODE_BILINEAR);
						m_GraphHandleParam.at(0)->DrawRotaGraph(
							WindowSizeParts->GetUIY(this->m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]),
							Scale, this->m_floatParam[1], this->m_boolParam[0]);
						SetDrawMode(prev);
					}
					else {
						m_GraphHandleParam.at(0)->DrawRotaGraph(
							WindowSizeParts->GetUIY(this->m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]),
							Scale, this->m_floatParam[1], this->m_boolParam[0]);
					}
				}
				break;
			case DrawType::ExtendGraph:
				if (m_GraphHandleParam.at(0)) {
					auto prev = GetDrawMode();
					SetDrawMode(DX_DRAWMODE_BILINEAR);
					m_GraphHandleParam.at(0)->DrawExtendGraph(WindowSizeParts->GetUIY(this->m_intParam[0]), WindowSizeParts->GetUIY(this->m_intParam[1]), WindowSizeParts->GetUIY(this->m_intParam[2]), WindowSizeParts->GetUIY(this->m_intParam[3]), this->m_boolParam[0]);
					SetDrawMode(prev);
				}
				break;
			default:
				break;
			}
		}
		//
		DrawControl::DrawControl(void) noexcept {
			auto* WindowSizeParts = WindowSizeControl::Instance();
			this->m_DrawDatas.resize((int)DrawLayer::Max);
			this->m_PrevDrawDatas.resize((int)DrawLayer::Max);

			m_BufferScreen.Make(WindowSizeParts->GetSizeXMax(), WindowSizeParts->GetSizeYMax(), true);
		}
		bool DrawControl::IsDrawOnWindow(int x1, int y1, int x2, int y2) noexcept {
			return HitRectangleToRectangle(0, 0, BaseScreenWidth, BaseScreenHeight, GetMin(x1, x2), GetMin(y1, y2), GetMax(x1, x2), GetMax(y1, y2));
		}
		// 
		void PopUp::PopUpDrawClass::Start(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();

			KeyGuideParts->SetGuideFlip();
			KeyGuideParts->ChangeGuide(
				[this]() {
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* Pad = PadControl::Instance();
					KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::RELOAD).GetAssign(), Pad->GetControlType()), "戻る");
					if (m_GuideDoing) {
						m_GuideDoing();
					}
				}
			);
			SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);

			m_Active = true;
			m_ActiveSwitch = true;
		}
		void PopUp::PopUpDrawClass::End(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();

			SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_CANCEL))->Play(DX_PLAYTYPE_BACK, TRUE);
			m_Active = false;
			m_ActiveSwitch = true;
			KeyGuideParts->SetGuideFlip();
			if (m_ExitDoing) {
				m_ExitDoing();
			}
		}
		void PopUp::PopUpDrawClass::Update(void) noexcept {
			auto* Pad = PadControl::Instance();
			m_ActiveSwitch = false;
			Easing(&m_ActivePer, m_Active ? 1.f : 0.f, m_Active ? 0.7f : 0.3f, EasingType::OutExpo);

			if (m_Active) {
				Pad->SetMouseMoveEnable(false);
				if (Pad->GetPadsInfo(PADS::RELOAD).GetKey().trigger()) {
					End();
				}
			}
		}
		void PopUp::PopUpDrawClass::Draw(int xcenter, int ycenter) noexcept {
			if (m_ActivePer < 1.f / 255.f) { return; }

			auto* DrawCtrls = UISystem::DrawControl::Instance();

			int xm1 = xcenter - WinSizeX / 2;
			int ym1 = ycenter - WinSizeY / 2;
			int xm2 = xcenter + WinSizeX / 2;
			int ym2 = ycenter + WinSizeY / 2;

			// 背景
			auto per = Clamp(m_ActivePer * 0.5f, 0.f, 1.f);
			DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, Clamp(static_cast<int>(255.f * per), 0, 255));
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, xm1, ym1, xm2, ym2, Gray50, true);
			DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);

			// タイトル
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight * 2, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
				xm1 + 32, ym1 + LineHeight / 4,
				White, Black, m_WindwoName);
			// 
			if (m_Active) {
				auto* Pad = PadControl::Instance();
				int xp1 = xm2 - 140;
				int yp1 = ym1 + LineHeight / 4 + LineHeight / 2 + 5;
				int xp2 = xm2 - 32;
				int yp2 = ym1 + LineHeight / 4 + LineHeight / 2 + LineHeight * 2 - 5;
				bool MouseOver = GetMouseOver(xp1, yp1, xp2, yp2);
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal,
					xp1, yp1, xp2, yp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : Red, true);
				bool ret = (MouseOver && (Pad->GetMouseClick().trigger()));

				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::MIDDLE, UISystem::FontYCenter::MIDDLE,
					(xp1 + xp2) / 2, (yp1 + yp2) / 2, White, Black, "閉じる");
				if (ret) {
					End();
				}
			}
			// 背景
			{
				int xp1 = xm1 + 24;
				int yp1 = ym1 + LineHeight * 3;
				int xp2 = xm2 - 24;
				int yp2 = ym2 - LineHeight;
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, Clamp(static_cast<int>(255.f * 0.5f), 0, 255));
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, xp1, yp1, xp2, yp2, Gray50, true);
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
				if (m_Doing) {
					m_Doing(xp1, yp1, xp2, yp2, m_ActiveSwitch);
				}
			}
		}

		void PopUp::Add(const char* WindowName, int sizex, int sizey,
			std::function<void(int xmin, int ymin, int xmax, int ymax, bool EndSwitch)> doing,
			std::function<void()> ExitDoing,
			std::function<void()> GuideDoing,
			bool IsInsert) noexcept {
			auto& Last = que.at(LastSel);
			Last.Set(WindowName, sizex, sizey, doing, ExitDoing, GuideDoing);
			if (!IsActivePop()) {
				Last.Start();
				auto* SceneParts = SceneControl::Instance();
				PrevPause = SceneParts->IsPause();
				SceneParts->ChangePause(true);
			}
			else if (IsInsert) {
				que.at(NowSel).End();
				NowSel = LastSel;
				Last.Start();
			}
			++LastSel %= que.size();
		}
		void PopUp::EndAll(void) noexcept {
			if (!IsActivePop()) {
				return;
			}
			que.at(NowSel).End();
			NowSel = LastSel;
		}
		void PopUp::Update(void) noexcept {
			if (!IsActivePop()) {
				return;
			}
			que.at(NowSel).Update();
			if (que.at(NowSel).IsEnd()) {
				++NowSel %= que.size();
				if (IsActivePop()) {
					que.at(NowSel).Start();
				}
				else {
					if (!PrevPause) {
						auto* SceneParts = SceneControl::Instance();
						SceneParts->ChangePause(false);
					}
				}
			}
		}
		// --------------------------------------------------------------------------------------------------
		// 
		// --------------------------------------------------------------------------------------------------
		int KeyGuide::KeyGuideGraph::GetDrawSize(void) const noexcept {
			return (xsize * 24 / ysize) + 3;
		}
		int KeyGuide::KeyGuideGraph::Draw(int x, int y) const noexcept {
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal,
				&GuideImg, x, y, x + (xsize * 24 / ysize), y + 24, true);
			return GetDrawSize();
		}
		// --------------------------------------------------------------------------------------------------
		// 
		// --------------------------------------------------------------------------------------------------
		int KeyGuide::KeyGuideOnce::GetDrawSize(void) const noexcept {
			int ofs = (m_GuideGraph) ? m_GuideGraph->GetDrawSize() : 0;
			if (GuideString != "") {
				ofs += UISystem::FontPool::Instance()->Get(UISystem::FontPool::FontType::MS_Gothic, LineHeight, 3)->GetStringWidth(InvalidID, GuideString) + 12;
			}
			return ofs;
		}
		int KeyGuide::KeyGuideOnce::Draw(int x, int y) const noexcept {
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			int ofs = (m_GuideGraph) ? m_GuideGraph->Draw(x, y) : 0;
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::MIDDLE,
				x + ofs, y + 24 / 2,
				White, Black, GuideString);
			return GetDrawSize();
		}
		// --------------------------------------------------------------------------------------------------
		// 
		// --------------------------------------------------------------------------------------------------
		KeyGuide::KeyGuide(void) noexcept {
			SetGuideFlip();
			m_GuideBaseImage.Load("data/key/OutputFont.png");
			{
				int count = 0;
				FileStreamDX FileStream("data/key/OutputFont.psf");
				while (true) {
					if (FileStream.ComeEof()) { break; }
					auto ALL = FileStream.SeekLineAndGetStr();
					if (ALL == "") { continue; }
					//=の右側の文字をカンマ区切りとして識別する
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
					//得た情報をもとに分割した画像を得る
					m_DerivationGuideImage.emplace_back(std::make_shared<KeyGuideGraph>());
					m_DerivationGuideImage.back()->AddGuide(Args.at(0), Args.at(1), Args.at(2), Args.at(3), m_GuideBaseImage);
					++count;
				}
			}
		}
		void KeyGuide::ChangeGuide(std::function<void()>Guide_Pad) noexcept {
			if (m_IsFlipGuide) {
				m_IsFlipGuide = false;
				Dispose();
				// 絶対出すガイド
				auto* Pad = PadControl::Instance();
				AddGuide(GetIDtoOffset(Pad->GetPadsInfo(PADS::Escape).GetAssign(), ControlType::PC), "終了");
				AddGuide(GetIDtoOffset(Pad->GetPadsInfo(PADS::INVENTORY).GetAssign(), Pad->GetControlType()), "ポーズ");
				// 追加のガイド
				Guide_Pad();
			}
		}
		void KeyGuide::Draw(void) const noexcept {
			int xp = 0;
			int y = BaseScreenHeight - (21 + 16);
			for (const auto& k : m_Key) {
				xp += k->Draw(32 + xp, y);
				//次の行へ移行
				if (xp > BaseScreenWidth / 2) {
					xp = 0;
					y -= 28;
				}
			}
		}
	}
}