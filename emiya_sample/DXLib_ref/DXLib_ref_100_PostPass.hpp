#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// �|�X�g�v���Z�X����(���C���̏������s������A���̌��ʂ����H����)���s���܂�
	// --------------------------------------------------------------------------------------------------
	// �e�|�X�g�v���Z�X�̊��N���X
	class PostPassBase {
	protected:
		bool m_PrevActive{ false };
	public:
		PostPassBase(void) noexcept {}
		virtual ~PostPassBase(void) noexcept {}
	protected:
		virtual void Load_Sub(void) noexcept {}
		virtual void Dispose_Sub(void) noexcept {}
		virtual bool IsActive_Sub(void) noexcept { return true; }
		virtual void SetEffect_Sub(GraphHandle*, GraphHandle*) noexcept {}
	public:
		bool IsActive(void) noexcept { return IsActive_Sub(); }
		void Init(void) noexcept {
			m_PrevActive = IsActive_Sub();
			if (m_PrevActive) {
				Load_Sub();
			}
		}
		void UpdateActive(void) noexcept {
			bool active = IsActive_Sub();
			if (m_PrevActive != active) {
				m_PrevActive = active;
				if (active) {
					Load_Sub();
				}
				else {
					Dispose_Sub();
				}
			}
		}
		void Dispose(void) noexcept {
			if (IsActive_Sub()) {
				Dispose_Sub();
			}
		}
		void SetEffect(GraphHandle* TargetGraph, GraphHandle* ColorGraph) noexcept {
			if (IsActive_Sub()) {
				SetEffect_Sub(TargetGraph, ColorGraph);
			}
		}
	};
	// ��L�|�X�g�v���Z�X�����܂Ƃ߂�N���X
	class PostPassEffect : public SingletonBase<PostPassEffect> {
	private:
		friend class SingletonBase<PostPassEffect>;
	private:
		GraphHandle BufferScreen;	// �`��X�N���[��
		GraphHandle ColorScreen;	// ���̂܂ܓ��߂Ȃ��ɂ����X�N���[��
		// �s���|�X�g�v���Z�X�̃��X�g
		std::vector<std::unique_ptr<PostPassBase>> m_PostPass;
		// �K���}�l�Ȃǂ̐F���␳���s�����߂̃p�����[�^�[
		int				InColorPerMin = 20;
		int				InColorPerMax = 255;
		float			InColorGamma = 1.1f;
		// �F�����̋����𒲐����邽�߂̃p�����[�^�[
		float			m_AberrationPower{ 1.f };
	public:// �����ł���p�����[�^�[�Q�ւ̃A�N�Z�X
		const auto&		GetAberrationPower(void) const noexcept { return m_AberrationPower; }
		void			SetAberrationPower(float value) noexcept { m_AberrationPower = value; }
	public:// �����Ώۂ̉��
		auto&			GetBufferScreen(void) noexcept { return BufferScreen; }
	public:
		// �K���}�l�Ȃǂ̐F���␳���s�����߂̃p�����[�^�[���w�肷��
		// �g�p����P�x�̍Œ�l�A�ō��l�A�K���}�l
		void SetLevelFilter(int inMin, int inMax, float gamma) noexcept {
			InColorPerMin = Clamp(inMin, 0, 255);
			InColorPerMax = Clamp(inMax, 0, 255);
			InColorGamma = GetMax(1.f, gamma);
		}
	private:
		// �R���X�g���N�^
		PostPassEffect(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		PostPassEffect(const PostPassEffect&) = delete;
		PostPassEffect(PostPassEffect&& o) = delete;
		PostPassEffect& operator=(const PostPassEffect&) = delete;
		PostPassEffect& operator=(PostPassEffect&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		void Update(void) noexcept;
		void DrawPostProcess(void) noexcept;
		void Dispose(void) noexcept;
	};
};
