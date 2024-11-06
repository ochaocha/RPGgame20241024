#pragma once
#include	"Header.hpp"
// 
namespace DXLIB_Sample {
	namespace Sceneclass {
		// ---------------------------------------------------------------------------------------------
		// 
		// ---------------------------------------------------------------------------------------------
		// 2Dエフェクト
		class Effect2DControl : public SingletonBase<Effect2DControl> {
		private:
			friend class SingletonBase<Effect2DControl>;
		private:
			class GuardPos {
			private:
				Vector2DX					m_Pos{};
				float						m_Per{};
				float						m_PerMax{};
				EffectType					m_EffectType{};
			public:
				void			SetEffect(const Vector2DX& Pos2D, EffectType Type, float Per) noexcept {
					this->m_Pos = Pos2D;
					this->m_EffectType = Type;
					this->m_Per = Per;
					this->m_PerMax = Per;
				}
				const auto IsActive() const noexcept { return this->m_Per > 0.f; }
				const auto GetEffectPer() const noexcept { return this->m_Per / this->m_PerMax; }
				const auto GetEffectRotate() const noexcept { return deg2rad(360.f * this->m_Per); }
				const auto GetEffectRadius() const noexcept { return this->m_PerMax * std::pow(1.f - GetEffectPer(), 0.5f); }
				const auto& GetEffectPos() const noexcept { return this->m_Pos; }
				const auto& GetEffectType() const noexcept { return this->m_EffectType; }
			public:
				void			Initialize() noexcept {
					m_Per = 0.f;
				}
				void			Update() noexcept {
					m_Per = GetMax(m_Per - DXLib_ref::Instance()->GetDeltaTime() / 0.5f, 0.f);
				}
			};
		private:
			std::array<GuardPos, 8>		m_GuardPos;
			int							m_GuardPosNum{};
		private:
			// コンストラクタ
			Effect2DControl(void) {}// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			Effect2DControl(const Effect2DControl&) = delete;
			Effect2DControl(Effect2DControl&& o) = delete;
			Effect2DControl& operator=(const Effect2DControl&) = delete;
			Effect2DControl& operator=(Effect2DControl&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		public:
			//エフェクトを配置(場所、種類、エフェクトの持続時間などのパワー)
			void			SetEffect(const Vector2DX& Pos2D, EffectType Type, float Per) noexcept {
				m_GuardPos.at(m_GuardPosNum).SetEffect(Pos2D, Type, Per);
				++m_GuardPosNum %= static_cast<int>(m_GuardPos.size());
			}
		public:
			void			Initialize() noexcept {
				for (auto& g : m_GuardPos) {
					g.Initialize();
				}
				m_GuardPosNum = 0;
			}
			void			Update() noexcept {
				for (auto& g : m_GuardPos) {
					g.Update();
				}
			}
			void			Draw() noexcept;
		};
		// キャラが纏うブラーエフェクトの描画処理クラス
		class Blur2DControl {
		private:
			class BlurParts {
				Vector2DX	m_Pos;
				float		m_Time{};
				float		m_TimeMax{};
			public:
				auto IsActive(void) const noexcept { return this->m_Time > 0.f; }
				auto GetPer(void) const noexcept { return this->m_Time / this->m_TimeMax; }
				const auto& GetPos(void) const noexcept { return this->m_Pos; }
			public:
				void Set(const Vector2DX& pos, float time) noexcept {
					this->m_Pos = pos;
					this->m_TimeMax = time;
					this->m_Time = this->m_TimeMax;
				}
				void Update(void) noexcept {
					this->m_Time = GetMax(this->m_Time - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
				}
			};
		private:
			std::array<BlurParts, 60>	m_Blur{};
			int				m_BlurNow{ 0 };
		public:
			const auto& GetBlur(void) const noexcept { return this->m_Blur; }
		public:
			// ブラーをかける
			void			AddBlur(float Blur, const Vector2DX& Pos, const Vector2DX& Vec) noexcept {
				int Max = static_cast<int>(GetMax(1.f, 60.f * DXLib_ref::Instance()->GetDeltaTime()));
				Vector2DX Goal = Pos - Vec * DXLib_ref::Instance()->GetDeltaTime();
				for (int i = 0; i < Max; i++) {
					this->m_Blur.at(static_cast<size_t>(this->m_BlurNow)).Set(Lerp(Goal, Pos, (static_cast<float>(i) / static_cast<float>(Max))), Blur);
					++this->m_BlurNow %= static_cast<int>(this->m_Blur.size());
				}
			}
		public:
			void			Update(void) noexcept {
				for (auto& b : this->m_Blur) {
					b.Update();
				}
			}
		};
		// ストーリーのテキストをIDで管理するクラス
		class StoryTextDataBase : public SingletonBase<StoryTextDataBase> {
		private:
			friend class SingletonBase<StoryTextDataBase>;
		public:
			using TextID = int;
			struct TextData {
				TextID		m_ID{ 0 };		// 照会する為のID
				char		m_Str[512]{};	// 保持しておくテキスト
			};
		private:
			std::vector<TextData> havehandle;
		private:
			// コンストラクタ
			StoryTextDataBase(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			StoryTextDataBase(const StoryTextDataBase&) = delete;
			StoryTextDataBase(StoryTextDataBase&& o) = delete;
			StoryTextDataBase& operator=(const StoryTextDataBase&) = delete;
			StoryTextDataBase& operator=(StoryTextDataBase&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		public:
			// 該当のIDのテキストを取得
			const char* Get(TextID type) noexcept {
				for (auto& s : this->havehandle) {
					if (s.m_ID == type) {
						return s.m_Str;
					}
				}
				return "";
			}
		public:
			// 保持していたテキストの破棄
			void Dispose(void) noexcept {
				// クリアにして
				this->havehandle.clear();
				// 確保していたメモリを破棄
				this->havehandle.shrink_to_fit();
			}
		};
		// プレイヤーやAIの入力を取得しておくクラス
		class InputControl {
		private:
			float				m_yRad{ 0.f };	// 真上から見た回転値
			unsigned long long	m_Flags{ 0 };	// ビットフラグでボタンの押す押さないを取得
			// ビットフラグは変数を2進数としたときの各桁のゼロイチをフラグとします boolよりもデータ効率(便宜上の名称)が良いです
		public:
			//コンストラクタ
			InputControl(void) noexcept {}
			//コピーコンストラクタ
			InputControl(const InputControl& tgt) noexcept {
				this->m_yRad = tgt.m_yRad;
				this->m_Flags = tgt.m_Flags;
			}
			//デストラクタ
			~InputControl(void) noexcept {}
		public:
			// 全てのフラグをリセット
			void			ResetAllInput(void) noexcept {
				this->m_yRad = 0.f;
				this->m_Flags = 0;
			}
			// 回転値を設定
			void			SetyRad(float yRad) noexcept { this->m_yRad = yRad; }
			// 特定のボタンのフラグを設定
			void			SetInputPADS(PADS select, bool value) noexcept {
				//trueの場合、左にn回ビットシフトした位置のビットを論理和で立てる
				if (value) { this->m_Flags |= ((unsigned long long)1 << (0 + static_cast<int>(select))); }
				//falseの場合、左にn回ビットシフトした位置のビットの反転値との論理積で折る
				else { this->m_Flags &= ~((unsigned long long)1 << (0 + static_cast<int>(select))); }
			}
		public:
			//回転値を取得
			const auto& GetyRad(void) const noexcept { return this->m_yRad; }
			//特定のボタン入力の推す、押さないを設定
			auto		GetPADSPress(PADS select) const noexcept { return (this->m_Flags & ((unsigned long long)1 << (0 + static_cast<int>(select)))) != 0; }
		};
		// 2Dカメラ演算　画面外の描画が発生するか否か、描画時のオフセットなどの演算を行います
		class Cam2DControl : public SingletonBase<Cam2DControl> {
		private:
			friend class SingletonBase<Cam2DControl>;
		private:
			Vector2DX					m_Pos{};					// カメラの現在の座標
		private:// カメラシェイク系の追加
			float						m_ShakePower{ 0.f };		// 揺れの強さ
			float						m_ShakeTotalTime{ 0.f };	// 揺れを起こすトータル時間
			float						m_ShakeTime{ 0.f };			// 揺れの経過時間(0秒に向けて減っていきます)
			Vector2DX					m_CamShake1;
			Vector2DX					m_CamShake2;				// 揺れを計算するための変数
		private:
			// コンストラクタ
			Cam2DControl(void) {}
			Cam2DControl(const Cam2DControl&) = delete;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			Cam2DControl(Cam2DControl&& o) = delete;
			Cam2DControl& operator=(const Cam2DControl&) = delete;
			Cam2DControl& operator=(Cam2DControl&& o) = delete;
			// デストラクタはシングルトンなので呼ばれません
		public:
			const Vector2DX GetCamPos(void) const noexcept { return this->m_Pos + m_CamShake2; }				// 見ている座標＋揺れた結果
		public:
			void			SetCamPos(const Vector2DX& Pos) noexcept {
				this->m_Pos = Pos;
			}
			// 場所の設定を補完
			void			SetCamAim(const Vector2DX& Pos) noexcept {
				Easing(&this->m_Pos, Pos, 0.9f, EasingType::OutExpo);
			}
			// 揺れを設定します。　揺れる時間と揺れの力の2点です
			void			SetCamShake(float time, float power) noexcept {
				this->m_ShakeTotalTime = time;
				this->m_ShakePower = power;
				this->m_ShakeTime = this->m_ShakeTotalTime;
			}
		public:
			// 空間座標から画面上への変換
			static Vector2DX	ConvertTiletoDisp(const Vector2DX& Pos2D) noexcept {
				auto* DrawParts = WindowSizeControl::Instance();
				auto* Cam2D = Cam2DControl::Instance();
				//空間座標とカメラ座標との座標差分を取得
				auto CamOffset = Pos2D - Cam2D->GetCamPos();
				//画面上の差分に変換後、中央からの移動差分として出力
				return Vector2DX::vget(
					static_cast<float>(DrawParts->GetScreenY(BaseScreenWidth / 2)) + Cam2DControl::GetTileToDispSize(CamOffset.x),
					static_cast<float>(DrawParts->GetScreenY(BaseScreenHeight / 2)) - Cam2DControl::GetTileToDispSize(CamOffset.y)
				);
			}
			// 空間座標系でのサイズから画面上のサイズを取得
			static int			GetTileToDispSize(float value) noexcept {
				auto* DrawParts = WindowSizeControl::Instance();
				return DrawParts->GetScreenY(static_cast<int>(value * Tile_DispSize));
			}
		public:
			void			Update(void) noexcept {
				//揺れパワーが残存している場合
				if (this->m_ShakeTotalTime > 0.f) {
					// この範囲でランダムな地点を取得し、その方向に毎フレームm_CamShakeを動かす
					auto RandRange = this->m_ShakeTime / this->m_ShakeTotalTime * this->m_ShakePower;
					// 2回補完を行うことで自然な揺れを演出
					Easing(&this->m_CamShake1, Vector2DX::vget(GetRandf(RandRange), GetRandf(RandRange)), 0.8f, EasingType::OutExpo);
					Easing(&this->m_CamShake2, this->m_CamShake1, 0.8f, EasingType::OutExpo);
					// 時間経過で弱まるようにするため時間を計測して減らします
					this->m_ShakeTime = GetMax(this->m_ShakeTime - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
				}
			}
		};

		static bool IsOnScreen(const Vector2DX& DispPos, int Radius) {
			auto* DrawParts = WindowSizeControl::Instance();
			// 範囲外
			return HitPointToRectangle(
				static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
				-Radius, -Radius, DrawParts->GetScreenY(BaseScreenWidth) + Radius, DrawParts->GetScreenY(BaseScreenHeight) + Radius);
		}
	}
}
