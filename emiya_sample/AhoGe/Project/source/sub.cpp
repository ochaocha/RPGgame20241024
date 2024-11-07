#include	"sub.hpp"

#include	"MainScene/Player/Player.hpp"

// シングルトンの実態定義
const DXLIB_Sample::Sceneclass::Effect2DControl* SingletonBase<DXLIB_Sample::Sceneclass::Effect2DControl>::m_Singleton = nullptr;
const DXLIB_Sample::Sceneclass::StoryTextDataBase* SingletonBase<DXLIB_Sample::Sceneclass::StoryTextDataBase>::m_Singleton = nullptr;
const DXLIB_Sample::Sceneclass::Cam2DControl* SingletonBase<DXLIB_Sample::Sceneclass::Cam2DControl>::m_Singleton = nullptr;

namespace DXLIB_Sample {
	namespace Sceneclass {
		//
		void Effect2DControl::Draw() noexcept {
			//半径5を基準として画面上でのサイズを取得
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(5.f));
			for (auto& g : m_GuardPos) {
				if (!g.IsActive()) { continue; }
				//タイル座標から表示上の座標に変換
				Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(g.GetEffectPos());
				//座標を基に描画
				switch (g.GetEffectType()) {
				case EffectType::Damage:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * g.GetEffectPer()), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Radius * g.GetEffectRadius(), 8, GetColor(192, 0, 0), TRUE, 1.f, static_cast<double>(g.GetEffectRotate()));
					break;
				case EffectType::Guard:
					SetDrawBlendMode(DX_BLENDMODE_ADD, std::clamp(static_cast<int>(128.f * g.GetEffectPer()), 0, 255));
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius * g.GetEffectRadius()), GetColor(150, 150, 255));
					break;
				case EffectType::WallHit:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * g.GetEffectPer()), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Radius * g.GetEffectRadius(), 5, GetColor(255, 192, 0), TRUE, 1.f, static_cast<double>(g.GetEffectRotate()));
					break;
				default:
					break;
				}
			}
			//上記でブレンドモードの設定を行ったのでリセット
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		//
		StoryTextDataBase::StoryTextDataBase(void) noexcept {
			// テキストデータを生成
			this->havehandle.reserve(256);					// あらかじめ一定量目盛を確保しておく
			FileStreamDX FileStream("data/StoryText.txt");	// 1行ずつファイルを解析し、ローカライズIDに合致したテキストを保存する
			while (true) {
				if (FileStream.ComeEof()) { break; }		// 文字送りが終わったらループを抜ける
				auto ALL = FileStream.SeekLineAndGetStr();	// 1行読みだして文字送り
				if (ALL == "") { continue; }				// 空白行はスルー
				// イコール(=)の左をID、右をテキストとして取得
				TextID ID = (TextID)(std::stoi(FileStreamDX::getleft(ALL)));
				std::string Text = FileStreamDX::getright(ALL);
				// もしすでにIDが登録済であれば、改行とみなして既存テキストの末尾に改行コードと一緒に追加する
				bool IsHit = false;
				for (auto& h : this->havehandle) {
					if (h.m_ID == ID) {
						std::string Before = h.m_Str;			// 改行して同じテキストとなるように組み立てる
						Before += "\n";
						Before += Text;
						sprintfDx(h.m_Str, Before.c_str());		//文字列を代入
						IsHit = true;							//登録されたとしてフラグを立てて抜ける
						break;
					}
				}
				// 登録されていないのであれば新規にハンドルを割り当てる
				if (!IsHit) {
					this->havehandle.emplace_back();
					this->havehandle.back().m_ID = ID;							//IDを保持
					sprintfDx(this->havehandle.back().m_Str, Text.c_str());		//文字列を代入
				}
			}
			//FileStreamのデストラクタでテキストデータをクローズ
		}
	}
}
