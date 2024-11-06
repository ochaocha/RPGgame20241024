#include	"Metal.hpp"

#include	"../BackGround/BackGround.hpp"
#include	"../Player/Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		MetalObject::MetalObject(void) noexcept {
			this->m_InputVec.Set(0.f, 0.f);
			SetObjType(static_cast<int>(Object2DType::Metal));
		}
		// 
		void MetalObject::OnHitObject_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* Effect2DParts = Effect2DControl::Instance();
			const auto& Obj = Obj2DParts->GetObj(GetHitUniqueID());
			if (Obj) {
				// 弾以外が当たった時は以下は通さない
				if (Obj->GetObjType() != static_cast<int>(Object2DType::Rocket) && Obj->GetObjType() != static_cast<int>(Object2DType::Bullet)) { return; }
				//ヒットエフェクトを出す
				Effect2DParts->SetEffect(Obj->GetPosition(), EffectType::Guard, 1.f);
				// Effect2DParts->Set(Obj->GetPosition(), EffectType::Damage, 0.25f);
				if (Obj->GetObjType() == static_cast<int>(Object2DType::Rocket)) {
					//ロケットランチャーにヒットした場合はHPを0にする
					this->m_HitPoint = 0;
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Bomb))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				else {
					//それ以外は体力を1減らすだけ
					this->m_HitPoint--;
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Guard))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				//体力が0になったら
				if (this->m_HitPoint == 0) {
					//爆発代わりのエフェクトを発生させ
					Effect2DParts->SetEffect(GetPosition(), EffectType::WallHit, 2.f);
					//自壊フラグを立てる
					SetDelete();
				}
			}
		}
		// 
		void MetalObject::Initialize_Sub(void) noexcept {
			SetIsHitOtherObject(true);
			SetMass(100000.f);
			m_FootTimer = 0.f;
			for (auto& g : m_MissileID) {
				g = 0;
			}
			m_MissileIDNum = 0;
			this->m_HitPoint = m_MaxHitPoint;
		}
		void MetalObject::Update_Sub(void) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* Cam2D = Cam2DControl::Instance();

			if (GetIsFirstLoop()) {
				m_BasePos = GetPosition();//行動の基準となる座標を取得(ここからあまり離れないようなルーチン)
				m_PosL = GetPosition();
				m_PosR = GetPosition();
			}
			//稼働フラグがオフなら動かない
			if (!m_CanMove) {
				SetVec(Vector2DX::zero());
				return;
			}
			//フラグをオフ　後のフレームでSetCanMoveが呼ばれないと機能停止となる
			m_CanMove = false;

			//ターゲットキャラを取得
			auto& TargetCharacter = PlayerMngr->GetPlayer((PlayerID)0)->GetChara();
			if (!TargetCharacter) {
				//ターゲットがいない場合停止
				SetVec(Vector2DX::zero());
				return;
			}
			//ターゲットとの距離を取得
			float Length = (GetPosition() - TargetCharacter->GetPosition()).magnitude();

			//ターゲットに向けて回転目標方向を決定
			float TargetRad = DX_PI_F - GetRadVec(TargetCharacter->GetPosition() - GetPosition());

			//車体を目標方向に回転
			this->m_Rad += GetRadRad2Rad(this->m_Rad, TargetRad) * 0.6f * DXLib_ref::Instance()->GetDeltaTime();
			{
				// 移動速度
				float SpeedLimit = 0.5f;

				//間合いに応じてWASD入力を仮想で行う
				bool WKey = false;
				bool SKey = false;
				bool AKey = false;
				bool DKey = false;
				//一定の範囲に入ろうとするように操作
				{
					Vector2DX LastFindPos = TargetCharacter->GetPosition();
					float Len = 0.1f;
					Vector2DX Vec = LastFindPos - GetPosition();
					if (Length > 17.5f) {
						WKey = (Vec.y > Len / 3.f);
						SKey = (Vec.y < -Len / 3.f);
						AKey = (Vec.x < -Len / 3.f);
						DKey = (Vec.x > Len / 3.f);
					}
					if (Length < 12.5f) {
						WKey = (Vec.y < -Len / 3.f);
						SKey = (Vec.y > Len / 3.f);
						AKey = (Vec.x > Len / 3.f);
						DKey = (Vec.x < -Len / 3.f);
					}
				}
				// あまり中央から外れると見栄えが悪いので一定値意向だと中央に戻る
				Vector2DX Vec = m_BasePos - GetPosition();
				if (Vec.y < -20) {
					WKey = false;
					SKey = true;
				}
				if (Vec.y > 20) {
					WKey = true;
					SKey = false;
				}
				if (Vec.x < -20) {
					AKey = true;
					DKey = false;
				}
				if (Vec.x > 20) {
					AKey = false;
					DKey = true;
				}
				// 上記仮想WASDを基に移動演算
				if (DKey && !AKey) {
					this->m_InputVec.x = std::clamp(this->m_InputVec.x + 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else if (AKey && !DKey) {
					this->m_InputVec.x = std::clamp(this->m_InputVec.x - 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else {
					Easing(&this->m_InputVec.x, 0.f, 0.95f, EasingType::OutExpo);
				}
				if (WKey && !SKey) {
					this->m_InputVec.y = std::clamp(this->m_InputVec.y + 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else if (SKey && !WKey) {
					this->m_InputVec.y = std::clamp(this->m_InputVec.y - 4.f * DXLib_ref::Instance()->GetDeltaTime(), -SpeedLimit, SpeedLimit);
				}
				else {
					Easing(&this->m_InputVec.y, 0.f, 0.95f, EasingType::OutExpo);
				}
				float Speed = GetMax(std::abs(this->m_InputVec.x), std::abs(this->m_InputVec.y));
				// 壁判定を行うと壁付近で重厚感が出ないので壁とは判定を行わない
				SetVec(this->m_InputVec.normalized() * Speed * 5.f);
			}
			{
				m_FootTimer += DXLib_ref::Instance()->GetDeltaTime();
				if (m_FootTimer > 2.f) {
					m_FootTimer -= 2.f;//2秒で1周期
				}

				//0.6~0.8秒目では左足を上げる
				if (0.6f < m_FootTimer && m_FootTimer < 0.8f) {
					//TargetRadに左足の座標、回転を寄せる
					this->m_RadL += GetRadRad2Rad(this->m_RadL, TargetRad) * 10.f * DXLib_ref::Instance()->GetDeltaTime();
					Easing(&m_PosR, GetPosition() + GetVec() * 1.f, 0.5f, EasingType::OutExpo);
					//脚を上げた判定を立てる
					m_FootUp = true;
				}
				else {
					//脚をおろしたとして振動を追加
					if (m_FootUp) {
						m_FootUp = false;
						if (Length < 30.f) {
							Cam2D->SetCamShake(0.1f, 20.f * 5.f / GetMax(Length, 1.f));
						}
					}
				}
				//1.6~1.8秒目では右足を上げる
				if (1.6f < m_FootTimer && m_FootTimer < 1.8f) {
					//TargetRadに右足の座標、回転を寄せる
					this->m_RadR += GetRadRad2Rad(this->m_RadR, TargetRad) * 10.f * DXLib_ref::Instance()->GetDeltaTime();
					Easing(&m_PosL, GetPosition() + GetVec() * 1.f, 0.5f, EasingType::OutExpo);
					//脚を上げた判定を立てる
					m_FootUp = true;
				}
				else {
					//脚をおろしたとして振動を追加
					if (m_FootUp) {
						m_FootUp = false;
						if (Length < 30.f) {
							Cam2D->SetCamShake(0.1f, 20.f * 5.f / GetMax(Length, 1.f));
						}
					}
				}
			}
			// 弾
			if (this->m_ShotCoolTime == 0.f) {
				//10~20mの範囲内でランダムな値が概ね1/10(10%)なら
				if (10.f < Length && Length < 20.f && (GetRand(10) < 1)) {
					//弾を発射して
					AddBullet(GetPlayerID(), GetPosition(), DX_PI_F - TargetRad, GetSize() / 2 + 0.75f, 3.f, 0.5f);
					//0.5秒のクールダウンを設定
					this->m_ShotCoolTime = 0.5f;
				}
				//20m以降の範囲でランダムな値が概ね1/10(10%)なら
				else if (20.f < Length && (GetRand(10) < 1)) {
					{
						//弾を発射して
						const auto Obj = AddBullet(GetPlayerID(), GetPosition(), DX_PI_F - TargetRad + DX_PI_F / 6, GetSize() / 2.f + 0.75f, 1.5f * 5.f, 0.5f);
						//誘導対象として設定
						SetMissileID(Obj->GetUniqueID());
					}
					{
						//弾を発射して
						const auto Obj = AddBullet(GetPlayerID(), GetPosition(), DX_PI_F - TargetRad - DX_PI_F / 6, GetSize() / 2.f + 0.75f, 1.5f * 5.f, 0.5f);
						//誘導対象として設定
						SetMissileID(Obj->GetUniqueID());
					}
					//5秒のクールダウンを設定
					this->m_ShotCoolTime = 5.f;
				}
			}
			else {
				//弾を放った後のクールダウン
				this->m_ShotCoolTime = GetMax(this->m_ShotCoolTime - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
			}
			//ミサイルとして登録されたオブジェクトをターゲットに向けて少しずつ誘導します
			for (auto& g : m_MissileID) {
				const auto& Obj = Obj2DParts->GetObj(g);
				if (Obj) {
					Vector2DX Vec; Vec = (TargetCharacter->GetPosition() - Obj->GetPosition()).normalized();
					Vector2DX Vec_R; Vec_R = Obj->GetVec().normalized();
					if (Vector2DX::Dot(Vec, Vec_R) > 0.f) {//ターゲットに向いている場合のみ誘導する
						Easing(&Vec_R, Vec, 0.975f, EasingType::OutExpo);
						Obj->SetVec(Vec_R.normalized() * Obj->GetVec().magnitude());
					}
				}
			}
			// 壁衝突演算抜きの移動処理
			SetPosition(this->GetPosition() + this->GetVec() * DXLib_ref::Instance()->GetDeltaTime());
		}
		// 影
		void MetalObject::DrawShadow_Sub(void) noexcept {
			auto* BackGround = BackGroundClassBase::Instance();
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(GetSize() / 2.f));
			Vector2DX DispPosA;
			Vector2DX DispPosB;
			// 右脚
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosL + GetVectorRotated(Vector2DX::vget(-1.2f, -1.2f) * (GetSize() / 2.f), m_RadR) + BackGround->GetAmbientLightVec() * 0.25f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.8f, -0.2f) * (GetSize() / 2.f), m_RadR) + BackGround->GetAmbientLightVec() * 0.25f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
			// 右脚
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosR + GetVectorRotated(Vector2DX::vget(1.2f, -1.2f) * (GetSize() / 2.f), m_RadL) + BackGround->GetAmbientLightVec() * 0.25f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(0.8f, -0.2f) * (GetSize() / 2.f), m_RadL) + BackGround->GetAmbientLightVec() * 0.25f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
			// 身体
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + BackGround->GetAmbientLightVec() * 0.25f);
			DrawCircle(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(Radius), Black);
			// 右手
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, -2.5f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, 0.5f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.15f) * 2);
			// 左手
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, 0.35f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, -0.35f) * (GetSize() / 2.f), this->m_Rad) + BackGround->GetAmbientLightVec() * 0.35f);
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), Black, static_cast<int>(Radius * 0.25f) * 2);
		}
		// 描画
		void MetalObject::Draw_Sub(void) noexcept {
			int Radius = Cam2DControl::GetTileToDispSize(GetSize() / 2.f);//基準となるサイズ
			Vector2DX DispPosA;
			Vector2DX DispPosB;
			// 右脚
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosL + GetVectorRotated(Vector2DX::vget(-1.2f, -1.2f) * (GetSize() / 2.f), m_RadL));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.8f, -0.2f) * (GetSize() / 2.f), m_RadL));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(128, 0, 0), Radius / 2);
			// 左脚
			DispPosB = Cam2DControl::ConvertTiletoDisp(m_PosR + GetVectorRotated(Vector2DX::vget(1.2f, -1.2f) * (GetSize() / 2.f), m_RadR));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(0.8f, -0.2f) * (GetSize() / 2.f), m_RadR));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(128, 0, 0), Radius / 2);
			// 身体
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition());
			DrawCircle(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), Radius, GetColor(192, 0, 0));
			// 右手
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, -2.5f) * (GetSize() / 2.f), this->m_Rad));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(-0.9f, 0.5f) * (GetSize() / 2.f), this->m_Rad));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(255, 0, 0), Radius * 3 / 10);
			// 左手
			DispPosB = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, 0.35f) * (GetSize() / 2.f), this->m_Rad));
			DispPosA = Cam2DControl::ConvertTiletoDisp(GetPosition() + GetVectorRotated(Vector2DX::vget(1.2f, -0.35f) * (GetSize() / 2.f), this->m_Rad));
			DrawLine(static_cast<int>(DispPosA.x), static_cast<int>(DispPosA.y), static_cast<int>(DispPosB.x), static_cast<int>(DispPosB.y), GetColor(255, 0, 0), Radius / 2);
		}
	}
}
