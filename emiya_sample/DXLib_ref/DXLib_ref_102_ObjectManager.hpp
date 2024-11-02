#pragma once
#include "DXLib_ref.h"

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// キャラクターなどの2Dオブジェクトを一元管理するクラス
	// --------------------------------------------------------------------------------------------------
	class Object2DManager : public SingletonBase<Object2DManager> {
	private:
		friend class SingletonBase<Object2DManager>;
	public:
		//UniqueIDであると明示するために基本型に名前を付けて管理
		using UniqueID = int;
		// オブジェクトの基底クラス
		class Base2DObject {
		private:
			UniqueID		m_UniqueID{ InvalidID };			// オブジェクトマネージャーにつけてもらうID オブジェクトマネージャーでDeleteAllされない限り各オブジェクトでID被りを起こさない
			bool			m_IsDelete{ false };				// オブジェクトがマネージャーに削除フラグを伝えるための変数
			bool			m_IsFirstLoop{ true };				// 初めてUpdate_Subを通る場合TRUEになる変数(継承先でのみ参照できるものとする)
			Vector2DX		m_PrevPos{};						// SetPositionで設定する前の座標
			Vector2DX		m_Pos{};							// 座標
			Vector2DX		m_Vec{};							// 移動ベクトル
			float			m_Size{ 1.f };						// サイズ
			float			m_Mass{ 1.f };						// 重量
			UniqueID		m_HitUniqueID{ InvalidID };			// オブジェクトに当たった場合に何のオブジェクトに当たったかを確認する為の変数
			bool			m_IsHitOtherObject{ false };		// 他のオブジェクトと当たるかどうか
			int				m_ObjType{};						// オブジェクトの種類
		protected:
			// 初めてUpdate_Subを通るかどうか
			const auto& GetIsFirstLoop(void) const noexcept { return this->m_IsFirstLoop; }
		public:
			// 自分のUniqueIDを取得
			const auto& GetUniqueID(void) const noexcept { return this->m_UniqueID; }
			// 自分をマネージャーに破棄してもらうフラグ
			const auto& GetIsDelete(void) const noexcept { return this->m_IsDelete; }
			// 
			const auto& GetPosition(void) const noexcept { return this->m_Pos; }
			const auto& GetPrevPos(void) const noexcept { return this->m_PrevPos; }
			const auto& GetVec(void) const noexcept { return this->m_Vec; }
			const auto& GetSize(void) const noexcept { return this->m_Size; }
			const auto& GetMass(void) const noexcept { return this->m_Mass; }
			const auto& GetHitUniqueID(void) const noexcept { return this->m_HitUniqueID; }
			const auto& GetIsHitOtherObject(void) const noexcept { return this->m_IsHitOtherObject; }
			const auto& GetObjType(void) const noexcept { return this->m_ObjType; }
		public:
			// 自分をマネージャーに破棄してもらうフラグを立てる
			void			SetDelete(void) noexcept { this->m_IsDelete = true; }
			void			SetPosition(const Vector2DX& value) noexcept {
				this->m_Pos = value;
				this->m_PrevPos = this->m_Pos;
			}
			void			SetVec(const Vector2DX& value) noexcept { this->m_Vec = value; }
			void			SetSize(float value) noexcept { this->m_Size = value; }
			void			SetMass(float value) noexcept { this->m_Mass = value; }
			void			SetHitUniqueID(UniqueID value) noexcept { this->m_HitUniqueID = value; }
			void			SetIsHitOtherObject(bool value) noexcept { this->m_IsHitOtherObject = value; }
			void			SetObjType(int value) noexcept { this->m_ObjType = value; }			// 自分が何タイプかを設定
		public:
			// コンストラクタ
			Base2DObject() noexcept {}
			// デストラクタ
			virtual ~Base2DObject() noexcept {}
		public:// マネージャーでまとめて行う処理
			// 初期化
			void			Init(UniqueID value) noexcept {
				this->m_UniqueID = value;
				Init_Sub();
				m_IsFirstLoop = true;
			}
			// 更新
			void			Update(void) noexcept {
				Update_Sub();
				// 何かに当たった
				if (this->m_HitUniqueID != InvalidID) {
					Update_OnHitObject();
					this->m_HitUniqueID = InvalidID;
				}
				m_IsFirstLoop = false;
			}
			// 影描画
			void			DrawShadow(void) noexcept { DrawShadow_Sub(); }
			// 描画
			void			Draw(void) noexcept { Draw_Sub(); }
			// オブジェクト削除
			void			Dispose(void) noexcept { Dispose_Sub(); }
		protected:// 任意でoverrideして各オブジェクト特有の処理を追加します
			virtual void	Init_Sub(void) noexcept {}			// 初期化時に通る
			virtual void	Update_Sub(void) noexcept {}		// 更新の際に通る
			virtual void	Update_OnHitObject(void) noexcept {}// 他のオブジェクトにヒットした瞬間に呼ぶ系のもの
			virtual void	DrawShadow_Sub(void) noexcept {}	// 影描画の際に通る
			virtual void	Draw_Sub(void) noexcept {}			// 描画の際に通る
			virtual void	Dispose_Sub(void) noexcept {}		// オブジェクトが削除される際に通る
		};
	private:
		// オブジェクトのsharedPtrをまとめたリスト
		std::vector<std::shared_ptr<Base2DObject>>		m_Object;
		// 次生成する際にオブジェクトに振り分けるユニークID(振り分けたら+1して別IDにします)
		UniqueID										m_LastUniqueID{ 0 };
	private:
		// コンストラクタ
		Object2DManager(void) noexcept {
			// リストのメモリ確保のみ先に行っておく(この段階では実際にリストには登録しない)
			this->m_Object.reserve(256);
		}
		Object2DManager(const Object2DManager&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		Object2DManager(Object2DManager&& o) = delete;
		Object2DManager& operator=(const Object2DManager&) = delete;
		Object2DManager& operator=(Object2DManager&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		// 作成されたオブジェクトをリストに追加
		void			AddObject(const std::shared_ptr<Base2DObject>& NewObj) noexcept;
		// UniqueIDで検索をかけてオブジェクトを取得(ない場合はnullptr)
		const std::shared_ptr<Base2DObject>	GetObj(UniqueID ID) noexcept;
		// UniqueIDで検索をかけてオブジェクトがあるか確認
		bool			HasObj(UniqueID ID) noexcept;
		// 外部でオブジェクトに対して処理を行う場合のためにリストを取得
		std::vector<std::shared_ptr<Base2DObject>>& GetObjList() noexcept { return m_Object; }
		// 該当のオブジェクトを削除
		void			DelObj(const std::shared_ptr<Base2DObject>& ptr) noexcept;
	public:
		// 共通のオブジェクト更新
		void			Update(void) noexcept;
		// オブジェクト全体を影描写する
		void			DrawShadow(void) noexcept;
		// オブジェクト全体を描写する
		void			Draw(void) noexcept;
		// すべてのオブジェクトを削除する
		void			DeleteAll(void) noexcept;
	};
};