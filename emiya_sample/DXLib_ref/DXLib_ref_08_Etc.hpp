#pragma once
#include "DXLib_ref.h"

/*------------------------------------------------------------------------------------------------------------------------------------------*/
// いろいろパーツ化したかったものが雑多に含まれています
/*------------------------------------------------------------------------------------------------------------------------------------------*/
namespace DXLibRef {
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// 補完
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// イージング
	enum class EasingType {
		OutExpo,
	};
	// 滑らかな補完を発生させるための積分演算
	extern float GetEasingRatio(EasingType EasingType, float ratio) noexcept;
	// 線形補完
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
	// 滑らかな補完
	template <class T>
	static void Easing(T* A, const T& B, float ratio, EasingType EasingType) noexcept { *A = Lerp(*A, B, GetEasingRatio(EasingType, ratio)); }
	//定積分で1Fあたりの速度を得る
	inline float EasingMove(float* NowSpeed, float TargetSpeed, float ratio) noexcept;
	//Vector2DX版overload
	inline Vector2DX EasingMove(Vector2DX* NowSpeed, Vector2DX TargetSpeed, float ratio) {
		Vector2DX answer;
		answer.x = EasingMove(&NowSpeed->x, TargetSpeed.x, ratio);
		answer.y = EasingMove(&NowSpeed->y, TargetSpeed.y, ratio);
		return answer;
	}
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLIBラッパー																																*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// +-m_argの範囲でのランダム値を取得
	static float GetRandf(float m_arg) noexcept { return -m_arg + static_cast<float>(GetRand(static_cast<int>(m_arg * 2.f * 10000.f))) / 10000.f; }

	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLIBのファイル読み込み補助クラス																											*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class FileStreamDX {
		int mdata = InvalidID;
	public:
		//コンストラクタ
		FileStreamDX() noexcept {}
		//宣言時にファイルオープン版
		FileStreamDX(const char* FilePath) noexcept { Open(FilePath); }
		//デストラクタ
		~FileStreamDX() noexcept { Close(); }
	public:
		//ファイルを開き、探索ポイントを始点に移動
		void Open(const char* FilePath) noexcept { mdata = DxLib::FileRead_open(FilePath, FALSE); }
		// 1行そのまま取得し、次の行に探索ポイントを移る
		std::string SeekLineAndGetStr() const noexcept {
			const int charLength = 512;
			char mstr[charLength] = "";
			DxLib::FileRead_gets(mstr, charLength, mdata);
			return std::string(mstr);
		}
		// 探索ポイントが終端(EOF)で終わる
		bool ComeEof() const noexcept { return DxLib::FileRead_eof(mdata) != 0; }
		//　閉じる
		void Close() noexcept {
			if (mdata != InvalidID) {
				DxLib::FileRead_close(mdata);
				mdata = InvalidID;
			}
		}
	public:
		// 文字列から=より右の値取得
		static std::string getleft(std::string tempname) noexcept { return tempname.substr(0, tempname.find('=')); }
		// 文字列から=より右の値取得
		static std::string getright(std::string tempname) noexcept { return tempname.substr(tempname.find('=') + 1); }
	};
}
