#pragma once
#include	"../Header.hpp"

#include	"../CommonScene/UI/CommonUIControl.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		class TitleScene : public TEMPSCENE {
		private:
			UI::FadeControl				m_FadeControl;
			bool						m_IsStartGame{ false };

			GraphHandle					m_TitleImage;
		public:
			TitleScene(void) noexcept { }
			TitleScene(const TitleScene&) = delete;
			TitleScene(TitleScene&& o) = delete;
			TitleScene& operator=(const TitleScene&) = delete;
			TitleScene& operator=(TitleScene&& o) = delete;

			virtual ~TitleScene(void) noexcept {}
		public:
			void			Set_Sub(void) noexcept override;
			bool			Update_Sub(void) noexcept override;
			void			Dispose_Sub(void) noexcept override;
			// 
			void			MainDraw_Sub(void) const noexcept override;
			// UI•\Ž¦
			void			DrawUI_Base_Sub(void) const noexcept override;
			void			DrawUI_In_Sub(void) const noexcept override {}
		};
	}
}
