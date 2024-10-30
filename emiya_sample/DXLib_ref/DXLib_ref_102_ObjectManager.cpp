#include "DXLib_ref_102_ObjectManager.hpp"


namespace DXLibRef {
	// シングルトンの実態定義
	const Object2DManager* SingletonBase<Object2DManager>::m_Singleton = nullptr;
	// 作成されたオブジェクトをリストに追加
	void			Object2DManager::AddObject(const std::shared_ptr<Base2DObject>& NewObj) noexcept {
		// オブジェクトの初期化+UniqueIDの設定
		NewObj->Init(this->m_LastUniqueID);
		this->m_LastUniqueID++;
		this->m_Object.emplace_back(NewObj);
	}
	// UniqueIDで検索をかけてオブジェクトを取得(ない場合はnullptr)
	const std::shared_ptr<Object2DManager::Base2DObject> Object2DManager::GetObj(UniqueID ID) noexcept {
		for (auto& o : this->m_Object) {
			if (o->GetUniqueID() == ID) {
				return o;
			}
		}
		return nullptr;
	}
	// UniqueIDで検索をかけてオブジェクトがあるか確認
	bool Object2DManager::HasObj(UniqueID ID) noexcept {
		for (auto& o : this->m_Object) {
			if (o->GetUniqueID() == ID) {
				return true;
			}
		}
		return false;
	}
	// 該当のオブジェクトを削除
	void			Object2DManager::DelObj(const std::shared_ptr<Base2DObject>& ptr) noexcept {
		for (auto& o : this->m_Object) {
			if (o == ptr) {
				// 削除時の処理を行って
				o->Dispose();
				// リストから削除
				this->m_Object.erase(this->m_Object.begin() + (&o - &this->m_Object.front()));// 順番の維持のためpop_backではなくerase
				return;
			}
		}
	}
	// 共通のオブジェクト更新
	void			Object2DManager::Update(void) noexcept {
		// 共通の更新
		for (auto& o : this->m_Object) {
			if (!o->GetIsDelete()) {
				o->Update();
			}
		}
		// オブジェクトの排除チェック
		for (int i = 0, Max = static_cast<int>(this->m_Object.size()); i < Max; i++) {
			auto& o = this->m_Object.at(static_cast<size_t>(i));
			// 削除フラグが立っていたら
			if (o->GetIsDelete()) {
				// 削除時の処理を行って
				o->Dispose();
				// リストから削除
				this->m_Object.erase(this->m_Object.begin() + i);// 順番の維持のためpop_backではなくerase
				// 自分がいた場所に次のオブジェクトが来ているので、それをスルーしないようにループ位置と総数を減らしておく
				i--;
				Max--;
			}
		}
		// オブジェクトどうしの衝突判定
		for (auto& o : this->m_Object) {
			// オブジェクトどうしの判定を行わない場合はスルー
			if (!o->GetIsHitOtherObject()) { continue; }
			for (auto& ot : this->m_Object) {
				if (ot == o) { continue; }
				// 自分が当たったら押し出し(質量差を換算)
				SEGMENT_SEGMENT_RESULT Result;
				GetSegmenttoSegment(o->GetPrevPos(), o->GetPos(), ot->GetPrevPos(), ot->GetPos(), &Result);// 線分の当たり判定を計算し
				// 結果が半径の合計よりも小さい場合
				float Distance = (std::sqrt(Result.SegA_SegB_MinDist_Square)) - (o->GetSize() / 2.f + ot->GetSize() / 2.f);
				if (Distance < 0.f) {
					// Distanceが0となるように双方を押す力を計算する
					{
						// それぞれの一番近い地点の平均を中心として
						Vector2DX pos;
						pos += Result.SegA_MinDist_Pos;
						pos += Result.SegB_MinDist_Pos;
						pos /= 2.f;
						// 相手を押し出すベクトルを計算し
						Vector2DX vec = (ot->GetPos() - pos).normalized() * Distance;
						// 双方に力をかける(重いほうがその場にとどまるように計算)
						ot->SetPos(ot->GetPos() - vec * (o->GetMass() / (o->GetMass() + ot->GetMass())));
						o->SetPos(o->GetPos() + vec * (ot->GetMass() / (o->GetMass() + ot->GetMass())));
					}
					// 当たったものとして通知
					o->SetHitUniqueID(ot->GetUniqueID());
					ot->SetHitUniqueID(o->GetUniqueID());
				}
			}
		}
	}
	// オブジェクト全体を影描写する
	void			Object2DManager::DrawShadow(void) noexcept {
		for (auto& o : this->m_Object) {
			o->DrawShadow();
		}
	}
	// オブジェクト全体を描写する
	void			Object2DManager::Draw(void) noexcept {
		for (auto& o : this->m_Object) {
			o->Draw();
		}
	}
	// すべてのオブジェクトを削除する
	void			Object2DManager::DeleteAll(void) noexcept {
		for (auto& o : this->m_Object) {
			if (o) {
				o->Dispose();
			}
		}
		this->m_Object.clear();
		this->m_LastUniqueID = 0;// 一旦ユニークIDもリセット
	}
};