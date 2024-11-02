#pragma once
// ---------------------------------------------------------------------------------------------
// Vector2(xy����Ȃ�2�����x�N�g��)�N���X�Ɣ��艉�Z�֐��Q
// ---------------------------------------------------------------------------------------------

namespace DXLibRef {
	// ---------------------------------------------------------------------------------------------
	// 2�����x�N�g���̉��Z�N���X
	// ---------------------------------------------------------------------------------------------
	class Vector2DX {
	public:
		float x{ 0 };
		float y{ 0 };
	public:
		// �������R���X�g���N�^:���̍쐬���ɉ���������ݒ肵�Ȃ��ꍇ�A0�ŏ��������܂�  ��)Vector2DX XXX;
		Vector2DX(void) noexcept : x(0), y(0) {}
		// �������R���X�g���N�^:���̍쐬����DX���C�u������VECTOR��ݒ肷��ꍇ�A���̒l�ŏ��������܂��BZ�͐؂�̂Ă��܂��B  ��)Vector2DX YYY = VGet(100.f,100.f,0.f);
		Vector2DX(const DxLib::VECTOR& value) noexcept { this->Set(value.x, value.y); }
		// ���g�ɒ��ڒl��ݒ肵�܂�
		void Set(float x1, float y1) noexcept {
			this->x = x1;
			this->y = y1;
		}
		// DX���C�u������VECTOR�ɕϊ����ďo�͂��܂�(DX���C�u�����Ƃ̎󂯓n���p�ł�)
		inline DxLib::VECTOR get(void) const noexcept { return DxLib::VGet(this->x, this->y, 0.f); }				// �ϊ�
		// Vector2DX�ł�VGet���쐬���܂�  ��)Vector2DX ZZZ = Vector2DX::vget(0.f, 0.f);
		inline static const Vector2DX vget(float x, float y) noexcept { return Vector2DX(DxLib::VGet(x, y, 0.f)); }
		// �v���Z�b�g�ł��B���E�㉺�E�S��1�E�S��0 ���������̂�����܂�
		static const Vector2DX right(void) noexcept { return vget(1.f, 0.f); }
		static const Vector2DX left(void) noexcept { return vget(-1.f, 0.f); }
		static const Vector2DX up(void) noexcept { return vget(0.f, 1.f); }
		static const Vector2DX down(void) noexcept { return vget(0.f, -1.f); }

		static const Vector2DX one(void) noexcept { return vget(1.f, 1.f); }
		static const Vector2DX zero(void) noexcept { return vget(0.f, 0.f); }
		// �x�N�g���̒�����2��𓾂�
		inline float sqrMagnitude(void) const noexcept { return this->x * this->x + this->y * this->y; }
		// �x�N�g���̒����𓾂�
		inline float magnitude(void) const noexcept {
#if FALSE // std::hypotf���g�p���邩�ۂ��@MSVC�ł́A�����I�Ȍ����ӂ��}������@�\���������Ă���ꍇ�����邽�ߑ��x�d���Ȃ�g��Ȃ��A���x�d���Ȃ�g��
			return std::hypotf(this->x, this->y);
#else
			float Square = sqrMagnitude();
			return Square >= 0.0000001f ? std::sqrtf(Square) : 0.0f;
#endif
		}
		// �x�N�g���̒�����1�Ƃ���x�N�g���𓾂�(0�̏ꍇ�ADX���C�u�����̃G���[�ɕ킢x,y�Ƃ���-1��Ԃ��܂�)
		inline Vector2DX normalized(void) const noexcept {
			float Size = magnitude();
			if (Size <= 0.0f) {
				return vget(-1.0f, -1.0f);
			}
			return *this / Size;
		}		// ���K��
		// �����ƈ����Ƃ����S�ɓ������ǂ������`�F�b�N���܂�(���������_���Ȃ̂Ŗ{���Ɋ��S�ɓ����������܂�)
		bool Equals(const Vector2DX& obj) const noexcept { return *this == obj; }

