#include	"MainSceneSub.hpp"

#include	"Player/Player.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		void PauseMenuControl::LoadPause(void) noexcept {
			auto* ButtonParts = ButtonControl::Instance();
			ButtonParts->ResetSel();
			ButtonParts->AddStringButton("Retire", 48, true, 1920 - 64, 1080 - 84 - 64 * 2, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Option", 48, true, 1920 - 64, 1080 - 84 - 64 * 1, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
			ButtonParts->AddStringButton("Return Game", 48, true, 1920 - 64, 1080 - 84 - 64 * 0, UISystem::FontXCenter::RIGHT, UISystem::FontYCenter::BOTTOM);
		}
		void PauseMenuControl::SetPause(void) noexcept {
			this->m_IsRetire = false;
		}
		void PauseMenuControl::UpdatePause(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* Pad = PadControl::Instance();
			auto* ButtonParts = ButtonControl::Instance();
			auto* SceneParts = SceneControl::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();

			if (SceneParts->IsPause()) {
				if (!SceneParts->IsExit() && !SceneParts->IsRestart()) {
					Pad->SetMouseMoveEnable(false);
					KeyGuideParts->ChangeGuide(
						[]() {
							auto* KeyGuideParts = UISystem::KeyGuide::Instance();
							auto* Pad = PadControl::Instance();
							KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "決定");
							KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::RELOAD).GetAssign(), Pad->GetControlType()), "戻る");
							KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
							KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
							KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "選択");
						});
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
		void PauseMenuControl::DrawPause(void) const noexcept {
			auto* ButtonParts = ButtonControl::Instance();
			auto* SceneParts = SceneControl::Instance();
			// ポーズ
			if (SceneParts->IsPause() && (!SceneParts->IsExit() && !SceneParts->IsRestart())) {
				ButtonParts->Draw();
			}
		}
		void PauseMenuControl::DisposePause(void) noexcept {
			auto* ButtonParts = ButtonControl::Instance();
			ButtonParts->Dispose();
		}
		// UI
		void InGameUIControl::DrawCharaUI_Back(PlayerID value) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto& p = PlayerMngr->GetPlayer(value);
			if (p->GetChara()) {
				float Radius = 10.f;
				auto* DrawParts = DXDraw::Instance();
				int R = Cam2DControl::GetTileToDispSize(Radius);
				Vector2DX DispPos;
				Cam2DControl::ConvertTiletoDisp(p->GetChara()->GetPosition(), &DispPos);
				// 範囲外
				if (!HitPointToRectangle(
					static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
					-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
					return;
				}
				double Deg = (double)p->GetChara()->GetViewRad() / (DX_PI * 2.0) * 100.0;
				double Watch;
				if (value == 0) {
					SetDrawBright(0, 0, 216);
					Watch = 15.0 / 360.0 * 100.0;
				}
				else {
					if (p->GetAI()->IsAlert()) {
						SetDrawBright(216, 0, 0);// 
					}
					else if (p->GetAI()->IsCaution()) {
						SetDrawBright(216, 216, 0);// 
					}
					else {
						SetDrawBright(0, 216, 0);// 
					}
					Watch = 45.0 / 360.0 * 100.0;
				}
				DrawCircleGauge(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), Deg + Watch, this->m_Watch.get(), Deg - Watch, (double)R / 64.0);
			}
		}
		void InGameUIControl::DrawCharaUI_Front(PlayerID value) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto& p = PlayerMngr->GetPlayer(value);
			if (p->GetChara() && p->GetChara()->CanLookPlayer0()) {
				if (p->GetAI()->GetGraphAlpha() <= 0.f) { return; }
				int R = Cam2DControl::GetTileToDispSize(1.f);
				Vector2DX DispPos;
				Cam2DControl::ConvertTiletoDisp(p->GetChara()->GetPosition(), &DispPos);
				// 範囲外
				if (!HitPointToRectangle(
					static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
					-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
					return;
				}
				int ShadowOfset = DrawParts->GetScreenY(3);
				float Scale = static_cast<float>(DrawParts->GetScreenY(128)) / 128.0f * p->GetAI()->GetGraphAlpha();
				if (p->GetAI()->IsAlert()) {
					SetDrawBright(0, 0, 0);// 
					this->m_Alert.DrawRotaGraph(static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32) + ShadowOfset, Scale, 0.f, true);
					SetDrawBright(255, 0, 0);// 
					this->m_Alert.DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32), Scale, 0.f, true);
				}
				else if (p->GetAI()->IsCaution()) {
					SetDrawBright(0, 0, 0);// 
					this->m_Caution.DrawRotaGraph(static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32) + ShadowOfset, Scale, 0.f, true);
					SetDrawBright(255, 255, 0);// 
					this->m_Caution.DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32), Scale, 0.f, true);
				}
				else {
					SetDrawBright(0, 0, 0);// 
					this->m_Caution.DrawRotaGraph(static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32) + ShadowOfset, Scale, 0.f, true);
					SetDrawBright(0, 255, 0);// 
					this->m_Caution.DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32), Scale, 0.f, true);
				}
			}
		}
		void InGameUIControl::LoadUI(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			this->m_Watch.Load("data/UI/Watch.png");
			this->m_Caution.Load("data/UI/Caution.png");
			this->m_Alert.Load("data/UI/Alert.png");
			this->m_Goal.Load("data/UI/baserad.png");

			this->m_ViewHandle.Make(DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), true);
		}
		void InGameUIControl::SetUI(void) noexcept {
			this->m_MapDrawTime = 5.f;
		}
		void InGameUIControl::UpdateUI(void) noexcept {
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
		void InGameUIControl::Dispose_LoadUI(void) noexcept {
			this->m_ViewHandle.Dispose();
			this->m_Watch.Dispose();
			this->m_Caution.Dispose();
			this->m_Alert.Dispose();
		}
		void InGameUIControl::SetupWatchScreen(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			this->m_ViewHandle.SetDraw_Screen(false);
			{
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
				// 視界
				for (int loop = 0; loop < 4; loop++) {
					for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
						auto& p = PlayerMngr->GetPlayer((PlayerID)i);
						if (p->GetChara()) {
							if (i == 0) {
								if (loop == 3) {
									DrawCharaUI_Back((PlayerID)i);
								}
							}
							else {
								if (p->GetAI()->IsAlert()) {
									if (loop == 2) {
										DrawCharaUI_Back((PlayerID)i);
									}
								}
								else if (p->GetAI()->IsCaution()) {
									if (loop == 1) {
										DrawCharaUI_Back((PlayerID)i);
									}
								}
								else {
									if (loop == 0) {
										DrawCharaUI_Back((PlayerID)i);
									}
								}
							}
						}
					}
				}
				SetDrawBright(255, 255, 255);
				{
					SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
					BackGround->GetShadowGraph().DrawExtendGraph(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
			}
		}
		void InGameUIControl::DrawUI_Back(void) const noexcept {
			SetDrawBlendMode(DX_BLENDMODE_MULA, 92);
			this->m_ViewHandle.DrawGraph(0, 0, false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		void InGameUIControl::DrawUI_Front(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					if (p->GetChara()->CanLookPlayer0()) {
						SetDrawBright(255, 255, 255);
						p->GetChara()->DrawHPBer();
					}
					// !マーク
					if (i != 0) {
						DrawCharaUI_Front((PlayerID)i);
					}
					// ID用デバッグ描画
					if (false) {
						SetDrawBright(255, 255, 255);
						PlayerMngr->GetPlayer((PlayerID)i)->GetAI()->Draw();
					}
				}
			}
			SetDrawBright(255, 255, 255);
			// 
			if (this->m_GoalPos.x != -1.f && this->m_GoalPos.y != -1.f) {
				int ShadowOfset = DrawParts->GetScreenY(3);
				auto& p = PlayerMngr->GetPlayer((PlayerID)0);
				if (p->GetChara()) {
					float Len = (this->m_GoalPos - p->GetChara()->GetPosition()).magnitude() / 1.f;
					float Rad = GetRadVec(this->m_GoalPos - p->GetChara()->GetPosition());
					if (Len > 1.f / 255.f) {
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * Len), 0, 255));
						SetDrawBright(0, 0, 0);
						this->m_Goal.DrawRotaGraph(DrawParts->GetScreenY(1920 / 2) + ShadowOfset, DrawParts->GetScreenY(1080 / 2) + ShadowOfset, static_cast<float>(DrawParts->GetScreenY(1024)) / 400.f, Rad, true);
						SetDrawBright(255, 255, 255);
						this->m_Goal.DrawRotaGraph(DrawParts->GetScreenY(1920 / 2), DrawParts->GetScreenY(1080 / 2), static_cast<float>(DrawParts->GetScreenY(1024)) / 400.f, Rad, true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					}
				}
			}
			for (auto& e : BackGround->GetEventChip()) {
				// 次マップへの遷移
				if (e.m_EventType == EventType::CutScene) {
					if (static_cast<int>(m_StartTime) < e.m_ActiveDelaySec) {
						continue;
					}
					auto* SaveDataParts = SaveDataClass::Instance();
					std::string SaveStr = "Cut_" + std::to_string(e.m_CutSceneID);
					if (SaveDataParts->GetParam(SaveStr) == -1) {
						int ShadowOfset = DrawParts->GetScreenY(3);
						auto& p = PlayerMngr->GetPlayer((PlayerID)0);

						Vector2DX Pos = BackGround->GetFloorData(e.m_index)->GetTileCenterPos();

						Vector2DX DispPos;
						Cam2DControl::ConvertTiletoDisp(Pos, &DispPos);

						if (!HitPointToRectangle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), 0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080))) {
							if (p->GetChara()) {
								float Len = (Pos - p->GetChara()->GetPosition()).magnitude() / 1.f;
								float Rad = GetRadVec(Pos - p->GetChara()->GetPosition());
								if (Len > 1.f / 255.f) {
									SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * std::clamp(Len, 0.f, 1.f)), 0, 255));
									SetDrawBright(0, 0, 0);
									this->m_Goal.DrawRotaGraph(DrawParts->GetScreenY(1920 / 2) + ShadowOfset, DrawParts->GetScreenY(1080 / 2) + ShadowOfset, static_cast<float>(DrawParts->GetScreenY(768)) / 400.f, Rad, true);
									SetDrawBright(255, 255, 255);
									this->m_Goal.DrawRotaGraph(DrawParts->GetScreenY(1920 / 2), DrawParts->GetScreenY(1080 / 2), static_cast<float>(DrawParts->GetScreenY(768)) / 400.f, Rad, true);
									SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
								}
							}
						}
						else {
							float Scale = static_cast<float>(DrawParts->GetScreenY(64)) / 128.0f;

							SetDrawBright(0, 0, 0);// 
							this->m_Caution.DrawRotaGraph(static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32) + ShadowOfset, Scale, 0.f, true);
							SetDrawBright(255, 128, 0);// 
							this->m_Caution.DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetScreenY(32), Scale, 0.f, true);
							SetDrawBright(255, 255, 255);
						}
					}
				}
			}

		}
		void InGameUIControl::DrawUI_MapName(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			if (this->m_MapDrawPer > 1.f / 255.f) {
				auto* Fonts = UISystem::FontPool::Instance();
				auto* LocalizeParts = LocalizePool::Instance();
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_MapDrawPer), 0, 255));
				Fonts->Get(UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(64), 0)->DrawString(InvalidID, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
					DrawParts->GetUIY(64), DrawParts->GetUIY(128), White, Black, LocalizeParts->Get(this->m_MapTextID));
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
		void CutSceneControl::SetCut(void) noexcept {
			this->m_IsCutScene = false;
			this->m_CutSceneAlpha = 0.f;
			this->m_MsgBoxSeek = 0.f;
			this->m_WaitMS = 0.f;
			this->m_CGFade = 0.f;
		}
		void CutSceneControl::UpdateCut(void) noexcept {
			auto* Pad = PadControl::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			auto* DrawParts = DXDraw::Instance();

			if (this->m_IsCutScene) {
				Pad->SetMouseMoveEnable(false);
				KeyGuideParts->ChangeGuide(
					[]() {
						auto* KeyGuideParts = UISystem::KeyGuide::Instance();
						auto* Pad = PadControl::Instance();
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "読み進める");
					});
			}
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
						auto* Fonts = UISystem::FontPool::Instance();
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
									if (Fonts->Get(UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(32), 0)->GetStringWidth(InvalidID, Tmp) < Limit) {
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

			if (this->m_CGFade > 1.f / 255.f) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * this->m_CGFade), 0, 255));
				DrawBox(0, 0, DrawParts->GetUIY(1920), DrawParts->GetUIY(1080), Black, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			if (this->m_CutSceneAlpha > 1.f / 255.f) {
				auto Color = GetColor(16, 16, 16);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_CutSceneAlpha), 0, 255));
				DrawBox(0, 0, DrawParts->GetUIY(1920), DrawParts->GetUIY(120), Color, TRUE);
				DrawBox(0, DrawParts->GetUIY(1080 - 120), DrawParts->GetUIY(1920), DrawParts->GetUIY(1080), Color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			if (this->m_MsgBoxAlpha > 1.f / 255.f) {
				float Per = Lerp(0.5f, 1.f, this->m_MsgBoxAlpha);
				int x1 = DrawParts->GetUIY(64);
				int y1 = DrawParts->GetUIY((1080 - 64) - static_cast<int>((320 - 64) * Per));
				int x2 = DrawParts->GetUIY(1920 - 48);
				int y2 = DrawParts->GetUIY(1080 - 64 - static_cast<int>((320 - 64) * (1.f - Per)));

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_MsgBoxAlpha), 0, 255));
				DrawBox(x1, y1, x2, y2, Gray50, TRUE);
				if (this->m_MsgBoxAlpha >= 1.f) {
					int NameID = 0;
					int MsgID = 0;
					if (0 <= this->m_CutSceneSeek && this->m_CutSceneSeek < static_cast<int>(this->m_CutSceneData.size())) {
						auto& Data = this->m_CutSceneData.at(static_cast<size_t>(this->m_CutSceneSeek));
						NameID = Data.m_NameID;
						MsgID = Data.m_MsgID;
					}
					if (MsgID != 0) {
						auto* Fonts = UISystem::FontPool::Instance();
						auto* LocalizeParts = LocalizePool::Instance();
						Fonts->Get(UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(18), 0)->DrawString(InvalidID, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
							x1 + DrawParts->GetUIY(32), y1 + DrawParts->GetUIY(32), White, Black, LocalizeParts->Get(NameID));
						for (auto& m : this->m_MsgString) {
							if (m == "") { continue; }
							int i = static_cast<int>(&m - &this->m_MsgString.front());
							Fonts->Get(UISystem::FontPool::FontType::MS_Gothic, DrawParts->GetUIY(32), 0)->DrawString(InvalidID, UISystem::FontXCenter::LEFT, UISystem::FontYCenter::TOP,
								x1 + DrawParts->GetUIY(64), y1 + DrawParts->GetUIY(64) + DrawParts->GetUIY(32 * i), White, Black, m);
						}
					}
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			if (this->m_CGFade > 1.f / 255.f) {
				int x1 = DrawParts->GetUIY(960 - 960 / 2);
				int y1 = DrawParts->GetUIY(400 - 540 / 2);
				int x2 = DrawParts->GetUIY(960 + 960 / 2);
				int y2 = DrawParts->GetUIY(400 + 540 / 2);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_CGFade), 0, 255));
				DrawBox(x1, y1, x2, y2, Gray50, TRUE);
				m_CGGraph.DrawExtendGraph(x1, y1, x2, y2, false);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
		// 
		void FadeControl::SetFade(void) noexcept {
			this->m_IsBlackOut = false;
			this->m_BlackOutAlpha = 1.f;
		}
		void FadeControl::UpdateFade(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			this->m_BlackOutAlpha = std::clamp(this->m_BlackOutAlpha + (this->m_IsBlackOut ? 1.f : -1.f) * DrawParts->GetDeltaTime() / 0.5f, 0.f, 1.f);
		}
		void FadeControl::DrawFade(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(255.f * this->m_BlackOutAlpha), 0, 255));
			DrawBox(0, 0, DrawParts->GetUIY(1920), DrawParts->GetUIY(1080), Black, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	};
};
