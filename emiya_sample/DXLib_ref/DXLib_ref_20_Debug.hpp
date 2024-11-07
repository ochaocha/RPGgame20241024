#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
#if DEBUG
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// �������Ԃ��v�����邱�Ƃ��ł���N���X ���������Ȃǂ̍ۂɂǂ��Ŏ��Ԃ�������̂��Ȃǂ��m�F����̂Ɏg���܂�
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class DebugClass : public SingletonBase<DebugClass> {
	private:
		friend class SingletonBase<DebugClass>;
	private:
		static const int	PointMax{ 12 };// �z�u�ł���ő�|�C���g��(SetPoint���T�˂��ꂾ���u���܂�)
		static const int	PointFrame{ 120 };// �L������ő�t���[����
	private:
		LONGLONG																m_StartTime{ 0 };// �v�����J�n�������Ԃ�ێ����܂�
		std::array<std::string, PointMax + 1>									m_Str;// �e�|�C���g�Ɏw�肵��������ێ����܂�
		std::vector<std::array<float, PointMax + 1>>							m_Point;// �e�|�C���g�̕�����PointFrame�Ԃ�ێ�����܂�
		size_t																	m_PointSel{ 0 };// SetPoint�����Ԗڂɒu���������ێ�����܂�
		switchs																	m_Switch;// F1�L�[�ŕ\����\�������߂邱�Ƃ��ł��܂�
		bool																	m_IsActive{ true };
	private:
		// �R���X�g���N�^
		DebugClass(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		DebugClass(const DebugClass&) = delete;
		DebugClass(DebugClass&& o) = delete;
		DebugClass& operator=(const DebugClass&) = delete;
		DebugClass& operator=(DebugClass&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		// �f�o�b�O�̌v���J�n�|�C���g�ɒu������
		void SetStartPoint(void) noexcept;
		// �C�ӂ̃|�C���g�ɒu���ƁA�O�ɏ�����SetPoint�Ƃ̎��ԍ��𑪂��悤�ɂȂ�܂�
		void SetPoint(const char* DebugMes) noexcept;
		// �v�����I��������|�C���g�ł�
		void SetEndPoint(void) noexcept;
		// ���ʂ�\�����܂�
		void DebugWindow(int xpos, int ypos) noexcept;
	};
#endif // DEBUG
};