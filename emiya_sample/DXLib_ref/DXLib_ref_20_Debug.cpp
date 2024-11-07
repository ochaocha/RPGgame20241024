#include "DXLib_ref_20_Debug.hpp"

namespace DXLibRef {
#if DEBUG
	// シングルトンの実態定義
	const DebugClass* SingletonBase<DebugClass>::m_Singleton = nullptr;
	// コンストラクタ
	DebugClass::DebugClass(void) noexcept {
		// ポイントを保持しておく部分を確保します
		m_Point.resize(static_cast<size_t>(PointFrame + 1));
		m_IsActive = true;
	}
	// デバッグの計測開始ポイントに置くもの
	void DebugClass::SetStartPoint(void) noexcept {
		if (!m_IsActive) {
			return;
		}
		m_StartTime = GetNowHiPerformanceCount();
		m_PointSel = 0;
		SetPoint("-----Start-----");
	}
	// 任意のポイントに置くと、前に書いたSetPointとの時間差を測れるようになります
	void DebugClass::SetPoint(const char* DebugMes) noexcept {
		if (!m_IsActive) {
			return;
		}
		if (m_PointSel < PointMax) {
			m_Point[0][m_PointSel] = static_cast<float>(GetNowHiPerformanceCount() - m_StartTime) / 1000.0f;
			m_Str[m_PointSel] = DebugMes;
			m_PointSel++;
			return;
		}
	}
	// 計測を終了させるポイントです
	void DebugClass::SetEndPoint(void) noexcept {
		m_Switch.Update(CheckHitKey(KEY_INPUT_F1) != 0);
		if (m_Switch.trigger()) {
			m_IsActive ^= 1;
		}
		if (!m_IsActive) {
			return;
		}
		auto PMax = PointMax + 1;
		// 最後の演算
		SetPoint("-----End-----");
		for (int index = 0; index < PMax; ++index) {
			if (!(static_cast<int>(m_PointSel) > index)) {
				m_Point[0][static_cast<std::size_t>(index)] = m_Point[0][m_PointSel - 1];
			}
			if (index == PointMax) {
				m_Point[0][static_cast<std::size_t>(index)] = 1000.0f * DXLib_ref::Instance()->GetDeltaTime();
			}

			for (int j = static_cast<int>(PointFrame - 1); j >= 1; --j) {
				m_Point[static_cast<std::size_t>(j)][static_cast<std::size_t>(index)] = m_Point[static_cast<std::size_t>(j - 1)][static_cast<std::size_t>(index)];
			}

			// 平均
			m_Point[static_cast<std::size_t>(PointFrame)][static_cast<std::size_t>(index)] = 0.f;
			for (int j = 0; j < PointFrame; ++j) {
				m_Point[static_cast<std::size_t>(PointFrame)][static_cast<std::size_t>(index)] += m_Point[static_cast<std::size_t>(j)][static_cast<std::size_t>(index)];
			}
			m_Point[static_cast<std::size_t>(PointFrame)][static_cast<std::size_t>(index)] /= PointFrame;
		}
		for (int index = PMax - 1; index >= 1; --index) {
			if (index > 0) {
				m_Point[static_cast<std::size_t>(PointFrame)][static_cast<std::size_t>(index)] = m_Point[static_cast<std::size_t>(PointFrame)][static_cast<std::size_t>(index)] - m_Point[static_cast<std::size_t>(PointFrame)][static_cast<std::size_t>(index) - 1];
			}
		}
	}

	// 結果を表示します
	void DebugClass::DebugWindow(int xpos, int ypos) noexcept {
		auto* OptionParts = OPTION::Instance();
		auto* DrawCtrls = UISystem::DrawControl::Instance();
		if (!m_IsActive) {
			return;
		}
		// 各ポイントに割り振る色を配列として置いておきます(重くない量なのでローカル変数にしています)
		const unsigned int Colors[PointMax + 1] = {
				Red,
				Yellow,
				Green,
				GetColor(0,255,255),
				GetColor(100,100,255),
				GetColor(255, 0,255),
				Red,
				Yellow,
				Green,
				GetColor(0,255,255),
				GetColor(100,100,255),
				GetColor(255, 0,255),
				Red,
		};
		// 処理時間をグラフとして描画
		{
			auto PMax = PointMax + 1;
			const int wide = 340;
			const int height = 360;
			const int border = height * 2 / 3;
			// 背景
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, xpos, ypos, xpos + wide, ypos + height, White, true);
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, xpos + 1, ypos + 1, xpos + wide - 1, ypos + height - 1, Black, true);

			{
				const int xp = xpos;
				const int yp = ypos;
				// 内容
				int value = OptionParts->GetParamInt(EnumSaveParam::FpsLimit);
				const float xs = static_cast<float>(wide) / PointFrame;
				const float ys = static_cast<float>(border) / (1000.0f / static_cast<float>(value));

				const int ye = yp + height;
				for (size_t j = static_cast<size_t>(PointFrame - 1 - 1); j > 0; --j) {
					int xnow = xp + static_cast<int>(static_cast<float>(j) * xs);
					int xnext = xp + static_cast<int>(static_cast<float>(j + 1) * xs);

					for (size_t index = static_cast<size_t>(PMax - 1); index > 0; --index) {
						int ynow = GetMax(yp, ye - static_cast<int>(m_Point[j][index] * ys));
						int ynext = GetMax(yp, ye - static_cast<int>(m_Point[j + 1][index] * ys));
						DrawCtrls->SetDrawQuadrangle(
							UISystem::DrawLayer::Normal,
							xnow, ynow, xnext, ynext,
							xnext, ye, xnow, ye,
							Colors[index],
							TRUE);
					}
				}
				DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, xp, ye - border, xp + wide, ye - border, White);// 基準線
			}
			ypos += height;
		}
		// FPSや各ポイントでの時間を表示
		{
			const int wide = 350;
			const int height = static_cast<int>(m_PointSel + 3 + 1) * LineHeight + 10;
			// 背景
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, xpos, ypos, xpos + wide, ypos + height, White, true);
			DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, xpos + 1, ypos + 1, xpos + wide - 1, ypos + height - 1, Black, true);

			xpos += 2;
			ypos += 2;
			int i = 0;
			// 内容
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
				xpos, ypos + (i * LineHeight), White, Black, "AsyncCount :%d", GetASyncLoadNum());
			++i;
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
				xpos, ypos + (i * LineHeight), White, Black, "Drawcall  :%d", GetDrawCallCount());
			++i;
			DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
				LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
				xpos, ypos + (i * LineHeight), White, Black, "FPS    :%5.2f fps", GetFPS());
			++i;
			for (size_t index = 1; index < static_cast<size_t>(m_PointSel + 1); ++index) {
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
					xpos, ypos + (i * LineHeight), Colors[index], DarkGreen, "%02d(%5.2fms)[%s]", index, m_Point[static_cast<size_t>(PointFrame)][index], m_Str[index - 1].c_str());
				++i;
			}
			{
				size_t index = static_cast<size_t>(PointMax);
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic,
					LineHeight, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
					xpos, ypos + (i * LineHeight), Colors[index], DarkGreen, "%02d(%5.2fms)[%s]", index, m_Point[static_cast<size_t>(PointFrame)][index], m_Str[index - 1].c_str());
				++i;
			}
		}
	}
#endif // DEBUG
};
