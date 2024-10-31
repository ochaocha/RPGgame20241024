#include	"CommonUIControl.hpp"

const FPS_n2::Sceneclass::ButtonControl* SingletonBase<FPS_n2::Sceneclass::ButtonControl>::m_Singleton = nullptr;
namespace FPS_n2 {
	namespace Sceneclass {
		// 
		bool ButtonControl::GetTriggerButton(void) const noexcept {
			auto* Pad = PadControl::Instance();
			return (select != InvalidID) && (this->m_MouseSelMode ? Pad->GetMouseClick().trigger() : Pad->GetKey(PADS::INTERACT).trigger());
		}
		ButtonControl::ButtonControl(void) noexcept {
			this->m_SelectBackImage.Load("data/UI/select.png");
			ResetSel();
		}
		ButtonControl::~ButtonControl(void) noexcept {
			Dispose();
			this->m_SelectBackImage.Dispose();
		}
		void ButtonControl::UpdateInput(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* Pad = PadControl::Instance();

			int preselect = select;
			bool preMouseSel = this->m_MouseSelMode;
			if (Pad->GetKey(PADS::MOVE_W).trigger() || Pad->GetKey(PADS::MOVE_A).trigger()) {
				if (select != InvalidID) {
					--select;
					if (select < 0) { select = static_cast<int>(ButtonSel.size()) - 1; }
				}
				else {
					select = 0;
				}
				this->m_MouseSelMode = false;
			}
			if (Pad->GetKey(PADS::MOVE_S).trigger() || Pad->GetKey(PADS::MOVE_D).trigger()) {
				if (select != InvalidID) {
					++select;
					if (select > static_cast<int>(ButtonSel.size()) - 1) { select = 0; }
				}
				else {
					select = 0;
				}
				this->m_MouseSelMode = false;
			}

			if (this->m_MouseSelMode) {
				select = InvalidID;
			}
			// 
			for (auto& y : ButtonSel) {
				if (y->GetInto()) {
					this->m_MouseSelMode = true;
					select = static_cast<int>(&y - &ButtonSel.front());
				}
			}
			if (preselect != select || preMouseSel != this->m_MouseSelMode) {
				if (select != InvalidID) {
					for (auto& y : ButtonSel) {
						y->SetNone();
					}
					ButtonSel.at(static_cast<size_t>(select))->SetFocus();
					SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				else {
					for (auto& y : ButtonSel) {
						y->SetReady();
					}
				}
			}
		}
		void ButtonControl::Update(void) noexcept {
			for (auto& y : ButtonSel) {
				y->Update();
			}
		}
		void ButtonControl::Draw(void) noexcept {
			for (auto& y : ButtonSel) {
				y->Draw();
			}
		}
		void ButtonControl::Dispose(void) noexcept {
			for (auto& y : ButtonSel) {
				y->Dispose();
				y.reset();
			}
			ButtonSel.clear();
		}
		// 
		void CreditControl::Init(void) noexcept {
			this->m_CreditCoulm = 0;
			FileStreamDX FileStream("data/Credit.txt");
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL.find('=') != std::string::npos) {
					auto LEFT = FileStreamDX::getleft(ALL);
					auto RIGHT = FileStreamDX::getright(ALL);
					sprintfDx(this->m_CreditStr.at(static_cast<size_t>(this->m_CreditCoulm)).first, LEFT.c_str());
					sprintfDx(this->m_CreditStr.at(static_cast<size_t>(this->m_CreditCoulm)).second, RIGHT.c_str());
				}
				else {
					sprintfDx(this->m_CreditStr.at(static_cast<size_t>(this->m_CreditCoulm)).first, ALL.c_str());
					sprintfDx(this->m_CreditStr.at(static_cast<size_t>(this->m_CreditCoulm)).second, "");
				}
				this->m_CreditCoulm++;
			}
		}
		void CreditControl::Draw(int xmin, int ymin, int xmax) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* Fonts = FontSystem::FontPool::Instance();

			int xp1, yp1;

			xp1 = xmin + DrawParts->GetUIY(24);
			yp1 = ymin + LineHeight;
			int Height = DrawParts->GetUIY(12);
			for (auto& c : this->m_CreditStr) {
				if (this->m_CreditCoulm < static_cast<int>(&c - &this->m_CreditStr.front())) { break; }
				int xpos = xp1 + DrawParts->GetUIY(6);
				int ypos = yp1 + Height / 2;
				Fonts->Get(FontSystem::FontPool::FontType::DIZ_UD_Gothic, Height, 3)->DrawString(InvalidID, FontSystem::FontXCenter::LEFT, FontSystem::FontYCenter::MIDDLE,
					xpos, ypos, White, Black, c.first);

				xpos = xmax - DrawParts->GetUIY(24);
				Fonts->Get(FontSystem::FontPool::FontType::DIZ_UD_Gothic, Height, 3)->DrawString(InvalidID, FontSystem::FontXCenter::RIGHT, FontSystem::FontYCenter::MIDDLE,
					xpos, ypos, White, Black, c.second);
				yp1 += Height;
			}
		}
		void CreditControl::Dispose(void) noexcept {
			this->m_CreditCoulm = 0;
			for (auto& c : this->m_CreditStr) {
				sprintfDx(c.first, "");
				sprintfDx(c.second, "");
			}
		}
	};
};
