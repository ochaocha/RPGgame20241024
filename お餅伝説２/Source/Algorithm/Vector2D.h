#pragma once
#include "DxLib.h"
#include <cmath>

// ---------------------------------------------------------------------------------------------
// 2�����x�N�g���̉��Z�N���X
// ---------------------------------------------------------------------------------------------
class Vector2DX
{
public:
	float x{ 0 };
	float y{ 0 };
public:
	//�������R���X�g���N�^:���̍쐬���ɉ���������ݒ肵�Ȃ��ꍇ�A0�ŏ��������܂�  ��)Vector2DX XXX;
	Vector2DX(void) noexcept : x(0), y(0) {}
	//�������R���X�g���N�^:���̍쐬����DX���C�u������VECTOR��ݒ肷��ꍇ�A���̒l�ŏ��������܂��BZ�͐؂�̂Ă��܂��B  ��)Vector2DX YYY = VGet(100.f,100.f,0.f);
	Vector2DX(const DxLib::VECTOR& value) noexcept { this->Set(value.x, value.y); }
	// ���g�ɒ��ڒl��ݒ肵�܂�
	void Set(float x1, float y1) noexcept {
		this->x = x1;
		this->y = y1;
	}
	//DX���C�u������VECTOR�ɕϊ����ďo�͂��܂�(DX���C�u�����Ƃ̎󂯓n���p�ł�)
	inline DxLib::VECTOR get(void) const noexcept { return DxLib::VGet(this->x, this->y, 0.f); }				// �ϊ�
	//Vector2DX�ł�VGet���쐬���܂�  ��)Vector2DX ZZZ = Vector2DX::vget(0.f, 0.f);
	inline static const Vector2DX vget(float x, float y) noexcept { return Vector2DX(DxLib::VGet(x, y, 0.f)); }
	// �v���Z�b�g�ł��B���E�㉺�E�S��1�E�S��0 ���������̂�����܂�
	static const Vector2DX right(void) noexcept { return vget(1.f, 0.f); }
	static const Vector2DX left(void) noexcept { return vget(-1.f, 0.f); }
	static const Vector2DX up(void) noexcept { return vget(0.f, 1.f); }
	static const Vector2DX down(void)noexcept { return vget(0.f, 1.f); }

	static const Vector2DX one(void) noexcept { return vget(1.f, 1.f); }
	static const Vector2DX zero(void) noexcept { return vget(0.f, 0.f); }
	// �x�N�g���̒�����2��𓾂�
	inline float sqrMagnitude(void) const noexcept { return this->x * this->x + this->y * this->y; }
	// �x�N�g���̒����𓾂�
	inline float magnitude(void) const noexcept {
#if FALSE //std::hypotf���g�p���邩�ۂ��@MSVC�ł́A�����I�Ȍ����ӂ��}������@�\���������Ă���ꍇ�����邽�ߑ��x�d���Ȃ�g��Ȃ��A���x�d���Ȃ�g��
		return std::hypotf(this->x, this->y);
#else
		float Square = sqrMagnitude();
		return Square >= 0.0000001f ? std::sqrtf(Square) : 0.0f;
#endif
	}
	//�x�N�g���̒�����1�Ƃ���x�N�g���𓾂�(0�̏ꍇ�ADX���C�u�����̃G���[�ɕ킢x,y�Ƃ���-1��Ԃ��܂�)
	inline Vector2DX normalized(void) const noexcept {
		float Size = magnitude();
		if (Size <= 0.0f) {
			return vget(-1.0f, -1.0f);
		}
		return *this / Size;
	}		// ���K��
	// �����ƈ����Ƃ����S�ɓ������ǂ������`�F�b�N���܂�(���������_���Ȃ̂Ŗ{���Ɋ��S�ɓ����������܂�)
	bool Equals(const Vector2DX& obj) const noexcept { return *this == obj; }

	//���_�����Ƃ���2�x�N�g���Ԃ̊p�x��radian�Ŏ擾  ��)float AAA = Vector2DX::Angle(Vector2DX::left(), Vector2DX::up());
	inline static float			Angle(const Vector2DX& A, const Vector2DX& B) noexcept { return std::acos(Dot(A, B) / (A.magnitude() * B.magnitude())); }

