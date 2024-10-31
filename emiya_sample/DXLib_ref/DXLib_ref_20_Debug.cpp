#include "DXLib_ref_20_Debug.hpp"

namespace DXLibRef {
#if DEBUG
	// �V���O���g���̎��Ԓ�`
	const DebugClass* SingletonBase<DebugClass>::m_Singleton = nullptr;
	// �R���X�g���N�^
	DebugClass::DebugClass(void) noexcept {
		// �|�C���g��ێ����Ă����������m�ۂ��܂�
		m_Point.resize(static_cast<size_t>(PointFrame + 1));
		m_Switch.Set(true);
	}
	// �f�o�b�O�̌v���J�n�|�C���g�ɒu������
	void DebugClass::SetStartPoint(void) noexcept {
		if (!m_Switch.on()) {
			return;
		}
		m_StartTime = GetNowHiPerformanceCount();
		m_PointSel = 0;
		SetPoint("-----Start-----");
	}
	// �C�ӂ̃|�C���g�ɒu���ƁA�O�ɏ�����SetPoint�Ƃ̎��ԍ��𑪂��悤�ɂȂ�܂�
	void DebugClass::SetPoint(const char* DebugMes) noexcept {
		if (!m_Switch.on()) {
			return;
		}
		if (m_PointSel < PointMax) {
			m_Point[0][m_PointSel] = static_cast<float>(GetNowHiPerformanceCount() - m_StartTime) / 1000.0f;
			m_Str[m_PointSel] = DebugMes;
			m_PointSel++;
			return;
		}
	}
	// �v�����I��������|�C���g�ł�
	void DebugClass::SetEndPoint(void) noexcept {
		m_Switch.Execute(CheckHitKeyWithCheck(KEY_INPUT_F1) != 0);
		if (!m_Switch.on()) {
			return;
		}
		auto PMax = PointMax + 1;
		// �Ō�̉��Z
		SetPoint("-----End-----");
		for (int index = 0; index < PMax; ++index) {
			if (!(static_cast<int>(m_PointSel) > index)) {
				m_Point[0][static_cast<std::size_t>(index)] = m_Point[0][m_PointSel - 1];
			}
			if (index == PointMax) {
				auto* DrawParts = DXDraw::Instance();
				m_Point[0][static_cast<std::size_t>(index)] = 1000.0f * DrawParts->GetDeltaTime();
			}

			for (int j = static_cast<int>(PointFrame - 1); j >= 1; --j) {
				m_Point[static_cast<std::size_t>(j)][static_cast<std::size_t>(index)] = m_Point[static_cast<std::size_t>(j - 1)][static_cast<std::size_t>(index)];
			}

			// ����
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

	// ���ʂ�\�����܂�
	void DebugClass::DebugWindow(int xpos, int ypos) noexcept {
		auto* DrawParts = DXDraw::Instance();
		auto* OptionParts = OPTION::Instance();
		auto* DrawCtrls = WindowSystem::DrawControl::Instance();
		if (!m_Switch.on()) {
			return;
		}
		// �e�|�C���g�Ɋ���U��F��z��Ƃ��Ēu���Ă����܂�(�d���Ȃ��ʂȂ̂Ń��[�J���ϐ��ɂ��Ă��܂�)
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
		// �������Ԃ��O���t�Ƃ��ĕ`��
		{
			auto PMax = PointMax + 1;
			const int wide = DrawParts->GetUIY(340);
			const int height = DrawParts->GetUIY(360);
			const int border = height * 2 / 3;
			// �w�i
			WindowSystem::SetBox(xpos, ypos, xpos + wide, ypos + height, White);
			WindowSystem::SetBox(xpos + 1, ypos + 1, xpos + wide - 1, ypos + height - 1, Black);

			{
				const int xp = xpos;
				const int yp = ypos;
				// ���e
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
							WindowSystem::DrawLayer::Normal,
							xnow, ynow, xnext, ynext,
							xnext, ye, xnow, ye,
							Colors[index],
							TRUE);
					}
				}
				DrawCtrls->SetDrawLine(WindowSystem::DrawLayer::Normal, xp, ye - border, xp + wide, ye - border, White);// ���
			}
			ypos += height;
		}
		// FPS��e�|�C���g�ł̎��Ԃ�\��
		{
			const int wide = DrawParts->GetUIY(350);
			const int height = static_cast<int>(m_PointSel + 3 + 1) * LineHeight + DrawParts->GetUIY(10);
			// �w�i
			WindowSystem::SetBox(xpos, ypos, xpos + wide, ypos + height, White);
			WindowSystem::SetBox(xpos + 1, ypos + 1, xpos + wide - 1, ypos + height - 1, Black);

			xpos += DrawParts->GetUIY(2);
			ypos += DrawParts->GetUIY(2);
			int i = 0;
			// ���e
			WindowSystem::SetMsg(xpos, ypos + (i * LineHeight) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, "AsyncCount :%d", GetASyncLoadNum());
			++i;
			WindowSystem::SetMsg(xpos, ypos + (i * LineHeight) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, "Drawcall  :%d", GetDrawCallCount());
			++i;
			WindowSystem::SetMsg(xpos, ypos + (i * LineHeight) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, White, Black, "FPS    :%5.2f fps", GetFPS());
			++i;
			for (size_t index = 1; index < static_cast<int>(m_PointSel + 1); ++index) {
				WindowSystem::SetMsg(xpos, ypos + (i * LineHeight) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, Colors[index], DarkGreen, "%02d(%5.2fms)[%s]", index, m_Point[static_cast<size_t>(PointFrame)][index], m_Str[index - 1].c_str());
				++i;
			}
			{
				size_t index = static_cast<size_t>(PointMax);
				WindowSystem::SetMsg(xpos, ypos + (i * LineHeight) + LineHeight / 2, LineHeight, FontSystem::FontXCenter::LEFT, Colors[index], DarkGreen, "%02d(%5.2fms)[%s]", index, m_Point[static_cast<size_t>(PointFrame)][index], m_Str[index - 1].c_str());
				++i;
			}
		}
	}
#endif // DEBUG
};
