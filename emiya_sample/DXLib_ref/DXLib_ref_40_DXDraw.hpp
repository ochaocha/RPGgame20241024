#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// DXLIB�̊J�n�A�I���A�E�B���h�E�T�C�Y����A��������������s���܂�
	// --------------------------------------------------------------------------------------------------
	class DXDraw : public SingletonBase<DXDraw> {
	private:
		friend class SingletonBase<DXDraw>;
	private:
		// 
		int							m_DispXSize{ deskx };
		int							m_DispYSize{ desky };
		// 
		int							m_DispXSize_Win{ deskx };
		int							m_DispYSize_Win{ desky };
		// 
		int							m_DispXSize_Border{ deskx };
		int							m_DispYSize_Border{ desky };
		// 
		int							m_ScreenXSize{ deskx };
		int							m_ScreenYSize{ desky };

		float						m_DeltaTime{ 0.f };
		// 
		LONGLONG					m_StartTime{ 0 };
	public:// �Q�b�^�[
		// UI�ȊO�̃X�N���[�����
		const auto		GetScreenX(int value) const noexcept { return value * this->m_ScreenXSize / BaseScreenWidth; }
		const auto		GetScreenY(int value) const noexcept { return value * this->m_ScreenYSize / BaseScreenHeight; }
		// UI�ȊO�̃X�N���[�����(1920,1080�̏ꍇ)
		const auto& GetScreenXMax(void) const noexcept { return this->m_ScreenXSize; }
		const auto& GetScreenYMax(void) const noexcept { return this->m_ScreenYSize; }
		// UI�p
		const auto		GetUIX(int value) const noexcept { return value * this->m_DispXSize / BaseScreenWidth; }
		const auto		GetUIY(int value) const noexcept { return value * this->m_DispYSize / BaseScreenHeight; }
		// UI�p(1920,1080�̏ꍇ)
		const auto& GetUIXMax(void) const noexcept { return this->m_DispXSize; }
		const auto& GetUIYMax(void) const noexcept { return this->m_DispYSize; }
		// UI�p(1920,1080�̏ꍇ)
		const auto GetSizeXMax(void) const noexcept { return GetMax(this->m_DispXSize_Border, m_DispXSize_Win); }
		const auto GetSizeYMax(void) const noexcept { return GetMax(this->m_DispYSize_Border, m_DispYSize_Win); }
		// UI�ɑΉ������}�E�X���W���擾
		void			GetMousePosition(int* MouseX, int* MouseY) const noexcept;
		// FPS�l�̎擾
		const auto& GetDeltaTime(void) const noexcept { return m_DeltaTime; }
		const auto		GetFps(void) const noexcept { return 1.f / m_DeltaTime; }
	public:
		// �ݒ�ɉ����ăE�B���h�E�T�C�Y�Ɛݒ��؂�ւ�
		void			SetWindowOrBorderless(void) noexcept;
	private:
		// �E�B���h�E�T�C�Y�𔽉f���镔���𔲂��o�����֐�
		void			UpdateWindowSize(void) const noexcept {
			// DPI�ݒ�
			int DPI = 96;
			GetMonitorDpi(NULL, &DPI);
			SetWindowSize(this->m_DispXSize * DPI / 96, this->m_DispYSize * DPI / 96);
		}
	private:
		// �R���X�g���N�^
		DXDraw(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		DXDraw(const DXDraw&) = delete;
		DXDraw(DXDraw&& o) = delete;
		DXDraw& operator=(const DXDraw&) = delete;
		DXDraw& operator=(DXDraw&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		// ���[�v�̍ŏ��ɒʂ�
		void			StartCount(void) noexcept;
		// �\��ʂɔ��f���A���������܂��͈���FPS�܂őҋ@����
		bool			Screen_Flip(void) const noexcept;
		// 
		void			DXLibEnd(void) noexcept;
	};
};
