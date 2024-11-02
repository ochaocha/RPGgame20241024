#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// �L�����N�^�[�Ȃǂ�2D�I�u�W�F�N�g���ꌳ�Ǘ�����N���X
	// --------------------------------------------------------------------------------------------------
	class Object2DManager : public SingletonBase<Object2DManager> {
	private:
		friend class SingletonBase<Object2DManager>;
	public:
		//UniqueID�ł���Ɩ������邽�߂Ɋ�{�^�ɖ��O��t���ĊǗ�
		using UniqueID = int;
		// �I�u�W�F�N�g�̊��N���X
		class Base2DObject {
		private:
			UniqueID		m_UniqueID{ InvalidID };			// �I�u�W�F�N�g�}�l�[�W���[�ɂ��Ă��炤ID �I�u�W�F�N�g�}�l�[�W���[��DeleteAll����Ȃ�����e�I�u�W�F�N�g��ID�����N�����Ȃ�
			bool			m_IsDelete{ false };				// �I�u�W�F�N�g���}�l�[�W���[�ɍ폜�t���O��`���邽�߂̕ϐ�
			bool			m_IsFirstLoop{ true };				// ���߂�Update_Sub��ʂ�ꍇTRUE�ɂȂ�ϐ�(�p����ł̂ݎQ�Ƃł�����̂Ƃ���)
			Vector2DX		m_PrevPos{};						// SetPosition�Őݒ肷��O�̍��W
			Vector2DX		m_Pos{};							// ���W
			Vector2DX		m_Vec{};							// �ړ��x�N�g��
			float			m_Size{ 1.f };						// �T�C�Y
			float			m_Mass{ 1.f };						// �d��
			UniqueID		m_HitUniqueID{ InvalidID };			// �I�u�W�F�N�g�ɓ��������ꍇ�ɉ��̃I�u�W�F�N�g�ɓ������������m�F����ׂ̕ϐ�
			bool			m_IsHitOtherObject{ false };		// ���̃I�u�W�F�N�g�Ɠ����邩�ǂ���
			int				m_ObjType{};						// �I�u�W�F�N�g�̎��
		protected:
			// ���߂�Update_Sub��ʂ邩�ǂ���
			const auto& GetIsFirstLoop(void) const noexcept { return this->m_IsFirstLoop; }
		public:
			// ������UniqueID���擾
			const auto& GetUniqueID(void) const noexcept { return this->m_UniqueID; }
			// �������}�l�[�W���[�ɔj�����Ă��炤�t���O
			const auto& GetIsDelete(void) const noexcept { return this->m_IsDelete; }
			// 
			const auto& GetPosition(void) const noexcept { return this->m_Pos; }
			const auto& GetPrevPos(void) const noexcept { return this->m_PrevPos; }
			const auto& GetVec(void) const noexcept { return this->m_Vec; }
			const auto& GetSize(void) const noexcept { return this->m_Size; }
			const auto& GetMass(void) const noexcept { return this->m_Mass; }
			const auto& GetHitUniqueID(void) const noexcept { return this->m_HitUniqueID; }
			const auto& GetIsHitOtherObject(void) const noexcept { return this->m_IsHitOtherObject; }
			const auto& GetObjType(void) const noexcept { return this->m_ObjType; }
		public:
			// �������}�l�[�W���[�ɔj�����Ă��炤�t���O�𗧂Ă�
			void			SetDelete(void) noexcept { this->m_IsDelete = true; }
			void			SetPosition(const Vector2DX& value) noexcept {
				this->m_Pos = value;
				this->m_PrevPos = this->m_Pos;
			}
			void			SetVec(const Vector2DX& value) noexcept { this->m_Vec = value; }
			void			SetSize(float value) noexcept { this->m_Size = value; }
			void			SetMass(float value) noexcept { this->m_Mass = value; }
			void			SetHitUniqueID(UniqueID value) noexcept { this->m_HitUniqueID = value; }
			void			SetIsHitOtherObject(bool value) noexcept { this->m_IsHitOtherObject = value; }
			void			SetObjType(int value) noexcept { this->m_ObjType = value; }			// ���������^�C�v����ݒ�
		public:
			// �R���X�g���N�^
			Base2DObject() noexcept {}
			// �f�X�g���N�^
			virtual ~Base2DObject() noexcept {}
		public:// �}�l�[�W���[�ł܂Ƃ߂čs������
			// ������
			void			Init(UniqueID value) noexcept {
				this->m_UniqueID = value;
				Init_Sub();
				m_IsFirstLoop = true;
			}
			// �X�V
			void			Update(void) noexcept {
				Update_Sub();
				// �����ɓ�������
				if (this->m_HitUniqueID != InvalidID) {
					Update_OnHitObject();
					this->m_HitUniqueID = InvalidID;
				}
				m_IsFirstLoop = false;
			}
			// �e�`��
			void			DrawShadow(void) noexcept { DrawShadow_Sub(); }
			// �`��
			void			Draw(void) noexcept { Draw_Sub(); }
			// �I�u�W�F�N�g�폜
			void			Dispose(void) noexcept { Dispose_Sub(); }
		protected:// �C�ӂ�override���Ċe�I�u�W�F�N�g���L�̏�����ǉ����܂�
			virtual void	Init_Sub(void) noexcept {}			// ���������ɒʂ�
			virtual void	Update_Sub(void) noexcept {}		// �X�V�̍ۂɒʂ�
			virtual void	Update_OnHitObject(void) noexcept {}// ���̃I�u�W�F�N�g�Ƀq�b�g�����u�ԂɌĂԌn�̂���
			virtual void	DrawShadow_Sub(void) noexcept {}	// �e�`��̍ۂɒʂ�
			virtual void	Draw_Sub(void) noexcept {}			// �`��̍ۂɒʂ�
			virtual void	Dispose_Sub(void) noexcept {}		// �I�u�W�F�N�g���폜�����ۂɒʂ�
		};
	private:
		// �I�u�W�F�N�g��sharedPtr���܂Ƃ߂����X�g
		std::vector<std::shared_ptr<Base2DObject>>		m_Object;
		// ����������ۂɃI�u�W�F�N�g�ɐU�蕪���郆�j�[�NID(�U�蕪������+1���ĕ�ID�ɂ��܂�)
		UniqueID										m_LastUniqueID{ 0 };
	private:
		// �R���X�g���N�^
		Object2DManager(void) noexcept {
			// ���X�g�̃������m�ۂ̂ݐ�ɍs���Ă���(���̒i�K�ł͎��ۂɃ��X�g�ɂ͓o�^���Ȃ�)
			this->m_Object.reserve(256);
		}
		Object2DManager(const Object2DManager&) = delete;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		Object2DManager(Object2DManager&& o) = delete;
		Object2DManager& operator=(const Object2DManager&) = delete;
		Object2DManager& operator=(Object2DManager&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		// �쐬���ꂽ�I�u�W�F�N�g�����X�g�ɒǉ�
		void			AddObject(const std::shared_ptr<Base2DObject>& NewObj) noexcept;
		// UniqueID�Ō����������ăI�u�W�F�N�g���擾(�Ȃ��ꍇ��nullptr)
		const std::shared_ptr<Base2DObject>	GetObj(UniqueID ID) noexcept;
		// UniqueID�Ō����������ăI�u�W�F�N�g�����邩�m�F
		bool			HasObj(UniqueID ID) noexcept;
		// �O���ŃI�u�W�F�N�g�ɑ΂��ď������s���ꍇ�̂��߂Ƀ��X�g���擾
		std::vector<std::shared_ptr<Base2DObject>>& GetObjList() noexcept { return m_Object; }
		// �Y���̃I�u�W�F�N�g���폜
		void			DelObj(const std::shared_ptr<Base2DObject>& ptr) noexcept;
	public:
		// ���ʂ̃I�u�W�F�N�g�X�V
		void			Update(void) noexcept;
		// �I�u�W�F�N�g�S�̂��e�`�ʂ���
		void			DrawShadow(void) noexcept;
		// �I�u�W�F�N�g�S�̂�`�ʂ���
		void			Draw(void) noexcept;
		// ���ׂẴI�u�W�F�N�g���폜����
		void			DeleteAll(void) noexcept;
	};
};