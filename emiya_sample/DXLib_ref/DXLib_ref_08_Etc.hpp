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

	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// カメラシェイク
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class CameraShake : public SingletonBase<CameraShake> {
	private:
		friend class SingletonBase<CameraShake>;
	private:
		float						m_ShakePower{ 0.f };		//揺れの強さ
		float						m_ShakeTotalTime{ 0.f };	//揺れを起こすトータル時間
		float						m_ShakeTime{ 0.f };			//揺れの経過時間(0秒に向けて減っていきます)
		Vector2DX					m_CamShake1;
		Vector2DX					m_CamShake2;				//揺れを計算するための変数
	public:
		//揺れた結果を取得します
		const auto& GetCamShake(void) const noexcept { return m_CamShake2; }
	public:
		//揺れを設定します。　揺れる時間と揺れの力の2点です
		void			SetCamShake(float time, float power) noexcept {
			this->m_ShakeTotalTime = time;
			this->m_ShakePower = power;
			this->m_ShakeTime = this->m_ShakeTotalTime;
		}
	private:
		// コンストラクタ
		CameraShake(void) noexcept {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		CameraShake(const CameraShake&) = delete;
		CameraShake(CameraShake&& o) = delete;
		CameraShake& operator=(const CameraShake&) = delete;
		CameraShake& operator=(CameraShake&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		void Update(void) noexcept;
	};
}
