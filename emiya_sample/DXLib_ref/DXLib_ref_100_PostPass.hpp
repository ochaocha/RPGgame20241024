#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// ポストプロセス処理(メインの処理を行った後、その結果を加工する)を行います
	// --------------------------------------------------------------------------------------------------
	// 各ポストプロセスの基底クラス
	class PostPassBase {
	protected:
		bool m_PrevActive{ false };
	public:
		PostPassBase(void) noexcept {}
		virtual ~PostPassBase(void) noexcept {}
	protected:
		virtual void Load_Sub(void) noexcept {}
		virtual void Dispose_Sub(void) noexcept {}
		virtual bool IsActive_Sub(void) noexcept { return true; }
		virtual void SetEffect_Sub(GraphHandle*, GraphHandle*) noexcept {}
	public:
		bool IsActive(void) noexcept { return IsActive_Sub(); }
		void Init(void) noexcept {
			m_PrevActive = IsActive_Sub();
			if (m_PrevActive) {
				Load_Sub();
			}
		}
		void UpdateActive(void) noexcept {
			bool active = IsActive_Sub();
			if (m_PrevActive != active) {
				m_PrevActive = active;
				if (active) {
					Load_Sub();
				}
				else {
					Dispose_Sub();
				}
			}
		}
		void Dispose(void) noexcept {
			if (IsActive_Sub()) {
				Dispose_Sub();
			}
		}
		void SetEffect(GraphHandle* TargetGraph, GraphHandle* ColorGraph) noexcept {
			if (IsActive_Sub()) {
				SetEffect_Sub(TargetGraph, ColorGraph);
			}
		}
	};
	// 上記ポストプロセスを取りまとめるクラス
	class PostPassEffect : public SingletonBase<PostPassEffect> {
	private:
		friend class SingletonBase<PostPassEffect>;
	private:
		GraphHandle BufferScreen;	// 描画スクリーン
		GraphHandle ColorScreen;	// そのまま透過なしにしたスクリーン
		// 行うポストプロセスのリスト
		std::vector<std::unique_ptr<PostPassBase>> m_PostPass;
		// ガンマ値などの色調補正を行うためのパラメーター
		int				InColorPerMin = 20;
		int				InColorPerMax = 255;
		float			InColorGamma = 1.1f;
		// 色収差の強さを調整するためのパラメーター
		float			m_AberrationPower{ 1.f };
	public:// 調整できるパラメーター群へのアクセス
		const auto&		GetAberrationPower(void) const noexcept { return m_AberrationPower; }
		void			SetAberrationPower(float value) noexcept { m_AberrationPower = value; }
	public:// 処理対象の画面
		auto&			GetBufferScreen(void) noexcept { return BufferScreen; }
	public:
		// ガンマ値などの色調補正を行うためのパラメーターを指定する
		// 使用する輝度の最低値、最高値、ガンマ値
		void SetLevelFilter(int inMin, int inMax, float gamma) noexcept {
			InColorPerMin = Clamp(inMin, 0, 255);
			InColorPerMax = Clamp(inMax, 0, 255);
			InColorGamma = GetMax(1.f, gamma);
		}
	private:
		// コンストラクタ
		PostPassEffect(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		PostPassEffect(const PostPassEffect&) = delete;
		PostPassEffect(PostPassEffect&& o) = delete;
		PostPassEffect& operator=(const PostPassEffect&) = delete;
		PostPassEffect& operator=(PostPassEffect&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		void Update(void) noexcept;
		void DrawPostProcess(void) noexcept;
		void Dispose(void) noexcept;
	};
};
