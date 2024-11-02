#pragma once
// ---------------------------------------------------------------------------------------------
// Vector2(xyからなる2次元ベクトル)クラスと判定演算関数群
// ---------------------------------------------------------------------------------------------

namespace DXLibRef {
	// ---------------------------------------------------------------------------------------------
	// 2次元ベクトルの演算クラス
	// ---------------------------------------------------------------------------------------------
	class Vector2DX {
	public:
		float x{ 0 };
		float y{ 0 };
	public:
		// 初期化コンストラクタ:実体作成時に何も引数を設定しない場合、0で初期化します  例)Vector2DX XXX;
		Vector2DX(void) noexcept : x(0), y(0) {}
		// 初期化コンストラクタ:実体作成時にDXライブラリのVECTORを設定する場合、その値で初期化します。Zは切り捨てられます。  例)Vector2DX YYY = VGet(100.f,100.f,0.f);
		Vector2DX(const DxLib::VECTOR& value) noexcept { this->Set(value.x, value.y); }
		// 自身に直接値を設定します
		void Set(float x1, float y1) noexcept {
			this->x = x1;
			this->y = y1;
		}
		// DXライブラリのVECTORに変換して出力します(DXライブラリとの受け渡し用です)
		inline DxLib::VECTOR get(void) const noexcept { return DxLib::VGet(this->x, this->y, 0.f); }				// 変換
		// Vector2DX版のVGetを作成します  例)Vector2DX ZZZ = Vector2DX::vget(0.f, 0.f);
		inline static const Vector2DX vget(float x, float y) noexcept { return Vector2DX(DxLib::VGet(x, y, 0.f)); }
		// プリセットです。左右上下・全部1・全部0 を示すものがあります
		static const Vector2DX right(void) noexcept { return vget(1.f, 0.f); }
		static const Vector2DX left(void) noexcept { return vget(-1.f, 0.f); }
		static const Vector2DX up(void) noexcept { return vget(0.f, 1.f); }
		static const Vector2DX down(void) noexcept { return vget(0.f, -1.f); }

		static const Vector2DX one(void) noexcept { return vget(1.f, 1.f); }
		static const Vector2DX zero(void) noexcept { return vget(0.f, 0.f); }
		// ベクトルの長さの2乗を得る
		inline float sqrMagnitude(void) const noexcept { return this->x * this->x + this->y * this->y; }
		// ベクトルの長さを得る
		inline float magnitude(void) const noexcept {
#if FALSE // std::hypotfを使用するか否か　MSVCでは、内部的な桁あふれを抑制する機能を実装している場合があるため速度重視なら使わない、精度重視なら使う
			return std::hypotf(this->x, this->y);
#else
			float Square = sqrMagnitude();
			return Square >= 0.0000001f ? std::sqrtf(Square) : 0.0f;
#endif
		}
		// ベクトルの長さを1とするベクトルを得る(0の場合、DXライブラリのエラーに倣いx,yともに-1を返します)
		inline Vector2DX normalized(void) const noexcept {
			float Size = magnitude();
			if (Size <= 0.0f) {
				return vget(-1.0f, -1.0f);
			}
			return *this / Size;
		}		// 正規化
		// 自分と引数とが完全に同じかどうかをチェックします(浮動小数点数なので本当に完全に同じかを見ます)
		bool Equals(const Vector2DX& obj) const noexcept { return *this == obj; }

		// 原点を軸とした2ベクトル間の角度をradianで取得  例)float AAA = Vector2DX::Angle(Vector2DX::left(), Vector2DX::up());
		inline static float			Angle(const Vector2DX& A, const Vector2DX& B) noexcept { return std::acos(Dot(A, B) / (A.magnitude() * B.magnitude())); }

		// ベクトルAの長さをLimitの範囲に制限する
		inline static Vector2DX		ClampMagnitude(const Vector2DX& A, float Limit) noexcept {
			float Clamp = A.magnitude();
			if (Clamp < 0.f) { Clamp = 0.f; }
			if (Clamp > Limit) { Clamp = Limit; }
			return A.normalized() * Clamp;
		}

