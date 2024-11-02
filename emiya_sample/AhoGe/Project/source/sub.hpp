#pragma once
#include	"Header.hpp"
// 
namespace FPS_n2 {
	namespace Sceneclass {
		// 
		enum class EffectType {
			Damage,
			Guard,
			WallHit,
		};
		class Effect2DControl : public SingletonBase<Effect2DControl> {
		private:
			friend class SingletonBase<Effect2DControl>;
		private:
			struct GuardPos {
				Vector2DX					m_Pos{};
				float						m_Per{};
				float						m_PerMax{};
				EffectType					m_EffectType{};
			};
		private:
			std::array<GuardPos, 8>		m_GuardPos;
			int							m_GuardPosNum{};
		private:
			Effect2DControl(void) {}
			Effect2DControl(const Effect2DControl&) = delete;
			Effect2DControl(Effect2DControl&& o) = delete;
			Effect2DControl& operator=(const Effect2DControl&) = delete;
			Effect2DControl& operator=(Effect2DControl&& o) = delete;
		public:
			void			SetEffect(const Vector2DX& Pos2D, EffectType Type, float Per) noexcept {
				m_GuardPos.at(m_GuardPosNum).m_Pos = Pos2D;
				m_GuardPos.at(m_GuardPosNum).m_EffectType = Type;
				m_GuardPos.at(m_GuardPosNum).m_Per = Per;
				m_GuardPos.at(m_GuardPosNum).m_PerMax = Per;
				++m_GuardPosNum %= static_cast<int>(m_GuardPos.size());
			}
		public:
			void			Init() noexcept;
			void			Update() noexcept;
			void			Draw() noexcept;
		};
		//
		class LocalizePool : public SingletonBase<LocalizePool> {
		private:
			friend class SingletonBase<LocalizePool>;
		public:
			typedef int LocalizeID;
		private:
			struct LocalizeStr {
				LocalizeID m_ID{ 0 };// 照会する為のID
				char m_Str[512]{};// 保持しておくテキスト
			};

			std::vector<LocalizeStr> havehandle;
		private:
			// コンストラクタ
			LocalizePool(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			LocalizePool(const LocalizePool&) = delete;
			LocalizePool(LocalizePool&& o) = delete;
			LocalizePool& operator=(const LocalizePool&) = delete;
			LocalizePool& operator=(LocalizePool&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		public:
			// 保持していたテキストの破棄
			void Dispose(void) noexcept {
				// クリアにして
				this->havehandle.clear();
				// 確保していたメモリを破棄
				this->havehandle.shrink_to_fit();
			}
		public:
			// 該当のIDのテキストを取得
			const char* Get(LocalizeID type) noexcept {
				for (auto& s : this->havehandle) {
					if (s.m_ID == type) {
						return s.m_Str;
					}
				}
				return "";
			}
		};
		// プレイヤーやAIの入力を取得しておくクラス
		class InputControl {
		private:
			float			m_yRad{ 0.f };
			unsigned long long	m_Flags{ 0 };
		public:
			InputControl(void) noexcept {}
			InputControl(const InputControl& tgt) noexcept {
				this->m_yRad = tgt.m_yRad;
				this->m_Flags = tgt.m_Flags;
			}
			InputControl(InputControl&& tgt) noexcept {
				this->m_yRad = tgt.m_yRad;
				this->m_Flags = tgt.m_Flags;
			}
			virtual ~InputControl(void) noexcept {}
		public:
			void			ResetAllInput(void) noexcept {
				this->m_yRad = 0.f;
				this->m_Flags = 0;
			}
			void			SetyRad(float yRad) noexcept { this->m_yRad = yRad; }
			void			SetInputPADS(PADS select, bool value) noexcept {
				if (value) { this->m_Flags |= ((unsigned long long)1 << (0 + static_cast<int>(select))); }
				else { this->m_Flags &= ~((unsigned long long)1 << (0 + static_cast<int>(select))); }
			}
		public:
			const auto& GetyRad(void) const noexcept { return this->m_yRad; }
			auto		GetPADSPress(PADS select) const noexcept { return (this->m_Flags & ((unsigned long long)1 << (0 + static_cast<int>(select)))) != 0; }
		};
		// 
		class Cam2DControl : public SingletonBase<Cam2DControl> {
		private:
			friend class SingletonBase<Cam2DControl>;
		private:
			Vector2DX	m_Pos{};
			float m_Height{ 0 };
		private://カメラシェイク系の追加
			float						m_ShakePower{ 0.f };		//揺れの強さ
			float						m_ShakeTotalTime{ 0.f };	//揺れを起こすトータル時間
			float						m_ShakeTime{ 0.f };			//揺れの経過時間(0秒に向けて減っていきます)
			Vector2DX					m_CamShake1;
			Vector2DX					m_CamShake2;				//揺れを計算するための変数
		private:
			Cam2DControl(void) {}
			Cam2DControl(const Cam2DControl&) = delete;
			Cam2DControl(Cam2DControl&& o) = delete;
			Cam2DControl& operator=(const Cam2DControl&) = delete;
			Cam2DControl& operator=(Cam2DControl&& o) = delete;
		public:
			const Vector2DX GetCamPos(void) const noexcept { return this->m_Pos + m_CamShake2; }				//見ている座標＋揺れた結果
			const float GetCamHeight(void) const noexcept { return m_Height; }
		public:
			void			SetCamPos(const Vector2DX& Pos, float Z) noexcept {
				this->m_Pos = Pos;
				this->m_Height = Z;
			}
			void			SetCamAim(const Vector2DX& Pos, float Z) noexcept {
				Easing(&this->m_Pos, Pos, 0.9f, EasingType::OutExpo);
				Easing(&this->m_Height, Z, 0.9f, EasingType::OutExpo);
			}
			//揺れを設定します。　揺れる時間と揺れの力の2点です
			void			SetCamShake(float time, float power) noexcept {
				this->m_ShakeTotalTime = time;
				this->m_ShakePower = power;
				this->m_ShakeTime = this->m_ShakeTotalTime;
			}
			void			Update(void) noexcept;
		public:
			// 空間上から画面上への変換
			static void ConvertTiletoDisp(const Vector2DX& Pos2D, Vector2DX* pRet) noexcept;
			// 画面上のタイルごとのサイズを取得
			static int GetTileToDispSize(float value) noexcept;

			static Vector2DX GetTileToDispSize(Vector2DX value) noexcept {
				Vector2DX Ret;
				Ret.Set(
					(float)GetTileToDispSize(value.x),
					(float)GetTileToDispSize(value.y)
				);
				return Ret;
			}
		};
	};
};
