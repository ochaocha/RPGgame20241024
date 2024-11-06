#pragma once
#include	"../Header.hpp"

#include	"../CommonScene/UI/CommonUIControl.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		// タイトルシーン
		// --------------------------------------------------------------------------------------------------
		class TitleScene : public TEMPSCENE {
		private:
			bool						m_IsStartGame{ false };	//ゲームスタートフラグが立った
			GraphHandle					m_TitleImage;			//タイトル画像
		public:
			//コンストラクタ
			TitleScene(void) noexcept {}
			TitleScene(const TitleScene&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			TitleScene(TitleScene&& o) = delete;
			TitleScene& operator=(const TitleScene&) = delete;
			TitleScene& operator=(TitleScene&& o) = delete;
			//デストラクタ 継承オブジェクトである為デストラクタにvirtualを指定
			virtual ~TitleScene(void) noexcept {}
		public:
			//このシーンで永続して使用するリソースの読み込み、破棄をおこなうもの
			void			Load_Sub(void) noexcept override;
			void			Dispose_Load_Sub(void) noexcept override;
			// 描画以外の初期化、更新、破棄に関するもの
			void			Initialize_Sub(void) noexcept override;
			bool			Update_Sub(void) noexcept override;
			void			Dispose_Sub(void) noexcept override;
			// 2D空間表示
			void			DrawMain_Sub(void) const noexcept override;
			// UI表示
			void			DrawUI_Base_Sub(void) const noexcept override;
			void			DrawUI_In_Sub(void) const noexcept override {}
		};
	}
}
