#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//
		// --------------------------------------------------------------------------------------------------
		class BulletObject : public Object2DManager::Base2DObject {
		private:
			PlayerID					m_ShotPlayerID{ 0 };
			float						m_Rad{ 0.f };
			Blur2DControl				m_Blur;
		public:
			//コンストラクタ
			BulletObject(void) noexcept;
			BulletObject(const BulletObject&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			BulletObject(BulletObject&& o) = delete;
			BulletObject& operator=(const BulletObject&) = delete;
			BulletObject& operator=(BulletObject&& o) = delete;
			//デストラクタ 継承オブジェクトである為デストラクタにvirtualを指定
			virtual ~BulletObject(void) noexcept {}
		public:
			void			SetShootPlayer(PlayerID value) noexcept { this->m_ShotPlayerID = value; }
		public:
			void OnHitObject_Sub(void) noexcept override;
		public:
			void Initialize_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override;
		};
	}
}
