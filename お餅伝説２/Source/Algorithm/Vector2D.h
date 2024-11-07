#pragma once
#include "DxLib.h"
#include <cmath>

// ---------------------------------------------------------------------------------------------
// 2次元ベクトルの演算クラス
// ---------------------------------------------------------------------------------------------
class Vector2DX
{
public:
	float x{ 0 };
	float y{ 0 };
public:
	//初期化コンストラクタ:実体作成時に何も引数を設定しない場合、0で初期化します  例)Vector2DX XXX;
	Vector2DX(void) noexcept : x(0), y(0) {}
	//初期化コンストラクタ:実体作成時にDXライブラリのVECTORを設定する場合、その値で初期化します。Zは切り捨てられます。  例)Vector2DX YYY = VGet(100.f,100.f,0.f);
	Vector2DX(const DxLib::VECTOR& value) noexcept { this->Set(value.x, value.y); }
	// 自身に直接値を設定します
	void Set(float x1, float y1) noexcept {
		this->x = x1;
		this->y = y1;
	}
	//DXライブラリのVECTORに変換して出力します(DXライブラリとの受け渡し用です)
	inline DxLib::VECTOR get(void) const noexcept { return DxLib::VGet(this->x, this->y, 0.f); }				// 変換
	//Vector2DX版のVGetを作成します  例)Vector2DX ZZZ = Vector2DX::vget(0.f, 0.f);
	inline static const Vector2DX vget(float x, float y) noexcept { return Vector2DX(DxLib::VGet(x, y, 0.f)); }
	// プリセットです。左右上下・全部1・全部0 を示すものがあります
	static const Vector2DX right(void) noexcept { return vget(1.f, 0.f); }
	static const Vector2DX left(void) noexcept { return vget(-1.f, 0.f); }
	static const Vector2DX up(void) noexcept { return vget(0.f, 1.f); }
	static const Vector2DX down(void)noexcept { return vget(0.f, 1.f); }

	static const Vector2DX one(void) noexcept { return vget(1.f, 1.f); }
	static const Vector2DX zero(void) noexcept { return vget(0.f, 0.f); }
	// ベクトルの長さの2乗を得る
	inline float sqrMagnitude(void) const noexcept { return this->x * this->x + this->y * this->y; }
	// ベクトルの長さを得る
	inline float magnitude(void) const noexcept {
#if FALSE //std::hypotfを使用するか否か　MSVCでは、内部的な桁あふれを抑制する機能を実装している場合があるため速度重視なら使わない、精度重視なら使う
		return std::hypotf(this->x, this->y);
#else
		float Square = sqrMagnitude();
		return Square >= 0.0000001f ? std::sqrtf(Square) : 0.0f;
#endif
	}
	//ベクトルの長さを1とするベクトルを得る(0の場合、DXライブラリのエラーに倣いx,yともに-1を返します)
	inline Vector2DX normalized(void) const noexcept {
		float Size = magnitude();
		if (Size <= 0.0f) {
			return vget(-1.0f, -1.0f);
		}
		return *this / Size;
	}		// 正規化
	// 自分と引数とが完全に同じかどうかをチェックします(浮動小数点数なので本当に完全に同じかを見ます)
	bool Equals(const Vector2DX& obj) const noexcept { return *this == obj; }

	//原点を軸とした2ベクトル間の角度をradianで取得  例)float AAA = Vector2DX::Angle(Vector2DX::left(), Vector2DX::up());
	inline static float			Angle(const Vector2DX& A, const Vector2DX& B) noexcept { return std::acos(Dot(A, B) / (A.magnitude() * B.magnitude())); }

	//ベクトルAの長さをLimitの範囲に制限する
	inline static Vector2DX		ClampMagnitude(const Vector2DX& A, float Limit) noexcept {
		float Clamp = A.magnitude();
		if (Clamp < 0.f) { Clamp = 0.f; }
		if (Clamp > Limit) { Clamp = Limit; }
		return A.normalized() * Clamp;
	}

	//ベクトルどうしの距離を計算する
	inline static float			Distance(const Vector2DX& A, const Vector2DX& B) noexcept { return (A - B).magnitude(); }
	//ベクトルどうしの外積を計算する
	inline static float			Cross(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.y - A.y * B.x; }
	//ベクトルどうしの内積を計算する
	inline static float			Dot(const Vector2DX& A, const Vector2DX& B) noexcept { return A.x * B.x + A.y * B.y; }
	//0~1の範囲をチェックせず線形補完
	static Vector2DX			LerpUnclamped(const Vector2DX& A, const Vector2DX& B, float Per) noexcept {
		return A + (B - A) * Per;
	}
	//0~1の範囲をチェックし、その範囲で線形補完
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
	//ベクトルどうしのxyで大きい方を選択して取得
	inline static Vector2DX		Max(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x > B.x) ? A.x : B.x, (A.y > B.y) ? A.y : B.y); }
	//ベクトルどうしのxyで小さい方を選択して取得
	inline static Vector2DX		Min(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x < B.x) ? A.x : B.x, (A.y < B.y) ? A.y : B.y); }
	//ベクトルの長さを1とするベクトルを得る
	inline static Vector2DX		Normalize(const Vector2DX& A) noexcept { return A.normalized(); }
	//ベクトルを法線で反射したベクトルを得る
	inline static Vector2DX		Reflect(const Vector2DX& inDirection, const Vector2DX& inNormal) noexcept { return inDirection + inNormal * (Dot(inNormal, inDirection * -1.f)) * 2.f; }
	//ベクトルとベクトルとでのスケーリングを行う
	inline static Vector2DX		Scale(const Vector2DX& A, const Vector2DX& B) noexcept { return vget((A.x * B.x), (A.y * B.y)); }

	//以下、Unityにはあるが使用頻度が多少低いのでオミットした関数
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
