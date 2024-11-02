#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// 1�V�[���Ԃ�̊��N���X
	// --------------------------------------------------------------------------------------------------
	class TEMPSCENE {
	private:
		bool			m_IsFirstLoop{ true };			// ����`�F�b�N
		bool			m_IsLoading{ false };			// ���Ɏ����̃V�[����Load��ʂ������ǂ���(DisposeLoad��ʂ�܂ł͍�Load�����Ȃ��悤�ɂ���)
		std::array<std::shared_ptr<TEMPSCENE>, 10> Next_ptr{ nullptr };// ���ɐi�ރV�[���̃��X�g
		size_t			m_Next_Select{ 0 };				// ���ɐi�ރV�[���̃��X�g�̂����ǂ�ɐi�ނ�
	public:
		// ���ɐi�ރV�[�������X�g�ɒǉ��ǉ�
		void		SetNextSceneList(int index, const std::shared_ptr<TEMPSCENE>& Next_scenes_ptr_t) noexcept { Next_ptr.at(static_cast<std::size_t>(index)) = Next_scenes_ptr_t; }
		// ���ɐi�ރV�[��������
		auto&		Get_Next(void) noexcept { return Next_ptr.at(this->m_Next_Select); }
		// ���X�g�̂����ǂ�ɐi�ނ�����
		void		SetNextSelect(size_t value) noexcept { this->m_Next_Select = value; }
	public:// �Q�b�^�[
		// Update_Sub�����߂ČĂяo���ꂽ���ǂ���
		const auto& GetIsFirstLoop(void) const noexcept { return m_IsFirstLoop; }
	public:
		// �R���X�g���N�^
		TEMPSCENE(void) noexcept {}
		TEMPSCENE(const TEMPSCENE&) = delete;
		TEMPSCENE(TEMPSCENE&& o) = delete;
		TEMPSCENE& operator=(const TEMPSCENE&) = delete;
		TEMPSCENE& operator=(TEMPSCENE&& o) = delete;
		// �f�X�g���N�^
		virtual ~TEMPSCENE(void) noexcept {}
	public:// ���C���X�V
		void Load(void) noexcept {
			if (!m_IsLoading) {
				m_IsLoading = true;
				Load_Sub();
			}
		}
		void Set(void) noexcept {
			this->m_Next_Select = 0;
			Set_Sub();
			m_IsFirstLoop = true;
		}
		bool Update(void) noexcept {
			auto ans = Update_Sub();
			m_IsFirstLoop = false;
			return ans;
		}
		// 
		void MainDraw(void) const noexcept { MainDraw_Sub(); }
		void DrawUI_Base(void) const noexcept { DrawUI_Base_Sub(); }
		void DrawUI_In(void) const noexcept { DrawUI_In_Sub(); }
		// �`��
		void Dispose(void) noexcept { Dispose_Sub(); }
		void Dispose_Load(void) noexcept {
			if (m_IsLoading) {
				m_IsLoading = false;
				Dispose_Load_Sub();
			}
		}
	protected:// �p���� �p����̃V�[���ł͂�����override������
		virtual void Load_Sub(void) noexcept {}
		virtual void Set_Sub(void) noexcept {}
		virtual bool Update_Sub(void) noexcept { return true; }

		virtual void MainDraw_Sub(void) const noexcept {}
		virtual void DrawUI_Base_Sub(void) const noexcept {}
		virtual void DrawUI_In_Sub(void) const noexcept {}

		virtual void Dispose_Sub(void) noexcept {}
		virtual void Dispose_Load_Sub(void) noexcept {}
	};
	// --------------------------------------------------------------------------------------------------
	// �V�[�������܂Ƃ߂�N���X
	// --------------------------------------------------------------------------------------------------
	class SceneControl : public SingletonBase<SceneControl> {
	private:
		friend class SingletonBase<SceneControl>;
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
		public:
			void	UpdatePause(void) noexcept;
			void	DrawPause(void) const noexcept;
		};
	private:
		bool						m_IsEndScene{ false };		// ���݂̃V�[�����I�������t���O
		bool						m_IsEndGame{ false };		// �Q�[���I���t���O
		bool						m_IsExitSelect{ false };		// �I���|�b�v�A�b�v���J���Ă��邩�̃t���O
		bool						m_IsRestartSelect{ false };		// �ċN���|�b�v�A�b�v���J���Ă��邩�̃t���O

		FPSDrawer					m_FPSDrawer;		// FPS�\���p�N���X�̎���
		PauseDrawer					m_PauseDrawer;		// �|�[�Y��ʕ\���p�N���X�̎���
		std::shared_ptr<TEMPSCENE>	m_NowScenesPtr;		// ���s���Ă���V�[��
		// �|�[�Y�����ǂ����̃t���O
		bool						m_IsPauseActive{ false };
	public:
		const auto		IsEndScene(void) const noexcept { return m_IsEndScene || m_IsEndGame; }
		const auto		IsEndGame(void) const noexcept { return m_IsEndGame; }
		// �|�[�Y�̗L���������擾
		const auto		IsPause(void) const noexcept { return m_IsPauseActive; }
		void			ChangePause(bool value) noexcept;
		// �I���A�ċN���t���O�������Ă��邩�O����m�F�ł��܂�
		const auto&		IsExit(void) const noexcept { return m_IsExitSelect; }
		const auto&		IsRestart(void) const noexcept { return m_IsRestartSelect; }
	private:
		// �R���X�g���N�^
		SceneControl(void) noexcept {}// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		SceneControl(const SceneControl&) = delete;
		SceneControl(SceneControl&& o) = delete;
		SceneControl& operator=(const SceneControl&) = delete;
		SceneControl& operator=(SceneControl&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		//�ŏ��ɐi�ރV�[����ݒ�
		void			SetFirstScene(const std::shared_ptr<TEMPSCENE>& ptr) noexcept { m_NowScenesPtr = ptr; }
		void			InitMainLoop(void) noexcept;
		void			UpdateMainLoop(void) noexcept;
		void			DrawMainLoop(void) const noexcept;
		void			ExitMainLoop(void) noexcept;
	};
};
