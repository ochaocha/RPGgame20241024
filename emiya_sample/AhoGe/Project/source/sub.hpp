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
			void			Set(const Vector2DX& Pos2D, EffectType Type,float Per) noexcept {
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
		// 
		class Cam2DControl : public SingletonBase<Cam2DControl> {
		private:
			friend class SingletonBase<Cam2DControl>;
		private:
			Vector2DX	m_Pos{};
			float m_Height{ 0 };
		private:
			Cam2DControl(void) {}
			Cam2DControl(const Cam2DControl&) = delete;
			Cam2DControl(Cam2DControl&& o) = delete;
			Cam2DControl& operator=(const Cam2DControl&) = delete;
			Cam2DControl& operator=(Cam2DControl&& o) = delete;
		public:
			const Vector2DX GetCamPos(void) const noexcept;
			const float GetCamHeight(void) const noexcept { return m_Height; }
		public:
			void			SetCamPos(const Vector2DX& Pos) noexcept {
				this->m_Pos = Pos;
			}
			void			SetCamAim(const Vector2DX& Pos) noexcept {
				Easing(&this->m_Pos, Pos, 0.9f, EasingType::OutExpo);
			}
			void			SetCamRangePos(float Z) noexcept {
				this->m_Height = Z;
			}
			void			SetCamRangeAim(float Z) noexcept {
				Easing(&this->m_Height, Z, 0.9f, EasingType::OutExpo);
			}
		};
		// 空間上から画面上への変換
		extern inline void Convert2DtoDisp(const Vector2DX& Pos2D, Vector2DX* pRet) noexcept;
		// 空間上から画面上への変換
		extern inline bool Is2DPositionInDisp(const Vector2DX& Pos2D, int Radius) noexcept;
		// ベクトルのxy間の角度を取得
		extern inline float GetRadVec2Vec(const Vector2DX& vec1, const Vector2DX& vec2) noexcept;
		// 空間上のタイルごとのサイズを取得
		extern inline float Get2DSize(float value) noexcept;
		// 空間上のサイズからタイル枚数を取得
		extern inline float Get2DSizetoTile(float value) noexcept;
		// 画面上のタイルごとのサイズを取得
		extern inline int GetDispSize(float value) noexcept;
	};
};
