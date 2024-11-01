#pragma once
#include "DXLib_ref.h"

/*------------------------------------------------------------------------------------------------------------------------------------------*/
// ���낢��p�[�c���������������̂��G���Ɋ܂܂�Ă��܂�
/*------------------------------------------------------------------------------------------------------------------------------------------*/
namespace DXLibRef {
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// �⊮
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// �C�[�W���O
	enum class EasingType {
		OutExpo,
	};
	// ���炩�ȕ⊮�𔭐������邽�߂̐ϕ����Z
	extern float GetEasingRatio(EasingType EasingType, float ratio) noexcept;
	// ���`�⊮
	template <class T>
	inline T Lerp(const T& A, const T& B, float Per) noexcept {
		if (Per == 0.f) {
			return A;
		}
		else if (Per == 1.f) {
			return B;
		}
		else {
			return A + (T)((B - A) * Per);
		}
	}
	// ���炩�ȕ⊮
	template <class T>
	static void Easing(T* A, const T& B, float ratio, EasingType EasingType) noexcept { *A = Lerp(*A, B, GetEasingRatio(EasingType, ratio)); }
	//��ϕ���1F������̑��x�𓾂�
	inline float EasingMove(float* NowSpeed, float TargetSpeed, float ratio) noexcept;
	//Vector2DX��overload
	inline Vector2DX EasingMove(Vector2DX* NowSpeed, Vector2DX TargetSpeed, float ratio) {
		Vector2DX answer;
		answer.x = EasingMove(&NowSpeed->x, TargetSpeed.x, ratio);
		answer.y = EasingMove(&NowSpeed->y, TargetSpeed.y, ratio);
		return answer;
	}
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLIB���b�p�[																																*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// +-m_arg�͈̔͂ł̃����_���l���擾
	static float GetRandf(float m_arg) noexcept { return -m_arg + static_cast<float>(GetRand(static_cast<int>(m_arg * 2.f * 10000.f))) / 10000.f; }

	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLIB�̃t�@�C���ǂݍ��ݕ⏕�N���X																											*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class FileStreamDX {
		int mdata = InvalidID;
	public:
		//�R���X�g���N�^
		FileStreamDX() noexcept {}
		//�錾���Ƀt�@�C���I�[�v����
		FileStreamDX(const char* FilePath) noexcept { Open(FilePath); }
		//�f�X�g���N�^
		~FileStreamDX() noexcept { Close(); }
	public:
		//�t�@�C�����J���A�T���|�C���g���n�_�Ɉړ�
		void Open(const char* FilePath) noexcept { mdata = DxLib::FileRead_open(FilePath, FALSE); }
		// 1�s���̂܂܎擾���A���̍s�ɒT���|�C���g���ڂ�
		std::string SeekLineAndGetStr() const noexcept {
			const int charLength = 512;
			char mstr[charLength] = "";
			DxLib::FileRead_gets(mstr, charLength, mdata);
			return std::string(mstr);
		}
		// �T���|�C���g���I�[(EOF)�ŏI���
		bool ComeEof() const noexcept { return DxLib::FileRead_eof(mdata) != 0; }
		//�@����
		void Close() noexcept {
			if (mdata != InvalidID) {
				DxLib::FileRead_close(mdata);
				mdata = InvalidID;
			}
		}
	public:
		// �����񂩂�=���E�̒l�擾
		static std::string getleft(std::string tempname) noexcept { return tempname.substr(0, tempname.find('=')); }
		// �����񂩂�=���E�̒l�擾
		static std::string getright(std::string tempname) noexcept { return tempname.substr(tempname.find('=') + 1); }
	};
}
