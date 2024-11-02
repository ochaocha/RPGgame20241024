#include	"sub.hpp"

#include	"MainScene/Player/Player.hpp"

// シングルトンの実態定義
const FPS_n2::Sceneclass::Effect2DControl* SingletonBase<FPS_n2::Sceneclass::Effect2DControl>::m_Singleton = nullptr;
const FPS_n2::Sceneclass::LocalizePool* SingletonBase<FPS_n2::Sceneclass::LocalizePool>::m_Singleton = nullptr;
const FPS_n2::Sceneclass::Cam2DControl* SingletonBase<FPS_n2::Sceneclass::Cam2DControl>::m_Singleton = nullptr;

namespace FPS_n2 {
	namespace Sceneclass {
		// コンストラクタでテキストデータを生成
		LocalizePool::LocalizePool(void) noexcept {
			// あらかじめ一定量目盛を確保しておく
			this->havehandle.reserve(256);
			// 1行ずつファイルを解析し、ローカライズIDに合致したテキストを保存する
			FileStreamDX FileStream("data/Localize/Jpn.txt");
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL == "") {
					continue;
				}
				// イコール(=)の左をID、右をテキストとして取得
				LocalizeID ID = (LocalizeID)(std::stoi(FileStreamDX::getleft(ALL)));
				std::string Text = FileStreamDX::getright(ALL);
				// もしすでにIDが登録済であれば、改行とみなして既存テキストの末尾に追加する
				bool IsHit = false;
				for (auto& h : this->havehandle) {
					if (h.m_ID == ID) {
						std::string Before = h.m_Str;
						sprintfDx(h.m_Str, (Before + "\n" + Text).c_str());// 改行して同じテキストとする
						IsHit = true;
						break;
					}
				}
				// 登録されていないのであれば新規にハンドルを割り当てる
				if (!IsHit) {
					this->havehandle.resize(this->havehandle.size() + 1);
					this->havehandle.back().m_ID = ID;
					sprintfDx(this->havehandle.back().m_Str, Text.c_str());
				}
			}
		}

		// 
		void Cam2DControl::Update(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			if (this->m_ShakeTotalTime > 0.f) {
				//この範囲でランダムな地点を取得し、その方向に毎フレームm_CamShakeを動かす
				auto RandRange = this->m_ShakeTime / this->m_ShakeTotalTime * this->m_ShakePower;
				//2回補完を行うことで自然な揺れを演出
				Easing(&this->m_CamShake1, Vector2DX::vget(GetRandf(RandRange), GetRandf(RandRange)), 0.8f, EasingType::OutExpo);
				Easing(&this->m_CamShake2, this->m_CamShake1, 0.8f, EasingType::OutExpo);
				//時間経過で弱まるようにするため時間を計測して減らします
				this->m_ShakeTime = GetMax(this->m_ShakeTime - DrawParts->GetDeltaTime(), 0.f);
			}
		}
		// 
		void Cam2DControl::ConvertTiletoDisp(const Vector2DX& Pos2D, Vector2DX* pRet) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto CamPos = Cam2DControl::GetTileToDispSize(Pos2D - Cam2DControl::Instance()->GetCamPos());

			pRet->x = static_cast<float>(DrawParts->GetScreenY(1920 / 2)) + CamPos.x;
			pRet->y = static_cast<float>(DrawParts->GetScreenY(1080 / 2)) - CamPos.y;
		}
		// 画面上のタイルごとのサイズを取得(タイルvalue個ぶん)
		int Cam2DControl::GetTileToDispSize(float value) noexcept {
			auto* DrawParts = DXDraw::Instance();
			return DrawParts->GetScreenY(static_cast<int>(value * Cam2DControl::Instance()->GetCamHeight()));
		}

		//
		void Effect2DControl::Init() noexcept {
			for (auto& g : m_GuardPos) {
				g.m_Per = 0.f;
			}
			m_GuardPosNum = 0;
		}
		void Effect2DControl::Update() noexcept {
			auto* DrawParts = DXDraw::Instance();
			for (auto& g : m_GuardPos) {
				g.m_Per = std::max(g.m_Per - DrawParts->GetDeltaTime() / 0.5f, 0.f);
			}
		}
		void Effect2DControl::Draw() noexcept {
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(5.f));
			Vector2DX DispPos;
			for (auto& g : m_GuardPos) {
				if (g.m_Per <= 0.f) { continue; }
				Cam2DControl::ConvertTiletoDisp(g.m_Pos, &DispPos);

				float Length = Radius * g.m_PerMax * std::pow(1.f - (g.m_Per / g.m_PerMax), 0.5f);
				switch (g.m_EffectType) {
				case EffectType::Damage:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * (g.m_Per / g.m_PerMax)), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Length, 8, GetColor(192, 0, 0), TRUE, 1.f, static_cast<double>(deg2rad(360.f * g.m_Per)));
					break;
				case EffectType::Guard:
					SetDrawBlendMode(DX_BLENDMODE_ADD, std::clamp(static_cast<int>(128.f * (g.m_Per / g.m_PerMax)), 0, 255));
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Length), GetColor(150, 150, 255));
					break;
				case EffectType::WallHit:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * (g.m_Per / g.m_PerMax)), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Length, 5, GetColor(255, 192, 0), TRUE, 1.f, static_cast<double>(deg2rad(360.f * g.m_Per)));
					break;
				default:
					break;
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	};
};
