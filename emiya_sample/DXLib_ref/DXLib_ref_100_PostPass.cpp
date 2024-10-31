#include "DXLib_ref_100_PostPass.hpp"

namespace DXLibRef {
	// �V���O���g���̎��Ԓ�`
	const PostPassEffect* SingletonBase<PostPassEffect>::m_Singleton = nullptr;
	// --------------------------------------------------------------------------------------------------
	// �|�X�g�v���Z�X�p���N���X
	// --------------------------------------------------------------------------------------------------
	// �P�x��250�ȏ�̏ꏊ���`�F�b�N���Ă��̕����������Ă���悤�Ȍ��ʂ�ǉ�����
	class PostPassBloom : public PostPassBase {
	private:
		static const int EXTEND = 4;
	private:
		GraphHandle GaussScreen_;	// �`��X�N���[��
		GraphHandle	BufScreen;
	public:
		PostPassBloom(void) noexcept {}
		PostPassBloom(const PostPassBloom&) = delete;
		PostPassBloom(PostPassBloom&& o) = delete;
		PostPassBloom& operator=(const PostPassBloom&) = delete;
		PostPassBloom& operator=(PostPassBloom&& o) = delete;

		virtual ~PostPassBloom(void) noexcept {}
	public:
		void Load_Sub(void) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			GaussScreen_.Make(DrawParts->GetScreenXMax() / EXTEND, DrawParts->GetScreenYMax() / EXTEND, true);
			BufScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
		}
		void Dispose_Sub(void) noexcept override {
			GaussScreen_.Dispose();
			BufScreen.Dispose();
		}
		bool IsActive_Sub(void) noexcept override {
			auto* OptionParts = OPTION::Instance();
			return OptionParts->GetParamBoolean(EnumSaveParam::bloom);
		}
		void SetEffect_Sub(GraphHandle* TargetGraph, GraphHandle*) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			BufScreen.GraphFilterBlt(*TargetGraph, DX_GRAPH_FILTER_TWO_COLOR, 250, Black, 255, Gray50, 255);
			GaussScreen_.GraphFilterBlt(BufScreen, DX_GRAPH_FILTER_DOWN_SCALE, EXTEND);
			GaussScreen_.GraphFilter(DX_GRAPH_FILTER_GAUSS, 16, 1000);
			TargetGraph->SetDraw_Screen(false);
			{
				auto Prev = GetDrawMode();
				SetDrawMode(DX_DRAWMODE_BILINEAR);
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				GaussScreen_.DrawExtendGraph(0, 0, DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
				GaussScreen_.DrawExtendGraph(0, 0, DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				SetDrawMode(Prev);
			}
		}
	};
	// �F������ǉ�����
	class PostPassAberration : public PostPassBase {
	private:
		std::array<GraphHandle, 3>		BufScreen;
	public:
		PostPassAberration(void) noexcept {}
		PostPassAberration(const PostPassAberration&) = delete;
		PostPassAberration(PostPassAberration&& o) = delete;
		PostPassAberration& operator=(const PostPassAberration&) = delete;
		PostPassAberration& operator=(PostPassAberration&& o) = delete;

