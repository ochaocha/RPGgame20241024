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

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include DXLIB																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// DXLIB
#include "DxLib.h"
// �ǉ�
#include "DXLib_ref_00_Util.hpp"
#include "DXLib_ref_01_Algorithm.hpp"
#include "DXLib_ref_02_Sound.hpp"
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
		float						m_DeltaTime{ 0.f };		//1�t���[���ɂ����������� �}�C�N���b
		LONGLONG					m_StartTime{ 0 };		// 1�t���[�����n�܂��Ă���̌o�ߎ��� �}�C�N���b
	private:
		// �R���X�g���N�^
		DXLib_ref(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		DXLib_ref(const DXLib_ref&) = delete;
		DXLib_ref(DXLib_ref&& o) = delete;
		DXLib_ref& operator=(const DXLib_ref&) = delete;
		DXLib_ref& operator=(DXLib_ref&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		// ���C�����[�v��V�[���J�ڊǗ����s��
		bool		MainLogic(void) noexcept;
	public:
		const auto& GetDeltaTime(void) const noexcept { return m_DeltaTime; }		// 1�t���[���ɂ����������Ԃ��擾
		const auto	GetFps(void) const noexcept { return 1.f / m_DeltaTime; }		// FPS�l�̎擾
	public:
		//���������̃t���O�ݒ�
		void		SetWaitVSync(void) noexcept;
		// ���[�v�̍ŏ��ɒʂ�
		void		StartCount(void) noexcept;
		// �\��ʂɔ��f���A���������܂��͈���FPS�܂őҋ@����
		bool		WaitCount(void) const noexcept;
	};
}
