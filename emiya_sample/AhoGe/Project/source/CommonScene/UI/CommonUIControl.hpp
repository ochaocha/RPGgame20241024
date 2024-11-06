#pragma once
#include	"../../Header.hpp"

namespace DXLIB_Sample {
	namespace UI {
		// --------------------------------------------------------------------------------------------------
		//
		// --------------------------------------------------------------------------------------------------
		class ButtonControl : public SingletonBase<ButtonControl> {
		private:
			friend class SingletonBase<ButtonControl>;
		private:
			enum class ButtonStatus {
				None,
				Ready,
				Focus,
			};
			enum class ButtonMode {
				String,
				Icon,
			};
			class ButtonClass {
				GraphHandle* m_SelectBackImage{ nullptr };

				int xp1{ 0 };
				int yp1{ 0 };
				int xsize{ 0 };
				int ysize{ 0 };
				UISystem::FontXCenter LMR{ UISystem::FontXCenter::LEFT };
				UISystem::FontYCenter TMB{ UISystem::FontYCenter::TOP };

				float SelYadd{ 0.f };

				ButtonStatus m_ButtonStatus{ ButtonStatus::Ready };
				ButtonMode m_ButtonMode{ ButtonMode::String };
				bool m_EnableSelect{ false };
			private:
				char m_String[64]{};
				GraphHandle m_Icon;

