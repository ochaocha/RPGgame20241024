#include "DXLib_ref_12_Draw2DSystem.hpp"

namespace DXLibRef {
	// シングルトンの実態定義
	const FontSystem::FontPool* SingletonBase<FontSystem::FontPool>::m_Singleton = nullptr;
	const WindowSystem::DrawControl* SingletonBase<WindowSystem::DrawControl>::m_Singleton = nullptr;
	const PopUp* SingletonBase<PopUp>::m_Singleton = nullptr;
	// 
	namespace FontSystem {
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
				Str = "CommonData/Font/MSG_";
				break;
			case FontType::DIZ_UD_Gothic:
				Str = "CommonData/Font/BIZUDG_";
				break;
			default:
				break;
			}
			Str += std::to_string(this->m_commonsize);
			Str += ".dft";
			this->m_scaleType = DX_DRAWMODE_BILINEAR;
			this->m_Handle.Load(Str, this->m_EdgeSize);
		}
	}
	namespace WindowSystem {
		void DrawData::Output() const noexcept {
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
				DxLib::DrawBox(m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_intParam[3], this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE);
				break;
			case DrawType::Quadrangle:
				DxLib::DrawQuadrangle(
					this->m_intParam[0], this->m_intParam[1],
					this->m_intParam[2], this->m_intParam[3],
					this->m_intParam[4], this->m_intParam[5],
					this->m_intParam[6], this->m_intParam[7],
					this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE);
				break;
			case DrawType::Circle:
				DxLib::DrawCircle(m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE, this->m_intParam[3]);
				break;
			case DrawType::Line:
				DxLib::DrawLine(m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_intParam[3], this->m_UintParam[0], this->m_intParam[4]);
				break;
			case DrawType::String:
				FontSystem::FontPool::Instance()->Get((FontSystem::FontPool::FontType)m_intParam[0], this->m_intParam[1], 3)->DrawString(
					InvalidID,
					(FontSystem::FontXCenter)m_intParam[2], (FontSystem::FontYCenter)m_intParam[3],
					m_intParam[4], this->m_intParam[5],
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::StringAutoFit:
				FontSystem::FontPool::Instance()->Get((FontSystem::FontPool::FontType)m_intParam[0], this->m_intParam[1], 3)->DrawStringAutoFit(
					m_intParam[2], m_intParam[3],
					m_intParam[4], this->m_intParam[5],
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::RotaGraph:
				if (m_GraphHandleParam.at(0)) {
					if (m_floatParam[0] < 0.9f && 1.1f < this->m_floatParam[0]) {
						auto prev = GetDrawMode();
						SetDrawMode(DX_DRAWMODE_BILINEAR);
						m_GraphHandleParam.at(0)->DrawRotaGraph(this->m_intParam[0], this->m_intParam[1], this->m_floatParam[0], this->m_floatParam[1], this->m_boolParam[0]);
						SetDrawMode(prev);
					}
					else {
						m_GraphHandleParam.at(0)->DrawRotaGraph(this->m_intParam[0], this->m_intParam[1], this->m_floatParam[0], this->m_floatParam[1], this->m_boolParam[0]);
					}
				}
				break;
			case DrawType::ExtendGraph:
				if (m_GraphHandleParam.at(0)) {
					auto prev = GetDrawMode();
					SetDrawMode(DX_DRAWMODE_BILINEAR);
					m_GraphHandleParam.at(0)->DrawExtendGraph(this->m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_intParam[3], this->m_boolParam[0]);
					SetDrawMode(prev);
				}
				break;
			case DrawType::CircleGauge:
				if (m_GraphHandleParam.at(0)) {
					DrawCircleGauge(this->m_intParam[0], this->m_intParam[1],
						(double)this->m_floatParam[0],
						m_GraphHandleParam.at(0)->get(),
						(double)this->m_floatParam[1],
						(double)this->m_floatParam[2]);
				}
				break;
			default:
				break;
			}
		}
		DrawControl::DrawControl(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			this->m_DrawDatas.resize((int)DrawLayer::Max);
			this->m_PrevDrawDatas.resize((int)DrawLayer::Max);

			m_BufferScreen.Make(DrawParts->GetSizeXMax(), DrawParts->GetSizeYMax(), true);
		}
		bool DrawControl::IsDrawOnWindow(int x1, int y1, int x2, int y2) noexcept {
			auto* DrawParts = DXDraw::Instance();
			return HitRectangleToRectangle(0, 0, DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), GetMin(x1, x2), GetMin(y1, y2), GetMax(x1, x2), GetMax(y1, y2));
		}
		// 箱
		void SetBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet) noexcept {
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1, yp1, xp2, yp2, colorSet, true);
		}
		// マウスでクリックできるボタン
		bool SetClickBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet, bool IsRepeat, bool IsActive) noexcept {
			auto* Pad = PadControl::Instance();

			bool MouseOver = IsActive && HitPointToRectangle(Pad->GetMS_X(), Pad->GetMS_Y(), xp1, yp1, xp2, yp2);
			SetBox(xp1, yp1, xp2, yp2, MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : colorSet);
			return (MouseOver && (IsRepeat ? Pad->GetMouseClick().repeat() : Pad->GetMouseClick().trigger()));
		};
		// 文字
		bool GetMsgPosOn(int* xp1, int* yp1, int ySize, int xSize, FontSystem::FontXCenter FontX) noexcept {
			auto* DrawParts = DXDraw::Instance();
			switch (FontX) {
			case FontSystem::FontXCenter::LEFT:
				*xp1 = *xp1 + DrawParts->GetUIY(6);
				return HitRectangleToRectangle(
					(*xp1), (*yp1 - ySize / 2), (*xp1 + xSize), (*yp1 + ySize / 2),
					0, 0, DrawParts->GetUIXMax(), DrawParts->GetUIYMax());
			case FontSystem::FontXCenter::MIDDLE:
				return HitRectangleToRectangle(
					(*xp1 - xSize / 2), (*yp1 - ySize / 2), (*xp1 + xSize / 2), (*yp1 + ySize),
					0, 0, DrawParts->GetUIXMax(), DrawParts->GetUIYMax());
			case FontSystem::FontXCenter::RIGHT:
				*xp1 = *xp1 - DrawParts->GetUIY(6);
				return HitRectangleToRectangle(
					(*xp1 - xSize), (*yp1 - ySize / 2), (*xp1), (*yp1 + ySize / 2),
					0, 0, DrawParts->GetUIXMax(), DrawParts->GetUIYMax());
			default:
				return false;
			}
		};
		// オンオフできるボタン
		bool CheckBox(int xp1, int yp1, bool switchturn) noexcept {
			auto* DrawParts = DXDraw::Instance();
			int EdgeSize = DXDraw::Instance()->GetUIY(2);
			int xp3 = xp1 + EdgeSize;
			int yp3 = yp1 + EdgeSize;
			int xp4 = xp1 + LineHeight * 2 - EdgeSize;
			int yp4 = yp1 + LineHeight - EdgeSize;

			auto* Pad = PadControl::Instance();

			bool MouseOver = HitPointToRectangle(Pad->GetMS_X(), Pad->GetMS_Y(), xp3, yp3, xp4, yp4);
			if (MouseOver && Pad->GetMouseClick().trigger()) {
				switchturn ^= 1;
				auto* SoundParts = SoundPool::Instance();
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
			unsigned int color = Gray25;
			int Edge = DrawParts->GetUIY(5);
			SetBox(xp3 + Edge, yp3 + Edge, xp4 - Edge, yp4 - Edge, Black);
			xp4 = xp1 + LineHeight * (switchturn ? 1 : 0) - EdgeSize;
			SetBox(xp3 + Edge, yp3 + Edge, xp4 + Edge, yp4 - Edge, Gray50);
			xp3 = xp1 + LineHeight * (switchturn ? 1 : 0) + EdgeSize;
			xp4 = xp1 + LineHeight * (switchturn ? 2 : 1) - EdgeSize;
			SetBox(xp3, yp3, xp4, yp4, color);
			return switchturn;
		}
		// スライダー
		int UpDownBar(int xmin, int xmax, int yp, int value, int valueMin, int valueMax) noexcept {
			int xpmin = xmin + 1;
			int xpmax = xmax - 1;

			auto* Pad = PadControl::Instance();

			bool MouseOver = HitPointToRectangle(Pad->GetMS_X(), Pad->GetMS_Y(), xpmin - 5, yp, xpmin + (xpmax - xpmin) + 5, yp + LineHeight);
			if (MouseOver && Pad->GetMouseClick().trigger()) {
				auto* SoundParts = SoundPool::Instance();
				SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
			}
			if (MouseOver && Pad->GetMouseClick().press()) {
				value = Clamp(((valueMax - valueMin) * (Pad->GetMS_X() - xpmin) / (xpmax - xpmin)) + valueMin, valueMin, valueMax);
			}

			SetBox(xpmin, yp, xpmin + (xpmax - xpmin), yp + LineHeight, DarkGreen);
			SetBox(xpmin, yp, xpmin + (xpmax - xpmin) * Clamp(value - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), yp + LineHeight,
				MouseOver ? (Pad->GetMouseClick().press() ? Gray25 : White) : Green);
			int xp = (xmin + (xmax - xmin) / 2);
			SetMsg(xp, yp + LineHeight / 2, LineHeight, FontSystem::FontXCenter::MIDDLE, White, Black, "%03d", value);
			return value;
		}
		// 0~valueMaxの選択制ボタン集
		int UpDownBox(int xmin, int xmax, int yp, int value, int valueMax) noexcept {
			int width = LineHeight;
			int r = LineHeight / 3;
			int xps = (xmax + xmin) / 2;
			int yps = yp + LineHeight / 2;
			for (int loop = 0; loop < valueMax; ++loop) {
				int xp1 = xps + loop * width - width * (valueMax - 1) / 2;
				if (SetClickBox(xp1 - r, yps - r, xp1 + r, yps + r, (value == loop) ? Green : DarkGreen, false, true)) {
					auto* SoundParts = SoundPool::Instance();
					SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);

					value = loop;
				}
			}
			return value;
		}
	};
	// 
	void PopUp::PopUpDrawClass::Start(void) noexcept {
		auto* SoundParts = SoundPool::Instance();
		auto* Pad = PadControl::Instance();

		Pad->SetGuideUpdate();
		Pad->ChangeGuide(
			[this]() {
				auto* Pad = PadControl::Instance();
				Pad->AddGuide(PADS::RELOAD, "戻る");
				if (m_GuideDoing) {
					m_GuideDoing();
				}
			}
		);
		SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);

		m_Active = true;
		m_ActiveSwitch = true;
	}
	void PopUp::PopUpDrawClass::End(void) noexcept {
		auto* SoundParts = SoundPool::Instance();
		auto* Pad = PadControl::Instance();

		SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_CANCEL))->Play(DX_PLAYTYPE_BACK, TRUE);
		m_Active = false;
		m_ActiveSwitch = true;
		Pad->SetGuideUpdate();
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
			if (Pad->GetKey(PADS::RELOAD).trigger()) {
				End();
			}
		}
	}
	void PopUp::PopUpDrawClass::Draw(int xcenter, int ycenter) noexcept {
		if (m_ActivePer < 1.f / 255.f) { return; }

		auto* DrawParts = DXDraw::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();

		int xm1 = xcenter - DrawParts->GetUIY(WinSizeX) / 2;
		int ym1 = ycenter - DrawParts->GetUIY(WinSizeY) / 2;
		int xm2 = xcenter + DrawParts->GetUIY(WinSizeX) / 2;
		int ym2 = ycenter + DrawParts->GetUIY(WinSizeY) / 2;

		// 背景
		auto per = Clamp(m_ActivePer * 0.5f, 0.f, 1.f);
		DrawCtrls->SetAlpha(WindowSystem::DrawLayer::Normal, Clamp(static_cast<int>(255.f * per), 0, 255));
		WindowSystem::SetBox(xm1, ym1, xm2, ym2, Gray50);
		DrawCtrls->SetAlpha(WindowSystem::DrawLayer::Normal, 255);

		// タイトル
		WindowSystem::SetMsg(
			xm1 + DrawParts->GetUIY(32), ym1 + LineHeight / 4 + LineHeight,
			LineHeight * 2, FontSystem::FontXCenter::LEFT, White, Black, m_WindwoName);
		// 
		if (m_Active) {
			int xp1 = xm2 - DrawParts->GetUIY(140);
			int yp1 = ym1 + LineHeight / 4 + LineHeight / 2;
			if (WindowSystem::SetMsgClickBox(xp1, yp1 + DrawParts->GetUIY(5), xp1 + DrawParts->GetUIY(108), yp1 + LineHeight * 2 - DrawParts->GetUIY(5), LineHeight, Red, false, true, "閉じる")) {
				End();
			}
		}
		// 背景
		{
			int xp1 = xm1 + DrawParts->GetUIY(24);
			int yp1 = ym1 + LineHeight * 3;
			int xp2 = xm2 - DrawParts->GetUIY(24);
			int yp2 = ym2 - LineHeight;
			DrawCtrls->SetAlpha(WindowSystem::DrawLayer::Normal, Clamp(static_cast<int>(255.f * 0.5f), 0, 255));
			WindowSystem::SetBox(xp1, yp1, xp2, yp2, Gray50);
			DrawCtrls->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
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
			auto* DXParts = DXLib_ref::Instance();
			PrevPause = DXParts->IsPause();
			DXParts->ChangePause(true);
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
					auto* DXParts = DXLib_ref::Instance();
					DXParts->ChangePause(false);
				}
			}
		}
	}
};
