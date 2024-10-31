#pragma once

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*�r���h�ݒ�																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#define DEBUG (TRUE)/*�f�o�b�O�\�������邩�ۂ�*/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*pragma																																	*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma warning(disable:4505)	/*�g�p����Ă��Ȃ��֐���j������|�̌x������邽�߂ɒǉ�*/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include																																	*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// ����
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <D3D11.h>
#include <cmath>

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*const																																		*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
constexpr float		FrameRate{ 60.f };								/*�����̊�ɂȂ�t���[�����[�g*/
constexpr float		GravityRate{ -9.8f };							/*�d�͉����x*/
constexpr int		InvalidID{ -1 };								/*���ʂ̖����l*/
constexpr int		BaseScreenWidth{ 1920 };						/*UI�`��Ȃǂ̊�ƂȂ�𑜓x*/
constexpr int		BaseScreenHeight{ 1080 };						/*UI�`��Ȃǂ̊�ƂȂ�𑜓x*/

// DPI�𔽉f����f�X�N�g�b�v�T�C�Y
const int deskx{ static_cast<int>(GetSystemMetrics(SM_CXSCREEN)) };
const int desky{ static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) };

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include DXLIB																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// DXLIB
#include "DxLib.h"
// �ǉ�
#include "DXLib_ref_00_Util.hpp"
#include "DXLib_ref_01_Vector.hpp"
#include "DXLib_ref_02_SoundHandle.hpp"
#include "DXLib_ref_03_GraphHandle.hpp"
#include "DXLib_ref_08_Etc.hpp"
#include "DXLib_ref_09_KeyControl.hpp"
#include "DXLib_ref_10_Option.hpp"
#include "DXLib_ref_11_FileControl.hpp"
#include "DXLib_ref_12_Draw2DSystem.hpp"
#include "DXLib_ref_20_Debug.hpp"
#include "DXLib_ref_40_DXDraw.hpp"
#include "DXLib_ref_100_PostPass.hpp"
#include "DXLib_ref_101_sequence.hpp"
#include "DXLib_ref_102_ObjectManager.hpp"

namespace DXLibRef {
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLibRef�̃��C�����[�v�ȂǑS�ʓI�ȊǗ����s���܂�																							*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class DXLib_ref : public SingletonBase<DXLib_ref> {
	private:
		friend class SingletonBase<DXLib_ref>;
	private:
		// FPS�\���p�N���X
		class FPSDrawer {
			std::array<float, 60>		FPSAvgs{};
			size_t						m_FPSAvgCount{ 0 };
			float						m_FPSAvg{ 0.f };
		public:
			// FPS�\��
			void	InitFPSCounter(void) noexcept;
			void	UpdateFPSCounter(void) noexcept;
			void	DrawFPSCounter(void) const noexcept;
		};
		// �|�[�Y��ʕ\���p�N���X
		class PauseDrawer {
			float						m_PauseFlashCount{ 0.f };
			// �|�[�Y�����ǂ����̃t���O
			bool						m_IsPauseActive{ false };
		public:
			void	UpdatePause(void) noexcept;
			void	DrawPause(void) const noexcept;
		public:
			// �|�[�Y�̗L���������擾
			const auto		IsPause(void) const noexcept { return m_IsPauseActive; }
			void			ChangePause(bool value) noexcept;
		};
	private:
		// FPS�\���p�N���X�̎���
		FPSDrawer					m_FPSDrawer;
		// �|�[�Y��ʕ\���p�N���X�̎���
		PauseDrawer					m_PauseDrawer;
		// ���݂̃V�[�����I�������t���O
		bool						m_IsEndScene{ false };
		// �Q�[���I���t���O
		bool						m_IsEndGame{ false };
		// ����N���t���O
		bool						m_IsFirstBoot{ false };
		// �I���|�b�v�A�b�v���J���Ă��邩�̃t���O
		bool						m_IsExitSelect{ false };
		// �ċN���|�b�v�A�b�v���J���Ă��邩�̃t���O
		bool						m_IsRestartSelect{ false };
	private:
		// ������
		void			InitFirstBootSetting(void) noexcept;
		void			UpdateFirstBootSetting(void) noexcept;
		void			DrawFirstBootSetting(void) const noexcept;
		// �ʏ���
		void			InitMainLoop(void) noexcept;
		void			UpdateMainLoop(void) noexcept;
		void			DrawMainLoop(void) const noexcept;
		void			ExitMainLoop(void) noexcept;
	public:
		// �|�[�Y�̗L���������擾
		const auto		IsPause(void) const noexcept { return m_PauseDrawer.IsPause(); }
		void			ChangePause(bool value) noexcept { m_PauseDrawer.ChangePause(value); }
		// �I���A�ċN���t���O�������Ă��邩�O����m�F�ł��܂�
		const auto&		IsExit(void) const noexcept { return m_IsExitSelect; }
		const auto&		IsRestart(void) const noexcept { return m_IsRestartSelect; }
	private:
		// �R���X�g���N�^
		DXLib_ref(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		DXLib_ref(const DXLib_ref&) = delete;
		DXLib_ref(DXLib_ref&& o) = delete;
		DXLib_ref& operator=(const DXLib_ref&) = delete;
		DXLib_ref& operator=(DXLib_ref&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		// ���W�b�N�J�n���ɒʂ�
		bool			StartLogic(void) noexcept;
		// ���C�����[�v��V�[���J�ڊǗ����s��
		bool			MainLogic(void) noexcept;
	};
};