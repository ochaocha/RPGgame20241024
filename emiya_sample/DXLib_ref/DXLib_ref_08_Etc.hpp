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

	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// �J�����V�F�C�N
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class CameraShake : public SingletonBase<CameraShake> {
	private:
		friend class SingletonBase<CameraShake>;
	private:
		float						m_ShakePower{ 0.f };		//�h��̋���
		float						m_ShakeTotalTime{ 0.f };	//�h����N�����g�[�^������
		float						m_ShakeTime{ 0.f };			//�h��̌o�ߎ���(0�b�Ɍ����Č����Ă����܂�)
		Vector2DX					m_CamShake1;
		Vector2DX					m_CamShake2;				//�h����v�Z���邽�߂̕ϐ�
	public:
		//�h�ꂽ���ʂ��擾���܂�
		const auto& GetCamShake(void) const noexcept { return m_CamShake2; }
	public:
		//�h���ݒ肵�܂��B�@�h��鎞�ԂƗh��̗͂�2�_�ł�
		void			SetCamShake(float time, float power) noexcept {
			this->m_ShakeTotalTime = time;
			this->m_ShakePower = power;
			this->m_ShakeTime = this->m_ShakeTotalTime;
		}
	private:
		// �R���X�g���N�^
		CameraShake(void) noexcept {}// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
		CameraShake(const CameraShake&) = delete;
		CameraShake(CameraShake&& o) = delete;
		CameraShake& operator=(const CameraShake&) = delete;
		CameraShake& operator=(CameraShake&& o) = delete;
		// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
	public:
		void Update(void) noexcept;
	};
}