		// ベクトルどうしの距離を計算する
		inline static float			Distance(const Vector2DX& A, const Vector2DX& B) noexcept { return (A - B).magnitude(); }
		// ベクトルどうしの外積を計算する
		inline static float			Cross(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.y - A.y * B.x; }
		// ベクトルどうしの内積を計算する
		inline static float			Dot(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.x + A.y * B.y; }
		// 0~1の範囲をチェックせず線形補完
		static Vector2DX			LerpUnclamped(const Vector2DX& A, const Vector2DX& B, float Per) noexcept {
			return A + (B - A) * Per;
		}
		// 0~1の範囲をチェックし、その範囲で線形補完
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
		// ベクトルどうしのxyで大きい方を選択して取得
		inline static Vector2DX		Max(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x > B.x) ? A.x : B.x, (A.y > B.y) ? A.y : B.y); }
		// ベクトルどうしのxyで小さい方を選択して取得
		inline static Vector2DX		Min(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x < B.x) ? A.x : B.x, (A.y < B.y) ? A.y : B.y); }
		// ベクトルの長さを1とするベクトルを得る
		inline static Vector2DX		Normalize(const Vector2DX& A) noexcept { return A.normalized(); }
		// ベクトルを法線で反射したベクトルを得る
		inline static Vector2DX		Reflect(const Vector2DX& inDirection, const Vector2DX& inNormal) noexcept { return inDirection + inNormal * (Dot(inNormal, inDirection * -1.f)) * 2.f; }
		// ベクトルとベクトルとでのスケーリングを行う
		inline static Vector2DX		Scale(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x * B.x), (A.y * B.y)); }

		// 以下、Unityにはあるが使用頻度が多少低いのでオミットした関数
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

		// 比較オペレーター
		inline bool operator==(const Vector2DX& obj) const noexcept { return (*this - obj).magnitude() <= 0.001f; }
		inline bool operator!=(const Vector2DX& obj) const noexcept { return !(*this == obj); }
		// 加算オペレーター
		inline Vector2DX operator+(const Vector2DX& obj) const noexcept { return vget(this->x + obj.x, this->y + obj.y); }
		inline void operator+=(const Vector2DX& obj) noexcept {
			this->x += obj.x;
			this->y += obj.y;
		}
		// 減算オペレーター
		inline Vector2DX operator-(const Vector2DX& obj) const noexcept { return vget(this->x - obj.x, this->y - obj.y); }
		inline void operator-=(const Vector2DX& obj) noexcept {
			this->x -= obj.x;
			this->y -= obj.y;
		}
		// 乗算オペレーター
		inline Vector2DX operator*(float p1) const noexcept { return vget(this->x * p1, this->y * p1); }
		inline void operator*=(float p1) noexcept {
			this->x *= p1;
			this->y *= p1;
		}
		// 除算オペレーター
		inline Vector2DX operator/(float p1) const noexcept { return *this * (1.f / p1); }
		inline void operator/=(float p1) noexcept { *this *= (1.f / p1); }
	};
	// ---------------------------------------------------------------------------------------------
	// DXLIBの判定演算ラッパー
	// ---------------------------------------------------------------------------------------------
	// 球と三角との判定
	static bool GetHitSphereToTriangle(const Vector2DX& pos, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, const Vector2DX& tri_p3) noexcept {
		return DxLib::HitCheck_Sphere_Triangle(pos.get(), size, tri_p1.get(), tri_p2.get(), tri_p3.get()) == TRUE;
	}
	// 三角とカプセルとの判定
	static bool GetHitCapsuleToTriangle(const Vector2DX& startpos, const Vector2DX& endpos, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, const Vector2DX& tri_p3) noexcept {
		return DxLib::HitCheck_Capsule_Triangle(startpos.get(), endpos.get(), size, tri_p1.get(), tri_p2.get(), tri_p3.get()) == TRUE;
	}
	// 球とカプセルとの判定
	static bool GetHitSphereToCapsule(const Vector2DX& pos, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, float size2) noexcept {
		return DxLib::HitCheck_Sphere_Capsule(pos.get(), size, tri_p1.get(), tri_p2.get(), size2) == TRUE;
	}
	// カプセルとカプセルとの判定
	static bool GetHitCheckToCapsule(const Vector2DX& pos1, const Vector2DX& pos2, float size, const Vector2DX& tri_p1, const Vector2DX& tri_p2, float size2) noexcept {
		return DxLib::HitCheck_Capsule_Capsule(pos1.get(), pos2.get(), size, tri_p1.get(), tri_p2.get(), size2) == TRUE;
	}
	// 直線と直線の一番近い距離
	static float GetMinLenSegmentToSegment(const Vector2DX& startpos, const Vector2DX& endpos, const Vector2DX& tgtstartpos, const Vector2DX& tgtendpos) noexcept {
		return DxLib::Segment_Segment_MinLength(startpos.get(), endpos.get(), tgtstartpos.get(), tgtendpos.get());
	}
	// 直線と点の一番近い点
	static float GetMinLenSegmentToPoint(const Vector2DX& startpos, const Vector2DX& endpos, const Vector2DX& tgt) noexcept {
		return DxLib::Segment_Point_MinLength(startpos.get(), endpos.get(), tgt.get());
	}
	// 平面と点の一番近い点
	static Vector2DX GetMinPosSegmentToPoint(const Vector2DX& startpos, const Vector2DX& endpos, const Vector2DX& tgt) noexcept {
		return DxLib::Plane_Point_MinLength_Position(startpos.get(), endpos.get(), tgt.get());
	}
	// 線分同士の交差判定
	static bool GetSegmenttoSegment(const Vector2DX& SegmentAPos1, const Vector2DX& SegmentAPos2, const Vector2DX& SegmentBPos1, const Vector2DX& SegmentBPos2, SEGMENT_SEGMENT_RESULT* Result) noexcept {
		VECTOR Pos1t = SegmentAPos1.get();
		VECTOR Pos2t = SegmentAPos2.get();
		VECTOR PosAt = SegmentBPos1.get();
		VECTOR PosBt = SegmentBPos2.get();

		DxLib::Segment_Segment_Analyse(&Pos1t, &Pos2t, &PosAt, &PosBt, Result);
		float len = 0.001f;
		return (Result->SegA_SegB_MinDist_Square <= (len * len));
	}
	// 線分と三角との交差判定
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
	// 線分と点との交差判定
	static bool GetSegmenttoPoint(const Vector2DX& SegmentAPos1, const Vector2DX& SegmentAPos2, const Vector2DX& PointPos, SEGMENT_POINT_RESULT* Result) noexcept {
		VECTOR Pos1t = SegmentAPos1.get();
		VECTOR Pos2t = SegmentAPos2.get();
		VECTOR PosAt = PointPos.get();

		DxLib::Segment_Point_Analyse(&Pos1t, &Pos2t, &PosAt, Result);
		float len = 0.001f;
		return (Result->Seg_Point_MinDist_Square <= (len * len));
	}
	// ---------------------------------------------------------------------------------------------
	// 判定演算
	// ---------------------------------------------------------------------------------------------
	// 点と矩形との判定
	static bool HitPointToRectangle(int xp, int yp, int x1, int y1, int x2, int y2) noexcept { return (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2); }
	// 矩形と矩形との判定
	static bool HitRectangleToRectangle(int xp1, int yp1, int xp2, int yp2, int x1, int y1, int x2, int y2) noexcept { return (xp1 < x2 && x1 < xp2) && (yp1 < y2 && y1 < yp2); }
	// 点と四角形との判定
	static bool HitPointToSquare(int xp, int yp, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) noexcept {
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x2 - x1), static_cast<float>(y2 - y1)), Vector2DX::vget(static_cast<float>(xp - x1), static_cast<float>(yp - y1)))) { return false; }
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x3 - x2), static_cast<float>(y3 - y2)), Vector2DX::vget(static_cast<float>(xp - x2), static_cast<float>(yp - y2)))) { return false; }
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x4 - x3), static_cast<float>(y4 - y3)), Vector2DX::vget(static_cast<float>(xp - x3), static_cast<float>(yp - y3)))) { return false; }
		if (0 > Vector2DX::Cross(Vector2DX::vget(static_cast<float>(x1 - x4), static_cast<float>(y1 - y4)), Vector2DX::vget(static_cast<float>(xp - x4), static_cast<float>(yp - y4)))) { return false; }
		return true;
	}

	// 値を特定の範囲に収める
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
	// 角度変換
	// --------------------------------------------------------------------------------------------------
	// 角度からラジアンに
	extern void* enabler;// ダミー変数
	template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type*& = enabler>
	constexpr float deg2rad(T p1) noexcept { return float(p1) * DX_PI_F / 180.f; }
	// ラジアンから角度に
	template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type*& = enabler>
	constexpr float rad2deg(T p1) noexcept { return float(p1) * 180.f / DX_PI_F; }
	// 余弦定理
	constexpr float GetCosFormula(float a, float b, float c) noexcept {
		if (b + c > a && c + a > b && a + b > c) {
			return Clamp((b * b + c * c - a * a) / (2.f * b * c), -1.f, 1.f);
		}
		return 1.f;
	}
	// ベクトルと上から右回りの角度を取得
	static inline float GetRadVec(const Vector2DX& vec1) noexcept { return std::atan2f(vec1.x, vec1.y); }

	//角度差計算(360度回る際の演算などで使用します)
	static inline float GetRadRad2Rad(float NowRadian, float TargetRadian) noexcept {
		Vector2DX Vec; Vec.Set(std::sin(NowRadian), std::cos(NowRadian));
		Vector2DX vec_a; vec_a.Set(std::sin(TargetRadian), std::cos(TargetRadian));
		float cost = Vector2DX::Cross(vec_a, Vec);
		float sint = sqrtf(std::abs(1.f - cost * cost));
		return GetRadVec(Vector2DX::vget(cost, sint));
	}
}
