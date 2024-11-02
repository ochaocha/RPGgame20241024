#pragma once

#include	"../../Header.hpp"
#include	"../../sub.hpp"
#include	"Bullet.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// --------------------------------------------------------------------------------------------------
		//メタルギア風　特殊オブジェクト(独自のAIを持つ)
		// --------------------------------------------------------------------------------------------------
		class MetalObject : public Object2DManager::Base2DObject {
		private:
			PlayerID					m_PlayerID{ 0 };		//プレイヤーID(発射時のIDかわり)

			float						m_Rad{ 0.f };			//胴体の回転角度
			float						m_RadL{ 0.f };			//左足の回転角度
			Vector2DX					m_PosL{};				//左足の座標
			float						m_RadR{ 0.f };			//右足の回転角度
			Vector2DX					m_PosR{};				//右足の座標

			float						m_FootTimer{ 0.f };		//脚の回転サイクル用タイマー
			bool						m_FootUp{ false };		//脚を上げた/下げたフラグ

			Vector2DX					m_BasePos{};			//AIの行動範囲の基準になる座標
			Vector2DX					m_InputVec{};			//AIの入力ベクトル

			float						m_ShotCoolTime{ 0.f };	//発射からのクールダウンタイマー
			std::array<int, 8>			m_MissileID{};			//誘導オブジェクトの保存リスト(8個以上は保存しません)
			int							m_MissileIDNum{};		//誘導オブジェクトIDの指定先

			bool						m_CanMove{ false };		//稼働するかのフラグ

			HitPoint					m_HitPoint{ 0 };		//現在の体力
			const HitPoint				m_MaxHitPoint{ 1000 };	//最大体力
		public:
			//コンストラクタ
			MetalObject(void) noexcept;
			MetalObject(const MetalObject&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			MetalObject(MetalObject&& o) = delete;
			MetalObject& operator=(const MetalObject&) = delete;
			MetalObject& operator=(MetalObject&& o) = delete;
			//デストラクタ 継承オブジェクトである為デストラクタにvirtualを指定
			virtual ~MetalObject(void) noexcept {}
		public:
			void		SetPlayerID(PlayerID value) noexcept { this->m_PlayerID = value; }
			void		SetCanMove(void) noexcept { m_CanMove = true; }//これが呼ばれる限りは稼働します

			const auto& GetPlayerID(void) const noexcept { return this->m_PlayerID; }
			const auto& GetHitPoint(void) const noexcept { return this->m_HitPoint; }
			const auto& GetHitPointMax(void) const noexcept { return m_MaxHitPoint; }
		private:
			//ミサイルとして誘導を行うIDを登録
			void		SetMissileID(int value) noexcept {
				m_MissileID.at(m_MissileIDNum) = value;
				++m_MissileIDNum %= static_cast<int>(m_MissileID.size());
			}
			//弾を発射
			std::shared_ptr<BulletObject>	AddBullet(PlayerID shootID, const Vector2DX& Pos, float Radian, float Offset, float Speed, float Size) noexcept {
				auto* Obj2DParts = Object2DManager::Instance();
				const auto& Obj = std::make_shared<BulletObject>();
				Obj2DParts->AddObject(Obj);
				Obj->SetShootPlayer(shootID);
				Vector2DX Vec = GetVecByRad(DX_PI_F - Radian);
				Obj->SetPosition(Pos + Vec * Offset);
				Obj->SetVec(Vec * Speed);
				Obj->SetSize(Size);
				return Obj;
			}
		protected:
			void Update_OnHitObject(void) noexcept override;
		protected:
			void Init_Sub(void) noexcept override;
			void Update_Sub(void) noexcept override;
			void DrawShadow_Sub(void) noexcept override;
			void Draw_Sub(void) noexcept override;
			void Dispose_Sub(void) noexcept override {}
		};
	}
}