			public:
				//コンストラクタ
				ButtonClass(void) noexcept {}
				ButtonClass(const ButtonClass&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
				ButtonClass(ButtonClass&& o) = delete;
				ButtonClass& operator=(const ButtonClass&) = delete;
				ButtonClass& operator=(ButtonClass&& o) = delete;
				~ButtonClass(void) noexcept {
					Dispose();
				}
			public:
				void			LoadCommon(GraphHandle* BGPath) noexcept {
					this->m_SelectBackImage = BGPath;
				}
				void			Load_Icon(const char* IconPath, bool IsEnableSelect) noexcept {
					this->m_Icon.Load(IconPath);
					this->m_Icon.GetSize(&xsize, &ysize);
					this->m_ButtonMode = ButtonMode::Icon;
					this->m_EnableSelect = IsEnableSelect;
				}
				void			Load_String(const char* String, int fontsize, bool IsEnableSelect) noexcept {
					snprintfDx(this->m_String, 64, String);
					xsize = UISystem::FontPool::Instance()->Get(UISystem::FontPool::FontType::MS_Gothic, fontsize, 3)->GetStringWidth(InvalidID, this->m_String);
					ysize = fontsize;
					this->m_ButtonMode = ButtonMode::String;
					this->m_EnableSelect = IsEnableSelect;
				}
				void			Set(int xp, int yp, UISystem::FontXCenter FontX, UISystem::FontYCenter FontY) noexcept {
					xp1 = xp;
					yp1 = yp;
					LMR = FontX;
					TMB = FontY;
					SelYadd = 0.f;
					this->m_ButtonStatus = ButtonStatus::Ready;
				}
				void			Update(void) noexcept {
					Easing(&SelYadd, 0.f, 0.93f, EasingType::OutExpo);
				}
				void			Dispose(void) noexcept {
					this->m_Icon.Dispose();
				}
			public:
				auto			IsFocus(void) const noexcept { return this->m_ButtonStatus == ButtonStatus::Focus; }
				void			SetNone(void) noexcept { this->m_ButtonStatus = ButtonStatus::None; }
				void			SetReady(void) noexcept { this->m_ButtonStatus = ButtonStatus::Ready; }
				void			SetFocus(void) noexcept {
					SelYadd = 10.f;
					this->m_ButtonStatus = ButtonStatus::Focus;
				}
			public:
				bool			GetInto(void) const noexcept {
					int xp = xp1;
					int yp = yp1;
					switch (LMR) {
					case UISystem::FontXCenter::LEFT:
						xp = xp1;
						break;
					case UISystem::FontXCenter::MIDDLE:
						xp = xp1 - xsize / 2;
						break;
					case UISystem::FontXCenter::RIGHT:
						xp = xp1 - xsize;
						break;
					default:
						break;
					}
					switch (TMB) {
					case UISystem::FontYCenter::TOP:
						yp = yp1;
						break;
					case UISystem::FontYCenter::MIDDLE:
						yp = yp1 - ysize / 2;
						break;
					case UISystem::FontYCenter::BOTTOM:
						yp = yp1 - ysize;
						break;
					default:
						break;
					}
					return UISystem::GetMouseOver(xp, yp, xp + xsize, yp + ysize);
				}
				void			Draw(void) noexcept {
					auto* DrawCtrls = UISystem::DrawControl::Instance();
					switch (this->m_ButtonMode) {
					case ButtonMode::String:
					{
						if (SelYadd > 0.f) {
							int xp = xp1;
							int yp = yp1;
							switch (LMR) {
							case UISystem::FontXCenter::LEFT:
								xp = xp1;
								break;
							case UISystem::FontXCenter::MIDDLE:
								xp = xp1 - xsize / 2;
								break;
							case UISystem::FontXCenter::RIGHT:
								xp = xp1 - xsize;
								break;
							default:
								break;
							}
							switch (TMB) {
							case UISystem::FontYCenter::TOP:
								yp = yp1;
								break;
							case UISystem::FontYCenter::MIDDLE:
								yp = yp1 - ysize / 2;
								break;
							case UISystem::FontYCenter::BOTTOM:
								yp = yp1 - ysize;
								break;
							default:
								break;
							}

							float per = std::clamp(SelYadd / 5.f, 0.f, 1.f);
							float per2 = 1.f - std::clamp(SelYadd / 10.f, 0.f, 1.f);
							DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(128.f * per), 0, 255));
							DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal,
								this->m_SelectBackImage,
								xp + xsize / 2 - static_cast<int>(static_cast<float>(xsize / 2 + 300) * per2), yp + ysize - 12 - static_cast<int>(static_cast<float>(ysize / 6) * per),
								xp + xsize / 2 + static_cast<int>(static_cast<float>(xsize / 2 + 300) * per2), yp + ysize - 12 + static_cast<int>(static_cast<float>(ysize / 6) * per),
								true);
							DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
						}
						unsigned int Color = Black;
						if (ysize > 50) {
							switch (this->m_ButtonStatus) {
							case ButtonStatus::None:
								Color = Gray75;
								break;
							case ButtonStatus::Ready:
								Color = GetColor(64, 192, 64);
								break;
							case ButtonStatus::Focus:
								Color = Green;
								break;
							default:
								break;
							}
						}
						else {
							switch (this->m_ButtonStatus) {
							case ButtonStatus::None:
								Color = Gray75;
								if (!this->m_EnableSelect) {
									Color = GetColor(64, 48, 48);
								}
								break;
							case ButtonStatus::Ready:
								Color = Gray15;
								if (!this->m_EnableSelect) {
									Color = Gray65;
								}
								break;
							case ButtonStatus::Focus:
								Color = Green;
								if (!this->m_EnableSelect) {
									Color = GetColor(216, 143, 143);
								}
								break;
							default:
								break;
							}
						}
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
							ysize,
							LMR, TMB, xp1, yp1 + static_cast<int>(SelYadd), Color, Black, this->m_String);
					}
					break;
					case ButtonMode::Icon:
					{
						if (SelYadd > 0.f) {
							float per1 = std::clamp(SelYadd / 5.f, 0.f, 1.f);
							float per2 = 1.f - std::clamp(SelYadd / 10.f, 0.f, 1.f);
							DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(128.f * per1), 0, 255));
							DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal,
								this->m_SelectBackImage,
								xp1 - static_cast<int>(static_cast<float>(xsize) * per2), yp1 - static_cast<int>(static_cast<float>(ysize) * per2),
								xp1 + static_cast<int>(static_cast<float>(xsize) * per2), yp1 + static_cast<int>(static_cast<float>(ysize) * per2),
								true);
							DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
						}
						switch (this->m_ButtonStatus) {
						case ButtonStatus::None:
							DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 128, 128, 128);
							break;
						case ButtonStatus::Ready:
							DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 216, 216, 216);
							break;
						case ButtonStatus::Focus:
							DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 255, 0);
							break;
						default:
							break;
						}
						DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Icon, xp1, yp1, 1.f + SelYadd / 50.f, 0.f, true);
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
					}
					break;
					default:
						break;
					}
				}
			};
		private:
			GraphHandle					m_SelectBackImage;
			std::vector<std::shared_ptr<ButtonClass>>	ButtonSel{};
			int							select{ 0 };
			bool						m_MouseSelMode{ false };
		public:
			const auto& GetSelect(void) const noexcept { return select; }
			bool		GetTriggerButton(void) const noexcept;
		private:
			//コンストラクタ
			ButtonControl(void) noexcept;
			ButtonControl(const ButtonControl&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			ButtonControl(ButtonControl&& o) = delete;
			ButtonControl& operator=(const ButtonControl&) = delete;
			ButtonControl& operator=(ButtonControl&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
			/*
			ButtonControl::~ButtonControl(void) noexcept {
				Dispose();
				this->m_SelectBackImage.Dispose();
			}
			//*/
		public:
			void ResetSel(void) noexcept {
				select = 0;
				this->m_MouseSelMode = false;
			}
			void UpdateInput(void) noexcept;
			void Update(void) noexcept;
			void Draw(void) noexcept;
			void Dispose(void) noexcept;
		public:
			void AddStringButton(
				const char* String, int fontsize, bool IsEnableSelect,
				int xp, int yp, UISystem::FontXCenter FontX, UISystem::FontYCenter FontY
			) noexcept {
				ButtonSel.emplace_back(std::make_shared<ButtonClass>());
				ButtonSel.back()->LoadCommon(&this->m_SelectBackImage);
				ButtonSel.back()->Load_String(String, fontsize, IsEnableSelect);
				ButtonSel.back()->Set(xp, yp, FontX, FontY);
			}
			void AddIconButton(
				const char* IconPath, bool IsEnableSelect,
				int xp, int yp, UISystem::FontXCenter FontX, UISystem::FontYCenter FontY
			) noexcept {
				ButtonSel.emplace_back(std::make_shared<ButtonClass>());
				ButtonSel.back()->LoadCommon(&this->m_SelectBackImage);
				ButtonSel.back()->Load_Icon(IconPath, IsEnableSelect);
				ButtonSel.back()->Set(xp, yp, FontX, FontY);
			}
		};
		// 
		class FadeControl : public SingletonBase<FadeControl> {
		private:
			friend class SingletonBase<FadeControl>;
		private:
			bool						m_IsBlackOut{ false };// カットシーン中フラグ
			float						m_BlackOutAlpha{ 0.f };
		private:
			//コンストラクタ
			FadeControl(void) noexcept {}
			FadeControl(const FadeControl&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			FadeControl(FadeControl&& o) = delete;
			FadeControl& operator=(const FadeControl&) = delete;
			FadeControl& operator=(FadeControl&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		public:
			auto			IsFadeClear(void) const noexcept { return this->m_BlackOutAlpha == 0.f; }
			auto			IsFadeAll(void) const noexcept { return this->m_BlackOutAlpha >= 1.f; }
		public:
			// true=FadeOut
			void			SetFade(bool value) noexcept { this->m_IsBlackOut = value; }
			void			SetFadeIn(void) noexcept;
			void			SetFadeOut(void) noexcept;
		public:
			void			Update(void) noexcept;
			void			DrawFade(void) const noexcept;
		};
	}
}
