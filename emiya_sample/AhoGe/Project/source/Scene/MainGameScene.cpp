#include	"MainGameScene.hpp"

#include	"../sub.hpp"

#include	"../MainScene/Player/Player.hpp"
#include	"../MainScene/BackGround/BackGround.hpp"

#include	"../MainScene/Object/Metal.hpp"

namespace FPS_n2 {
	namespace Sceneclass {
		// ロード
		void			MainGameScene::Load_Sub(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::RunFoot), 6, "data/Sound/SE/move/runfoot.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Shot1), 6, "data/Sound/SE/move/shot1.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Shot2), 6, "data/Sound/SE/move/shot2.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Shot3), 6, "data/Sound/SE/move/shot3.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Bomb), 1, "data/Sound/SE/move/bomb.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Hit), 6, "data/Sound/SE/move/hit.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Guard), 6, "data/Sound/SE/move/guard.wav");
			SoundParts->Add(SoundType::SE, static_cast<int>(SESelect::Normal), 2, "data/Sound/SE/Normal.wav");

			SoundParts->Add(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Normal), 1, "data/Sound/BGM/normal.wav");
			SoundParts->Add(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Caution), 1, "data/Sound/BGM/Caution.wav");
			SoundParts->Add(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Alert), 1, "data/Sound/BGM/Alert.wav");

			InGameUIControl::LoadUI();
			PauseMenuControl::LoadPause();
			// 初期マップ
			this->m_IsPrologue = false;
			auto* SaveDataParts = SaveDataClass::Instance();
			if (SaveDataParts->GetParam("LastMap") == -1) {// 初期データがない場合はここから始める
				SaveDataParts->SetParam("LastMap", 0);
				SaveDataParts->SetParam("LastEntry", 0);
				this->m_IsPrologue = true;
			}
			this->m_MapName = "map" + std::to_string(SaveDataParts->GetParam("LastMap"));
			this->m_EntryID = static_cast<int>(SaveDataParts->GetParam("LastEntry"));
			this->m_CutSceneID = InvalidID;
		}
		void			MainGameScene::Set_Sub(void) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* Cam2D = Cam2DControl::Instance();
			auto* SoundParts = SoundPool::Instance();
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::RunFoot))->SetLocalVolume(128);
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Normal))->SetLocalVolume(128);

			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot1))->SetLocalVolume(64);
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot2))->SetLocalVolume(64);
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Shot3))->SetLocalVolume(64);
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Bomb))->SetLocalVolume(64);
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Hit))->SetLocalVolume(64);
			SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Guard))->SetLocalVolume(64);
			PauseMenuControl::SetPause();
			BackGround->Init(this->m_MapName);
			PlayerMngr->Init(static_cast<int>(BackGround->GetPlayerSpawn().size()));
			m_BossUniqueID = InvalidID;
			m_WinCutSceneID = InvalidID;
			// 全キャラの設定
			Vector2DX					GoalPos;GoalPos.Set(-1.f, -1.f);
			for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
				AddCharacter((PlayerID)i);

				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if ((PlayerID)i == this->m_MyPlayerID) {
					for (auto& e : BackGround->GetEventChip()) {
						if (e.m_EventType == EventType::Entry) {
							if (e.m_EventID == this->m_EntryID) {
								p->GetChara()->SetPos(BackGround->GetFloorData(e.m_index)->GetPos());
							}
							else {
								GoalPos = BackGround->GetFloorData(e.m_index)->GetPos();
							}
						}
						if (e.m_EventType == EventType::Boss) {
							auto* Obj2DParts = Object2DManager::Instance();
							const auto& Obj = std::make_shared<MetalObject>();
							Obj2DParts->AddObject(Obj);
							Obj->SetPlayerID(1);
							Obj->SetPos(BackGround->GetFloorData(e.m_index)->GetPos());
							Obj->SetSize(Get2DSize(10.f));
							m_BossUniqueID = Obj->GetUniqueID();
							m_WinCutSceneID = e.m_WinCutSceneID;
						}
					}
				}
				else {
					p->GetChara()->SetPos(BackGround->GetFloorData(BackGround->GetPlayerSpawn().at(static_cast<size_t>(i)).m_index)->GetPos());
					p->GetChara()->SetGunType(GunType::Handgun);
				}
			}
			auto& Chara = PlayerMngr->GetPlayer(this->m_MyPlayerID)->GetChara();
			// カメラ
			Cam2D->SetCamPos(Chara->GetPos());
			Cam2D->SetCamRangePos(BackGround->GetCamScale());
			// 
			this->m_PrevXY = BackGround->GetNumToXY(BackGround->GetNearestFloors(Chara->GetPos()));
			// 
			InGameUIControl::SetMap(BackGround->GetMapTextID(), GoalPos);
			// 
			CutSceneControl::SetCut();
			FadeControl::SetFade();
			InGameUIControl::SetUI();
			Effect2DControl::Instance()->Init();
			if (m_CutSceneID != InvalidID) {
				CutSceneControl::StartCutScene(m_CutSceneID);
			}
			else if (m_IsPrologue) {
				CutSceneControl::StartCutScene(500);
			}
			// 
			this->m_IsCautionBGM = false;
			this->m_CautionBGM = 0.f;
			this->m_IsAlertBGM = false;
			this->m_AlertBGM = 0.f;
			// 
			this->m_IsGoNext = false;
			this->m_IsEnd = false;
			this->m_IsGoodEnd = false;
			this->m_IsBadEnd = false;
			this->m_StartTime = 0.f;

			auto* SaveDataParts = SaveDataClass::Instance();
			Chara->SetGunType(GunType::None);
			if (SaveDataParts->GetParam("Cut_10") != -1) {
				Chara->SetGunType(GunType::Handgun);
			}
			if (SaveDataParts->GetParam("Cut_20") != -1) {
				Chara->SetGunType(GunType::Rifle);
			}
			if (SaveDataParts->GetParam("Cut_30") != -1) {
				Chara->SetGunType(GunType::Rocket);
			}
		}
		bool			MainGameScene::Update_Sub(void) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Pad = PadControl::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* Cam2D = Cam2DControl::Instance();
			auto* SoundParts = SoundPool::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* DXLib_refParts = DXLib_ref::Instance();

			auto& Chara = PlayerMngr->GetPlayer(this->m_MyPlayerID)->GetChara();

			PauseMenuControl::UpdatePause();
			if (DXLib_refParts->IsPause()) {
				return true;
			}
			if ((this->m_IsEnd || this->m_IsGoNext) && FadeControl::IsFadeAll()) {
				return false;
			}
			if (this->m_IsPlayable) {
				Pad->SetMouseMoveEnable(false);
				Pad->ChangeGuide(
					[]() {
						auto* Pad = PadControl::Instance();
						Pad->AddGuide(PADS::MOVE_W, "");
						Pad->AddGuide(PADS::MOVE_S, "");
						Pad->AddGuide(PADS::MOVE_A, "");
						Pad->AddGuide(PADS::MOVE_D, "");
						Pad->AddGuide(PADS::MOVE_STICK, "移動");
						Pad->AddGuide(PADS::RUN, "走る");
						Pad->AddGuide(PADS::WALK, "ゆっくり歩く");
						Pad->AddGuide(PADS::JUMP, "ドッジロール");
						Pad->AddGuide(PADS::SHOT, "射撃");
						Pad->AddGuide(PADS::AIM, "注目");
					});
			}

			if (m_BossUniqueID != InvalidID) {
				if (Obj2DParts->HasObj(m_BossUniqueID)) {
					const auto& Obj = Obj2DParts->GetObj(m_BossUniqueID);
					auto& P = (std::shared_ptr<MetalObject>&)(Obj);
					P->SetCanMove(this->m_IsPlayable);
				}
				else if(m_WinCutSceneID != InvalidID && !m_IsBadEnd){
					CutSceneControl::StartCutScene(m_WinCutSceneID);
					m_WinCutSceneID = InvalidID;
				}
			}
			// 
			CutSceneControl::UpdateCut();
			FadeControl::IsFadeAll();
			FadeControl::UpdateFade();
			// BGM
			{
				bool Prev = this->m_IsCautionBGM || this->m_IsAlertBGM;
				this->m_IsCautionBGM = false;
				this->m_IsAlertBGM = false;
				for (int i = 0;i < PlayerMngr->GetPlayerNum();i++) {
					auto& p = PlayerMngr->GetPlayer((PlayerID)i);
					if (p->GetChara()) {
						if (p->GetAI()->IsAlert()) {
							this->m_IsAlertBGM = true;
						}
						else if (p->GetAI()->IsCaution()) {
							this->m_IsCautionBGM = true;
						}
					}
				}
				if (m_BossUniqueID != InvalidID) {
				}
				if (this->m_IsAlertBGM) {
					this->m_IsCautionBGM = false;
				}
				this->m_IsNormalBGM = !(this->m_IsCautionBGM || this->m_IsAlertBGM);

				if (m_BossUniqueID != InvalidID) {
					// ボスキャラがいたら
					if (Obj2DParts->HasObj(m_BossUniqueID)) {
						this->m_IsAlertBGM = true;
					}
					else {
						this->m_IsNormalBGM = false;
						this->m_IsCautionBGM = false;
						this->m_IsAlertBGM = false;
					}
				}

				if (Prev && Prev != (this->m_IsCautionBGM || this->m_IsAlertBGM)) {
					SoundParts->Get(SoundType::SE, static_cast<int>(SESelect::Normal))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				// 
				if (this->m_IsNormalBGM) {
					if (this->m_NormalBGM < 1.f) {
						this->m_NormalBGM = 1.f;
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Normal))->Play(DX_PLAYTYPE_LOOP, TRUE);
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Normal))->SetLocalVolume(static_cast<int>(255 * this->m_NormalBGM));
					}
				}
				else {
					bool IsPlay = (this->m_NormalBGM > 0.f);
					this->m_NormalBGM = std::max(this->m_NormalBGM - DrawParts->GetDeltaTime(), 0.f);
					if (this->m_NormalBGM > 0.f) {
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Normal))->SetLocalVolume(static_cast<int>(255 * this->m_NormalBGM));
					}
					else {
						if (IsPlay) {
							SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Normal))->StopAll();
						}
					}
				}
				// 
				if (this->m_IsCautionBGM) {
					if (this->m_CautionBGM < 1.f) {
						this->m_CautionBGM = 1.f;
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Caution))->Play(DX_PLAYTYPE_LOOP, TRUE);
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Caution))->SetLocalVolume(static_cast<int>(255 * this->m_CautionBGM));
					}
				}
				else {
					bool IsPlay = (this->m_CautionBGM > 0.f);
					this->m_CautionBGM = std::max(this->m_CautionBGM - DrawParts->GetDeltaTime(), 0.f);
					if (this->m_CautionBGM > 0.f) {
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Caution))->SetLocalVolume(static_cast<int>(255 * this->m_CautionBGM));
					}
					else {
						if (IsPlay) {
							SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Caution))->StopAll();
						}
					}
				}
				// 
				if (this->m_IsAlertBGM) {
					if (this->m_AlertBGM < 1.f) {
						this->m_AlertBGM = 1.f;
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Alert))->Play(DX_PLAYTYPE_LOOP, TRUE);
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Alert))->SetLocalVolume(static_cast<int>(255 * this->m_AlertBGM));
					}
				}
				else {
					bool IsPlay = (this->m_AlertBGM > 0.f);
					this->m_AlertBGM = std::max(this->m_AlertBGM - DrawParts->GetDeltaTime(), 0.f);
					if (this->m_AlertBGM > 0.f) {
						SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Alert))->SetLocalVolume(static_cast<int>(255 * this->m_AlertBGM));
					}
					else {
						if (IsPlay) {
							SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Alert))->StopAll();
						}
					}
				}
			}
			// イベント等制御
			{
				auto Prev = this->m_IsPlayable;
				this->m_IsPlayable = FadeControl::IsFadeClear();
				if (CutSceneControl::IsCutScene()) {
					this->m_IsPlayable = false;
					// 射撃能力
					if (Chara) {
						Chara->SetGunType(GunType::None);
						if (SaveDataParts->GetParam("Cut_10") != -1) {
							Chara->SetGunType(GunType::Handgun);
						}
						if (SaveDataParts->GetParam("Cut_20") != -1) {
							Chara->SetGunType(GunType::Rifle);
						}
						if (SaveDataParts->GetParam("Cut_30") != -1) {
							Chara->SetGunType(GunType::Rocket);
						}
					}
				}
				if (Prev != this->m_IsPlayable) {
					Pad->SetGuideUpdate();
					if (this->m_IsPlayable && (m_BossUniqueID != InvalidID) && (m_WinCutSceneID == InvalidID)) {
						this->m_IsEnd = true;
						this->m_IsGoodEnd = true;
						SaveDataParts->Reset();
					}
				}
				if (PauseMenuControl::IsRetire()) {
					this->m_IsEnd = true;
				}
				if (!Chara) {
					if (!m_IsBadEnd) {
						CutSceneControl::StartCutScene(999);
						this->m_IsBadEnd = true;
					}
					else {
						if (!CutSceneControl::IsCutScene()) {
							this->m_IsEnd = true;
						}
					}
				}
				FadeControl::SetBlackOut(this->m_IsEnd || this->m_IsGoNext);
				if (this->m_IsPlayable) {
					m_StartTime += DrawParts->GetDeltaTime();
					InGameUIControl::SetStartTime(m_StartTime);
				}
			}
			// 
			Effect2DControl::Instance()->Update();
			// 
			BackGround->Update();
			// 入力制御
			for (int i = 0;i < PlayerMngr->GetPlayerNum();i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					if (this->m_IsPlayable) {
						if (i == this->m_MyPlayerID) {
							InputControl MyInput;
							MyInput.ResetAllInput();
							MyInput.SetInputPADS(PADS::MOVE_W, Pad->GetKey(PADS::MOVE_W).press());
							MyInput.SetInputPADS(PADS::MOVE_S, Pad->GetKey(PADS::MOVE_S).press());
							MyInput.SetInputPADS(PADS::MOVE_A, Pad->GetKey(PADS::MOVE_A).press());
							MyInput.SetInputPADS(PADS::MOVE_D, Pad->GetKey(PADS::MOVE_D).press());
							MyInput.SetInputPADS(PADS::RUN, Pad->GetKey(PADS::RUN).press());
							MyInput.SetInputPADS(PADS::WALK, Pad->GetKey(PADS::WALK).press());
							MyInput.SetInputPADS(PADS::JUMP, Pad->GetKey(PADS::JUMP).press());
							MyInput.SetInputPADS(PADS::SHOT, Pad->GetKey(PADS::SHOT).press());
							// 視点操作
							if (std::abs(Pad->GetLS_X()) > 0.1f || std::abs(Pad->GetLS_Y()) > 0.1f) {
								MyInput.SetyRad(std::atan2f(Pad->GetLS_X(), Pad->GetLS_Y()));
							}
							else if (std::abs(p->GetChara()->GetVec().x) > 0.1f || std::abs(p->GetChara()->GetVec().y) > 0.1f) {
								MyInput.SetyRad(std::atan2f(p->GetChara()->GetVec().x, p->GetChara()->GetVec().y));
							}
							else {
								MyInput.SetyRad(p->GetChara()->GetViewRad());
							}
							p->GetChara()->UpdateInput(MyInput);

							// カメラ制御
							Vector2DX CamAddPos;
							if (Pad->GetKey(PADS::AIM).press()) {
								float ViewLimit = Get2DSize(10.f);
								CamAddPos.Set(std::sin(MyInput.GetyRad()) * ViewLimit, std::cos(MyInput.GetyRad()) * ViewLimit);
							}
							Easing(&this->m_CamAddPos, CamAddPos, 0.9f, EasingType::OutExpo);
						}
						else {
							p->GetAI()->Update();
							p->GetChara()->UpdateInput(p->GetAI()->GetAIInput());
						}
					}
					else {
						InputControl MyInput;
						MyInput.ResetAllInput();
						p->GetChara()->UpdateInput(MyInput);
					}
				}
			}
			// 死亡制御
			{
				for (int i = 0;i < PlayerMngr->GetPlayerNum();i++) {
					auto& p = PlayerMngr->GetPlayer((PlayerID)i);
					if (p->GetChara()) {
						if (p->GetChara()->GetIsDelete()) {
							DelCharacter((PlayerID)i);
						}
					}
				}
			}
			// オブジェクトのアップデート
			Obj2DParts->Update();
			//イベントシーン用 イベントでのカメラオフセット値を制御
			if (!this->m_IsPlayable && CutSceneControl::IsCutScene()) {
				auto MyIndex = BackGround->GetNumToXY(BackGround->GetNearestFloors(Chara->GetPos()));
				MyIndex.first += CutSceneControl::GetAddViewPointX();
				MyIndex.second += CutSceneControl::GetAddViewPointY();
				Vector2DX CamAddPos = BackGround->GetFloorData(BackGround->GetXYToNum(MyIndex.first, MyIndex.second))->GetPos() - Chara->GetPos();
				Easing(&this->m_CamAddPos, CamAddPos, 0.9f, EasingType::OutExpo);
			}
			// カメラ制御
			Cam2D->SetCamAim(Chara->GetPos() + this->m_CamAddPos);
			Cam2D->SetCamRangeAim(BackGround->GetCamScale());
			// 
			if (Chara) {
				BackGround->SetPointLight(Chara->GetPos());
			}
			BackGround->SetAmbientLight(120.f, deg2rad(30));
			BackGround->SetupShadow([]() {
				auto* Obj2DParts = Object2DManager::Instance();
				Obj2DParts->DrawShadow();
									});

			InGameUIControl::UpdateUI();
			if (FadeControl::IsFadeClear() && Chara) {
				// 範囲に入ったらイベント
				auto MyIndex = BackGround->GetNumToXY(BackGround->GetNearestFloors(Chara->GetPos()));
				auto IsNearIndex = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
					return  (std::abs(a.first - b.first) <= 3 && std::abs(a.second - b.second) <= 3);
				};
				if (!IsNearIndex(this->m_PrevXY, MyIndex)) {
					this->m_PrevXY = MyIndex;
					if (this->m_IsPlayable) {
						for (auto& e : BackGround->GetEventChip()) {
							if (IsNearIndex(BackGround->GetNumToXY(e.m_index), MyIndex)) {// 該当のチップに踏み込んだ
								// 次マップへの遷移
								if (e.m_EventType == EventType::Entry) {
									if (e.m_EntryID!= InvalidID) {
										this->m_MapName = e.m_MapName;
										this->m_EntryID = e.m_EntryID;
										this->m_CutSceneID = e.m_CutSceneID;
										this->m_IsGoNext = true;
										break;
									}
								}
								// 
								if (e.m_EventType == EventType::CutScene) {
									if (static_cast<int>(m_StartTime) > e.m_ActiveDelaySec) {
										std::string SaveStr = "Cut_" + std::to_string(e.m_CutSceneID);
										if (SaveDataParts->GetParam(SaveStr) == -1) {
											SaveDataParts->SetParam(SaveStr, 0);
											CutSceneControl::StartCutScene(e.m_CutSceneID);
										}
										break;
									}
								}
							}
						}
					}
				}
			}
			InGameUIControl::SetupWatchScreen();
			return true;
		}
		void			MainGameScene::Dispose_Sub(void) noexcept {
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* SoundParts = SoundPool::Instance();
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Normal))->StopAll();
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Caution))->StopAll();
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Alert))->StopAll();
			// リソース
			for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					DelCharacter((PlayerID)i);
				}
			}
			PlayerMngr->Dispose();
			BackGround->Dispose();
			// セーブ
			if(!this->m_IsGoodEnd){
				// 最後に訪れたマップ
				SaveDataParts->SetParam("LastMap", std::stoi(this->m_MapName.substr(3)));
				SaveDataParts->SetParam("LastEntry", this->m_EntryID);
			}
			SaveDataParts->Save();
			if (this->m_IsEnd) {// タイトルに戻る
				SetNextSelect(0);
			}
			else {// 次のシーンへ
				SetNextSelect(1);
			}
		}
		void			MainGameScene::Dispose_Load_Sub(void) noexcept {
			auto* SoundParts = SoundPool::Instance();
			auto* Obj2DParts = Object2DManager::Instance();

			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::RunFoot));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Shot1));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Shot2));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Shot3));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Bomb));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Hit));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Guard));
			SoundParts->Delete(SoundType::SE, static_cast<int>(SESelect::Normal));

			SoundParts->Delete(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Normal));
			SoundParts->Delete(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Caution));
			SoundParts->Delete(SoundType::BGM, static_cast<int>(FPS_n2::Sceneclass::BGMSelect::Alert));

			Obj2DParts->DeleteAll();

			PauseMenuControl::DisposePause();
			InGameUIControl::Dispose_LoadUI();
		}
		// 
		void			MainGameScene::MainDraw_Sub(void) const noexcept {
			auto* BackGround = BackGroundClassBase::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			BackGround->Draw();
			InGameUIControl::DrawUI_Back();
			Obj2DParts->Draw();
			Effect2DControl::Instance()->Draw();
			BackGround->DrawFront();
			InGameUIControl::DrawUI_Front();

			if (m_BossUniqueID != InvalidID) {
				// ボスの頭上に出す体力バー
				if (Obj2DParts->HasObj(m_BossUniqueID)) {
					const auto& Obj = Obj2DParts->GetObj(m_BossUniqueID);
					auto& P = (std::shared_ptr<MetalObject>&)(Obj);
					P->DrawHPBer();
				}
			}
		}
		void			MainGameScene::DrawUI_Base_Sub(void) const noexcept {
			InGameUIControl::DrawUI_MapName();
			FadeControl::DrawFade();
			CutSceneControl::DrawCut();
		}
		void			MainGameScene::DrawUI_In_Sub(void) const noexcept {
			PauseMenuControl::DrawPause();
		}
		void			MainGameScene::AddCharacter(PlayerID value) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto& p = PlayerMngr->GetPlayer(value);

			const auto& Obj = std::make_shared<CharacterObject>();
			Obj2DParts->AddObject(Obj);
			p->SetChara(Obj);
			Obj->SetPlayerID(value);
			// AIを作成
			p->SetAI(std::make_shared<AIControl>());
			p->GetAI()->SetCharacter(value, 0);
			p->GetAI()->Init();
		}
		void			MainGameScene::DelCharacter(PlayerID value) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto& p = PlayerMngr->GetPlayer(value);
			Obj2DParts->DelObj(p->GetChara());
			p->GetAI()->Dispose();
			p->Dispose();
		}
	};
};
