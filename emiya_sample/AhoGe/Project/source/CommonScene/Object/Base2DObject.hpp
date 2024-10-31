#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		// 今プロジェクト上の追加設定を入れた2Dオブジェクトの基底クラス
		class Base2DObjectKai : public Object2DManager::Base2DObject {
		private:
			Object2DType	m_ObjType{};						// オブジェクトの種類
			bool			m_IsCheckOnlyHitWall{ false };// マップの壁と当たり判定でずり判定をしない(当たった段階で消える)かどうか
		public:
			const auto&		GetObjType(void) const noexcept { return this->m_ObjType; }
		public:
			// 自分が何タイプかを設定
			void			SetObjType(Object2DType value) noexcept { this->m_ObjType = value; }
			void			SetIsCheckOnlyHitWall(bool value) noexcept { this->m_IsCheckOnlyHitWall = value; }
		public:
			Base2DObjectKai() noexcept {}
			virtual ~Base2DObjectKai() noexcept {}
		protected:
			void			UpdateWallHit(void) noexcept;
		protected:
			// ヒットした瞬間に呼ぶ系のもの
			virtual void	Update_OnHitWall(void) noexcept {}
		};

		// キャラが纏うブラーエフェクトの描画処理クラス
		class Blur2DControl {
		private:
			class BlurParts {
				Vector2DX	m_Pos;
				float		m_Time{};
				float		m_TimeMax{};
			public:
				void Set(const Vector2DX& pos, float time) noexcept {
					this->m_Pos = pos;
					this->m_TimeMax = time;
					this->m_Time = this->m_TimeMax;
				}
			public:
				auto IsActive(void) const noexcept { return this->m_Time > 0.f; }
				auto GetPer(void) const noexcept { return this->m_Time / this->m_TimeMax; }
				const auto& GetPos(void) const noexcept { return this->m_Pos; }
			public:
				void Update(void) noexcept;
			};
		private:
			std::array<BlurParts, 60>	m_Blur{};
			int				m_BlurNow{ 0 };
		public:
			const auto& GetBlur(void) const noexcept { return this->m_Blur; }
		public:
			// 自分が何タイプかを設定
			void			AddBlur(float Blur, const Vector2DX& Pos, const Vector2DX& Vec) noexcept;
		public:
			void			Update(void) noexcept {
				for (auto& b : this->m_Blur) {
					b.Update();
				}
			}
		};
	};
};
