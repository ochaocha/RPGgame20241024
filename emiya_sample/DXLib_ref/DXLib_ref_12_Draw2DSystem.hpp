#pragma once
#include "DXLib_ref.h"

#include <stdexcept>

// 共通で使うサイズを代入できるマクロ
#define LineHeight	DXDraw::Instance()->GetUIY(18)

/*------------------------------------------------------------------------------------------------------------------------------------------*/
// UI等の描画を行うための仕組みをそろえています
/*------------------------------------------------------------------------------------------------------------------------------------------*/
namespace DXLibRef {
	// カラープリセット
	static const unsigned int Red{ GetColor(255, 0, 0) };
	static const unsigned int Red25{ GetColor(192, 0, 0) };
	static const unsigned int Red50{ GetColor(128, 0, 0) };

	static const unsigned int Green{ GetColor(0, 255, 0) };
	static const unsigned int DarkGreen{ GetColor(0, 64, 0) };

	static const unsigned int Blue{ GetColor(0, 0, 255) };

	static const unsigned int Yellow{ GetColor(255, 255, 0) };

	static const unsigned int WhiteSel{ GetColor(216, 255, 216) };

	static const unsigned int White{ GetColor(255, 255, 255) };
	static const unsigned int Gray10{ GetColor(230, 230, 230) };
	static const unsigned int Gray15{ GetColor(216, 216, 216) };
	static const unsigned int Gray25{ GetColor(192, 192, 192) };
	static const unsigned int Gray50{ GetColor(128, 128, 128) };
	static const unsigned int Gray65{ GetColor(96, 96, 96) };
	static const unsigned int Gray75{ GetColor(64, 64, 64) };
	static const unsigned int Black{ GetColor(0, 0, 0) };


	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// DXLIBに実装されていないタイプの描画関数類
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// 縁ぬき四角
	static void DrawBoxLine_2D(int p1x, int p1y, int p2x, int p2y, const unsigned int& color, int thickness = 1) noexcept {
		if (thickness == 1) {
			DxLib::DrawBox(p1x, p1y, p2x, p2y, color, FALSE);
		}
		else {
			DxLib::DrawLine(p1x, p1y, p1x, p2y, color, thickness);
			DxLib::DrawLine(p1x, p1y, p2x, p1y, color, thickness);
			DxLib::DrawLine(p1x, p2y, p2x, p2y, color, thickness);
			DxLib::DrawLine(p2x, p1y, p2x, p2y, color, thickness);
		}
	}
	// グラデーションのある矩形を描画
	static void DrawGradationBox_2D(int x1, int y1, int x2, int y2, COLOR_U8 color1, COLOR_U8 color2, const unsigned char UorL = 255) noexcept {
		VERTEX2D Vertex[6]{};
		// 左上の頂点の情報をセット
		Vertex[0].pos.x = static_cast<float>(x1);
		Vertex[0].pos.y = static_cast<float>(y1);
		Vertex[0].pos.z = 0.0f;
		Vertex[0].rhw = 1.0f;
		Vertex[0].dif = color1;
		Vertex[0].u = 0.0f;
		Vertex[0].v = 0.0f;

		// 右上の頂点の情報をセット
		Vertex[1].pos.x = static_cast<float>(x2);
		Vertex[1].pos.y = static_cast<float>(y1);
		Vertex[1].pos.z = 0.0f;
		Vertex[1].rhw = 1.0f;
		Vertex[1].dif = color2;
		Vertex[1].u = 0.0f;
		Vertex[1].v = 0.0f;

		Vertex[5] = Vertex[1];

		// 左下の頂点の情報をセット
		Vertex[2].pos.x = static_cast<float>(x1);
		Vertex[2].pos.y = static_cast<float>(y2);
		Vertex[2].pos.z = 0.0f;
		Vertex[2].rhw = 1.0f;
		Vertex[2].dif = color1;
		Vertex[2].u = 0.0f;
		Vertex[2].v = 0.0f;

		Vertex[4] = Vertex[2];

		// 右下の頂点の情報をセット
		Vertex[3].pos.x = static_cast<float>(x2);
		Vertex[3].pos.y = static_cast<float>(y2);
		Vertex[3].pos.z = 0.0f;
		Vertex[3].rhw = 1.0f;
		Vertex[3].u = 0.0f;
		Vertex[3].v = 0.0f;

		switch (UorL) {
		case 3:// 横(右)
			// 左上の頂点の情報をセット
			Vertex[0].dif = color1;
			// 右上の頂点の情報をセット
			Vertex[1].dif = color2;
			Vertex[5] = Vertex[1];
			// 左下の頂点の情報をセット
			Vertex[2].dif = color1;
			Vertex[4] = Vertex[2];
			// 右下の頂点の情報をセット
			Vertex[3].dif = color2;
			break;
		case 2:// 縦(下)
			// 左上の頂点の情報をセット
			Vertex[0].dif = color1;
			// 右上の頂点の情報をセット
			Vertex[1].dif = color1;
			Vertex[5] = Vertex[1];
			// 左下の頂点の情報をセット
			Vertex[2].dif = color2;
			Vertex[4] = Vertex[2];
			// 右下の頂点の情報をセット
			Vertex[3].dif = color2;
			break;
		case 1:// 横(左)
			// 左上の頂点の情報をセット
			Vertex[0].dif = color2;
			// 右上の頂点の情報をセット
			Vertex[1].dif = color1;
			Vertex[5] = Vertex[1];
			// 左下の頂点の情報をセット
			Vertex[2].dif = color2;
			Vertex[4] = Vertex[2];
			// 右下の頂点の情報をセット
			Vertex[3].dif = color1;
			break;
		case 0:// 縦(上)
			// 左上の頂点の情報をセット
			Vertex[0].dif = color2;
			// 右上の頂点の情報をセット
			Vertex[1].dif = color2;
			Vertex[5] = Vertex[1];
			// 左下の頂点の情報をセット
			Vertex[2].dif = color1;
			Vertex[4] = Vertex[2];
			// 右下の頂点の情報をセット
			Vertex[3].dif = color1;
			break;
		default:
			// 左上の頂点の情報をセット
			Vertex[0].dif = color1;
			// 右上の頂点の情報をセット
			Vertex[1].dif = color1;
			Vertex[5] = Vertex[1];
			// 左下の頂点の情報をセット
			Vertex[2].dif = color1;
			Vertex[4] = Vertex[2];
			// 右下の頂点の情報をセット
			Vertex[3].dif = color1;
			break;
		}
		// ポリゴンを2個描画
		DrawPolygon2D(Vertex, 2, DX_NONE_GRAPH, FALSE);
	}
	// 	x1,y1 Angleが0の場合の左上座標
	// 	x2,y2 Angleが0の場合の右下座標
	// 	xminp,yminp 左上角からの固定長さ
	// 	xmaxp,ymaxp 右下角からの固定長さ
	// 	XCenter,YCenter　　: 画像を回転描画する画像上の中心座標(左上を(0.0f,0.0f)、右下を(1.0f,1.0f)とした割合)
	// 	Angle　　　: 描画角度（ラジアン指定）
	// 	GrHandle　 : 描画するグラフィックの識別番号（グラフィックハンドル）
	// 	TransFlag　: 画像の透明度を有効にするかどうか( TRUE：有効にする　FALSE：無効にする )
	// 	TilingFlag : 角以外の部分をタイリングするか拡縮させるか( TRUE：タイリング　FALSE：拡縮 )
	static void Draw9SliceGraph(
		int x1, int y1, int x2, int y2,
		int xminp, int yminp, int xmaxp, int ymaxp,
		float XCenter, float YCenter, float Angle,
		int GrHandle, bool TransFlag, bool TilingFlag) noexcept {
		// 最低限のサイズを指定
		if (x2 < x1 + xminp + xmaxp) {
			x2 = x1 + xminp + xmaxp;
		}
		if (y2 < y1 + yminp + ymaxp) {
			y2 = y1 + yminp + ymaxp;
		}
		// 用意する頂点情報
		std::vector<VERTEX2D> Vertex;
		std::vector<unsigned short> Index;

		float xs = static_cast<float>(x2 - x1);
		float ys = static_cast<float>(y2 - y1);

		float CenterX = static_cast<float>(x1) + xs * XCenter;
		float CenterY = static_cast<float>(y1) + ys * YCenter;

		auto SetUpPoint = [&](float xper, float yper, int xc, int yc) {
			Vertex.resize(Vertex.size() + 1);
			Vertex.back().pos = VGet(
				static_cast<float>(x1) + xs * xper - CenterX,
				static_cast<float>(y1) + ys * yper - CenterY,
				0.f);

			Vertex.back().pos = VGet(
				CenterX + Vertex.back().pos.x * std::cos(Angle) - Vertex.back().pos.y * std::sin(Angle),
				CenterY + Vertex.back().pos.x * std::sin(Angle) + Vertex.back().pos.y * std::cos(Angle),
				0.f);

			Vertex.back().rhw = 1.0f;
			Vertex.back().dif = GetColorU8(255, 255, 255, 255);
			Vertex.back().u = static_cast<float>(xc) / 3.f;
			Vertex.back().v = static_cast<float>(yc) / 3.f;
			return (unsigned short)(Vertex.size() - 1);
			};
		auto SetUpBox = [&](float xmin, float ymin, float xmax, float ymax, int xc, int yc) {
			Index.emplace_back(SetUpPoint(xmin, ymin, xc, yc));// 左上の頂点の情報をセット
			auto RU = SetUpPoint(xmax, ymin, xc + 1, yc);
			auto LD = SetUpPoint(xmin, ymax, xc, yc + 1);
			Index.emplace_back(RU);// 右上の頂点の情報をセット
			Index.emplace_back(LD);// 左下の頂点の情報をセット
			Index.emplace_back(SetUpPoint(xmax, ymax, xc + 1, yc + 1));// 右下の頂点の情報をセット
			Index.emplace_back(LD);// 左下の頂点の情報をセット
			Index.emplace_back(RU);// 右上の頂点の情報をセット
			};

		float xminpt = static_cast<float>(xminp) / xs;
		float xmaxpt = static_cast<float>(xmaxp) / xs;
		float xmaxt = 1.f - xmaxpt;
		float xmidt = xmaxt - xminpt;

		float yminpt = static_cast<float>(yminp) / ys;
		float ymaxpt = static_cast<float>(ymaxp) / ys;
		float ymaxt = 1.f - ymaxpt;
		float ymidt = ymaxt - yminpt;

		int xtile = 1;
		int ytile = 1;
		// タイリング
		if (TilingFlag) {
			xtile = static_cast<int>(xmidt / ((xminpt + xmaxpt) / 2.f)) + 1;
			if (xtile <= 0) {
				xtile = 1;
			}
			ytile = static_cast<int>(ymidt / ((yminpt + ymaxpt) / 2.f)) + 1;
			if (ytile <= 0) {
				ytile = 1;
			}
		}

		Vertex.reserve(static_cast<size_t>(3 * 2 * ((xtile + 2) * (ytile + 2))));
		float xmin = 0.f;
		float xmax = xminpt;
		int xc = 0;
		for (int x = 0; x < xtile + 2; ++x) {
			float ymin = 0.f;
			float ymax = yminpt;
			int yc = 0;
			for (int y = 0; y < ytile + 2; ++y) {
				SetUpBox(xmin, ymin, xmax, ymax, xc, yc);
				// 次
				ymin = ymax;
				ymax = TilingFlag ? (ymin + ymidt / static_cast<float>(ytile)) : ymaxt;
				if (y == 0) {
					yc = 1;
				}
				if (y == ytile) {
					ymax = 1.f;
					yc = 2;
				}
			}
			// 次
			xmin = xmax;
			xmax = TilingFlag ? (xmin + xmidt / static_cast<float>(xtile)) : xmaxt;
			if (x == 0) {
				xc = 1;
			}
			if (x == xtile) {
				xmax = 1.f;
				xc = 2;
			}
		}
		DrawPolygonIndexed2D(Vertex.data(), static_cast<int>(Vertex.size()), Index.data(), static_cast<int>(Index.size()) / 3, GrHandle, TransFlag ? TRUE : FALSE);
	}
	// きちんとセンタリングして文字を描画する(yumetodo氏のQiitaで公開されているコードそのまま)
	namespace draw_string_center_impl {
		static std::vector<DRAWCHARINFO> get_draw_string_char_info(const std::basic_string<TCHAR>& string, int font_handle) {
			std::vector<DRAWCHARINFO> info;
			info.resize(string.size());
			auto char_info_num = GetDrawStringCharInfoToHandle(info.data(), info.size(), string.c_str(), static_cast<int>(string.length() * sizeof(TCHAR)), font_handle, false);
			if (char_info_num < 0) throw std::runtime_error("fail in function DxLib::GetDrawStringCharInfoToHandle");
			if (info.size() < static_cast<std::size_t>(char_info_num)) {
				info.resize(static_cast<std::size_t>(char_info_num) + 1);
				// 再取得
				char_info_num = GetDrawStringCharInfoToHandle(info.data(), info.size(), string.c_str(), static_cast<int>(string.length() * sizeof(TCHAR)), font_handle, false);
				if (char_info_num < 0 || static_cast<int>(info.size()) < char_info_num) throw std::runtime_error("fail to detect draw info.");
			}
			info.resize(static_cast<std::size_t>(char_info_num));
			return info;
		}
	}
	static float draw_string_center(
		float draw_area_x_left, float draw_area_x_right, float draw_area_y_top, float draw_area_y_bottom,
		const std::basic_string<TCHAR>& string,
		unsigned int color, int font_handle,
		unsigned int edge_color = 0
	) {
		if (0 == string.length()) throw std::invalid_argument("empty string not allowed.");
		if (draw_area_x_right < draw_area_x_left || draw_area_y_bottom < draw_area_y_top) throw std::invalid_argument("");

		// 一文字ずつの描画幅情報を取得する
		const auto info = draw_string_center_impl::get_draw_string_char_info(string, font_handle);

		// ManageData->LineSpaceValidFlag ? ManageData->LineSpace : ManageData->BaseInfo.FontHeight
		const auto line_space = DxLib::GetFontLineSpaceToHandle(font_handle);
		const float area_width = draw_area_x_right - draw_area_x_left;
		const auto total_draw_width = info.back().DrawX + info.back().SizeX - info.front().DrawX;
		if (total_draw_width <= area_width) {
			// 一行ですむ場合
			const float padding = (area_width - total_draw_width) / 2.0f;
			DxLib::DrawStringFToHandle(draw_area_x_left + padding, draw_area_y_top, string.c_str(), color, font_handle, edge_color, false);
			return static_cast<float>(line_space);
		}

		// 複数行になる場合

		const float area_height = draw_area_y_bottom - draw_area_y_top;

		// 描画開始
		std::size_t current_string_byte_pos = 0;
		std::size_t line_front_string_byte_pos = 0;
		float current_y_relative = 0.0f;
		auto line_front_it = info.begin();
		for (auto it = info.begin(); it < info.end(); current_string_byte_pos += it->Bytes, ++it) {
			const auto line_width_contain_current_it_point_char = it->DrawX + it->SizeX - line_front_it->DrawX;
			if (area_width < line_width_contain_current_it_point_char) {
				using namespace std::string_literals;
				// 次の行に行く前に描画、itが指す文字は含まない
				const std::size_t str_len_byte = current_string_byte_pos - line_front_string_byte_pos;
				// it->DrawXは前の文字の右端に等しい
				// const float line_width = it->DrawX - line_front_it->DrawX;
				const float padding = 0.f;// (area_width - line_width) / 2.0f;
				const auto line_string = string.substr(line_front_string_byte_pos / sizeof(TCHAR), (str_len_byte / sizeof(TCHAR)));
				DxLib::DrawStringFToHandle(draw_area_x_left + padding, draw_area_y_top + current_y_relative, line_string.c_str(), color, font_handle, edge_color, false);
				// itが指す文字が先頭になる
				line_front_string_byte_pos = current_string_byte_pos;
				current_y_relative += static_cast<float>(line_space);
				line_front_it = it;
				if (area_height < current_y_relative) return current_y_relative;// 描画可能領域(y)を超えたら終了
			}
		}
		// 最終行の描画
		// const auto last_line_width = info.back().DrawX + info.back().SizeX - line_front_it->DrawX;
		const float padding = 0.f;// (area_width - last_line_width) / 2.0f;
		const auto line_string = string.substr(line_front_string_byte_pos / sizeof(TCHAR));
		DxLib::DrawStringFToHandle(draw_area_x_left + padding, draw_area_y_top + current_y_relative, line_string.c_str(), color, font_handle, edge_color, false);
		return current_y_relative + static_cast<float>(line_space);
	}
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// フォント
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	namespace UISystem {
		enum class FontXCenter : int {
			LEFT,
			MIDDLE,
			RIGHT,
		};
		enum class FontYCenter : int {
			TOP,
			MIDDLE,
			BOTTOM,
		};
		class FontHandle : public DXHandle {
		public:
		protected:
			void Dispose_Sub(void) noexcept override {
				DeleteFontToHandle(DXHandle::get());
			}
		public:
			// 長さ取得
			int GetDrawWidth(const std::basic_string<TCHAR>& String) const noexcept { return DxLib::GetDrawNStringWidthToHandle(String.data(), String.size(), DXHandle::get(), FALSE); }
			template <typename... Args>
			int GetDrawWidthFormat(const std::string& String, Args&&... args) const noexcept { return DxLib::GetDrawFormatStringWidthToHandle(DXHandle::get(), String.c_str(), args...); }
			int GetDrawExtendWidth(float siz, const std::basic_string<TCHAR>& String) const noexcept { return DxLib::GetDrawExtendNStringWidthToHandle(static_cast<double>(siz), String.data(), String.size(), DXHandle::get(), FALSE); }
			template <typename... Args>
			int GetDrawExtendWidthFormat(float siz, const std::string& String, Args&&... args) const noexcept { return DxLib::GetDrawExtendFormatStringWidthToHandle(static_cast<double>(siz), DXHandle::get(), String.c_str(), args...); }
			// 
			int GetFontSize(void) const noexcept { return GetFontSizeToHandle(DXHandle::get()); }
			// 文字描画
			template <typename... Args>
			void DrawString(int x, int y, unsigned int Color, unsigned int EdgeColor, const std::string& String, Args&&... args) const noexcept {
				DxLib::DrawFormatString2ToHandle(x, y, Color, EdgeColor, DXHandle::get(), String.c_str(), args...);
			}
			template <typename... Args>
			void DrawExtendString(int x, int y, float xsiz, float ysiz, unsigned int Color, unsigned int EdgeColor, const std::string& String, Args&&... args) const noexcept {
				DxLib::DrawExtendFormatString2ToHandle(x, y, static_cast<double>(xsiz), static_cast<double>(ysiz), Color, EdgeColor, DXHandle::get(), String.c_str(), args...);
			}
			// 文字描画
			auto DrawStringAutoFit(int x1, int y1, int x2, int y2, unsigned int Color, unsigned int EdgeColor, const std::string& String) const noexcept {
				return draw_string_center(static_cast<float>(x1), static_cast<float>(x2), static_cast<float>(y1), static_cast<float>(y2), String, Color, DXHandle::get(), EdgeColor);
			}
		public:
			// DXフォント用ハンドル作成
			void Load(const std::basic_string<TCHAR>& FontDataPath, int Edge) noexcept {
				DXHandle::SetHandleDirect(DxLib::LoadFontDataToHandleWithStrLen(FontDataPath.data(), FontDataPath.length(), Edge));
			}
		};
		// フォントプール
		class FontPool : public SingletonBase<FontPool> {
		private:
			friend class SingletonBase<FontPool>;
		public:
			enum class FontType {
				MS_Gothic,			// MSゴシック
				DIZ_UD_Gothic,		// DIZ UD ゴシック
			};
			class Fonthave {
				// カスタム項目
				FontType		m_Type{};
				int				m_EdgeSize{ InvalidID };// エッジサイズ
				int				m_CustomSize{ 0 };// フォントハンドル固有のサイズ
				// 
				int				m_scaleType{ DX_DRAWMODE_BILINEAR };
				int				m_commonsize{ 0 };// フォントハンドル固有のサイズ
				FontHandle		m_Handle;
			public:
				Fonthave(void) noexcept {}
				Fonthave(const Fonthave&) = delete;
				Fonthave(Fonthave&& o) = delete;
				Fonthave& operator=(const Fonthave&) = delete;
				Fonthave& operator=(Fonthave&& o) = delete;

				~Fonthave(void) noexcept {}
			public:
				// 文字の設定を行う
				void			Set(FontType type, int fontSize, int edgeSize) noexcept;
				// 設定が引数のものとすべて同じかどうか
				bool			Equal(FontType type, int fontSize, int edgeSize) const noexcept { return ((this->m_Type == type) && (this->m_CustomSize == fontSize) && (this->m_EdgeSize == edgeSize)); }
			public:
				// DrawStringを行った場合の文字の長さを取得
				template <typename... Args>
				auto			GetStringWidth(int fontSize, const std::string& String, Args&&... args) const noexcept {
					if (fontSize == InvalidID) {
						fontSize = this->m_CustomSize;
					}
					if (fontSize == this->m_commonsize) {
						return this->m_Handle.GetDrawWidthFormat(String, args...);
					}
					else {
						return this->m_Handle.GetDrawExtendWidthFormat(static_cast<float>(fontSize) / static_cast<float>(this->m_commonsize), String, args...);
					}
				}
				// 文字を描画
				template <typename... Args>
				void			DrawString(int fontSize, FontXCenter FontX, FontYCenter FontY, int x, int y, unsigned int Color, unsigned int EdgeColor, const std::string& String, Args&&... args) const noexcept {
					// X方向の調整
					switch (FontX) {
					case FontXCenter::LEFT:
						break;
					case FontXCenter::MIDDLE:
						x -= GetStringWidth(fontSize, String, args...) / 2;
						break;
					case FontXCenter::RIGHT:
						x -= GetStringWidth(fontSize, String, args...);
						break;
					default:
						break;
					}

					if (fontSize == InvalidID) {
						fontSize = this->m_CustomSize;
					}
					if (fontSize == this->m_commonsize) {
						// Y方向の調整
						switch (FontY) {
						case FontYCenter::TOP:
							break;
						case FontYCenter::MIDDLE:
							y -= this->m_Handle.GetFontSize() / 2;
							break;
						case FontYCenter::BOTTOM:
							y -= this->m_Handle.GetFontSize();
							break;
						default:
							break;
						}
						// 拡大不要なのでそのまま描画
						this->m_Handle.DrawString(x, y, Color, EdgeColor, String, args...);
					}
					else {
						// 指定した拡大モードに変更
						auto prev = GetDrawMode();
						SetDrawMode(this->m_scaleType);
						// Y方向の調整
						float Size = static_cast<float>(fontSize) / static_cast<float>(this->m_commonsize);
						switch (FontY) {
						case FontYCenter::TOP:
							break;
						case FontYCenter::MIDDLE:
							y -= static_cast<int>(static_cast<float>(this->m_Handle.GetFontSize() / 2) * Size);
							break;
						case FontYCenter::BOTTOM:
							y -= static_cast<int>(static_cast<float>(this->m_Handle.GetFontSize()) * Size);
							break;
						default:
							break;
						}
						// 拡縮して描画
						this->m_Handle.DrawExtendString(x, y, Size, Size, Color, EdgeColor, String, args...);
						// モードを戻す
						SetDrawMode(prev);
					}
				}
				// 文字を指定範囲内に描画
				auto			DrawStringAutoFit(int x1, int y1, int x2, int y2, unsigned int Color, unsigned int EdgeColor, const std::string& String) const noexcept {
					return this->m_Handle.DrawStringAutoFit(x1, y1, x2, y2, Color, EdgeColor, String);
				}
			};
		private:
			std::vector<std::unique_ptr<Fonthave>> m_Pools;
		private:
			// コンストラクタ
			FontPool(void) noexcept {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			FontPool(const FontPool&) = delete;
			FontPool(FontPool&& o) = delete;
			FontPool& operator=(const FontPool&) = delete;
			FontPool& operator=(FontPool&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		private:
			size_t Add(FontType type, int fontSize, int edgeSize) noexcept {
				size_t index = 0;
				for (auto & h : this->m_Pools) {
					if (h->Equal(type, fontSize, edgeSize)) {
						return index;
					}
					index++;
				}
				this->m_Pools.emplace_back(std::make_unique<Fonthave>());
				this->m_Pools.back()->Set(type, fontSize, edgeSize);
				return this->m_Pools.size() - 1;
			}
		public:
			std::unique_ptr<Fonthave>& Get(FontType type, int fontSize, int edgeSize) noexcept { return this->m_Pools[Add(type, fontSize, edgeSize)]; }
		};
	}
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// 描画
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	namespace UISystem {
		// 
		enum class DrawType : int {
			Alpha,
			Add,
			Bright,
			Box,
			Quadrangle,
			Circle,
			Line,
			String,
			StringAutoFit,
			RotaGraph,
			ExtendGraph,
			CircleGauge,
		};
		class DrawData {
			DrawType								m_type{ DrawType::Box };
			std::array<int, 8>						m_intParam{};
			std::array<unsigned int, 2>				m_UintParam{};
			std::array<float, 6>					m_floatParam{};
			std::array<bool, 1>						m_boolParam{};
			std::array<const GraphHandle*, 1>		m_GraphHandleParam{};
			std::string								m_string;
		public:
			void InputType(DrawType type) noexcept { this->m_type = type; }
			void InputintParam(int ID, int param) noexcept { this->m_intParam.at(ID) = param; }
			void InputUintParam(int ID, unsigned int param) noexcept { this->m_UintParam.at(ID) = param; }
			void InputfloatParam(int ID, float param) noexcept { this->m_floatParam.at(ID) = param; }
			void InputboolParam(int ID, bool param) noexcept { this->m_boolParam.at(ID) = param; }
			void InputGraphHandleParam(int ID, const GraphHandle* param) noexcept { this->m_GraphHandleParam.at(ID) = param; }
			void InputStringParam(const std::string& param) noexcept { this->m_string = param; }
		public:
			void Output() const noexcept;
		public:
			void		operator=(const DrawData& tgt) noexcept {
				this->m_type = tgt.m_type;
				this->m_intParam = tgt.m_intParam;
				this->m_UintParam = tgt.m_UintParam;
				this->m_floatParam = tgt.m_floatParam;
				this->m_boolParam = tgt.m_boolParam;
				this->m_GraphHandleParam = tgt.m_GraphHandleParam;
				this->m_string = tgt.m_string;
			}
			bool		operator==(const DrawData& tgt) const noexcept {
				return (
					this->m_type == tgt.m_type &&
					this->m_intParam == tgt.m_intParam &&
					this->m_UintParam == tgt.m_UintParam &&
					this->m_floatParam == tgt.m_floatParam &&
					this->m_boolParam == tgt.m_boolParam &&
					this->m_GraphHandleParam == tgt.m_GraphHandleParam &&
					this->m_string == tgt.m_string
					);
			}
		};
		// 
		enum class DrawLayer : int {
			BackGround,
			Back,
			Back2,
			Back3,
			Back4,
			Normal,
			Front,

			Max,
		};
		// 文字
		template <typename... Args>
		extern int GetMsgLen(int ySize, const std::string& String, Args&&... args) noexcept {
			return UISystem::FontPool::Instance()->Get(UISystem::FontPool::FontType::MS_Gothic, ySize, 3)->GetStringWidth(InvalidID, ((std::string)String).c_str(), args...);
		}
		// 
		class DrawControl : public SingletonBase<DrawControl> {
		private:
			friend class SingletonBase<DrawControl>;
		private:
			std::vector<std::vector<DrawData>>	m_DrawDatas;
			std::vector<std::vector<DrawData>>	m_PrevDrawDatas;

			GraphHandle				m_BufferScreen;
		private:
			// コンストラクタ
			DrawControl(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			DrawControl(const DrawControl&) = delete;
			DrawControl(DrawControl&& o) = delete;
			DrawControl& operator=(const DrawControl&) = delete;
			DrawControl& operator=(DrawControl&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません

			DrawData* GetBack(DrawLayer Layer) noexcept {
				this->m_DrawDatas.at((int)Layer).resize(this->m_DrawDatas.at((int)Layer).size() + 1);
				return &this->m_DrawDatas.at((int)Layer).back();
			}
		public:
			bool	IsDrawOnWindow(int x1, int y1, int x2, int y2) noexcept;
			// 
			void	SetAlpha(DrawLayer Layer, int Alpha) noexcept {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Alpha);
				Back->InputintParam(0, Alpha);
			}
			// 
			void	SetAdd(DrawLayer Layer, int Add) noexcept {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Add);
				Back->InputintParam(0, Add);
			}
			// 
			void	SetBright(DrawLayer Layer, int valueR, int valueG, int valueB) noexcept {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Bright);
				Back->InputintParam(0, valueR);
				Back->InputintParam(1, valueG);
				Back->InputintParam(2, valueB);
			}
			// 
			void	SetDrawBox(DrawLayer Layer, int x1, int y1, int x2, int y2, unsigned int color1, bool IsFill) noexcept {
				if (!IsDrawOnWindow(x1, y1, x2, y2)) { return; }				// 画面外は表示しない
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Box);
				Back->InputintParam(0, x1);
				Back->InputintParam(1, y1);
				Back->InputintParam(2, x2);
				Back->InputintParam(3, y2);
				Back->InputUintParam(0, color1);
				Back->InputboolParam(0, IsFill);
			}
			// 
			void	SetDrawQuadrangle(DrawLayer Layer, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int color1, bool IsFill) noexcept {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Quadrangle);
				Back->InputintParam(0, x1);
				Back->InputintParam(1, y1);
				Back->InputintParam(2, x2);
				Back->InputintParam(3, y2);
				Back->InputintParam(4, x3);
				Back->InputintParam(5, y3);
				Back->InputintParam(6, x4);
				Back->InputintParam(7, y4);
				Back->InputUintParam(0, color1);
				Back->InputboolParam(0, IsFill);
			}
			// 
			void	SetDrawCircle(DrawLayer Layer, int x1, int y1, int radius, unsigned int color1, bool IsFill = true, int LineThickness = 1) {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Circle);
				Back->InputintParam(0, x1);
				Back->InputintParam(1, y1);
				Back->InputintParam(2, radius);
				Back->InputUintParam(0, color1);
				Back->InputboolParam(0, IsFill);
				Back->InputintParam(3, LineThickness);
			}
			// 
			void	SetDrawLine(DrawLayer Layer, int x1, int y1, int x2, int y2, unsigned int color1, int   Thickness = 1) noexcept {
				if (!IsDrawOnWindow(x1, y1, x2, y2)) { return; }				// 画面外は表示しない
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::Line);
				Back->InputintParam(0, x1);
				Back->InputintParam(1, y1);
				Back->InputintParam(2, x2);
				Back->InputintParam(3, y2);
				Back->InputUintParam(0, color1);
				Back->InputintParam(4, Thickness);
			}
			// 
			void SetDrawRotaGraph(DrawLayer Layer, const GraphHandle* pGraphHandle, int posx, int posy, float Exrate, float rad, bool trns) noexcept {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::RotaGraph);
				Back->InputGraphHandleParam(0, pGraphHandle);
				Back->InputintParam(0, posx);
				Back->InputintParam(1, posy);
				Back->InputfloatParam(0, Exrate);
				Back->InputfloatParam(1, rad);
				Back->InputboolParam(0, trns);
			}
			// 
			void SetDrawExtendGraph(DrawLayer Layer, const GraphHandle* pGraphHandle, int x1, int y1, int x2, int y2, bool trns) noexcept {
				if (!IsDrawOnWindow(x1, y1, x2, y2)) { return; }				// 画面外は表示しない
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::ExtendGraph);
				Back->InputGraphHandleParam(0, pGraphHandle);
				Back->InputintParam(0, x1);
				Back->InputintParam(1, y1);
				Back->InputintParam(2, x2);
				Back->InputintParam(3, y2);
				Back->InputboolParam(0, trns);
			}
			// 
			void SetDrawCircleGauge(DrawLayer Layer, const GraphHandle* pGraphHandle, int   CenterX, int   CenterY, float Percent, float StartPercent = 0.f, float Scale = 1.0f) noexcept {
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::CircleGauge);
				Back->InputGraphHandleParam(0, pGraphHandle);
				Back->InputintParam(0, CenterX);
				Back->InputintParam(1, CenterY);
				Back->InputfloatParam(0, Percent);
				Back->InputfloatParam(1, StartPercent);
				Back->InputfloatParam(2, Scale);
			}
			// 
			template <typename... Args>
			void	SetString(DrawLayer Layer, UISystem::FontPool::FontType type, int fontSize, UISystem::FontXCenter FontX, UISystem::FontYCenter FontY, int x, int y, unsigned int Color, unsigned int EdgeColor, const std::string& Str, Args&&... args) noexcept {
				if (Str == "") { return; }
				int xSize = UISystem::GetMsgLen(fontSize, Str.c_str(), args...);
				switch (FontX) {
				case UISystem::FontXCenter::LEFT:
					if (!IsDrawOnWindow((x), (y - fontSize), (x + xSize), (y + fontSize))) { return; }				// 画面外は表示しない
					break;
				case UISystem::FontXCenter::MIDDLE:
					if (!IsDrawOnWindow((x - xSize / 2), (y - fontSize), (x + xSize / 2), (y + fontSize))) { return; }				// 画面外は表示しない
					break;
				case UISystem::FontXCenter::RIGHT:
					if (!IsDrawOnWindow((x - xSize), (y - fontSize), (x), (y + fontSize))) { return; }				// 画面外は表示しない
					break;
				default:
					break;
				}

				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::String);

				Back->InputintParam(0, (int)type);
				Back->InputintParam(1, fontSize);
				Back->InputintParam(2, (int)FontX);
				Back->InputintParam(3, (int)FontY);
				Back->InputintParam(4, x);
				Back->InputintParam(5, y);
				Back->InputUintParam(0, Color);
				Back->InputUintParam(1, EdgeColor);

				char ptr[1024] = "";
				snprintfDx(ptr, 1024, Str.c_str(), args...);
				Back->InputStringParam(ptr);
			}
			// 
			const auto	SetStringAutoFit(DrawLayer Layer, UISystem::FontPool::FontType type, int fontSize, int x1, int y1, int x2, int y2, unsigned int Color, unsigned int EdgeColor, const std::string& Str) noexcept {
				if (Str == "") { return 0.f; }
				DrawData* Back = GetBack(Layer);
				Back->InputType(DrawType::StringAutoFit);

				Back->InputintParam(0, (int)type);
				Back->InputintParam(1, fontSize);
				Back->InputintParam(2, x1);
				Back->InputintParam(3, y1);
				Back->InputintParam(4, x2);
				Back->InputintParam(5, y2);
				Back->InputUintParam(0, Color);
				Back->InputUintParam(1, EdgeColor);

				Back->InputStringParam(Str);

				return UISystem::FontPool::Instance()->Get((UISystem::FontPool::FontType)type, fontSize, 3)->DrawStringAutoFit(
					x1 + BaseScreenWidth, y1 + BaseScreenHeight,
					x2 + BaseScreenWidth, y2 + BaseScreenHeight,
					Color,
					EdgeColor,
					Str
				);
			}
			// 
		public:
			void	ClearList(void) noexcept {
				size_t index = 0;
				for (auto& d : this->m_DrawDatas) {
					auto& pd = this->m_PrevDrawDatas.at(index);
					pd.clear();
					for (auto& d2 : d) {
						pd.resize(pd.size() + 1);
						pd.back() = d2;
					}
					index++;
				}
				for (auto& d : this->m_DrawDatas) {
					d.clear();
				}
			}
			void	Draw(void) noexcept {
				auto* DrawCtrls = UISystem::DrawControl::Instance();
				bool IsHit = false;
				// 同じかどうかチェック
				size_t index = 0;
				for (auto& d : this->m_DrawDatas) {
					auto& pd = this->m_PrevDrawDatas.at(index);
					if (pd.size() == d.size()) {
						size_t index2 = 0;
						for (auto& d2 : d) {
							auto& pd2 = pd.at(index2);
							if (!(pd2 == d2)) {
								IsHit = true;
								break;
							}
							index2++;
						}
					}
					else {
						IsHit = true;
					}
					index++;
				}
				// 
				if (IsHit) {
					{
						auto NowScreen = GetDrawScreen();
						m_BufferScreen.SetDraw_Screen(true);
						{
							for (auto& d : this->m_DrawDatas) {
								for (auto& d2 : d) {
									d2.Output();
								}
								DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
								SetDrawBright(255, 255, 255);
							}
						}
						GraphHandle::SetDraw_Screen(NowScreen, false);
					}
				}
				// 前に描画したものをそのまま出す
				m_BufferScreen.DrawGraph(0, 0, true);
			}
			void	Dispose(void) noexcept {
				for (auto& d : this->m_DrawDatas) {
					d.clear();
				}
				this->m_DrawDatas.clear();

				for (auto& d : this->m_PrevDrawDatas) {
					d.clear();
				}
				this->m_PrevDrawDatas.clear();
			}
		};
		// 箱
		extern void SetBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet) noexcept;
		// マウスでクリックできるボタン
		extern bool SetClickBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet, bool IsRepeat, bool IsActive) noexcept;
		// 文字
		bool GetMsgPosOn(int* xp1, int* yp1, int ySize, int xSize, UISystem::FontXCenter FontX) noexcept;
		// 文字の長さを取得
		template <typename... Args>
		extern void SetMsg(int xp1, int yp1, int ySize, UISystem::FontXCenter FontX, unsigned int Color, unsigned int EdleColor, const std::string& String, Args&&... args) noexcept {
			if (String == "") { return; }
			// if (!GetMsgPosOn(&xp1, &yp1, ySize, GetMsgLen(ySize, String, args...), FontX)) { return; }
			DrawControl::Instance()->SetString(DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic, ySize,
				FontX, UISystem::FontYCenter::MIDDLE, xp1, yp1, Color, EdleColor, ((std::string)String).c_str(), args...);
		}
		// クリックできる文字付のボックス
		template <typename... Args>
		extern bool SetMsgClickBox(int xp1, int yp1, int xp2, int yp2, int StringYSizeMax, unsigned int defaultcolor, bool IsRepeat, bool IsActive, const std::string& String, Args&&... args) noexcept {
			bool ret = SetClickBox(xp1, yp1, xp2, yp2, defaultcolor, IsRepeat, IsActive);
			SetMsg((xp1 + xp2) / 2, (yp1 + yp2) / 2, GetMin(StringYSizeMax, yp2 - yp1), UISystem::FontXCenter::MIDDLE, White, Black, String, args...);
			return ret;
		}
		// 文字付のボックス
		template <typename... Args>
		extern void SetMsgBox(int xp1, int yp1, int xp2, int yp2, int StringYSizeMax, unsigned int defaultcolor, const std::string& String, Args&&... args) noexcept {
			SetBox(xp1, yp1, xp2, yp2, defaultcolor);
			SetMsg((xp1 + xp2) / 2, (yp1 + yp2) / 2, GetMin(StringYSizeMax, yp2 - yp1), UISystem::FontXCenter::MIDDLE, White, Black, String, args...);
		}
		// オンオフできるボタン
		extern bool CheckBox(int xp1, int yp1, bool switchturn) noexcept;
		// スライダー
		extern int UpDownBar(int xmin, int xmax, int yp, int value, int valueMin, int valueMax) noexcept;
		// 0~valueMaxの選択制ボタン集
		extern int UpDownBox(int xmin, int xmax, int yp, int value, int valueMax) noexcept;

		/*------------------------------------------------------------------------------------------------------------------------------------------*/
		// ポップアップ
		/*------------------------------------------------------------------------------------------------------------------------------------------*/
		class PopUp : public SingletonBase<PopUp> {
		private:
			friend class SingletonBase<PopUp>;
		private:
			class PopUpDrawClass {
			private:
				bool m_Active{ false };
				bool m_ActiveSwitch{ false };
				float m_ActivePer{ 0.f };
				char m_WindwoName[64]{};

				int WinSizeX{ 720 };
				int WinSizeY{ 720 };

				std::function<void(int xmin, int ymin, int xmax, int ymax, bool EndSwitch)> m_Doing{ nullptr };
				std::function<void()> m_ExitDoing{ nullptr };
				std::function<void()> m_GuideDoing{ nullptr };
			public:
				PopUpDrawClass(void) noexcept {}
				PopUpDrawClass(const PopUpDrawClass&) = delete;
				PopUpDrawClass(PopUpDrawClass&& o) = delete;
				PopUpDrawClass& operator=(const PopUpDrawClass&) = delete;
				PopUpDrawClass& operator=(PopUpDrawClass&& o) = delete;

				~PopUpDrawClass(void) noexcept {}
			public:
				void			Set(const char* WindowName, int sizex, int sizey,
					std::function<void(int xmin, int ymin, int xmax, int ymax, bool EndSwitch)> doing,
					std::function<void()> ExitDoing,
					std::function<void()> GuideDoing
				) noexcept {
					strcpy_sDx(m_WindwoName, 64, WindowName);
					WinSizeX = sizex;
					WinSizeY = sizey;
					m_Doing = doing;
					m_ExitDoing = ExitDoing;
					m_GuideDoing = GuideDoing;
				}
				void			Start(void) noexcept;
				void			End(void) noexcept;
				void			Update(void) noexcept;
				void			Draw(int xcenter, int ycenter) noexcept;
			public:
				auto IsEnd(void) const noexcept { return !m_Active && !(m_ActivePer > 1.f / 255.f); }
			};
		private:
			std::array<PopUpDrawClass, 24> que;
			size_t NowSel{ 0 };
			size_t LastSel{ 0 };
			bool PrevPause{ false };
		private:
			// コンストラクタ
			PopUp(void) noexcept {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			PopUp(const PopUp&) = delete;
			PopUp(PopUp&& o) = delete;
			PopUp& operator=(const PopUp&) = delete;
			PopUp& operator=(PopUp&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		public:
			auto IsActivePop(void) const noexcept { return (NowSel != LastSel); }
		public:
			void Add(const char* WindowName, int sizex, int sizey,
				std::function<void(int xmin, int ymin, int xmax, int ymax, bool EndSwitch)> doing,
				std::function<void()> ExitDoing,
				std::function<void()> GuideDoing,
				bool IsInsert = false) noexcept;
			void EndAll(void) noexcept;
			void Update(void) noexcept;
			void Draw(int xcenter, int ycenter) noexcept {
				if (!IsActivePop()) {
					return;
				}
				que.at(static_cast<size_t>(NowSel)).Draw(xcenter, ycenter);
			}
		};
		// --------------------------------------------------------------------------------------------------
		// キーガイド
		// --------------------------------------------------------------------------------------------------
		class KeyGuide : public SingletonBase<KeyGuide> {
		private:
			friend class SingletonBase<KeyGuide>;
		private:
			// コンストラクタ
			KeyGuide(void) noexcept;
			// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			KeyGuide(const KeyGuide&) = delete;
			KeyGuide(KeyGuide&& o) = delete;
			KeyGuide& operator=(const KeyGuide&) = delete;
			KeyGuide& operator=(KeyGuide&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		private:
			class KeyGuideGraph {
				int xsize{ 0 }, ysize{ 0 };
				GraphHandle GuideImg;
			public:
				KeyGuideGraph(void) noexcept {}
				KeyGuideGraph(const KeyGuideGraph&) = delete;
				KeyGuideGraph(KeyGuideGraph&& o) = delete;
				KeyGuideGraph& operator=(const KeyGuideGraph&) = delete;
				KeyGuideGraph& operator=(KeyGuideGraph&& o) = delete;

				~KeyGuideGraph(void) noexcept {}
			public:
				void AddGuide(int x, int y, int xs, int ys, const GraphHandle& baseImage) noexcept {
					GuideImg.DerivationGraph(x, y, xs, ys, baseImage);
					xsize = xs;
					ysize = ys;
				}
				void Reset(void) noexcept { GuideImg.Dispose(); }
				int GetDrawSize(void) const noexcept;
				int Draw(int x, int y) const noexcept;
			};
			class KeyGuideOnce {
				std::shared_ptr<KeyGuideGraph> m_GuideGraph;
				std::string GuideString;
			public:
				KeyGuideOnce(void) noexcept {}
				KeyGuideOnce(const KeyGuideOnce&) = delete;
				KeyGuideOnce(KeyGuideOnce&& o) = delete;
				KeyGuideOnce& operator=(const KeyGuideOnce&) = delete;
				KeyGuideOnce& operator=(KeyGuideOnce&& o) = delete;

				~KeyGuideOnce(void) noexcept {}
			public:
				void AddGuide(const std::shared_ptr<KeyGuideGraph>& pGuide, const std::string& GuideStr) noexcept {
					m_GuideGraph = pGuide;
					GuideString = GuideStr;
				}
				void Reset(void) noexcept {
					if (m_GuideGraph) {
						m_GuideGraph.reset();
					}
					GuideString = "";
				}
				int GetDrawSize(void) const noexcept;
				int Draw(int x, int y) const noexcept;
			};
		private:
			bool													m_IsUpdateGuide{ true };				// ガイドのコントロール
			GraphHandle												m_GuideBaseImage;						//分割前の画像
			std::vector<std::shared_ptr<KeyGuideGraph>>				m_DerivationGuideImage;	//分割後の画像
			std::vector<std::unique_ptr<KeyGuideOnce>>				m_Key;
		public:
			void SetGuideUpdate(void) noexcept { m_IsUpdateGuide = true; }
		public:
			const int GetIDtoOffset(int ID, ControlType ControlType) const noexcept {
				switch (ControlType) {
				case ControlType::XBox:
					for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
						if (XBoxID[i] == ID) {
							return static_cast<int>(i + KeyNum + XBoxNum);
						}
					}
					break;
				case ControlType::PS4:
					for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
						if (DS4ID[i] == ID) {
							return static_cast<int>(i + KeyNum);
						}
					}
					break;
				case ControlType::PC:
					for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
						if (KeyID[i] == ID) {
							return static_cast<int>(i);
						}
					}
					break;
				default:
					break;
				}
				return InvalidID;
			}
			void ChangeGuide(std::function<void()>Guide_Pad) noexcept;
			void AddGuide(int graphOffset, const std::string& GuideStr) noexcept {
				m_Key.emplace_back(std::make_unique<KeyGuideOnce>());
				m_Key.back()->AddGuide((graphOffset != InvalidID) ? m_DerivationGuideImage.at(graphOffset) : nullptr, GuideStr);
			}
			void Reset(void) noexcept {
				for (auto& k : m_Key) {
					k->Reset();
					k.reset();
				}
				m_Key.clear();
			}
		public:
			void Draw(void) const noexcept;
			void DrawButton(int x, int y, int graphOffset) const noexcept {
				m_DerivationGuideImage.at(graphOffset)->Draw(x, y);
			}
		};
	}
}
