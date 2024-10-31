#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		// ���v���W�F�N�g��̒ǉ��ݒ����ꂽ2D�I�u�W�F�N�g�̊��N���X
		class Base2DObjectKai : public Object2DManager::Base2DObject {
		private:
			Object2DType	m_ObjType{};						// �I�u�W�F�N�g�̎��
			bool			m_IsCheckOnlyHitWall{ false };// �}�b�v�̕ǂƓ����蔻��ł��蔻������Ȃ�(���������i�K�ŏ�����)���ǂ���
		public:
			const auto&		GetObjType(void) const noexcept { return this->m_ObjType; }
		public:
			// ���������^�C�v����ݒ�
			void			SetObjType(Object2DType value) noexcept { this->m_ObjType = value; }
			void			SetIsCheckOnlyHitWall(bool value) noexcept { this->m_IsCheckOnlyHitWall = value; }
		public:
			Base2DObjectKai() noexcept {}
			virtual ~Base2DObjectKai() noexcept {}
		protected:
			void			UpdateWallHit(void) noexcept;
		protected:
			// �q�b�g�����u�ԂɌĂԌn�̂���
			virtual void	Update_OnHitWall(void) noexcept {}
		};

		// �L�������Z���u���[�G�t�F�N�g�̕`�揈���N���X
		class Blur2DControl {
		private:
			class BlurParts {
				Vector2DX	m_Pos;
				float		m_Time{};
				float		m_TimeMax{};
			public:
				void Set(const Vector2DX& pos, float time) noexcept {
					this->m_Pos = pos;
					this->m_TimeMax = time;
					this->m_Time = this->m_TimeMax;
				}
			public:
				auto IsActive(void) const noexcept { return this->m_Time > 0.f; }
				auto GetPer(void) const noexcept { return this->m_Time / this->m_TimeMax; }
				const auto& GetPos(void) const noexcept { return this->m_Pos; }
			public:
				void Update(void) noexcept;
			};
		private:
			std::array<BlurParts, 60>	m_Blur{};
			int				m_BlurNow{ 0 };
		public:
			const auto& GetBlur(void) const noexcept { return this->m_Blur; }
		public:
			// ���������^�C�v����ݒ�
			void			AddBlur(float Blur, const Vector2DX& Pos, const Vector2DX& Vec) noexcept;
		public:
			void			Update(void) noexcept {
				for (auto& b : this->m_Blur) {
					b.Update();
				}
			}
		};
	};
};