		virtual ~PostPassAberration(void) noexcept {}
	public:
		void Load_Sub(void) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			for (auto& buf : BufScreen) {
				buf.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
			}
		}
		void Dispose_Sub(void) noexcept override {
			for (auto& buf : BufScreen) {
				buf.Dispose();
			}
		}
		bool IsActive_Sub(void) noexcept override {
			auto* OptionParts = OPTION::Instance();
			return OptionParts->GetParamBoolean(EnumSaveParam::ScreenEffect);
		}
		void SetEffect_Sub(GraphHandle* TargetGraph, GraphHandle*) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			auto* PostPassParts = PostPassEffect::Instance();
			BufScreen[0].SetDraw_Screen(false);
			BufScreen[0].FillGraph(0, 0, 0);
			BufScreen[1].SetDraw_Screen(false);
			BufScreen[1].FillGraph(0, 0, 0);
			BufScreen[2].SetDraw_Screen(false);
			BufScreen[2].FillGraph(0, 0, 0);
			BufScreen[0].GraphBlend(*TargetGraph, 255, DX_GRAPH_BLEND_RGBA_SELECT_MIX,
				DX_RGBA_SELECT_BLEND_R, DX_RGBA_SELECT_SRC_G, DX_RGBA_SELECT_SRC_B, DX_RGBA_SELECT_SRC_A);
			BufScreen[1].GraphBlend(*TargetGraph, 255, DX_GRAPH_BLEND_RGBA_SELECT_MIX,
				DX_RGBA_SELECT_SRC_R, DX_RGBA_SELECT_BLEND_G, DX_RGBA_SELECT_SRC_B, DX_RGBA_SELECT_SRC_A);
			BufScreen[2].GraphBlend(*TargetGraph, 255, DX_GRAPH_BLEND_RGBA_SELECT_MIX,
				DX_RGBA_SELECT_SRC_R, DX_RGBA_SELECT_SRC_G, DX_RGBA_SELECT_BLEND_B, DX_RGBA_SELECT_SRC_A);
			TargetGraph->SetDraw_Screen(false);
			{
				TargetGraph->FillGraph(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				BufScreen[0].DrawRotaGraph(DrawParts->GetScreenXMax() / 2, DrawParts->GetScreenYMax() / 2, 1.f + 0.005f * PostPassParts->GetAberrationPower(), 0.f, true);
				BufScreen[1].DrawRotaGraph(DrawParts->GetScreenXMax() / 2, DrawParts->GetScreenYMax() / 2, 1.f, 0.f, true);
				BufScreen[2].DrawRotaGraph(DrawParts->GetScreenXMax() / 2, DrawParts->GetScreenYMax() / 2, 1.f - 0.005f * PostPassParts->GetAberrationPower(), 0.f, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		}
	};
	// �ߋ��t���[���̉摜���d�˂ċO�Ղ��c��悤�Ȍ��ʂ�ǉ�����
	class PostPassMotionBlur : public PostPassBase {
	private:
		class BlurScreen {
			static const int MAX = 3;
			std::array<GraphHandle, MAX> m_screen;
			int m_current{ 0 };
			int m_alpha{ 0 };
			int m_screenWidth{ 0 }, m_screenHeight{ 0 };
			int m_offsetX1{ 0 }, m_offsetX2{ 0 }, m_offsetY1{ 0 }, offsetY2{ 0 };
			int m_notBlendDraw{ 0 };
		public:
			BlurScreen(void) noexcept {}
			BlurScreen(const BlurScreen&) = delete;
			BlurScreen(BlurScreen&& o) = delete;
			BlurScreen& operator=(const BlurScreen&) = delete;
			BlurScreen& operator=(BlurScreen&& o) = delete;

			~BlurScreen(void) noexcept {}
		public:
			void Init(int t_alpha, int t_offsetX1, int t_offsetY1, int t_offsetX2, int t_offsetY2) noexcept {
				auto* DrawParts = DXDraw::Instance();
				for (int i = 0; i < MAX; ++i) {
					m_screen.at(static_cast<size_t>(i)).Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax());
				}
				m_current = 0;
				m_alpha = t_alpha;
				m_offsetX1 = t_offsetX1;
				m_offsetX2 = t_offsetX2;
				m_offsetY1 = t_offsetY1;
				offsetY2 = t_offsetY2;

				m_notBlendDraw = 0;
			}
			void Release(void) noexcept {
				for (int i = 0; i < MAX; ++i) {
					m_screen.at(static_cast<size_t>(i)).Dispose();
				}
			}
		public:
			auto* PostRenderBlurScreen(std::function<void()> doing) noexcept {
				auto* DrawParts = DXDraw::Instance();
				auto next = (m_current != 0) ? (m_current - 1) : MAX - 1;
				m_screen[static_cast<size_t>(m_current)].SetDraw_Screen();
				{
					doing();
					if (++m_notBlendDraw > MAX) {
						int drawMode = GetDrawMode();
						SetDrawMode(DX_DRAWMODE_BILINEAR);
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
						m_screen[static_cast<size_t>(next)].DrawExtendGraph(m_offsetX1, m_offsetY1, DrawParts->GetScreenXMax() + m_offsetX2, DrawParts->GetScreenYMax() + offsetY2, false);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
						SetDrawMode(drawMode);
					}
				}
				auto Cur = m_current;
				m_current = next;
				return &m_screen[static_cast<size_t>(Cur)];
			}
		};
	private:
		BlurScreen				m_BlurScreen;
	public:
		PostPassMotionBlur(void) noexcept {}
		PostPassMotionBlur(const PostPassMotionBlur&) = delete;
		PostPassMotionBlur(PostPassMotionBlur&& o) = delete;
		PostPassMotionBlur& operator=(const PostPassMotionBlur&) = delete;
		PostPassMotionBlur& operator=(PostPassMotionBlur&& o) = delete;

		virtual ~PostPassMotionBlur(void) noexcept {}
	public:
		void Load_Sub(void) noexcept override {
			m_BlurScreen.Init(96, -6, -6, 6, 6);
		}
		void Dispose_Sub(void) noexcept override {
			m_BlurScreen.Release();
		}
		bool IsActive_Sub(void) noexcept override {
			auto* OptionParts = OPTION::Instance();
			return OptionParts->GetParamBoolean(EnumSaveParam::MotionBlur);
		}
	public:
		void SetEffect_Sub(GraphHandle* TargetGraph, GraphHandle*) noexcept override {
			GraphHandle* buf = m_BlurScreen.PostRenderBlurScreen([&]() {
				TargetGraph->DrawGraph(0, 0, false);
				});
			TargetGraph->SetDraw_Screen(false);
			{
				buf->DrawGraph(0, 0, false);
			}
		}
	};
	// ��ʒ[�ɍs���قǂڂ₯��悤�Ȍ��ʂ�ǉ�����
	class PostPassCornerBlur : public PostPassBase {
	private:
		static const int EXTEND = 4;
	private:
		GraphHandle AberrationScreen;	// �`��X�N���[��
		GraphHandle bkScreen2;
		GraphHandle	BufScreen;
	public:
		PostPassCornerBlur(void) noexcept {}
		PostPassCornerBlur(const PostPassCornerBlur&) = delete;
		PostPassCornerBlur(PostPassCornerBlur&& o) = delete;
		PostPassCornerBlur& operator=(const PostPassCornerBlur&) = delete;
		PostPassCornerBlur& operator=(PostPassCornerBlur&& o) = delete;

