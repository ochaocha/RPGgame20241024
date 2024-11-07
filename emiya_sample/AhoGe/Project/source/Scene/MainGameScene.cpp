#include	"MainGameScene.hpp"

#include	"../sub.hpp"

#include	"../MainScene/Player/Player.hpp"
#include	"../MainScene/BackGround/BackGround.hpp"

#include	"../MainScene/Object/Metal.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		// ���[�h
		void			MainGameScene::Load_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* DrawParts = WindowSizeControl::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();

			// �e�탁�C���V�[�����ł����ƕێ�����K�v�����郊�\�[�X�̓ǂݍ���
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::RunFoot), 6, "data/Sound/SE/move/runfoot.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Shot1), 6, "data/Sound/SE/move/shot1.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Shot2), 6, "data/Sound/SE/move/shot2.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Shot3), 6, "data/Sound/SE/move/shot3.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Bomb), 1, "data/Sound/SE/move/bomb.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Hit), 6, "data/Sound/SE/move/hit.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Guard), 6, "data/Sound/SE/move/guard.wav");
			SoundParts->Add(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Normal), 2, "data/Sound/SE/Normal.wav");
			SoundParts->Add(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Normal), 1, "data/Sound/BGM/normal.wav");
			SoundParts->Add(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Caution), 1, "data/Sound/BGM/Caution.wav");
			SoundParts->Add(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Alert), 1, "data/Sound/BGM/Alert.wav");
			this->m_Watch.Load("data/UI/Watch.png");
			this->m_Caution.Load("data/UI/Caution.png");
			this->m_Alert.Load("data/UI/Alert.png");
			this->m_Goal.Load("data/UI/baserad.png");
			this->m_ViewHandle.Make(DrawParts->GetScreenY(BaseScreenWidth), DrawParts->GetScreenY(BaseScreenHeight), true);
			m_PauseMenuControl.Load();
			// �Q�[�����n�߂�ۂ̃}�b�v��I��
			if ((SaveDataParts->GetParam("LastMap") == -1)) {// �����f�[�^���Ȃ��ꍇ�̓v�����[�O�Ƃ���
				// �v�����[�O�Ȃ̂Ńf�[�^�ɐV�K�ł��邱�Ƃ𖾋L����悤�ɒǉ�
				SaveDataParts->SetParam("LastMap", 0);
				SaveDataParts->SetParam("LastEntry", 0);
				// �v�����[�O�Ƃ��ĊY������C�x���g���w��
				m_CutSceneID = 500;
			}
			// �Z�[�u�f�[�^����}�b�v�A�ǂ���������������擾
			this->m_MapName = "map" + std::to_string(SaveDataParts->GetParam("LastMap"));
			this->m_EntryID = static_cast<int>(SaveDataParts->GetParam("LastEntry"));
		}
		void			MainGameScene::Dispose_Load_Sub(void) noexcept {
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* Obj2DParts = Object2DManager::Instance();

			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::RunFoot));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Shot1));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Shot2));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Shot3));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Bomb));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Hit));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Guard));
			SoundParts->Delete(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Normal));

			SoundParts->Delete(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Normal));
			SoundParts->Delete(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Caution));
			SoundParts->Delete(SoundSystem::SoundType::BGM, static_cast<int>(DXLIB_Sample::BGMSelect::Alert));

			Obj2DParts->DeleteAll();

			m_PauseMenuControl.DisposeLoad();
			this->m_ViewHandle.Dispose();
			this->m_Watch.Dispose();
			this->m_Caution.Dispose();
			this->m_Alert.Dispose();
		}
		//
		void			MainGameScene::Initialize_Sub(void) noexcept {
			auto* Cam2D = Cam2DControl::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* EventParts = EventDataBase::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* Pad = PadControl::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			auto* Effect2DParts = Effect2DControl::Instance();

			m_PauseMenuControl.Set();
			BackGround->Initialize(this->m_MapName);
			BackGround->SetAmbientLight(3.f, deg2rad(30));// 3�^�C�����Ƃ��ďォ��30�x�X����
			PlayerMngr->Initialize(static_cast<int>(EventParts->GetPlayerSpawn().size()));
			m_BossUniqueID = InvalidID;
			m_WinCutSceneID = InvalidID;
			// 
			// ���i�K�̃C�x���g����K�v�ȏ��������s
			Vector2DX PlayerSpawnPoint;						// �v���C���[�̗N���ʒu
			this->m_GoalPos.Set(-1.f, -1.f);				// �v���C���[�����B���ׂ��ꏊ
			for (auto& e : EventParts->GetEventChip()) {
				if (e.m_EventType == EventType::Entry) {
					// �o����������ꍇ�A���ꂼ���ێ�
					if (e.m_EventID == this->m_EntryID) {
						PlayerSpawnPoint = BackGround->GetFloorData(e.m_index)->GetTileCenterPos();
					}
					else {
						this->m_GoalPos = BackGround->GetFloorData(e.m_index)->GetTileCenterPos();
					}
				}
				if (e.m_EventType == EventType::Boss) {
					// �{�X�C�x���g������ꍇ�A������X�|�[��
					const auto& Obj = std::make_shared<MetalObject>();
					Obj2DParts->AddObject(Obj);
					Obj->SetPlayerID(1);// PlayerCharacter�łȂ��K���Ȓl
					Obj->SetPosition(BackGround->GetFloorData(e.m_index)->GetTileCenterPos());
					Obj->SetSize(10.f);
					m_BossUniqueID = Obj->GetUniqueID();
					m_WinCutSceneID = e.m_WinCutSceneID;	// ������ɗ����J�b�g�V�[��ID
				}
			}
			// �S�L�����̐ݒ�
			for (int i = 0; i < static_cast<int>(EventParts->GetPlayerSpawn().size()); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				const auto& Obj = std::make_shared<CharacterObject>();
				Obj2DParts->AddObject(Obj);
				p->SetChara(Obj);
				Obj->SetPlayerID((PlayerID)i);
				if ((PlayerID)i == PlayerCharacter) {
					// ���L�����N�^�[
					// �����ʒu���w��
					p->GetChara()->SetPosition(PlayerSpawnPoint);
					// ����������w��
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
					// AI���쐬
					p->SetAI(std::make_shared<AIControl>());
					p->GetAI()->SetCharacter((PlayerID)i, PlayerCharacter);
					p->GetAI()->Initialize();
					// �����ʒu���w��
					p->GetChara()->SetPosition(BackGround->GetFloorData(EventParts->GetPlayerSpawn().at(static_cast<size_t>(i)).m_index)->GetTileCenterPos());
					// ����������w��
					p->GetChara()->SetGunType(GunType::Handgun);
				}
			}
			// �J����
			Cam2D->SetCamPos(PlayerSpawnPoint);
			m_CutSceneControl.Set();						// �J�b�g�V�[���̏����ݒ�
			if (m_CutSceneID != InvalidID) {
				m_CutSceneControl.StartCutScene(m_CutSceneID);			// ����̍ۂɉ����J�b�g�V�[���̐ݒ肪����������s
			}

			m_MapNameDrawControl.Set();						// �X�e�[�W�J�ڎ��Ƀ}�b�v����\������@�\�̏�����
			Effect2DParts->Initialize();			// �G�t�F�N�g�̏�����
			// �e��ϐ��̏�����
			this->m_PrevXY = BackGround->GetNumToXY(BackGround->GetNearestFloors(PlayerSpawnPoint));// �J�b�g�V�[�����������锻��p�̍��W�ۑ��p�ϐ�
			this->m_IsCautionBGM = false;
			this->m_CautionBGM = 0.f;
			this->m_IsAlertBGM = false;
			this->m_AlertBGM = 0.f;

			this->m_IsEndUpdate = false;			// ���̃V�[���ɍs���t���O�̏�����
			this->m_StartTime = 0.f;			// �V�[�����J�n����Ă���v���C�A�u����Ԃł���������
			FadeInParts->SetFadeIn();			// �V�[���̐؂�ւ��̓t�F�[�h�ŃC��
			Pad->SetMouseMoveEnable(false);
		}
		bool			MainGameScene::Update_Sub(void) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Pad = PadControl::Instance();
			auto* KeyGuideParts = UISystem::KeyGuide::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* EventParts = EventDataBase::Instance();
			auto* Cam2D = Cam2DControl::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* SceneParts = SceneControl::Instance();
			auto* FadeInParts = UI::FadeControl::Instance();
			auto* Effect2DParts = Effect2DControl::Instance();

			m_PauseMenuControl.Update();				// �|�[�Y�����̍X�V
			// �L�[�K�C�h�̍X�V
			KeyGuideParts->ChangeGuide(
				[this]() {
					auto* KeyGuideParts = UISystem::KeyGuide::Instance();
					auto* Pad = PadControl::Instance();
					auto* SceneParts = SceneControl::Instance();
					// �|�[�Y���̓��̓K�C�h
					if (SceneParts->IsPause()) {
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "����");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::RELOAD).GetAssign(), Pad->GetControlType()), "�߂�");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "�I��");
						return;
					}
					// �J�b�g�V�[���K�C�h
					if (m_CutSceneControl.IsCutScene()) {
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::INTERACT).GetAssign(), Pad->GetControlType()), "�ǂݐi�߂�");
						return;
					}
					// ����ȊO�̏ꍇ
					if (this->m_IsPlayable) {
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_W).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_S).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_A).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_D).GetAssign(), Pad->GetControlType()), "");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::MOVE_STICK).GetAssign(), Pad->GetControlType()), "�ړ�");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::RUN).GetAssign(), Pad->GetControlType()), "����");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::WALK).GetAssign(), Pad->GetControlType()), "����������");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::JUMP).GetAssign(), Pad->GetControlType()), "�h�b�W���[��");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::SHOT).GetAssign(), Pad->GetControlType()), "�ˌ�");
						KeyGuideParts->AddGuide(KeyGuideParts->GetIDtoOffset(Pad->GetPadsInfo(PADS::AIM).GetAssign(), Pad->GetControlType()), "����");
					}
				});
			if (SceneParts->IsPause()) { return true; }// �|�[�Y�Ȃ炱�̒i�K�ňȉ��̏�����ł��؂�

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
					// �{�X�L������������
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
					SoundParts->Get(SoundSystem::SoundType::SE, static_cast<int>(SESelect::Normal))->Play(DX_PLAYTYPE_BACK, TRUE);
				}
				// 
				if (this->m_IsNormalBGM) {
					if (this->m_NormalBGM < 1.f) {
						this->m_NormalBGM = 1.f;
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Normal))->Play(DX_PLAYTYPE_LOOP, TRUE);
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Normal))->SetLocalVolume(static_cast<int>(255 * this->m_NormalBGM));
					}
				}
				else {
					bool IsPlay = (this->m_NormalBGM > 0.f);
					this->m_NormalBGM = GetMax(this->m_NormalBGM - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
					if (this->m_NormalBGM > 0.f) {
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Normal))->SetLocalVolume(static_cast<int>(255 * this->m_NormalBGM));
					}
					else {
						if (IsPlay) {
							SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Normal))->StopAll();
						}
					}
				}
				// 
				if (this->m_IsCautionBGM) {
					if (this->m_CautionBGM < 1.f) {
						this->m_CautionBGM = 1.f;
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Caution))->Play(DX_PLAYTYPE_LOOP, TRUE);
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Caution))->SetLocalVolume(static_cast<int>(255 * this->m_CautionBGM));
					}
				}
				else {
					bool IsPlay = (this->m_CautionBGM > 0.f);
					this->m_CautionBGM = GetMax(this->m_CautionBGM - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
					if (this->m_CautionBGM > 0.f) {
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Caution))->SetLocalVolume(static_cast<int>(255 * this->m_CautionBGM));
					}
					else {
						if (IsPlay) {
							SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Caution))->StopAll();
						}
					}
				}
				// 
				if (this->m_IsAlertBGM) {
					if (this->m_AlertBGM < 1.f) {
						this->m_AlertBGM = 1.f;
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Alert))->Play(DX_PLAYTYPE_LOOP, TRUE);
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Alert))->SetLocalVolume(static_cast<int>(255 * this->m_AlertBGM));
					}
				}
				else {
					bool IsPlay = (this->m_AlertBGM > 0.f);
					this->m_AlertBGM = GetMax(this->m_AlertBGM - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
					if (this->m_AlertBGM > 0.f) {
						SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Alert))->SetLocalVolume(static_cast<int>(255 * this->m_AlertBGM));
					}
					else {
						if (IsPlay) {
							SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Alert))->StopAll();
						}
					}
				}
			}
			// �C�x���g������
			m_CutSceneControl.Update();
			if (m_CutSceneControl.IsCutScene()) {
				// �ˌ��\�͂̍X�V
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
				this->m_IsPlayable = FadeInParts->IsFadeClear() && !m_CutSceneControl.IsCutScene();
				if (Prev != this->m_IsPlayable) {
					// �v���C�A�u�����ۂ��ŃK�C�h��ς��܂�
					KeyGuideParts->SetGuideFlip();
				}
			}
			if (this->m_IsPlayable) {
				// �{�X�L����������ꍇ
				if (m_BossUniqueID != InvalidID) {
					if (Obj2DParts->HasObj(m_BossUniqueID)) {
						// �{�X�L�����������Ă���Ȃ�{�X�̓��͏���
						const auto& Obj = Obj2DParts->GetObj(m_BossUniqueID);
						auto& P = (std::shared_ptr<MetalObject>&)(Obj);
						P->SetCanMove();
					}
					else {
						// �{�X�L������|������
						m_CutSceneControl.StartCutScene(m_WinCutSceneID);
						// �{�X��ID��j��(�����̏�����ʂ�Ȃ��Ȃ�)
						m_BossUniqueID = InvalidID;
						this->m_IsGoodEnd = true;
					}
				}
				// �v���C�A�u���o�ߕb���J�E���g
				m_StartTime += DXLib_ref::Instance()->GetDeltaTime();
			}
			// ���͐���
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
							// ���_����
							MyInput.SetyRad(p->GetChara()->GetViewRad());
							if (Pad->GetControlType() == ControlType::PC) {
								float XV = static_cast<float>(Pad->GetMS_X() - BaseScreenWidth / 2);
								float YV = static_cast<float>(Pad->GetMS_Y() - BaseScreenHeight / 2);
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

							// �J��������
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
			FadeInParts->Update();					// �t�F�[�h�A�E�g�\���̍X�V
			m_MapNameDrawControl.Update();			// �}�b�v���\���̍X�V
			Effect2DParts->Update();	// �G�t�F�N�g�̍X�V
			BackGround->Update();					// �w�i�̍X�V
			Obj2DParts->Update();					// �I�u�W�F�N�g�̃A�b�v�f�[�g
			PlayerMngr->UpdateDelete();				// ���S�m�F(���S�t���O���o������폜)
			// 
			auto& Chara = PlayerMngr->GetPlayer(PlayerCharacter)->GetChara();
			if (!Chara) {
				// ���L���������Ȃ�(����)�ꍇ�̏���
				if (!m_IsBadEnd) {
					// �܂��o�b�h�G���h���J�n����Ă��Ȃ��ꍇ�̓o�b�h�G���h�C�x���g�����{
					this->m_IsBadEnd = true;
					m_CutSceneControl.StartCutScene(999);
				}
			}
			else {
				auto MyIndex = BackGround->GetNumToXY(BackGround->GetNearestFloors(Chara->GetPosition()));
				if (this->m_IsPlayable) {
					// ����ł�����
					auto IsNearIndex = [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return (std::abs(a.first - b.first) <= 3 && std::abs(a.second - b.second) <= 3); };
					// �C�x���g����������ꏊ�ɐN���������ǂ����̃`�F�b�N
					if (!IsNearIndex(this->m_PrevXY, MyIndex)) {// �ȑO�̃|�W�V�������痣�ꂽ�������`�F�b�N
						this->m_PrevXY = MyIndex;
						for (auto& e : EventParts->GetEventChip()) {
							if (IsNearIndex(BackGround->GetNumToXY(e.m_index), MyIndex)) {// �Y���̃`�b�v�ɓ��ݍ���
								if (e.m_EventType == EventType::Entry) {
									// ���}�b�v�ւ̑J��
									if (e.m_EntryID != InvalidID) {
										this->m_MapName = e.m_MapName;
										this->m_EntryID = e.m_EntryID;
										this->m_CutSceneID = e.m_CutSceneID;
										SetSceneEnd();
										break;
									}
								}
								if (e.m_EventType == EventType::CutScene) {
									// �J�b�g�V�[���̏ꍇ
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
					// �C�x���g���̃J�����I�t�Z�b�g�l�𐧌�
					if (m_CutSceneControl.IsCutScene()) {
						MyIndex.first += m_CutSceneControl.GetAddViewPointX();
						MyIndex.second += m_CutSceneControl.GetAddViewPointY();
						Vector2DX CamAddPos = BackGround->GetFloorData(BackGround->GetXYToNum(MyIndex.first, MyIndex.second))->GetTileCenterPos() - Chara->GetPosition();
						Easing(&this->m_CamAddPos, CamAddPos, 0.9f, EasingType::OutExpo);
					}
				}
				Cam2D->SetCamAim(Chara->GetPosition() + this->m_CamAddPos);		// �J��������
				BackGround->SetPointLight(Chara->GetPosition());				// �|�C���g���C�g�p�̍��W�X�V
				Cam2D->Update();
			}
			// ���^�C�A�Ȃ̂ŏI���t���O�𗧂Ă�
			if (m_PauseMenuControl.IsRetire()) {
				SetSceneEnd();
				this->m_IsEnd = true;
			}
			// �O�b�hor�o�b�h�G���h�̃J�b�g�V�[�����I������̂ŏI���t���O�𗧂Ă�
			if (this->m_IsGoodEnd || this->m_IsBadEnd) {
				if (!m_CutSceneControl.IsCutScene()) {
					SetSceneEnd();
					this->m_IsEnd = true;
				}
			}
			if (this->m_IsEndUpdate && FadeInParts->IsFadeAll()) { return false; }// ����̃t���O�������������Ńt�F�[�h�A�E�g���������玟�̃V�[���֑J��
			// ����ȊO�̓V�[������
			return true;
		}
		void			MainGameScene::Dispose_Sub(void) noexcept {
			auto* SaveDataParts = SaveDataClass::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* SoundParts = SoundSystem::SoundPool::Instance();
			SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Normal))->StopAll();
			SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Caution))->StopAll();
			SoundParts->Get(SoundSystem::SoundType::BGM, static_cast<int>(BGMSelect::Alert))->StopAll();
			// ���\�[�X
			PlayerMngr->Dispose();
			BackGround->Dispose();
			// �Z�[�u
			if (!this->m_IsGoodEnd) {
				// �Ō�ɖK�ꂽ�}�b�v
				SaveDataParts->SetParam("LastMap", std::stoi(this->m_MapName.substr(3)));
				SaveDataParts->SetParam("LastEntry", this->m_EntryID);
			}
			else {
				// �N���A�����̂Ń��Z�b�g
				SaveDataParts->Dispose();
			}
			SaveDataParts->Save();
			if (this->m_IsEnd) {// �^�C�g���ɖ߂�
				SetNextSelect(0);
			}
			else {// ���̃V�[����
				SetNextSelect(1);
			}
			// �G���h�����p�t���O�̃��Z�b�g
			this->m_IsEnd = false;
			this->m_IsGoodEnd = false;
			this->m_IsBadEnd = false;
		}
		// 
		void			MainGameScene::DrawMain_Sub(void) const noexcept {
			auto* DrawParts = WindowSizeControl::Instance();
			auto* EventParts = EventDataBase::Instance();
			auto* PlayerMngr = PlayerManager::Instance();
			auto* Obj2DParts = Object2DManager::Instance();
			auto* BackGround = BackGroundClassBase::Instance();
			auto* DrawCtrls = UISystem::DrawControl::Instance();
			auto* Effect2DParts = Effect2DControl::Instance();

			// ���F����\���p�̃o�b�t�@�[��`��
			{
				auto Prev = GetDrawScreen();
				this->m_ViewHandle.SetDraw_Screen(false);
				{
					DrawBox(0, 0, DrawParts->GetScreenY(BaseScreenWidth), DrawParts->GetScreenY(BaseScreenHeight), White, true);
					// ���E
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
						BackGround->GetShadowGraph().DrawExtendGraph(0, 0, DrawParts->GetScreenY(BaseScreenWidth), DrawParts->GetScreenY(BaseScreenHeight), false);
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
			Effect2DParts->Draw();
			BackGround->DrawFront();

			for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
				auto& p = PlayerMngr->GetPlayer((PlayerID)i);
				if (p->GetChara()) {
					if (!p->GetChara()->CanLookPlayer0()) {
						continue;
					}
					int R = Cam2DControl::GetTileToDispSize(1.f);
					Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(p->GetChara()->GetPosition());
					// �͈͊O
					if (!IsOnScreen(DispPos, R)) {
						continue;
					}
					// �̗̓o�[
					{
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
						int xmin = -50;
						int ymin = -50;
						int xmax = 50;
						int xper = xmin + (xmax - xmin) * p->GetChara()->GetHitPoint() / p->GetChara()->GetHitPointMax();
						DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xmax), static_cast<int>(DispPos.y + ymin), Gray75, 10);
						DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xper), static_cast<int>(DispPos.y + ymin), Green, 10);
					}
					// !�}�[�N
					if (i != 0) {
						int XP = static_cast<int>(DispPos.x);
						int YP = static_cast<int>(DispPos.y) - 32;
						{
							if (p->GetAI()->GetGraphAlpha() <= 0.f) { continue; }
							int ShadowOfset = 3;
							if (p->GetAI()->IsAlert()) {
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Alert, XP + ShadowOfset, YP + ShadowOfset, p->GetAI()->GetGraphAlpha(), 0.f, true);
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Alert, XP, YP, p->GetAI()->GetGraphAlpha(), 0.f, true);
							}
							else if (p->GetAI()->IsCaution()) {
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, XP + ShadowOfset, YP + ShadowOfset, p->GetAI()->GetGraphAlpha(), 0.f, true);
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, XP, YP, p->GetAI()->GetGraphAlpha(), 0.f, true);
							}
							else {
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, XP + ShadowOfset, YP + ShadowOfset, p->GetAI()->GetGraphAlpha(), 0.f, true);
								DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 255, 0);// 
								DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Caution, XP, YP, p->GetAI()->GetGraphAlpha(), 0.f, true);
							}
						}
					}
				}
			}
			DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
			// 
			if (this->m_GoalPos.x != -1.f && this->m_GoalPos.y != -1.f) {
				int ShadowOfset = 3;
				auto& p = PlayerMngr->GetPlayer((PlayerID)0);
				if (p->GetChara()) {
					float Len = (this->m_GoalPos - p->GetChara()->GetPosition()).magnitude() / 1.f;
					float Rad = GetRadVec(this->m_GoalPos - p->GetChara()->GetPosition());
					if (Len > 1.f / 255.f) {
						DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(255.f * Len), 0, 255));
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);
						DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, BaseScreenWidth / 2 + ShadowOfset, BaseScreenHeight / 2 + ShadowOfset, 1024.f / 400.f, Rad, true);
						DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
						DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, BaseScreenWidth / 2, BaseScreenHeight / 2, 1024.f / 400.f, Rad, true);
						DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
					}
				}
			}
			for (auto& e : EventParts->GetEventChip()) {
				// ���}�b�v�ւ̑J��
				if (e.m_EventType == EventType::CutScene) {
					if (static_cast<int>(m_StartTime) < e.m_ActiveDelaySec) {
						continue;
					}
					auto* SaveDataParts = SaveDataClass::Instance();
					std::string SaveStr = "Cut_" + std::to_string(e.m_CutSceneID);
					if (SaveDataParts->GetParam(SaveStr) == -1) {
						auto& p = PlayerMngr->GetPlayer((PlayerID)0);

						Vector2DX Pos = BackGround->GetFloorData(e.m_index)->GetTileCenterPos();

						Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(Pos);

						if (!IsOnScreen(DispPos, 0)) {
							int ShadowOfset = 3;
							if (p->GetChara()) {
								float Len = (Pos - p->GetChara()->GetPosition()).magnitude() / 1.f;
								float Rad = GetRadVec(Pos - p->GetChara()->GetPosition());
								if (Len > 1.f / 255.f) {
									DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, std::clamp(static_cast<int>(128.f * std::clamp(Len, 0.f, 1.f)), 0, 255));
									DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 0, 0, 0);
									DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, BaseScreenWidth / 2 + ShadowOfset, BaseScreenHeight / 2 + ShadowOfset, 768.f / 400.f, Rad, true);
									DrawCtrls->SetBright(UISystem::DrawLayer::Normal, 255, 255, 255);
									DrawCtrls->SetDrawRotaGraph(UISystem::DrawLayer::Normal, &this->m_Goal, BaseScreenWidth / 2, BaseScreenHeight / 2, 768.f / 400.f, Rad, true);
									DrawCtrls->SetAlpha(UISystem::DrawLayer::Normal, 255);
								}
							}
						}
						else {
							int ShadowOfset = DrawParts->GetScreenY(3);
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
				// �{�X�̓���ɏo���̗̓o�[
				if (Obj2DParts->HasObj(m_BossUniqueID)) {
					const auto& Obj = Obj2DParts->GetObj(m_BossUniqueID);
					auto& P = (std::shared_ptr<MetalObject>&)(Obj);
					int R = Cam2DControl::GetTileToDispSize(1.f);
					Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(P->GetPosition());
					// �͈͊O
					if (!IsOnScreen(DispPos, R)) {
						return;
					}
					int xmin = -50;
					int ymin = -50;
					int xmax = 50;
					int xper = xmin + (xmax - xmin) * P->GetHitPoint() / P->GetHitPointMax();
					DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xmax), static_cast<int>(DispPos.y + ymin), Gray75, 10);
					DrawCtrls->SetDrawLine(UISystem::DrawLayer::Normal, static_cast<int>(DispPos.x + xmin), static_cast<int>(DispPos.y + ymin), static_cast<int>(DispPos.x + xper), static_cast<int>(DispPos.y + ymin), Green, 10);
				}
			}
		}
		void			MainGameScene::DrawUI_Base_Sub(void) const noexcept {
			auto* FadeInParts = UI::FadeControl::Instance();
			m_MapNameDrawControl.Draw();
			FadeInParts->DrawFade();
			m_CutSceneControl.DrawCut();
		}
		void			MainGameScene::DrawUI_In_Sub(void) const noexcept {
			m_PauseMenuControl.Draw();
		}
		// UI
		void			MainGameScene::DrawCharaUI_Back(PlayerID value) const noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			auto& p = PlayerMngr->GetPlayer(value);

			if (!p->GetChara()) { return; }
			float Radius = 10.f;
			int R = Cam2DControl::GetTileToDispSize(Radius);
			Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(p->GetChara()->GetPosition());
			// �͈͊O
			if (!IsOnScreen(DispPos, R)) {
				return;
			}
			double Deg = (double)-p->GetChara()->GetViewRad() / (DX_PI * 2.0) * 100.0 + 100.0;// �Q�[�W��-100~100�͈̔͂Ȃ̂�+100
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
			auto* FadeInParts = UI::FadeControl::Instance();
			if (!this->m_IsEndUpdate) {
				FadeInParts->SetFadeOut();
			}
			this->m_IsEndUpdate = true;
		}
	}
}
