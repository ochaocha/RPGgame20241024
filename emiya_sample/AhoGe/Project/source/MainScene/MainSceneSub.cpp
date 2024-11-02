#include	"MainSceneSub.hpp"

#include	"Player/Player.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		void PauseMenuControl::Load(void) noexcept {
			auto* ButtonParts = ButtonControl::Instance();
			ButtonParts->ResetSel();
			ButtonParts->AddStringButton("Retire", 48, true, 1920 - 64, 1080 - 84 - 64 * 2, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Option", 48, true, 1920 - 64, 1080 - 84 - 64 * 1, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Return Game", 48, true, 1920 - 64, 1080 - 84 - 64 * 0, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
		}
		void PauseMenuControl::Set(void) noexcept {
			this->m_IsRetire = false;
		}
		void PauseMenuControl::Update(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* Pad = PadControl::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			auto* SceneParts = SceneControl::Instance();

			if (SceneParts->IsPause()) {
				if (!SceneParts->IsExit() && !SceneParts->IsRestart()) {
					if (!OptionWindowClass::Instance()->IsActive()) {
						ButtonParts->UpdateInput();
						// 選択時の挙動
						if (ButtonParts->GetTriggerButton()) {
							switch (ButtonParts->GetSelect()) {
							case 0:
								this->m_IsRetire = true;
								SceneParts->ChangePause(false);
								break;
							case 1:
								OptionWindowClass::Instance()->SetActive();
								break;
							case 2:
								SceneParts->ChangePause(false);
								break;
							default:
								SceneParts->ChangePause(false);
								break;
							}
							SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_OK))->Play(DX_PLAYTYPE_BACK, TRUE);
						}
						if (Pad->GetPadsInfo(PADS::RELOAD).GetKey().trigger()) {
							SoundParts->Get(SoundType::SE, static_cast<int>(SoundSelectCommon::UI_CANCEL))->Play(DX_PLAYTYPE_BACK, TRUE);
							SceneParts->ChangePause(false);
						}
						// 
						ButtonParts->Update();
					}
				}
			}
			else {
				ButtonParts->ResetSel();
			}
		}
		void PauseMenuControl::Draw(void) const noexcept {
			auto* ButtonParts = ButtonControl::Instance();
			auto* SceneParts = SceneControl::Instance();
			// ポーズ
			if (SceneParts->IsPause() && (!SceneParts->IsExit() && !SceneParts->IsRestart())) {
				ButtonParts->Draw();
			}
		}
		void PauseMenuControl::DisposeLoad(void) noexcept {
			auto* ButtonParts = ButtonControl::Instance();
			ButtonParts->Dispose();
		}

		void MapNameDrawControl::Set(void) noexcept {
			auto* BackGround = BackGroundClassBase::Instance();
			this->m_MapDrawTime = 5.f;
			this->m_MapTextID = BackGround->GetMapTextID();
		}
		void MapNameDrawControl::Update(void) noexcept {
			auto* DrawParts = DXDraw::Instance();

			this->m_MapDrawTime = std::max(this->m_MapDrawTime - DrawParts->GetDeltaTime(), 0.f);
			float Per = 1.f;
			float StartTimer = 0.5f;
			if (this->m_MapDrawTime > 5.f - StartTimer) {
				Per = 1.f - (this->m_MapDrawTime - (5.f - StartTimer)) / StartTimer;
			}
			float EndTimer = 0.5f;
			if (this->m_MapDrawTime < 0.5f) {
				Per = (this->m_MapDrawTime - (0.5f - EndTimer)) / EndTimer;
			}
			this->m_MapDrawPer = std::clamp(Per, 0.f, 1.f);
		}
		void MapNameDrawControl::Draw(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			if (this->m_MapDrawPer > 1.f / 255.f) {
				auto* LocalizeParts = LocalizePool::Instance();
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * this->m_MapDrawPer), 0, 255));
				DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(64), UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
					DrawParts->GetUIY(64), DrawParts->GetUIY(128), White, Black, LocalizeParts->Get(this->m_MapTextID));
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
			}
		}
		// カットシーン制御
		void CutSceneControl::StartCutScene(int ID) noexcept {
			this->m_CutSceneData.clear();
			std::string Path; Path.reserve(24);
			sprintfDx(Path.data(), "data/CutScene/%03d.txt", ID);

			FileStreamDX FileStream(Path.c_str());
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL == "") { continue; }
				auto LEFT = FileStreamDX::getleft(ALL);
				auto RIGHT = FileStreamDX::getright(ALL);

				// int targetID = std::stoi(LEFT.substr(0, 3));
				// イベントタイプ
				if (LEFT.find("Type") != std::string::npos) {
					this->m_CutSceneData.resize(this->m_CutSceneData.size() + 1);
					this->m_CutSceneData.back().m_CGSel = InvalidID;
					for (int i = 0; i < static_cast<int>(CutSceneType::Max); i++) {
						if (RIGHT.find(g_CutSceneStr[i]) != std::string::npos) {
							this->m_CutSceneData.back().m_Type = (CutSceneType)i;
							break;
						}
					}
				}
				// メッセージ
				else if (LEFT.find("Name") != std::string::npos) {
					this->m_CutSceneData.back().m_NameID = std::stoi(RIGHT);
				}
				else if (LEFT.find("Msg") != std::string::npos) {
					this->m_CutSceneData.back().m_MsgID = std::stoi(RIGHT);
				}
				// カメラ移動
				else if (LEFT.find("Point") != std::string::npos) {
					auto X = std::stoi(RIGHT.substr(0, RIGHT.find(',')));
					auto Y = std::stoi(RIGHT.substr(RIGHT.find(',') + 1));
					this->m_CutSceneData.back().m_PointX = X;
					this->m_CutSceneData.back().m_PointY = Y;
				}
				// 待機
				else if (LEFT.find("WaitMilSec") != std::string::npos) {
					this->m_CutSceneData.back().m_WatiMS = std::stoi(RIGHT);
				}
				// CG
				if (LEFT.find("CG") != std::string::npos) {
					this->m_CutSceneData.back().m_CGSel = std::stoi(RIGHT);
				}
			}
			this->m_IsCutScene = true;
			this->m_CutSceneSeek = 0;
		}
		void CutSceneControl::Set(void) noexcept {
			this->m_IsCutScene = false;
			this->m_CutSceneAlpha = 0.f;
			this->m_MsgBoxSeek = 0.f;
			this->m_WaitMS = 0.f;
			this->m_CGFade = 0.f;
		}
		void CutSceneControl::Update(void) noexcept {
			auto* Pad = PadControl::Instance();
			auto* DrawParts = DXDraw::Instance();
			// カットシーン全体
			this->m_CutSceneAlpha = std::clamp(this->m_CutSceneAlpha + (this->m_IsCutScene ? 1.f : -1.f) * DrawParts->GetDeltaTime() / 0.5f, 0.f, 1.f);
			// メッセージシーク
			this->m_MsgBoxAlpha = std::clamp(this->m_MsgBoxAlpha + (this->m_IsMsgBox ? 1.f : -1.f) * DrawParts->GetDeltaTime() / 0.1f, 0.f, 1.f);
			if (this->m_MsgBoxAlpha >= 1.f) {
				this->m_MsgBoxSeek += DrawParts->GetDeltaTime() / 0.1f;
			}
			this->m_CGFade = std::clamp(this->m_CGFade + ((this->m_CGSel != InvalidID) ? 1.f : -1.f) * DrawParts->GetDeltaTime() / 1.f, 0.f, 1.f);
			// カットシーン中のボタン制御
			if (this->m_CutSceneAlpha >= 1.f) {
				if (0 <= this->m_CutSceneSeek && this->m_CutSceneSeek < static_cast<int>(this->m_CutSceneData.size())) {
					this->m_IsMsgBox = false;
					bool IsGoNext = false;
					auto& Data = this->m_CutSceneData.at(static_cast<size_t>(this->m_CutSceneSeek));
					switch (Data.m_Type) {
					case CutSceneType::MsgBox:
						this->m_IsMsgBox = true;
						break;
					case CutSceneType::End:
						this->m_IsCutScene = false;// おわり
						break;
					case CutSceneType::ViewPoint:
						this->m_PointX = Data.m_PointX;
						this->m_PointY = Data.m_PointY;
						IsGoNext = true;
						break;
					case CutSceneType::WaitMilSec:
						IsGoNext = static_cast<int>(this->m_WaitMS * 1000.f) > Data.m_WatiMS;
						this->m_WaitMS += DrawParts->GetDeltaTime();
						break;
					case CutSceneType::CG:
						this->m_CGSel = Data.m_CGSel;
						if (this->m_CGSel != InvalidID) {
							std::string Str = "data/UI/CG";
							Str += std::to_string(this->m_CGSel);
							Str += ".png";
							m_CGGraph.Dispose();
							m_CGGraph.Load(Str);
						}
						IsGoNext = true;
						break;
					case CutSceneType::Max:
					default:
						break;
					}
					if (Pad->GetPadsInfo(PADS::INTERACT).GetKey().trigger() || (IsGoNext)) {
						if (!IsGoNext) {
							auto* SoundParts = SoundPool::Instance();
							SoundParts->Get(SoundType::SE, (int)SoundSelectCommon::UI_OK)->Play(DX_PLAYTYPE_BACK, TRUE);
						}
						for (auto& m : this->m_MsgString) {
							m = "";
						}
						this->m_WaitMS = 0.f;
						this->m_MsgBoxSeek = 0.f;
						this->m_PrevMsgBoxSeek = 0;
						this->m_CutSceneSeek++;
					}
				}
				else {
					this->m_IsCutScene = false;// おわり
				}
			}
			else {
				this->m_PointX = 0;
				this->m_PointY = 0;
			}

			// 文字表示変数系の更新
			if (this->m_MsgBoxAlpha > 1.f / 255.f) {
				int x1 = DrawParts->GetUIY(64);
				int x2 = DrawParts->GetUIY(1920 - 48);

				if (this->m_MsgBoxAlpha >= 1.f) {
					int NameID = 0;
					int MsgID = 0;
					if (0 <= this->m_CutSceneSeek && this->m_CutSceneSeek < static_cast<int>(this->m_CutSceneData.size())) {
						auto& Data = this->m_CutSceneData.at(static_cast<size_t>(this->m_CutSceneSeek));
						NameID = Data.m_NameID;
						MsgID = Data.m_MsgID;
					}
					if (MsgID != 0) {
						auto* LocalizeParts = LocalizePool::Instance();

						int NowC = static_cast<int>(this->m_MsgBoxSeek);
						if (this->m_PrevMsgBoxSeek != NowC) {
							this->m_PrevMsgBoxSeek = NowC;
							std::string NowMsg; NowMsg.reserve(512);
							strncpy2_sDx(NowMsg.data(), 512, LocalizeParts->Get(MsgID), std::min(512, NowC));
							NowMsg = NowMsg.c_str();
							for (auto& m : this->m_MsgString) {
								m = "";
							}
							for (int i = 0; i < 10; i++) {
								if (NowMsg == "") { break; }
								std::string Tmp; Tmp.reserve(512);

								int Limit = ((x2 - x1) - DrawParts->GetUIY(64) * 2);
								int column = Limit / DrawParts->GetUIY(32);// 超えない範囲
								while (true) {
									if (NowC <= column) {
										column = NowC;
										break;
									}
									strncpy2_sDx(Tmp.data(), 512, NowMsg.c_str(), column); Tmp = Tmp.c_str();
									if (UISystem::GetMsgLen(DrawParts->GetUIY(32), Tmp) < Limit) {
										column++;
									}
									else {
										column--;
										break;
									}
								}
								NowC -= column;
								strncpy2_sDx(Tmp.data(), 512, NowMsg.c_str(), column); Tmp = Tmp.c_str();
								this->m_MsgString.at(static_cast<size_t>(i)) = Tmp;
								{
									while (true) {
										if (strchrDx(Tmp.c_str(), '\n')) {
											i++;
											strpncpy2_sDx(Tmp.data(), 512, Tmp.c_str(), strchr2Dx(Tmp.c_str(), '\n') + 1, static_cast<int>(strlen2Dx(Tmp.c_str()))); Tmp = Tmp.c_str();
										}
										else {
											break;
										}
									}
								}
								if (static_cast<int>(strlen2Dx(NowMsg.c_str())) > column) {
									strpncpy2_sDx(NowMsg.data(), 512, NowMsg.c_str(), column + 1, static_cast<int>(strlen2Dx(NowMsg.c_str()))); NowMsg = NowMsg.c_str();
								}
								else {
									NowMsg = "";
								}
							}
						}
					}
				}
			}
		}
		void CutSceneControl::DrawCut(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();

			if (this->m_CGFade > 1.f / 255.f) {
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(128.f * this->m_CGFade), 0, 255));
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, 0, DrawParts->GetUIY(1920), DrawParts->GetUIY(1080), Black, TRUE);
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
			}
			if (this->m_CutSceneAlpha > 1.f / 255.f) {
				auto Color = GetColor(16, 16, 16);
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * this->m_CutSceneAlpha), 0, 255));
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, 0, DrawParts->GetUIY(1920), DrawParts->GetUIY(120), Color, TRUE);
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, 0, DrawParts->GetUIY(1080 - 120), DrawParts->GetUIY(1920), DrawParts->GetUIY(1080), Color, TRUE);
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
			}
			if (this->m_MsgBoxAlpha > 1.f / 255.f) {
				float Per = Lerp(0.5f, 1.f, this->m_MsgBoxAlpha);
				int x1 = DrawParts->GetUIY(64);
				int y1 = DrawParts->GetUIY((1080 - 64) - static_cast<int>((320 - 64) * Per));
				int x2 = DrawParts->GetUIY(1920 - 48);
				int y2 = DrawParts->GetUIY(1080 - 64 - static_cast<int>((320 - 64) * (1.f - Per)));

				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * this->m_MsgBoxAlpha), 0, 255));
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, x1, y1, x2, y2, Gray50, TRUE);
				if (this->m_MsgBoxAlpha >= 1.f) {
					int NameID = 0;
					int MsgID = 0;
					if (0 <= this->m_CutSceneSeek && this->m_CutSceneSeek < static_cast<int>(this->m_CutSceneData.size())) {
						auto& Data = this->m_CutSceneData.at(static_cast<size_t>(this->m_CutSceneSeek));
						NameID = Data.m_NameID;
						MsgID = Data.m_MsgID;
					}
					if (MsgID != 0) {
						auto* LocalizeParts = LocalizePool::Instance();
						DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(18), UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
							x1 + DrawParts->GetUIY(32), y1 + DrawParts->GetUIY(32), White, Black, LocalizeParts->Get(NameID));
						for (auto& m : this->m_MsgString) {
							if (m == "") { continue; }
							int i = static_cast<int>(&m - &this->m_MsgString.front());
							DrawCtrls->SetString(UISystem::DrawLayer::Normal, UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(32), UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
								x1 + DrawParts->GetUIY(64), y1 + DrawParts->GetUIY(64) + DrawParts->GetUIY(32 * i), White, Black, m);
						}
					}
				}
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
			}

			if (this->m_CGFade > 1.f / 255.f) {
				int x1 = DrawParts->GetUIY(960 - 960 / 2);
				int y1 = DrawParts->GetUIY(400 - 540 / 2);
				int x2 = DrawParts->GetUIY(960 + 960 / 2);
				int y2 = DrawParts->GetUIY(400 + 540 / 2);
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * this->m_CGFade), 0, 255));
				DrawCtrls->SetDrawBox(UISystem::DrawLayer::Normal, x1, y1, x2, y2, Gray50, TRUE);
				DrawCtrls->SetDrawExtendGraph(UISystem::DrawLayer::Normal, &m_CGGraph, x1, y1, x2, y2, false);
				DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
			}
		}
};
};
