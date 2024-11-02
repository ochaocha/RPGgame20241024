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
			auto* DrawParts = DXDraw::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();

			//各種メインシーン内でずっと保持する必要があるリソースの読み込み
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
			this->m_Watch.Load("data/UI/Watch.png");
			this->m_Caution.Load("data/UI/Caution.png");
			this->m_Alert.Load("data/UI/Alert.png");
			this->m_Goal.Load("data/UI/baserad.png");
			this->m_ViewHandle.Make(DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), true);
			m_PauseMenuControl.Load();
			// ゲームを始める際のマップを選択
			if ((SaveDataParts->GetParam("LastMap") == -1)) {// 初期データがない場合はプロローグとする
				//プロローグなのでデータに新規であることを明記するように追加
				SaveDataParts->SetParam("LastMap", 0);
				SaveDataParts->SetParam("LastEntry", 0);
				//プロローグとして該当するイベントを指定
				m_CutSceneID = 500;
			}
			//セーブデータからマップ、どこから入ったかを取得
			this->m_MapName = "map" + std::to_string(SaveDataParts->GetParam("LastMap"));
			this->m_EntryID = static_cast<int>(SaveDataParts->GetParam("LastEntry"));
		}
		void			MainGameScene::Set_Sub(void) noexcept {
			auto* Cam2D = Cam2DControl::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* EventParts = EventDataBase::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* Pad = PadControl::Instance();

			m_PauseMenuControl.Set();
			BackGround->Init(this->m_MapName);
			BackGround->SetAmbientLight(3.f, deg2rad(30));//3タイル分として上から30度傾けた
			PlayerMngr->Init(static_cast<int>(EventParts->GetPlayerSpawn().size()));
			m_BossUniqueID = InvalidID;
			m_WinCutSceneID = InvalidID;
			//
			//現段階のイベントから必要な処理を実行
			Vector2DX PlayerSpawnPoint;						//プレイヤーの湧き位置
			this->m_GoalPos.Set(-1.f, -1.f);				//プレイヤーが到達すべき場所
			for (auto& e : EventParts->GetEventChip()) {
				if (e.m_EventType == EventType::Entry) {
					//出入口がある場合、それぞれを保持
					if (e.m_EventID == this->m_EntryID) {
						PlayerSpawnPoint = BackGround->GetFloorData(e.m_index)->GetTileCenterPos();
					}
					else {
						this->m_GoalPos = BackGround->GetFloorData(e.m_index)->GetTileCenterPos();
					}
				}
				if (e.m_EventType == EventType::Boss) {
					//ボスイベントがある場合、それをスポーン
					const auto& Obj = std::make_shared<MetalObject>();
					Obj2DParts->AddObject(Obj);
					Obj->SetPlayerID(1);//PlayerCharacterでない適当な値
					Obj->SetPosition(BackGround->GetFloorData(e.m_index)->GetTileCenterPos());
					Obj->SetSize(10.f);
					m_BossUniqueID = Obj->GetUniqueID();
					m_WinCutSceneID = e.m_WinCutSceneID;	//勝利後に流すカットシーンID
				}
			}
			// 全キャラの設定
			for (int i = 0; i < static_cast<int>(EventParts->GetPlayerSpawn().size()); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				const auto& Obj = std::make_shared<CharacterObject>();
				Obj2DParts->AddObject(Obj);
				p->SetChara(Obj);
				Obj->SetPlayerID((PlayerID)i);
				if ((PlayerID)i == PlayerCharacter) {
					//自キャラクター
					//初期位置を指定
					p->GetChara()->SetPosition(PlayerSpawnPoint);
					//初期武器を指定
					p->GetChara()->SetGunType(GunType::None);
					if (SaveDataParts->GetParam("Cut_10") != -1) {
						p->GetChara()->SetGunType(GunType::Handgun);
					}
					if (SaveDataParts->GetParam("Cut_20") != -1) {
						p->GetChara()->SetGunType(GunType::Rifle);
					}
					if (SaveDataParts->GetParam("Cut_30") != -1) {
						p->GetChara()->SetGunType(GunType::Rocket);
					}
				}
				else {
					// AIを作成
					p->SetAI(std::make_shared<AIControl>());
					p->GetAI()->SetCharacter((PlayerID)i, PlayerCharacter);
					p->GetAI()->Init();
					//初期位置を指定
					p->GetChara()->SetPosition(BackGround->GetFloorData(EventParts->GetPlayerSpawn().at(static_cast<size_t>(i)).m_index)->GetTileCenterPos());
					//初期武器を指定
					p->GetChara()->SetGunType(GunType::Handgun);
				}
			}
			// カメラ
			Cam2D->SetCamPos(PlayerSpawnPoint, Tile_DispSize);
			m_CutSceneControl.Set();						// カットシーンの初期設定
			if (m_CutSceneID != InvalidID) {
				m_CutSceneControl.StartCutScene(m_CutSceneID);			//入りの際に何かカットシーンの設定があったら実行
			}

			m_MapNameDrawControl.Set();						//ステージ遷移時にマップ名を表示する機能の初期化
			Effect2DControl::Instance()->Init();			//エフェクトの初期化
			// 各種変数の初期化
			this->m_PrevXY = BackGround->GetNumToXY(BackGround->GetNearestFloors(PlayerSpawnPoint));//カットシーンが発生する判定用の座標保存用変数
			this->m_IsCautionBGM = false;
			this->m_CautionBGM = 0.f;
			this->m_IsAlertBGM = false;
			this->m_AlertBGM = 0.f;

			this->m_IsEndUpdate = false;			//次のシーンに行くフラグの初期化
			this->m_StartTime = 0.f;			//シーンが開始されてからプレイアブル状態であった時間
			m_FadeControl.SetFadeIn();			//シーンの切り替わりはフェードでイン
			Pad->SetMouseMoveEnable(false);
		}
		bool			MainGameScene::Update_Sub(void) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Pad = PadControl::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* EventParts = EventDataBase::Instance();
			auto* Cam2D = Cam2DControl::Instance();
			auto* SoundParts = SoundPool::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* SceneParts = SceneControl::Instance();

			m_PauseMenuControl.Update();				//ポーズ処理の更新
			//キーガイドの更新
			KeyGuideParts->ChangeGuide(
				[this]() {
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* Pad = PadControl::Instance();
					auto* SceneParts = SceneControl::Instance();
					//ポーズ時の入力ガイド
					if (SceneParts->IsPause()) {
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "決定");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::RELOAD).GetAssign(), Pad->GetControlType()), "戻る");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "選択");
						return;
					}
					//カットシーンガイド
					if (m_CutSceneControl.IsCutScene()) {
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "読み進める");
						return;
					}
					//それ以外の場合
					if (this->m_IsPlayable) {
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_A).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_D).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "移動");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::RUN).GetAssign(), Pad->GetControlType()), "走る");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::WALK).GetAssign(), Pad->GetControlType()), "ゆっくり歩く");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::JUMP).GetAssign(), Pad->GetControlType()), "ドッジロール");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::SHOT).GetAssign(), Pad->GetControlType()), "射撃");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::AIM).GetAssign(), Pad->GetControlType()), "注目");
					}
				});
			if (SceneParts->IsPause()) { return true; }//ポーズならこの段階で以下の処理を打ち切る

			// BGM
			{
				bool Prev = this->m_IsCautionBGM || this->m_IsAlertBGM;
				this->m_IsCautionBGM = false;
				this->m_IsAlertBGM = false;
				for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
					auto& p = PlayerMngr->GetPlayer((PlayerID)i);
					if (p->GetChara() && p->GetAI()) {
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
			m_CutSceneControl.Update();
			if (m_CutSceneControl.IsCutScene()) {
				// 射撃能力の更新
				auto& Chara = PlayerMngr->GetPlayer(PlayerCharacter)->GetChara();
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
			{
				auto Prev = this->m_IsPlayable;
				this->m_IsPlayable = m_FadeControl.IsFadeClear() && !m_CutSceneControl.IsCutScene();
				if (Prev != this->m_IsPlayable) {
					//プレイアブルか否かでガイドを変えます
					KeyGuideParts->SetGuideUpdate();
				}
			}
			if (this->m_IsPlayable) {
				//ボスキャラがいる場合
				if (m_BossUniqueID != InvalidID) {
					if (Obj2DParts->HasObj(m_BossUniqueID)) {
						//ボスキャラが生きているならボスの入力処理
						const auto& Obj = Obj2DParts->GetObj(m_BossUniqueID);
						auto& P = (std::shared_ptr<MetalObject>&)(Obj);
						P->SetCanMove();
					}
					else {
						//ボスキャラを倒したら
						m_CutSceneControl.StartCutScene(m_WinCutSceneID);
						//ボスのIDを破棄(ここの処理を通らなくなる)
						m_BossUniqueID = InvalidID;
						this->m_IsGoodEnd = true;
					}
				}
				//プレイアブル経過秒数カウント
				m_StartTime += DrawParts->GetDeltaTime();
			}
			// 入力制御
			for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					if (this->m_IsPlayable) {
						if (i == PlayerCharacter) {
							InputControl MyInput;
							MyInput.ResetAllInput();
							MyInput.SetInputPADS(PADS::MOVE_W, Pad->GetPadsInfo(PADS::MOVE_W).GetKey().press());
							MyInput.SetInputPADS(PADS::MOVE_S, Pad->GetPadsInfo(PADS::MOVE_S).GetKey().press());
							MyInput.SetInputPADS(PADS::MOVE_A, Pad->GetPadsInfo(PADS::MOVE_A).GetKey().press());
							MyInput.SetInputPADS(PADS::MOVE_D, Pad->GetPadsInfo(PADS::MOVE_D).GetKey().press());
							MyInput.SetInputPADS(PADS::RUN, Pad->GetPadsInfo(PADS::RUN).GetKey().press());
							MyInput.SetInputPADS(PADS::WALK, Pad->GetPadsInfo(PADS::WALK).GetKey().press());
							MyInput.SetInputPADS(PADS::JUMP, Pad->GetPadsInfo(PADS::JUMP).GetKey().press());
							MyInput.SetInputPADS(PADS::SHOT, Pad->GetPadsInfo(PADS::SHOT).GetKey().press());
							// 視点操作
							MyInput.SetyRad(p->GetChara()->GetViewRad());
							if (Pad->GetControlType() == ControlType::PC) {
								float XV = static_cast<float>(Pad->GetMS_X() - DrawParts->GetUIXMax() / 2);
								float YV = static_cast<float>(Pad->GetMS_Y() - DrawParts->GetUIYMax() / 2);
								if (std::abs(XV) > 0.1f || std::abs(YV) > 0.1f) {
									MyInput.SetyRad(GetRadVec(Vector2DX::vget(-XV, -YV)));
								}
							}
							else {
								if (std::abs(Pad->GetLS_X()) > 0.1f || std::abs(Pad->GetLS_Y()) > 0.1f) {
									MyInput.SetyRad(-GetRadVec(Vector2DX::vget(Pad->GetLS_X(), Pad->GetLS_Y())));
								}
							}
							if (p->GetChara()->GetSpeed() > 1.f) {
								MyInput.SetyRad(-GetRadVec(p->GetChara()->GetVec()));
							}
							p->GetChara()->UpdateInput(MyInput);

							// カメラ制御
							Vector2DX CamAddPos;
							if (Pad->GetPadsInfo(PADS::AIM).GetKey().press()) {
								float ViewLimit = 10.f;
								CamAddPos = GetVecByRad(DX_PI_F - MyInput.GetyRad()) * ViewLimit;
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
						MyInput.SetyRad(p->GetChara()->GetViewRad());
						p->GetChara()->UpdateInput(MyInput);
					}
				}
			}
			//
			m_FadeControl.Update();					// フェードアウト表示の更新
			m_MapNameDrawControl.Update();			//マップ名表示の更新
			Effect2DControl::Instance()->Update();	// エフェクトの更新
			BackGround->Update();					// 背景の更新
			Obj2DParts->Update();					// オブジェクトのアップデート
			PlayerMngr->UpdateDelete();				// 死亡確認(死亡フラグが経ったら削除)
			//
			auto& Chara = PlayerMngr->GetPlayer(PlayerCharacter)->GetChara();
			if (!Chara) {
				//自キャラがいない(死んだ)場合の処理
				if (!m_IsBadEnd) {
					//まだバッドエンドが開始されていない場合はバッドエンドイベントを実施
					this->m_IsBadEnd = true;
					m_CutSceneControl.StartCutScene(999);
				}
			}
			else {
				auto MyIndex = BackGround->GetNumToXY(BackGround->GetNearestFloors(Chara->GetPosition()));
				if (this->m_IsPlayable) {
					//操作できる状態
					auto IsNearIndex = [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return (std::abs(a.first - b.first) <= 3 && std::abs(a.second - b.second) <= 3); };
					//イベントが発生する場所に侵入したかどうかのチェック
					if (!IsNearIndex(this->m_PrevXY, MyIndex)) {//以前のポジションから離れた時だけチェック
						this->m_PrevXY = MyIndex;
						for (auto& e : EventParts->GetEventChip()) {
							if (IsNearIndex(BackGround->GetNumToXY(e.m_index), MyIndex)) {// 該当のチップに踏み込んだ
								if (e.m_EventType == EventType::Entry) {
									// 次マップへの遷移
									if (e.m_EntryID != InvalidID) {
										this->m_MapName = e.m_MapName;
										this->m_EntryID = e.m_EntryID;
										this->m_CutSceneID = e.m_CutSceneID;
										SetSceneEnd();
										break;
									}
								}
								if (e.m_EventType == EventType::CutScene) {
									// カットシーンの場合
									if (static_cast<int>(m_StartTime) > e.m_ActiveDelaySec) {
										std::string SaveStr = "Cut_" + std::to_string(e.m_CutSceneID);
										if (SaveDataParts->GetParam(SaveStr) == -1) {
											SaveDataParts->SetParam(SaveStr, 0);
											m_CutSceneControl.StartCutScene(e.m_CutSceneID);
										}
										break;
									}
								}
							}
						}
					}
				}
				else {
					//イベント中のカメラオフセット値を制御
					if (m_CutSceneControl.IsCutScene()) {
						MyIndex.first += m_CutSceneControl.GetAddViewPointX();
						MyIndex.second += m_CutSceneControl.GetAddViewPointY();
						Vector2DX CamAddPos = BackGround->GetFloorData(BackGround->GetXYToNum(MyIndex.first, MyIndex.second))->GetTileCenterPos() - Chara->GetPosition();
						Easing(&this->m_CamAddPos, CamAddPos, 0.9f, EasingType::OutExpo);
					}
				}
				Cam2D->SetCamAim(Chara->GetPosition() + this->m_CamAddPos, Tile_DispSize);		// カメラ制御
				BackGround->SetPointLight(Chara->GetPosition());								//ポイントライト用の座標更新
				Cam2DControl::Instance()->Update();
			}
			//リタイアなので終了フラグを立てる
			if (m_PauseMenuControl.IsRetire()) {
				SetSceneEnd();
				this->m_IsEnd = true;
			}
			//グッドorバッドエンドのカットシーンが終わったので終了フラグを立てる
			if (this->m_IsGoodEnd || this->m_IsBadEnd) {
				if (!m_CutSceneControl.IsCutScene()) {
					SetSceneEnd();
					this->m_IsEnd = true;
				}
			}
			if (this->m_IsEndUpdate && m_FadeControl.IsFadeAll()) { return false; }//特定のフラグが立ったうえでフェードアウトしきったら次のシーンへ遷移
			//それ以外はシーン持続
			return true;
		}
		void			MainGameScene::Dispose_Sub(void) noexcept {
			auto* Obj2DParts = Object2DManager::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* SoundParts = SoundPool::Instance();
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Normal))->StopAll();
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Caution))->StopAll();
			SoundParts->Get(SoundType::BGM, static_cast<int>(BGMSelect::Alert))->StopAll();
			// リソース
			PlayerMngr->Dispose();
			BackGround->Dispose();
			// セーブ
			if (!this->m_IsGoodEnd) {
				// 最後に訪れたマップ
				SaveDataParts->SetParam("LastMap", std::stoi(this->m_MapName.substr(3)));
				SaveDataParts->SetParam("LastEntry", this->m_EntryID);
			}
			else {
				//クリアしたのでリセット
				SaveDataParts->Reset();
			}
			SaveDataParts->Save();
			if (this->m_IsEnd) {// タイトルに戻る
				SetNextSelect(0);
			}
			else {// 次のシーンへ
				SetNextSelect(1);
			}
			//エンド処理用フラグのリセット
			this->m_IsEnd = false;
			this->m_IsGoodEnd = false;
			this->m_IsBadEnd = false;
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

			m_PauseMenuControl.DisposeLoad();
			this->m_ViewHandle.Dispose();
			this->m_Watch.Dispose();
			this->m_Caution.Dispose();
			this->m_Alert.Dispose();
		}
		// 
		void			MainGameScene::MainDraw_Sub(void) const noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* EventParts = EventDataBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();

			//視認判定表示用のバッファーを描画
			{
				auto Prev = GetDrawScreen();
				this->m_ViewHandle.SetDraw_Screen(false);
				{
					DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
					// 視界
					for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
						auto& p = PlayerMngr->GetPlayer((PlayerID)i);
						if (!p->GetChara()) { continue; }
						if (i == PlayerCharacter) {
							DrawCharaUI_Back((PlayerID)i);
						}
						else {
							if (p->GetAI()->IsAlert()) {
								DrawCharaUI_Back((PlayerID)i);
							}
							else if (p->GetAI()->IsCaution()) {
								DrawCharaUI_Back((PlayerID)i);
							}
							else {
								DrawCharaUI_Back((PlayerID)i);
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
				GraphHandle::SetDraw_Screen(Prev, false);
			}

			BackGround->Draw();
			SetDrawBlendMode(DX_BLENDMODE_MULA, 92);
			this->m_ViewHandle.DrawGraph(0, 0, false);
			DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);

			Obj2DParts->Draw();
			Effect2DControl::Instance()->Draw();
			BackGround->DrawFront();

			for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					if (!p->GetChara()->CanLookPlayer0()) {
						continue;
					}
					int R = Cam2DControl::GetTileToDispSize(1.f);
					Vector2DX DispPos;
					Cam2DControl::ConvertTiletoDisp(p->GetChara()->GetPosition(), &DispPos);
					// 範囲外
					if (!HitPointToRectangle(
						static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
						-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
						continue;
					}
					DispPos = DispPos * (float)DrawParts->GetUIY(100) / (float)DrawParts->GetScreenY(100);
					//体力バー
					{
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
						int xmin = DrawParts->GetUIY(-50);
						int ymin = DrawParts->GetUIY(-50);
						int xmax = DrawParts->GetUIY(50);
						int xper = xmin + (xmax - xmin) * p->GetChara()->GetHitPoint() / p->GetChara()->GetHitPointMax();
						DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xmax), static_cast<int>(DispPos.y + ymin), Gray75, DrawParts->GetUIY(10));
						DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xper), static_cast<int>(DispPos.y + ymin), Green, DrawParts->GetUIY(10));
					}
					// !マーク
					if (i != 0) {
						{
							if (p->GetAI()->GetGraphAlpha() <= 0.f) { continue; }
							int ShadowOfset = DrawParts->GetUIY(3);
							float Scale = static_cast<float>(DrawParts->GetUIY(128)) / 128.0f * p->GetAI()->GetGraphAlpha();
							if (p->GetAI()->IsAlert()) {
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Alert, static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetUIY(32) + ShadowOfset, Scale, 0.f, true);
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Alert, static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetUIY(32), Scale, 0.f, true);
							}
							else if (p->GetAI()->IsCaution()) {
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetUIY(32) + ShadowOfset, Scale, 0.f, true);
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetUIY(32), Scale, 0.f, true);
							}
							else {
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, static_cast<int>(DispPos.x) + ShadowOfset, static_cast<int>(DispPos.y) - DrawParts->GetUIY(32) + ShadowOfset, Scale, 0.f, true);
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 255, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, static_cast<int>(DispPos.x), static_cast<int>(DispPos.y) - DrawParts->GetUIY(32), Scale, 0.f, true);
							}
						}
					}
				}
			}
			DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
			// 
			if (this->m_GoalPos.x != -1.f && this->m_GoalPos.y != -1.f) {
				int ShadowOfset = DrawParts->GetUIY(3);
				auto& p = PlayerMngr->GetPlayer((PlayerID)0);
				if (p->GetChara()) {
					float Len = (this->m_GoalPos - p->GetChara()->GetPosition()).magnitude() / 1.f;
					float Rad = GetRadVec(this->m_GoalPos - p->GetChara()->GetPosition());
					if (Len > 1.f / 255.f) {
						DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * Len), 0, 255));
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);
						DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, DrawParts->GetUIY(1920 / 2) + ShadowOfset, DrawParts->GetUIY(1080 / 2) + ShadowOfset, static_cast<float>(DrawParts->GetUIY(1024)) / 400.f, Rad, true);
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
						DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, DrawParts->GetUIY(1920 / 2), DrawParts->GetUIY(1080 / 2), static_cast<float>(DrawParts->GetUIY(1024)) / 400.f, Rad, true);
						DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
					}
				}
			}
			for (auto& e : EventParts->GetEventChip()) {
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
									DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(128.f * std::clamp(Len, 0.f, 1.f)), 0, 255));
									DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);
									DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, DrawParts->GetScreenY(1920 / 2) + ShadowOfset, DrawParts->GetScreenY(1080 / 2) + ShadowOfset, static_cast<float>(DrawParts->GetScreenY(768)) / 400.f, Rad, true);
									DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
									DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, DrawParts->GetScreenY(1920 / 2), DrawParts->GetScreenY(1080 / 2), static_cast<float>(DrawParts->GetScreenY(768)) / 400.f, Rad, true);
									DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
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
			if (m_BossUniqueID != InvalidID) {
				// ボスの頭上に出す体力バー
				if (Obj2DParts->HasObj(m_BossUniqueID)) {
					const auto& Obj = Obj2DParts->GetObj(m_BossUniqueID);
					auto& P = (std::shared_ptr<MetalObject>&)(Obj);
					int R = Cam2DControl::GetTileToDispSize(1.f);
					Vector2DX DispPos;
					Cam2DControl::ConvertTiletoDisp(P->GetPosition(), &DispPos);
					// 範囲外
					if (!HitPointToRectangle(
						static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
						-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
						return;
					}
					int xmin = DrawParts->GetScreenY(-50);
					int ymin = DrawParts->GetScreenY(-50);
					int xmax = DrawParts->GetScreenY(50);

					int xper = xmin + (xmax - xmin) * P->GetHitPoint() / P->GetHitPointMax();

					DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, DrawParts->GetUIY(static_cast<int>(DispPos.x + xmin)), DrawParts->GetUIY(static_cast<int>(DispPos.y + ymin)), DrawParts->GetUIY(static_cast<int>(DispPos.x + xmax)), DrawParts->GetUIY(static_cast<int>(DispPos.y + ymin)), Gray75, DrawParts->GetUIY(10));
					DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, DrawParts->GetUIY(static_cast<int>(DispPos.x + xmin)), DrawParts->GetUIY(static_cast<int>(DispPos.y + ymin)), DrawParts->GetUIY(static_cast<int>(DispPos.x + xper)), DrawParts->GetUIY(static_cast<int>(DispPos.y + ymin)), Green, DrawParts->GetUIY(10));
				}
			}
		}
		void			MainGameScene::DrawUI_Base_Sub(void) const noexcept {
			m_MapNameDrawControl.Draw();
			m_FadeControl.DrawFade();
			m_CutSceneControl.DrawCut();
		}
		void			MainGameScene::DrawUI_In_Sub(void) const noexcept {
			m_PauseMenuControl.Draw();
		}
		// UI
		void			MainGameScene::DrawCharaUI_Back(PlayerID value) const noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto& p = PlayerMngr->GetPlayer(value);

			if (!p->GetChara()) { return; }
			float Radius = 10.f;
			int R = Cam2DControl::GetTileToDispSize(Radius);
			Vector2DX DispPos;
			Cam2DControl::ConvertTiletoDisp(p->GetChara()->GetPosition(), &DispPos);
			// 範囲外
			if (!HitPointToRectangle(
				static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
				-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
				return;
			}
			double Deg = (double)-p->GetChara()->GetViewRad() / (DX_PI * 2.0) * 100.0 + 100.0;//ゲージが-100~100の範囲なので+100
			double Watch;
			if (value == PlayerCharacter) {
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
		void			MainGameScene::SetSceneEnd(void) noexcept {
			if (!this->m_IsEndUpdate) {
				m_FadeControl.SetFadeOut();
			}
			this->m_IsEndUpdate = true;
		}
	};
};
