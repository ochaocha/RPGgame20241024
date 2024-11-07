#pragma once
#include	"../Header.hpp"
#include	"../CommonScene/UI/CommonUIControl.hpp"
#include	"../MainScene/MainSceneSub.hpp"
#include	"../MainScene/Player/CPU.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//
		// --------------------------------------------------------------------------------------------------
		class MainGameScene : public TEMPSCENE {
		private:
			PauseMenuControl			m_PauseMenuControl;
			MapNameDrawControl			m_MapNameDrawControl;
			CutSceneControl				m_CutSceneControl;

			GraphHandle					m_ViewHandle;
			GraphHandle					m_Watch;
			GraphHandle					m_Caution;
			GraphHandle					m_Alert;
			GraphHandle					m_Goal;

			Vector2DX					m_GoalPos;
			float						m_StartTime{ 0.f };

			bool						m_IsGoodEnd{ false };// �V�[���S�̂̏I���t���O

			bool						m_IsBadEnd{ false };// �V�[���S�̂̏I���t���O

			bool						m_IsEndUpdate{ false };// ���V�[���ɍs���t���O
			bool						m_IsEnd{ false };// �V�[���S�̂̏I���t���O
			bool						m_IsPlayable{ false };// ����\
			// 
			Vector2DX					m_CamAddPos;
			// 
			std::string					m_MapName;
			int							m_EntryID{};
			int							m_CutSceneID{ InvalidID };// �J�b�g�V�[��ID

			int							m_BossUniqueID{};
			int							m_WinCutSceneID{};// �J�b�g�V�[��ID
			// 
			std::pair<int, int>			m_PrevXY{};
			// 
			bool						m_IsNormalBGM{ false };
			float						m_NormalBGM{ 0.f };
			bool						m_IsCautionBGM{ false };
			float						m_CautionBGM{ 0.f };
			bool						m_IsAlertBGM{ false };
			float						m_AlertBGM{ 0.f };
		private:
			void			DrawCharaUI_Back(PlayerID value) const noexcept;
			void			SetSceneEnd(void) noexcept;
		public:
			//�R���X�g���N�^
			MainGameScene(void) noexcept { }
			MainGameScene(const MainGameScene&) = delete;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			MainGameScene(MainGameScene&& o) = delete;
			MainGameScene& operator=(const MainGameScene&) = delete;
			MainGameScene& operator=(MainGameScene&& o) = delete;
			//�f�X�g���N�^ �p���I�u�W�F�N�g�ł���׃f�X�g���N�^��virtual���w��
			virtual ~MainGameScene(void) noexcept {}
		public:
			//���̃V�[���ŉi�����Ďg�p���郊�\�[�X�̓ǂݍ��݁A�j���������Ȃ�����
			void			Load_Sub(void) noexcept override;
			void			Dispose_Load_Sub(void) noexcept override;
			// �`��ȊO�̏������A�X�V�A�j���Ɋւ������
			void			Initialize_Sub(void) noexcept override;
			bool			Update_Sub(void) noexcept override;
			void			Dispose_Sub(void) noexcept override;
			// 2D��ԕ\��
			void			DrawMain_Sub(void) const noexcept override;
			// UI�\��
			void			DrawUI_Base_Sub(void) const noexcept override;
			void			DrawUI_In_Sub(void) const noexcept override;
		};
	}
}
