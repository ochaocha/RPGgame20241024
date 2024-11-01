#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
#if DEBUG
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// 処理時間を計測することができるクラス 処理落ちなどの際にどこで時間がかかるのかなどを確認するのに使えます
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class DebugClass : public SingletonBase<DebugClass> {
	private:
		friend class SingletonBase<DebugClass>;
	private:
		static const int	PointMax{ 12 };// 配置できる最大ポイント数(SetPointを概ねこれだけ置けます)
		static const int	PointFrame{ 120 };// 記憶する最大フレーム数
	private:
		LONGLONG																m_StartTime{ 0 };// 計測を開始した時間を保持します
		std::array<std::string, PointMax + 1>									m_Str;// 各ポイントに指定した文字を保持します
		std::vector<std::array<float, PointMax + 1>>							m_Point;// 各ポイントの文字がPointFrameぶん保持されます
		size_t																	m_PointSel{ 0 };// SetPointを何番目に置いたかが保持されます
		switchs																	m_Switch;// F1キーで表示非表示を決めることができます
		bool																	m_IsActive{ true };
	private:
		// コンストラクタ
		DebugClass(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		DebugClass(const DebugClass&) = delete;
		DebugClass(DebugClass&& o) = delete;
		DebugClass& operator=(const DebugClass&) = delete;
		DebugClass& operator=(DebugClass&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		// デバッグの計測開始ポイントに置くもの
		void SetStartPoint(void) noexcept;
		// 任意のポイントに置くと、前に書いたSetPointとの時間差を測れるようになります
		void SetPoint(const char* DebugMes) noexcept;
		// 計測を終了させるポイントです
		void SetEndPoint(void) noexcept;
		// 結果を表示します
		void DebugWindow(int xpos, int ypos) noexcept;
	};
#endif // DEBUG
};