		// ���_�����Ƃ���2�x�N�g���Ԃ̊p�x��radian�Ŏ擾  ��)float AAA = Vector2DX::Angle(Vector2DX::left(), Vector2DX::up());
		inline static float			Angle(const Vector2DX& A, const Vector2DX& B) noexcept { return std::acos(Dot(A, B) / (A.magnitude() * B.magnitude())); }

		// �x�N�g��A�̒�����Limit�͈̔͂ɐ�������
		inline static Vector2DX		ClampMagnitude(const Vector2DX& A, float Limit) noexcept {
			float Clamp = A.magnitude();
			if (Clamp < 0.f) { Clamp = 0.f; }
			if (Clamp > Limit) { Clamp = Limit; }
			return A.normalized() * Clamp;
		}

		// �x�N�g���ǂ����̋������v�Z����
		inline static float			Distance(const Vector2DX& A, const Vector2DX& B) noexcept { return (A - B).magnitude(); }
		// �x�N�g���ǂ����̊O�ς��v�Z����
		inline static float			Cross(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.y - A.y * B.x; }
		// �x�N�g���ǂ����̓��ς��v�Z����
		inline static float			Dot(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.x + A.y * B.y; }
		// 0~1�͈̔͂��`�F�b�N�������`�⊮
		static Vector2DX			LerpUnclamped(const Vector2DX& A, const Vector2DX& B, float Per) noexcept {
			return A + (B - A) * Per;
		}
		// 0~1�͈̔͂��`�F�b�N���A���͈̔͂Ő��`�⊮
		static Vector2DX			Lerp(const Vector2DX& A, const Vector2DX& B, float Per) noexcept {
			if (Per <= 0.f) {
				return A;
			}
			else if (Per >= 1.f) {
				return B;
			}
			else {
				return LerpUnclamped(A, B, Per);
			}
		}
		// �x�N�g���ǂ�����xy�ő傫������I�����Ď擾
		inline static Vector2DX		Max(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x > B.x) ? A.x : B.x, (A.y > B.y) ? A.y : B.y); }
		// �x�N�g���ǂ�����xy�ŏ���������I�����Ď擾
		inline static Vector2DX		Min(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x < B.x) ? A.x : B.x, (A.y < B.y) ? A.y : B.y); }
		// �x�N�g���̒�����1�Ƃ���x�N�g���𓾂�
		inline static Vector2DX		Normalize(const Vector2DX& A) noexcept { return A.normalized(); }
		// �x�N�g����@���Ŕ��˂����x�N�g���𓾂�
		inline static Vector2DX		Reflect(const Vector2DX& inDirection, const Vector2DX& inNormal) noexcept { return inDirection + inNormal * (Dot(inNormal, inDirection * -1.f)) * 2.f; }
		// �x�N�g���ƃx�N�g���Ƃł̃X�P�[�����O���s��
		inline static Vector2DX		Scale(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x * B.x), (A.y * B.y)); }

		// �ȉ��AUnity�ɂ͂��邪�g�p�p�x�������Ⴂ�̂ŃI�~�b�g�����֐�
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
	// ---------------------------------------------------------------------------------------------
	// DXLIB�̔��艉�Z���b�p�[
	// ---------------------------------------------------------------------------------------------
	// ���ƎO�p�Ƃ̔���
	static bool GetHitSphereToTriangle(const Vector2DX& pos, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, const Vector2DX& tri_p3) noexcept {
		return DxLib::HitCheck_Sphere_Triangle(pos.get(), size, tri_p1.get(), tri_p2.get(), tri_p3.get()) == TRUE;
	}
	// �O�p�ƃJ�v�Z���Ƃ̔���
	static bool GetHitCapsuleToTriangle(const Vector2DX& startpos, const Vector2DX& endpos, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, const Vector2DX& tri_p3) noexcept {
		return DxLib::HitCheck_Capsule_Triangle(startpos.get(), endpos.get(), size, tri_p1.get(), tri_p2.get(), tri_p3.get()) == TRUE;
	}
	// ���ƃJ�v�Z���Ƃ̔���
	static bool GetHitSphereToCapsule(const Vector2DX& pos, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, float size2) noexcept {
		return DxLib::HitCheck_Sphere_Capsule(pos.get(), size, tri_p1.get(), tri_p2.get(), size2) == TRUE;
	}
	// �J�v�Z���ƃJ�v�Z���Ƃ̔���
	static bool GetHitCheckToCapsule(const Vector2DX& pos1, const Vector2DX& pos2, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, float size2) noexcept {
		return DxLib::HitCheck_Capsule_Capsule(pos1.get(), pos2.get(), size, tri_p1.get(), tri_p2.get(), size2) == TRUE;
	}
	// �����ƒ����̈�ԋ߂�����
	static float GetMinLenSegmentToSegment(const Vector2DX& startpos, const Vector2DX& endpos, const Vector2DX& tgtstartpos, const Vector2DX& tgtendpos) noexcept {
		return DxLib::Segment_Segment_MinLength(startpos.get(), endpos.get(), tgtstartpos.get(), tgtendpos.get());
	}
	// �����Ɠ_�̈�ԋ߂��_
	static float GetMinLenSegmentToPoint(const Vector2DX& startpos, const Vector2DX& endpos, const Vector2DX& tgt) noexcept {
		return DxLib::Segment_Point_MinLength(startpos.get(), endpos.get(), tgt.get());
	}
	// ���ʂƓ_�̈�ԋ߂��_
	static Vector2DX GetMinPosSegmentToPoint(const Vector2DX& startpos, const Vector2DX& endpos, const Vector2DX& tgt) noexcept {
		return DxLib::Plane_Point_MinLength_Position(startpos.get(), endpos.get(), tgt.get());
	}
	// �������m�̌�������
	static bool GetSegmenttoSegment(const Vector2DX& SegmentAPos1, const Vector2DX& SegmentAPos2, const Vector2DX& SegmentBPos1, const Vector2DX& SegmentBPos2, SEGMENT_SEGMENT_RESULT* Result) noexcept {
		VECTOR Pos1t = SegmentAPos1.get();
		VECTOR Pos2t = SegmentAPos2.get();
		VECTOR PosAt = SegmentBPos1.get();
		VECTOR PosBt = SegmentBPos2.get();

		DxLib::Segment_Segment_Analyse(&Pos1t, &Pos2t, &PosAt, &PosBt, Result);
		float len = 0.001f;
		return (Result->SegA_SegB_MinDist_Square <= (len * len));
	}
	// �����ƎO�p�Ƃ̌�������
	static bool GetSegmenttoTriangle(const Vector2DX& SegmentAPos1, const Vector2DX& SegmentAPos2, const Vector2DX& SegmentBPos1, const Vector2DX& SegmentBPos2, const Vector2DX& SegmentBPos3, SEGMENT_TRIANGLE_RESULT* Result) noexcept {
		VECTOR Pos1t = SegmentAPos1.get();
		VECTOR Pos2t = SegmentAPos2.get();
		VECTOR PosAt = SegmentBPos1.get();
		VECTOR PosBt = SegmentBPos2.get();
		VECTOR PosCt = SegmentBPos3.get();

		DxLib::Segment_Triangle_Analyse(&Pos1t, &Pos2t, &PosAt, &PosBt, &PosCt, Result);
		float len = 0.001f;
		return (Result->Seg_Tri_MinDist_Square <= (len * len));
	}
	// �����Ɠ_�Ƃ̌�������
	static bool GetSegmenttoPoint(const Vector2DX& SegmentAPos1, const Vector2DX& SegmentAPos2, const Vector2DX& PointPos, SEGMENT_POINT_RESULT* Result) noexcept {
		VECTOR Pos1t = SegmentAPos1.get();
		VECTOR Pos2t = SegmentAPos2.get();
		VECTOR PosAt = PointPos.get();

		DxLib::Segment_Point_Analyse(&Pos1t, &Pos2t, &PosAt, Result);
		float len = 0.001f;
		return (Result->Seg_Point_MinDist_Square <= (len * len));
	}
	// ---------------------------------------------------------------------------------------------
	// ���艉�Z
	// ---------------------------------------------------------------------------------------------
	// �_�Ƌ�`�Ƃ̔���
	static bool HitPointToRectangle(int xp, int yp, int x1, int y1, int x2, int y2) noexcept { return (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2); }
	// ��`�Ƌ�`�Ƃ̔���
	static bool HitRectangleToRectangle(int xp1, int yp1, int xp2, int yp2, int x1, int y1, int x2, int y2) noexcept { return (xp1 < x2 && x1 < xp2) && (yp1 < y2 && y1 < yp2); }
	// �_�Ǝl�p�`�Ƃ̔���
	static bool HitPointToSquare(int xp, int yp, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) noexcept {
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x2 - x1), static_cast<float>(y2 - y1)), Vector2DX::vget(static_cast<float>(xp - x1), static_cast<float>(yp - y1)))) { return false; }
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x3 - x2), static_cast<float>(y3 - y2)), Vector2DX::vget(static_cast<float>(xp - x2), static_cast<float>(yp - y2)))) { return false; }
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x4 - x3), static_cast<float>(y4 - y3)), Vector2DX::vget(static_cast<float>(xp - x3), static_cast<float>(yp - y3)))) { return false; }
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x1 - x4), static_cast<float>(y1 - y4)), Vector2DX::vget(static_cast<float>(xp - x4), static_cast<float>(yp - y4)))) { return false; }
		return true;
	}

	// �l�����͈̔͂Ɏ��߂�
	static float Clamp(float In, float MinParam, float MaxParam) noexcept {
		if (In < MinParam) { return MinParam; }
		if (In > MaxParam) { return MaxParam; }
		return In;
	}
	static int Clamp(int In, int MinParam, int MaxParam) noexcept {
		if (In < MinParam) { return MinParam; }
		if (In > MaxParam) { return MaxParam; }
		return In;
	}
	static float GetMin(float In, float MinParam) noexcept {
		if (In > MinParam) { return MinParam; }
		return In;
	}
	static int GetMin(int In, int MinParam) noexcept {
		if (In > MinParam) { return MinParam; }
		return In;
	}
	static float GetMax(float In, float MaxParam) noexcept {
		if (In < MaxParam) { return MaxParam; }
		return In;
	}
	static int GetMax(int In, int MaxParam) noexcept {
		if (In < MaxParam) { return MaxParam; }
		return In;
	}
	// --------------------------------------------------------------------------------------------------
	// �p�x�ϊ�
	// --------------------------------------------------------------------------------------------------
	// �p�x���烉�W�A����
	extern void* enabler;// �_�~�[�ϐ�
	template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type*& = enabler>
	constexpr float deg2rad(T p1) noexcept { return float(p1) * DX_PI_F / 180.f; }
	// ���W�A������p�x��
	template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type*& = enabler>
	constexpr float rad2deg(T p1) noexcept { return float(p1) * 180.f / DX_PI_F; }
	// �]���藝
	constexpr float GetCosFormula(float a, float b, float c) noexcept {
		if (b + c > a && c + a > b && a + b > c) {
			return Clamp((b * b + c * c - a * a) / (2.f * b * c), -1.f, 1.f);
		}
		return 1.f;
	}
	// �x�N�g���Əォ��E���̊p�x���擾
	static inline float GetRadVec(const Vector2DX& vec1) noexcept { return std::atan2f(vec1.x, vec1.y); }

	//�p�x���v�Z(360�x���ۂ̉��Z�ȂǂŎg�p���܂�)
	static inline float GetRadRad2Rad(float NowRadian, float TargetRadian) noexcept {
		Vector2DX Vec; Vec.Set(std::sin(NowRadian), std::cos(NowRadian));
		Vector2DX vec_a; vec_a.Set(std::sin(TargetRadian), std::cos(TargetRadian));
		float cost = Vector2DX::Cross(vec_a, Vec);
		float sint = sqrtf(std::abs(1.f - cost * cost));
		return GetRadVec(Vector2DX::vget(cost, sint));
	}
}
