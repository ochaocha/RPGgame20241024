#include	"CommonUIControl.hpp"

// シングルトンの実態定義
const DXLIB_Sample::UI::ButtonControl* SingletonBase<DXLIB_Sample::UI::ButtonControl>::m_Singleton = nullptr;
const DXLIB_Sample::UI::FadeControl* SingletonBase<DXLIB_Sample::UI::FadeControl>::m_Singleton = nullptr;

namespace DXLIB_Sample {
	namespace UI {
		// 
		bool ButtonControl::GetTriggerButton(void) const noexcept {
			auto* Pad = PadControl::Instance();
			return (select != InvalidID) && (this->m_MouseSelMode ? Pad->GetMouseClick().trigger() : Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger());
		}
		ButtonControl::ButtonControl(void) noexcept {
			this->m_SelectBackImage.Load("data/UI/select.png");
			ResetSel();
		}
		void ButtonControl::UpdateInput(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* Pad = PadControl::Instance();

			int preselect = select;
			bool preMouseSel = this->m_MouseSelMode;
			if (Pad->GetPadsInfo(PADS::MOVE_W).GetKey().trigger() || Pad->GetPadsInfo(PADS::MOVE_A).GetKey().trigger()) {
				if (select != InvalidID) {
					--select;
					if (select < 0) { select = static_cast<int>(ButtonSel.size()) - 1; }
				}
				else {
					select = 0;
				}
				this->m_MouseSelMode = false;
			}
			if (Pad->GetPadsInfo(PADS::MOVE_S).GetKey().trigger() || Pad->GetPadsInfo(PADS::MOVE_D).GetKey().trigger()) {
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
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SoundSystem::SoundSelectCommon::UI_Select))->Play(DX_PLAYTYPE_BACK, TRUE);
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
		void FadeControl::SetFadeIn(void) noexcept {
			this->m_IsBlackOut = false;
			this->m_BlackOutAlpha = 1.f;
		}
		void FadeControl::SetFadeOut(void) noexcept {
			this->m_IsBlackOut = true;
			this->m_BlackOutAlpha = 0.f;
		}
		void FadeControl::Update(void) noexcept {
			this->m_BlackOutAlpha = std::clamp(this->m_BlackOutAlpha + (this->m_IsBlackOut ? 1.f : -1.f) * DXLib_ref::Instance()->GetDeltaTime() / 0.5f, 0.f, 1.f);
		}
		void FadeControl::DrawFade(void) const noexcept {
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * this->m_BlackOutAlpha), 0, 255));
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, 0, BaseScreenWidth, BaseScreenHeight, Black, TRUE);
			DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
		}
	}
}