		virtual ~PostPassCornerBlur(void) noexcept {}
	public:
		void Load_Sub(void) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			AberrationScreen.Make(DrawParts->GetScreenXMax() / EXTEND, DrawParts->GetScreenYMax() / EXTEND, true);
			{
				bkScreen2.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), false);
				bkScreen2.SetDraw_Screen(false);
				{
					bkScreen2.FillGraph(0, 0, 0);
					int xr = DrawParts->GetScreenXMax() * 60 / 100;
					int yr = DrawParts->GetScreenYMax() * 70 / 100;

					DrawOval(DrawParts->GetScreenXMax() / 2, DrawParts->GetScreenYMax() / 2, xr, yr, White, TRUE);

					int r = 0, c = 0, p = 2;

					p = 1;
					for (r = 0; r < 255; r += p) {
						c = 255 - int(std::powf(float(255 - r) / 255.f, 1.5f) * 255.f);

						DrawOval(DrawParts->GetScreenXMax() / 2, DrawParts->GetScreenYMax() / 2, xr - r / p, yr - r / p, GetColor(c, c, c), FALSE, 2);
					}
				}
			}
			BufScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
		}
		void Dispose_Sub(void) noexcept override {
			AberrationScreen.Dispose();
			bkScreen2.Dispose();
			BufScreen.Dispose();
		}
		bool IsActive_Sub(void) noexcept override {
			auto* OptionParts = OPTION::Instance();
			return OptionParts->GetParamBoolean(EnumSaveParam::ScreenEffect);
		}
		void SetEffect_Sub(GraphHandle* TargetGraph, GraphHandle*) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			AberrationScreen.GraphFilterBlt(*TargetGraph, DX_GRAPH_FILTER_DOWN_SCALE, EXTEND);
			AberrationScreen.GraphFilter(DX_GRAPH_FILTER_GAUSS, 16, 1000);
			BufScreen.GraphBlendBlt(*TargetGraph, bkScreen2, 255, DX_GRAPH_BLEND_RGBA_SELECT_MIX,
				DX_RGBA_SELECT_SRC_R, DX_RGBA_SELECT_SRC_G, DX_RGBA_SELECT_SRC_B, DX_RGBA_SELECT_BLEND_R);
			TargetGraph->SetDraw_Screen(false);
			{
				AberrationScreen.DrawExtendGraph(0, 0, DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), false);
				BufScreen.DrawGraph(0, 0, true);
			}
		}
	};
	// �r�l�b�g��ǉ�����
	class PostPassVignette : public PostPassBase {
	private:
		GraphHandle bkScreen;
		GraphHandle	BufScreen;
	public:
		PostPassVignette(void) noexcept {}
		PostPassVignette(const PostPassVignette&) = delete;
		PostPassVignette(PostPassVignette&& o) = delete;
		PostPassVignette& operator=(const PostPassVignette&) = delete;
		PostPassVignette& operator=(PostPassVignette&& o) = delete;

		virtual ~PostPassVignette(void) noexcept {}
	public:
		void Load_Sub(void) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			{
				bkScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), false);
				bkScreen.SetDraw_Screen(true);
				bkScreen.FillGraph(255, 255, 255);
				{
					int y = 0, c = 0, p = 2;
					p = 1;
					for (y = 0; y < 255; y += p) {
						c = 255 - int(std::powf(float(255 - y) / 255.f, 1.5f) * 64.f);
						DxLib::DrawLine(0, y / p, DrawParts->GetScreenXMax(), y / p, GetColor(c, c, c));
					}
					p = 2;
					for (y = 0; y < 255; y += p) {
						c = 255 - int(std::powf(float(255 - y) / 255.f, 1.5f) * 128.f);
						DxLib::DrawLine(0, DrawParts->GetScreenYMax() - y / p, DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax() - y / p, GetColor(c, c, c));
					}
				}
			}
			BufScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
		}
		void Dispose_Sub(void) noexcept override {
			bkScreen.Dispose();
			BufScreen.Dispose();
		}
		bool IsActive_Sub(void) noexcept override {
			auto* OptionParts = OPTION::Instance();
			return OptionParts->GetParamBoolean(EnumSaveParam::ScreenEffect);
		}
		void SetEffect_Sub(GraphHandle* TargetGraph, GraphHandle*) noexcept override {
			BufScreen.GraphBlendBlt(*TargetGraph, bkScreen, 255, DX_GRAPH_BLEND_RGBA_SELECT_MIX,
				DX_RGBA_SELECT_SRC_R, DX_RGBA_SELECT_SRC_G, DX_RGBA_SELECT_SRC_B, DX_RGBA_SELECT_BLEND_R);
			TargetGraph->SetDraw_Screen(false);
			{
				TargetGraph->FillGraph(0, 0, 0);
				BufScreen.DrawGraph(0, 0, true);
			}
		}
	};
	// �ʃ����Y��`�����悤�Șc�݂�ǉ�����
	class PostPassDistortion : public PostPassBase {
	private:
		GraphHandle	BufScreen;
	public:
		PostPassDistortion(void) noexcept {}
		PostPassDistortion(const PostPassDistortion&) = delete;
		PostPassDistortion(PostPassDistortion&& o) = delete;
		PostPassDistortion& operator=(const PostPassDistortion&) = delete;
		PostPassDistortion& operator=(PostPassDistortion&& o) = delete;

		virtual ~PostPassDistortion(void) noexcept {}
	private:
		// ��ʂ�c�܂��Ȃ���`�悷��֐�
		void DrawCircleScreen(
			int CenterX,			// �~�̒��S���WX
			int CenterY,			// �~�̒��S���WY
			float Radius,			// �~�̃T�C�Y
			float Absorption,		// �����̉~�Ɉ������܂��h�b�g��
			const GraphHandle& ScreenHandle// ��ʃO���t�B�b�N�n���h��
		) {
			auto* DrawParts = DXDraw::Instance();
			const int CIRCLE_ANGLE_VERTEX_NUM = 16;			// �~���̒��_��
			const int CIRCLE_RADIUS_VERTEX_NUM = 8;			// ���a�̒��_��

			float CenterDistance;
			float GraphCenterDistance;
			float AbsorptionDistance;
			float AbsorptionMoveX, AbsorptionMoveY;
			float Angle;
			float Sin, Cos;
			COLOR_U8 DiffuseColor;
			int i;
			VERTEX2D* Vert;
			WORD* Ind;
			std::array<float, CIRCLE_ANGLE_VERTEX_NUM> AngleCosTable{};
			std::array<float, CIRCLE_ANGLE_VERTEX_NUM> AngleSinTable{};
			std::array<float, CIRCLE_RADIUS_VERTEX_NUM> InCircleCosTable{};

			// �X�^�b�N�ɐςނɂ͑傫���̂� static �z��ɂ��܂���
			static VERTEX2D Vertex[CIRCLE_RADIUS_VERTEX_NUM * CIRCLE_ANGLE_VERTEX_NUM];
			static WORD Index[CIRCLE_ANGLE_VERTEX_NUM * 6 * (CIRCLE_RADIUS_VERTEX_NUM - 1)];

			// �ŏ��ɕ��ʂɕ`��
			ScreenHandle.DrawGraph(0, 0, false);

			// �`��J���[���쐬���Ă���
			DiffuseColor = GetColorU8(255, 255, 255, 255);

			// �O�������p�� Sin, Cos �e�[�u�����쐬����
			Angle = 0.0f;
			for (i = 0; i < CIRCLE_ANGLE_VERTEX_NUM; ++i, Angle += DX_PI_F * 2.0f / CIRCLE_ANGLE_VERTEX_NUM) {
				AngleSinTable.at(static_cast<size_t>(i)) = std::sin(Angle);
				AngleCosTable.at(static_cast<size_t>(i)) = std::cos(Angle);
			}

			// �����̐���オ���Ă���悤�Ɍ�����ӏ��Ŏg�p���� Cos �e�[�u�����쐬����
			Angle = 0.0f;
			for (i = 0; i < CIRCLE_RADIUS_VERTEX_NUM; ++i, Angle += (DX_PI_F / 2.0f) / (CIRCLE_RADIUS_VERTEX_NUM - 1)) {
				InCircleCosTable.at(static_cast<size_t>(i)) = std::cos(Angle);
			}

			// �|���S�����_�C���f�b�N�X�̏���
			Ind = Index;
			for (i = 0; i < CIRCLE_ANGLE_VERTEX_NUM; ++i) {
				for (WORD j = 0; j < CIRCLE_RADIUS_VERTEX_NUM - 1; ++j, Ind += 6) {
					Ind[0] = static_cast<WORD>(i * CIRCLE_RADIUS_VERTEX_NUM + j);
					Ind[1] = static_cast<WORD>(Ind[0] + 1);
					if (i == CIRCLE_ANGLE_VERTEX_NUM - 1) {
						Ind[2] = j;
						Ind[3] = static_cast<WORD>(j + 1);
					}
					else {
						Ind[2] = static_cast<WORD>(Ind[0] + CIRCLE_RADIUS_VERTEX_NUM);
						Ind[3] = static_cast<WORD>(Ind[0] + 1 + CIRCLE_RADIUS_VERTEX_NUM);
					}
					Ind[4] = Ind[2];
					Ind[5] = Ind[1];
				}
			}

			// �o�C���j�A��ԕ`��ɂ���
			SetDrawMode(DX_DRAWMODE_BILINEAR);


			// �O���̃h�[�i�c������`��

			// ���S�Ɍ������ɂ��������Ē��S�����Ƀe�N�X�`�����W�����炷
			Vert = Vertex;
			for (i = 0; i < CIRCLE_ANGLE_VERTEX_NUM; ++i) {
				// �g�p���� Sin, Cos �̒l���Z�b�g
				Sin = AngleSinTable.at(static_cast<size_t>(i));
				Cos = AngleCosTable.at(static_cast<size_t>(i));

				for (int j = 0; j < CIRCLE_RADIUS_VERTEX_NUM; ++j, ++Vert) {
					// �~�̒��S�܂ł̋������Z�o
					CenterDistance = Radius;

					// ���S�Ɉ������܂�鋗�����Z�o
					AbsorptionDistance = Absorption * static_cast<float>(j) / static_cast<float>(CIRCLE_RADIUS_VERTEX_NUM - 1);

					// ���S�Ɍ������Ĉړ����鋗�����Z�o
					AbsorptionMoveX = Cos * AbsorptionDistance;
					AbsorptionMoveY = Sin * AbsorptionDistance;

					// �X�N���[�����W�̌���
					Vert->pos.x = Cos * CenterDistance + static_cast<float>(CenterX);
					Vert->pos.y = Sin * CenterDistance + static_cast<float>(CenterY);
					Vert->pos.z = 0.0f;

					// �e�N�X�`�����W�̃Z�b�g
					Vert->u = (Vert->pos.x + AbsorptionMoveX) / static_cast<float>(DrawParts->GetScreenXMax());
					Vert->v = (Vert->pos.y + AbsorptionMoveY) / static_cast<float>(DrawParts->GetScreenYMax());

					// ���̑��̃p�����[�^���Z�b�g
					Vert->rhw = 1.0f;
					Vert->dif = DiffuseColor;
				}
			}

			// �c�񂾃h�[�i�c�̕`��
			DrawPrimitiveIndexed2D(Vertex, sizeof(Vertex) / sizeof(VERTEX2D), Index, sizeof(Index) / sizeof(WORD), DX_PRIMTYPE_TRIANGLELIST, ScreenHandle.get(), FALSE);


			// �����̐���オ���Ă���悤�Ɍ����镔����`��

			// Cos�e�[�u���ɂ��������ăe�N�X�`�����W�����炷
			Vert = Vertex;
			for (i = 0; i < CIRCLE_ANGLE_VERTEX_NUM; ++i) {
				// �g�p���� Sin, Cos �̒l���Z�b�g
				Sin = AngleSinTable.at(static_cast<size_t>(i));
				Cos = AngleCosTable.at(static_cast<size_t>(i));

				for (int j = 0; j < CIRCLE_RADIUS_VERTEX_NUM; ++j, ++Vert) {
					// �~�̒��S�܂ł̋������Z�o
					CenterDistance = InCircleCosTable[static_cast<size_t>(j)] * Radius;

					// �摜���W���_�ł̉~�̒��S�܂ł̋������Z�o
					GraphCenterDistance = static_cast<float>((CIRCLE_RADIUS_VERTEX_NUM - 1) - j) * (Absorption + Radius) / static_cast<float>(CIRCLE_RADIUS_VERTEX_NUM - 1);

					// �X�N���[�����W�̌���
					Vert->pos.x = Cos * CenterDistance + static_cast<float>(CenterX);
					Vert->pos.y = Sin * CenterDistance + static_cast<float>(CenterY);
					Vert->pos.z = 0.0f;

					// �e�N�X�`�����W�̃Z�b�g
					Vert->u = (Cos * GraphCenterDistance + static_cast<float>(CenterX)) / static_cast<float>(DrawParts->GetScreenXMax());
					Vert->v = (Sin * GraphCenterDistance + static_cast<float>(CenterY)) / static_cast<float>(DrawParts->GetScreenYMax());

					// ���̑��̃p�����[�^���Z�b�g
					Vert->rhw = 1.0f;
					Vert->dif = DiffuseColor;
				}
			}

			// ���S�̐���オ���Č����镔����`��
			DrawPrimitiveIndexed2D(Vertex, sizeof(Vertex) / sizeof(VERTEX2D), Index, sizeof(Index) / sizeof(WORD), DX_PRIMTYPE_TRIANGLELIST, ScreenHandle.get(), FALSE);
		}
	public:
		void Load_Sub(void) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			BufScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
		}
		void Dispose_Sub(void) noexcept override {
			BufScreen.Dispose();
		}
		bool IsActive_Sub(void) noexcept override {
			auto* OptionParts = OPTION::Instance();
			return OptionParts->GetParamBoolean(EnumSaveParam::ScreenEffect) && OptionParts->GetParamBoolean(EnumProjectSettingParam::Distortion);
		}
		void SetEffect_Sub(GraphHandle* TargetGraph, GraphHandle*) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			BufScreen.SetDraw_Screen();
			{
				TargetGraph->DrawGraph(0, 0, true);
			}
			// TargetGraph->GraphFilterBlt(BufScreen, DX_GRAPH_FILTER_DOWN_SCALE, 1);
			TargetGraph->SetDraw_Screen();
			{
				// ��ʂ�c�܂��ĕ`��
				DrawCircleScreen(
					DrawParts->GetScreenXMax() / 2, DrawParts->GetScreenYMax() / 2,
					static_cast<float>(DrawParts->GetScreenXMax() * 2 / 3), 120.0f, BufScreen);
			}
		}
	};
	// --------------------------------------------------------------------------------------------------
	// ��L�|�X�g�v���Z�X�����܂Ƃ߂�N���X
	// --------------------------------------------------------------------------------------------------
	// �R���X�g���N�^
	PostPassEffect::PostPassEffect(void) noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto Prev = GetCreateDrawValidGraphZBufferBitDepth();
		SetCreateDrawValidGraphZBufferBitDepth(24);
		BufferScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), true);
		ColorScreen.Make(DrawParts->GetScreenXMax(), DrawParts->GetScreenYMax(), false);
		SetCreateDrawValidGraphZBufferBitDepth(Prev);
		// �e�|�X�g�v���Z�X���������X�g�ɒǉ�(�ǉ����ɏ�������Ă����܂�)
		m_PostPass.emplace_back(std::make_unique<PostPassBloom>());
		m_PostPass.emplace_back(std::make_unique<PostPassDistortion>());
		m_PostPass.emplace_back(std::make_unique<PostPassAberration>());
		m_PostPass.emplace_back(std::make_unique<PostPassMotionBlur>());
		m_PostPass.emplace_back(std::make_unique<PostPassVignette>());
		m_PostPass.emplace_back(std::make_unique<PostPassCornerBlur>());
		for (auto& P : m_PostPass) {
			P->Init();
		}
	}
	// �|�X�g�v���Z�X�����̗L���������X�V
	void PostPassEffect::Update(void) noexcept {
		for (auto& P : m_PostPass) {
			P->UpdateActive();
		}
	}
	// �|�X�g�v���Z�X������BufferScreen�ɑ΂��čs��
	void PostPassEffect::DrawPostProcess(void) noexcept {
		// ���Z�b�g�ւ��
		ColorScreen.SetDraw_Screen();
		// �Ώۂ��ꉞ�߂��Ă���
		BufferScreen.SetDraw_Screen(false);
		// �F���␳
		BufferScreen.GraphFilter(DX_GRAPH_FILTER_LEVEL, InColorPerMin, InColorPerMax, int(InColorGamma * 100), 0, 255);
		// �e�|�X�g�v���Z�X��o�^���Ɏ��{
		for (auto& P : m_PostPass) {
			ColorScreen.GraphFilterBlt(BufferScreen, DX_GRAPH_FILTER_DOWN_SCALE, 1);
			P->SetEffect(&BufferScreen, &ColorScreen);
		}
	}
	void PostPassEffect::Dispose(void) noexcept {
		for (auto& P : m_PostPass) {
			P.reset();
		}
		m_PostPass.clear();
	}
};