	//�x�N�g��A�̒�����Limit�͈̔͂ɐ�������
	inline static Vector2DX		ClampMagnitude(const Vector2DX& A, float Limit) noexcept {
		float Clamp = A.magnitude();
		if (Clamp < 0.f) { Clamp = 0.f; }
		if (Clamp > Limit) { Clamp = Limit; }
		return A.normalized() * Clamp;
	}

	//�x�N�g���ǂ����̋������v�Z����
	inline static float			Distance(const Vector2DX& A, const Vector2DX& B) noexcept { return (A - B).magnitude(); }
	//�x�N�g���ǂ����̊O�ς��v�Z����
	inline static float			Cross(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.y - A.y * B.x; }
	//�x�N�g���ǂ����̓��ς��v�Z����
	inline static float			Dot(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.x + A.y * B.y; }
	//0~1�͈̔͂��`�F�b�N�������`�⊮
	static Vector2DX			LerpUnclamped(const Vector2DX& A, const Vector2DX& B, float Per) noexcept {
		return A + (B - A) * Per;
	}
	//0~1�͈̔͂��`�F�b�N���A���͈̔͂Ő��`�⊮
	static Vector2DX			Lerp(const Vector2DX& A, const Vector2DX& B, float Per) noexcept {
		if (Per <= 0.f) {
			return A;
		}
		else if (Per >= 1.f) {
			return B;
		}
		else 
		{
			return LerpUnclamped(A, B, Per);
		}
	}
	//�x�N�g���ǂ�����xy�ő傫������I�����Ď擾
	inline static Vector2DX		Max(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x > B.x) ? A.x : B.x, (A.y > B.y) ? A.y : B.y); }
	//�x�N�g���ǂ�����xy�ŏ���������I�����Ď擾
	inline static Vector2DX		Min(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x < B.x) ? A.x : B.x, (A.y < B.y) ? A.y : B.y); }
	//�x�N�g���̒�����1�Ƃ���x�N�g���𓾂�
	inline static Vector2DX		Normalize(const Vector2DX& A) noexcept { return A.normalized(); }
	//�x�N�g����@���Ŕ��˂����x�N�g���𓾂�
	inline static Vector2DX		Reflect(const Vector2DX& inDirection, const Vector2DX& inNormal) noexcept { return inDirection + inNormal * (Dot(inNormal, inDirection * -1.f)) * 2.f; }
	//�x�N�g���ƃx�N�g���Ƃł̃X�P�[�����O���s��
	inline static Vector2DX		Scale(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x * B.x), (A.y * B.y)); }

	//�ȉ��AUnity�ɂ͂��邪�g�p�p�x�������Ⴂ�̂ŃI�~�b�g�����֐�
	/*
	// MoveTowards
	// OrthoNormalize
	// Project
	// ProjectOnPlane
	// RotateTowards
	// SignedAngle
	// Slerp
	// SlerpUnclamped
	// SmoothDamp
	*/

	// ��r�I�y���[�^�[
	inline bool operator==(const Vector2DX& obj) const noexcept { return (*this - obj).magnitude() <= 0.001f; }
	inline bool operator!=(const Vector2DX& obj) const noexcept { return !(*this == obj); }
	// ���Z�I�y���[�^�[
	inline Vector2DX operator+(const Vector2DX& obj) const noexcept { return vget(this->x + obj.x, this->y + obj.y); }
	inline void operator+=(const Vector2DX& obj) noexcept {
		this->x += obj.x;
		this->y += obj.y;
	}
	// ���Z�I�y���[�^�[
	inline Vector2DX operator-(const Vector2DX& obj) const noexcept { return vget(this->x - obj.x, this->y - obj.y); }
	inline void operator-=(const Vector2DX& obj) noexcept {
		this->x -= obj.x;
		this->y -= obj.y;
	}
	// ��Z�I�y���[�^�[
	inline Vector2DX operator*(float p1) const noexcept { return vget(this->x * p1, this->y * p1); }
	inline void operator*=(float p1) noexcept {
		this->x *= p1;
		this->y *= p1;
	}
	// ���Z�I�y���[�^�[
	inline Vector2DX operator/(float p1) const noexcept { return *this * (1.f / p1); }
	inline void operator/=(float p1) noexcept { *this *= (1.f / p1); }
};
