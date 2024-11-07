#include	"Player.hpp"
#include	"../../sub.hpp"

// シングルトンの実態定義
const DXLIB_Sample::Sceneclass::PlayerManager* SingletonBase<DXLIB_Sample::Sceneclass::PlayerManager>::m_Singleton = nullptr;

namespace DXLIB_Sample {
	namespace Sceneclass {
		void PlayerManager::UpdateDelete(void) noexcept {
			auto* Obj2DParts = Object2DManager::Instance();
			// 特定のプレイヤーを削除
			for (int i = 0; i < GetPlayerNum(); i++) {
				auto& p = GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					if (p->GetChara()->GetIsDelete()) {
						Obj2DParts->DelObj(p->GetChara());
						if (p->GetAI()) {
							p->GetAI()->Dispose();
						}
						p->Dispose();
					}
				}
			}
		}
		void PlayerManager::Dispose(void) noexcept {
			auto* Obj2DParts = Object2DManager::Instance();
			// 全プレイヤーを削除してリストをクリア
			for (auto& p : this->m_Player) {
				if (p->GetChara()) {
					Obj2DParts->DelObj(p->GetChara());
					if (p->GetAI()) {
						p->GetAI()->Dispose();
					}
				}
				p->Dispose();
				p.reset();
			}
			this->m_Player.clear();
		}
	}
}
