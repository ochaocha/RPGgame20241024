#pragma once
#include "DXLib_ref.h"
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// 画像ハンドルの管理を行うクラス
/*------------------------------------------------------------------------------------------------------------------------------------------*/
namespace DXLibRef {
	class GraphHandle : public DXHandle {
	public:
		// 破棄する際に呼ばれる関数
		void	Dispose_Sub(void) noexcept override {
			DeleteGraph(DXHandle::get());
		}
	public:
		// 左上座標を指定して画像を描画
		void DrawGraph(int posx, int posy, bool trns) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			DxLib::DrawGraph(posx, posy, DXHandle::get(), (trns ? TRUE : FALSE));
		}
		// 中心座標を指定したうえで回転、拡大を指定しての描画
		void DrawRotaGraph(int posx, int posy, float Exrate, float rad, bool trns) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			DxLib::DrawRotaGraph(posx, posy, static_cast<double>(Exrate), static_cast<double>(rad), DXHandle::get(), (trns ? TRUE : FALSE));
		}
		// DrawRotaGraphのFloat版
		void DrawRotaGraphF(float posx, float posy, float Exrate, float rad, bool trns) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			DxLib::DrawRotaGraphF(posx, posy, static_cast<double>(Exrate), static_cast<double>(rad), DXHandle::get(), (trns ? TRUE : FALSE));
		}
		// DrawRotaGraph3
		void DrawRotaGraph3(int posx, int posy,
			int cx, int cy,
			float ExtRateX, float ExtRateY, float rad, bool trns) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			DxLib::DrawRotaGraph3(posx, posy, cx, cy, static_cast<double>(ExtRateX), static_cast<double>(ExtRateY), static_cast<double>(rad), DXHandle::get(), (trns ? TRUE : FALSE));
		}
		// 左上、右下を指定して拡大描画
		void DrawExtendGraph(int posx1, int posy1, int posx2, int posy2, bool trns) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			DxLib::DrawExtendGraph(posx1, posy1, posx2, posy2, DXHandle::get(), (trns ? TRUE : FALSE));
		}
		// 指定した色で自身を塗りつぶす
		void FillGraph(int r, int g, int b) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			DxLib::FillGraph(DXHandle::get(), r, g, b);
		}
		// 自身にフィルタ加工を行う
		template <typename... Args>
		void GraphFilter(int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */, Args&&... args) noexcept {
			DxLib::GraphFilter(DXHandle::get(), FilterType, args...);
		}
		// 渡された画像のフィルタ加工結果を自分に書き込む
		template <typename... Args>
		void GraphFilterBlt(const GraphHandle& targetImage, int FilterType /* DX_GRAPH_FILTER_GAUSS 等 */, Args&&... args) noexcept {
			DxLib::GraphFilterBlt(targetImage.get(), DXHandle::get(), FilterType, args...);
		}
		// 自分と渡された画像とのブレンドを行う
		template <typename... Args>
		void GraphBlend(const GraphHandle& BlendImage, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */, int BlendType /* DX_GRAPH_BLEND_ADD 等 */, Args&&... args) noexcept {
			DxLib::GraphBlend(DXHandle::get(), BlendImage.get(), BlendRatio, BlendType, args...);
		}
		// 渡された2枚のブレンド結果を自分に書き込む
		template <typename... Args>
		void GraphBlendBlt(const GraphHandle& BaseImage, const GraphHandle& BlendImage, int BlendRatio /* ブレンド効果の影響度( 0:０％  255:１００％ ) */, int BlendType /* DX_GRAPH_BLEND_ADD 等 */, Args&&... args) noexcept {
			DxLib::GraphBlendBlt(BaseImage.get(), BlendImage.get(), DXHandle::get(), BlendRatio, BlendType, args...);
		}
		// 自分の縦横の画像サイズを取得
		void GetSize(int* xsize, int* ysize) const noexcept {
			if (!DXHandle::IsActive()) { return; }
			GetGraphSize(DXHandle::get(), xsize, ysize);
		}
		// 自分を描画対象に設定する
		void SetDraw_Screen(const bool& Clear = true) const noexcept {
			SetDrawScreen(DXHandle::get());
			if (Clear) {
				ClearDrawScreen();
			}
		}
		static void SetDraw_Screen(int handle, const bool& Clear = true) noexcept {
			SetDrawScreen(handle);
			if (Clear) {
				ClearDrawScreen();
			}
		}
		// シェーダーへ自分が描画対象/使用テクスチャであることを指定する
		void SetUseTextureToShader(int ID) const noexcept {
			DxLib::SetUseTextureToShader(ID, DXHandle::get());
		}
		void SetRenderTargetToShader(int ID, int SurfaceIndex = 0, int MipLevel = 0) const noexcept {
			DxLib::SetRenderTargetToShader(ID, DXHandle::get(), SurfaceIndex, MipLevel);
		}
	public:// 読み込み系
		// 画像を読み込み
		void Load(const std::basic_string<TCHAR>& FileName, bool NotUse3DFlag = false) noexcept {
			DXHandle::SetHandleDirect(DxLib::LoadGraphWithStrLen(FileName.data(), FileName.length(), NotUse3DFlag));
		}
		// 分割して画像を読み込み(static)
		static void LoadDiv(const std::basic_string<TCHAR>& FileName, int AllNum, int XNum, int YNum, int  XSize, int  YSize, std::vector<GraphHandle>* Handles, bool NotUse3DFlag = false) noexcept {
			int* HandleArray = new int[static_cast<size_t>(AllNum)];
			DxLib::LoadDivGraphWithStrLen(FileName.data(), FileName.length(), AllNum, XNum, YNum, XSize, YSize, HandleArray, NotUse3DFlag);

			Handles->clear();
			for (int i = 0; i < AllNum; ++i) {
				Handles->resize(Handles->size() + 1);
				Handles->back().SetHandleDirect(HandleArray[static_cast<size_t>(i)]);
			}
			delete[] HandleArray;

			return;
		}
		// 渡された画像ハンドルの一部を切り抜いた画像ハンドルを作成
		void DerivationGraph(int x, int y, int xsize, int ysize, const GraphHandle& baseImage) noexcept {
			DXHandle::SetHandleDirect(DxLib::DerivationGraph(x, y, xsize, ysize, baseImage.get()));
		}
		// SetDrawScreenの対象にできる画像ハンドルを作成
		void Make(int SizeX, int SizeY, bool trns = false) noexcept {
			DXHandle::SetHandleDirect(DxLib::MakeScreen(SizeX, SizeY, (trns ? TRUE : FALSE)));
		}
		// ビットマップから画像ハンドルを作成
		void CreateGraphFromBmp(const BITMAPINFO* RGBBmpInfo, const void* RGBBmpImage, const BITMAPINFO* AlphaBmpInfo = nullptr, const void* AlphaBmpImage = nullptr, bool TextureFlag = true, bool ReverseFlag = false) noexcept {
			DXHandle::SetHandleDirect(DxLib::CreateGraphFromBmp(RGBBmpInfo, RGBBmpImage, AlphaBmpInfo, AlphaBmpImage, TextureFlag ? TRUE : FALSE, ReverseFlag ? TRUE : FALSE));
		}
	};
};
