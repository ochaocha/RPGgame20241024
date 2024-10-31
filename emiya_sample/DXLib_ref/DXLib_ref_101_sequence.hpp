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
		std::vector<std::shared_ptr<TEMPSCENE>> m_ScenesPtr{};	// �S�V�[���̃��X�g
		std::shared_ptr<TEMPSCENE>	m_NowScenesPtr;				// ���s���Ă���V�[��
	private:
		// �R���X�g���N�^
		SceneControl(void) noexcept {}// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		SceneControl(const SceneControl&) = delete;
		SceneControl(SceneControl&& o) = delete;
		SceneControl& operator=(const SceneControl&) = delete;
		SceneControl& operator=(SceneControl&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		// �����s���Ă���V�[�����擾
		const auto& GetNowScene(void) const noexcept { return this->m_NowScenesPtr; }
	public:
		// �V�[�������X�g�ɒǉ�
		void	AddList(const std::shared_ptr<TEMPSCENE>& ptr) noexcept {
			this->m_ScenesPtr.emplace_back(ptr);
			// ���łɏ���̏ꍇ�͂��̃V�[������Đ����邱�ƂƂ���
			if (this->m_ScenesPtr.size() == 1) {
				this->m_NowScenesPtr = this->m_ScenesPtr.back();
			}
		}
	public:
		// ���̃V�[���֓]������
		void	NextScene(void) noexcept {
			GetNowScene()->Dispose();							// ���̃V�[������̉��
			if (GetNowScene() != GetNowScene()->Get_Next()) {	// ���̃V�[���Ǝ��̃V�[���Ƃ��ʂ̃V�[���Ȃ�
				GetNowScene()->Dispose_Load();					// ���[�h���Ă����f�[�^��j��
			}
			this->m_NowScenesPtr = GetNowScene()->Get_Next();	// ���̃V�[���֑J��
		}
		void	Dispose(void) noexcept {
			for (auto& s : this->m_ScenesPtr) {
				s->Dispose();
			}
		}
	};
};
