#pragma once
#include	"../Header.hpp"

#include	"../CommonScene/UI/CommonUIControl.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		// �^�C�g���V�[��
		// --------------------------------------------------------------------------------------------------
		class TitleScene : public TEMPSCENE {
		private:
			bool						m_IsStartGame{ false };	//�Q�[���X�^�[�g�t���O��������
			GraphHandle					m_TitleImage;			//�^�C�g���摜
		public:
			//�R���X�g���N�^
			TitleScene(void) noexcept {}
			TitleScene(const TitleScene&) = delete;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			TitleScene(TitleScene&& o) = delete;
			TitleScene& operator=(const TitleScene&) = delete;
			TitleScene& operator=(TitleScene&& o) = delete;
			//�f�X�g���N�^ �p���I�u�W�F�N�g�ł���׃f�X�g���N�^��virtual���w��
			virtual ~TitleScene(void) noexcept {}
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
			void			DrawUI_In_Sub(void) const noexcept override {}
		};
	}
}
