#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// DXLIB�̊J�n�A�I���A�E�B���h�E�T�C�Y����A��������������s���܂�
	// --------------------------------------------------------------------------------------------------
	class WindowSizeControl : public SingletonBase<WindowSizeControl> {
	private:
		friend class SingletonBase<WindowSizeControl>;
	private:
		// 
		int							m_DispXSize{ BaseScreenWidth };
		int							m_DispYSize{ BaseScreenHeight };
		// 
		int							m_DispXSize_Win{ BaseScreenWidth };
		int							m_DispYSize_Win{ BaseScreenHeight };
		// 
		int							m_DispXSize_Border{ BaseScreenWidth };
		int							m_DispYSize_Border{ BaseScreenHeight };
	private:
		// �R���X�g���N�^
		WindowSizeControl(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		WindowSizeControl(const WindowSizeControl&) = delete;
		WindowSizeControl(WindowSizeControl&& o) = delete;
		WindowSizeControl& operator=(const WindowSizeControl&) = delete;
		WindowSizeControl& operator=(WindowSizeControl&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:// �Q�b�^�[
		// �傫���ق��̃X�N���[���T�C�Y�𓾂�(�X�N���[���o�b�t�@�p)
		const auto	GetSizeXMax(void) const noexcept { return GetMax(this->m_DispXSize_Border, m_DispXSize_Win); }
		const auto	GetSizeYMax(void) const noexcept { return GetMax(this->m_DispYSize_Border, m_DispYSize_Win); }
		// UI�ȊO�̃X�N���[�����
		const auto	GetScreenX(int value) const noexcept { return value * GetSizeXMax() / this->m_DispXSize_Win; }
		const auto	GetScreenY(int value) const noexcept { return value * GetSizeYMax() / this->m_DispYSize_Win; }
		// UI�p
		const auto	GetUIX(int value) const noexcept { return value * this->m_DispXSize / BaseScreenWidth; }
		const auto	GetUIY(int value) const noexcept { return value * this->m_DispYSize / BaseScreenHeight; }
	public:
		// �ݒ�ɉ����ăE�B���h�E�T�C�Y�Ɛݒ��؂�ւ�
		void		SetWindowOrBorderless(void) noexcept;
	};
};
