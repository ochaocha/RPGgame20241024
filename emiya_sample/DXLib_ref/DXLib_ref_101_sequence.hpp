#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// 1シーンぶんの基底クラス
	// --------------------------------------------------------------------------------------------------
	class TEMPSCENE {
	private:
		bool			m_IsFirstLoop{ true };			// 初回チェック
		bool			m_IsLoading{ false };			// 既に自分のシーンがLoadを通ったかどうか(DisposeLoadを通るまでは再Loadさせないようにする)
		std::array<std::shared_ptr<TEMPSCENE>, 10> Next_ptr{ nullptr };// 次に進むシーンのリスト
		size_t			m_Next_Select{ 0 };				// 次に進むシーンのリストのうちどれに進むか
	public:
		// 次に進むシーンをリストに追加追加
		void		SetNextSceneList(int index, const std::shared_ptr<TEMPSCENE>& Next_scenes_ptr_t) noexcept { Next_ptr.at(static_cast<std::size_t>(index)) = Next_scenes_ptr_t; }
		// 次に進むシーンを示す
		auto&		Get_Next(void) noexcept { return Next_ptr.at(this->m_Next_Select); }
		// リストのうちどれに進むか決定
		void		SetNextSelect(size_t value) noexcept { this->m_Next_Select = value; }
	public:// ゲッター
		// Update_Subが初めて呼び出されたかどうか
		const auto& GetIsFirstLoop(void) const noexcept { return m_IsFirstLoop; }
	public:
		// コンストラクタ
		TEMPSCENE(void) noexcept {}
		TEMPSCENE(const TEMPSCENE&) = delete;
		TEMPSCENE(TEMPSCENE&& o) = delete;
		TEMPSCENE& operator=(const TEMPSCENE&) = delete;
		TEMPSCENE& operator=(TEMPSCENE&& o) = delete;
		// デストラクタ
		virtual ~TEMPSCENE(void) noexcept {}
	public:// メイン更新
		void Load(void) noexcept {
			if (!m_IsLoading) {
				m_IsLoading = true;
				Load_Sub();
			}
		}
		void Set(void) noexcept {
			this->m_Next_Select = 0;
			Set_Sub();
			m_IsFirstLoop = true;
		}
		bool Update(void) noexcept {
			auto ans = Update_Sub();
			m_IsFirstLoop = false;
			return ans;
		}
		// 
		void MainDraw(void) const noexcept { MainDraw_Sub(); }
		void DrawUI_Base(void) const noexcept { DrawUI_Base_Sub(); }
		void DrawUI_In(void) const noexcept { DrawUI_In_Sub(); }
		// 描画
		void Dispose(void) noexcept { Dispose_Sub(); }
		void Dispose_Load(void) noexcept {
			if (m_IsLoading) {
				m_IsLoading = false;
				Dispose_Load_Sub();
			}
		}
	protected:// 継承物 継承先のシーンではこれらをoverrideさせる
		virtual void Load_Sub(void) noexcept {}
		virtual void Set_Sub(void) noexcept {}
		virtual bool Update_Sub(void) noexcept { return true; }

		virtual void MainDraw_Sub(void) const noexcept {}
		virtual void DrawUI_Base_Sub(void) const noexcept {}
		virtual void DrawUI_In_Sub(void) const noexcept {}

		virtual void Dispose_Sub(void) noexcept {}
		virtual void Dispose_Load_Sub(void) noexcept {}
	};
	// --------------------------------------------------------------------------------------------------
	// シーンを取りまとめるクラス
	// --------------------------------------------------------------------------------------------------
	class SceneControl : public SingletonBase<SceneControl> {
	private:
		friend class SingletonBase<SceneControl>;
	private:
		std::vector<std::shared_ptr<TEMPSCENE>> m_ScenesPtr{};	// 全シーンのリスト
		std::shared_ptr<TEMPSCENE>	m_NowScenesPtr;				// 今行っているシーン
	private:
		// コンストラクタ
		SceneControl(void) noexcept {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		SceneControl(const SceneControl&) = delete;
		SceneControl(SceneControl&& o) = delete;
		SceneControl& operator=(const SceneControl&) = delete;
		SceneControl& operator=(SceneControl&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		// 今実行しているシーンを取得
		const auto& GetNowScene(void) const noexcept { return this->m_NowScenesPtr; }
	public:
		// シーンをリストに追加
		void	AddList(const std::shared_ptr<TEMPSCENE>& ptr) noexcept {
			this->m_ScenesPtr.emplace_back(ptr);
			// ついでに初回の場合はそのシーンから再生することとする
			if (this->m_ScenesPtr.size() == 1) {
				this->m_NowScenesPtr = this->m_ScenesPtr.back();
			}
		}
	public:
		// 次のシーンへ転換する
		void	NextScene(void) noexcept {
			GetNowScene()->Dispose();							// 今のシーンからの解放
			if (GetNowScene() != GetNowScene()->Get_Next()) {	// 今のシーンと次のシーンとが別のシーンなら
				GetNowScene()->Dispose_Load();					// ロードしていたデータを破棄
			}
			this->m_NowScenesPtr = GetNowScene()->Get_Next();	// 次のシーンへ遷移
		}
		void	Dispose(void) noexcept {
			for (auto& s : this->m_ScenesPtr) {
				s->Dispose();
			}
		}
	};
};
