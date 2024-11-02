#include	"CPU.hpp"
#include	"Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		enum class ENUM_AI_PHASE {
			Normal,		// ���ɋC�ɂ�������
			Check,		// �������������̂ł�����ɂ��
			Caution,	// �x�����Ȃ��珄��
			Alert,		// ���������̂Œǂ�
			Dead,
		};
		// AI�p
		class AIControl::Impl {
		private:
			// �o�H�T��
			class PathChecker {
			private:
				// �o�H�T�������p�̂P�n�_�̏��
				class PathplanningUnit {
				private:
					int						m_Index{ 0 };					// �ԍ�
					PathplanningUnit* m_NextUnit{ nullptr };			// �o�H�T���Ŋm�肵���o�H��̈��̒n�_( ���n�_���o�H��ɖ����ꍇ�� nullptr )
					PathplanningUnit* m_PrevUnit{ nullptr };			// �o�H�T���Ŋm�肵���o�H��̈�O�̒n�_( ���n�_���o�H��ɖ����ꍇ�� nullptr )
					PathplanningUnit* m_ActiveNextUnit{ nullptr };	// �o�H�T�������ΏۂɂȂ��Ă��鎟�̒n�_�̃������A�h���X���i�[����ϐ�
					float					m_TotalDistance{ 0.f };			// �o�H�T���ł��̒n�_�ɓ��B����܂łɒʉ߂����n�_�Ԃ̋����̍��v
				public:
					const auto& GetIndex(void) const noexcept { return this->m_Index; }
					const auto& GetNextUnit(void) const noexcept { return this->m_NextUnit; }
				public:
					void			Reset(int index) noexcept {
						this->m_Index = index;
						this->m_TotalDistance = 0.0f;
						this->m_PrevUnit = nullptr;
						this->m_NextUnit = nullptr;
						this->m_ActiveNextUnit = nullptr;
					}
				public:
					static bool		SetStartToGoal(const std::vector<PathplanningUnit>& UnitArray, PathplanningUnit* pStart, PathplanningUnit* pGoal) noexcept {
						auto* BackGround = BackGroundClassBase::Instance();
						PathplanningUnit* ActiveFirstUnit = pStart;		// �o�H�T�������Ώۂ̒n�_�Ƃ��ăX�^�[�g�n�_�ɂ���n�_��o�^����
						// �o�H��T�����ăS�[���n�_�̒n�_�ɂ��ǂ蒅���܂Ń��[�v���J��Ԃ�
						while (true) {
							bool Goal = false;
							// �o�H�T�������ΏۂɂȂ��Ă���n�_�����ׂď���
							PathplanningUnit* PUnit = ActiveFirstUnit;
							ActiveFirstUnit = nullptr;
							while (true) {
								// �n�_�̕ӂ̐������J��Ԃ�
								auto& Link = BackGround->GetFloorData(PUnit->GetIndex())->GetLinkIndex();
								// �t������
								for (auto i = Link.rbegin(), e = Link.rend(); i != e; ++i) {
									int Index = *i;
									if (Index == InvalidID) { continue; }								// �ӂɗאڂ���n�_�������ꍇ�͉������Ȃ�
									if (Index == pStart->GetIndex()) { continue; }				// �X�^�[�g�n�_�̒n�_�������ꍇ�͉������Ȃ�
									PathplanningUnit* NowUnit = (PathplanningUnit*)&UnitArray.at(static_cast<size_t>(Index));
									// Prev����
									{
										auto trisdistance = PUnit->m_TotalDistance + (BackGround->GetFloorData(Index)->GetTileCenterPos() + BackGround->GetFloorData(PUnit->GetIndex())->GetTileCenterPos()).magnitude();
										if (NowUnit->m_TotalDistance > trisdistance) {		// �אڂ���n�_�����Ɍo�H�T���������s���Ă��āA����苗���̒����o�H�ƂȂ��Ă���ꍇ�͉������Ȃ�
											NowUnit->m_TotalDistance = trisdistance;		// �אڂ���n�_�ɂ����ɓ��B����܂ł̋�����������
										}
										else {
											if (NowUnit->m_PrevUnit) { continue; }
										}
										NowUnit->m_PrevUnit = PUnit;// �אڂ���n�_�Ɍo�H���ƂȂ鎩���̒n�_�̔ԍ���������
									}
									// ���̃��[�v�ōs���o�H�T�������Ώۂɒǉ�����A���ɒǉ�����Ă�����ǉ����Ȃ�
									{
										PathplanningUnit* PTmp = ActiveFirstUnit;
										while (true) {
											if (!PTmp) { break; }
											if (PTmp == NowUnit) { break; }// ���ɒǉ�����Ƃ�
											PTmp = PTmp->m_ActiveNextUnit;
										}
										if (!PTmp) {
											NowUnit->m_ActiveNextUnit = ActiveFirstUnit;
											ActiveFirstUnit = NowUnit;
										}
									}
									// �אڂ���n�_���S�[���n�_�ɂ���n�_��������S�[���ɒH�蒅�����t���O�𗧂Ă�
									if (Index == pGoal->GetIndex()) {
										Goal = true;
									}
								}
								PUnit = PUnit->m_ActiveNextUnit;
								if (!PUnit) { break; }
							}

							if (!ActiveFirstUnit) { return false; }			// �X�^�[�g�n�_�ɂ���n�_����S�[���n�_�ɂ���n�_�ɒH�蒅���Ȃ��Ƃ������ƂȂ̂� false ��Ԃ�
							if (Goal) { break; }
						}
						// �S�[���n�_�̒n�_����X�^�[�g�n�_�̒n�_�ɒH���Čo�H��̒n�_�Ɏ��Ɉړ����ׂ��n�_�̔ԍ���������
						PathplanningUnit* PUnit = pGoal;
						while (true) {
							auto* PrevPUnitIndex = PUnit;
							PUnit = PUnit->m_PrevUnit;
							PUnit->m_NextUnit = PrevPUnitIndex;
							if (PUnit == pStart) { break; }
						}
						return true;
					}
					static void		SetByPatrol(const std::vector<PathplanningUnit>& UnitArray, int StartPoint, PlayerID MyID) noexcept {
						auto* EventParts = EventDataBase::Instance();
						auto& PatList = EventParts->GetPlayerSpawn().at(static_cast<size_t>(MyID)).m_Patrol;
						PathplanningUnit* Now = (PathplanningUnit*)&UnitArray.at(static_cast<size_t>(PatList.at(static_cast<size_t>(StartPoint))));
						int ListNum = static_cast<int>(PatList.size());
						for (int i = 0; i < ListNum; i++) {
							++StartPoint %= ListNum;
							Now->m_NextUnit = (PathplanningUnit*)&UnitArray.at(static_cast<size_t>(PatList.at(static_cast<size_t>(StartPoint))));
							Now = Now->m_NextUnit;
						}
					}
					static bool		GetNextUnit(const std::vector<PathplanningUnit>& UnitArray, int* NowIndex) noexcept {// �Y�����j�b�g�̎��̐i�H���m�F
						auto* BackGround = BackGroundClassBase::Instance();
						auto* NextPtr = UnitArray.at(static_cast<size_t>(*NowIndex)).GetNextUnit();
						if (!NextPtr) { return false; }
						bool IsHit = false;
						for (const auto& Index : BackGround->GetFloorData(*NowIndex)->GetLinkIndex()) {
							// �����ӂƐڂ��Ă���n�_���ڕW���W��ɑ��݂���n�_�������� �J�n���W����ڕW���W��܂œr�؂�Ȃ��n�_�����݂���
							if (Index == NextPtr->GetIndex()) {
								IsHit = true;
								break;
							}
						}
						if (!IsHit) { return false; }
						*NowIndex = NextPtr->GetIndex();													// �`�F�b�N�Ώۂ��o�H��̍X�Ɉ��̒n�_�ɕύX����
						return true;
					}
				};
			private:
				Vector2DX						m_GoalPosition;					// �ڕW�ʒu
				std::vector<PathplanningUnit>	m_UnitArray;					// �o�H�T�������Ŏg�p����S�n�_�̏��z�񂪊i�[���ꂽ�������̈�̐擪�������A�h���X���i�[����ϐ�
				PathplanningUnit* m_pStartUnit{ nullptr };		// �o�H�̃X�^�[�g�n�_�ɂ���n�_���ւ̃������A�h���X���i�[����ϐ�
				PathplanningUnit* m_pGoalUnit{ nullptr };			// �o�H�̃S�[���n�_�ɂ���n�_���ւ̃������A�h���X���i�[����ϐ�
				int								m_TargetPathPlanningIndex{ 0 };	// ���̒��Ԓn�_�ƂȂ�o�H��̒n�_�̌o�H�T����񂪊i�[����Ă��郁�����A�h���X���i�[����ϐ�
			public:
				auto	CalcNextPosition(const Vector2DX& NowPosition, bool* IsGoal = nullptr) noexcept {
					auto* BackGround = BackGroundClassBase::Instance();
					int NowIndex = BackGround->GetNearestFloors(NowPosition);
					if (!this->m_pGoalUnit || NowIndex != this->m_pGoalUnit->GetIndex()) {						// ���ݏ���Ă���n�_���S�[���n�_�ɂ���n�_�̏ꍇ�͏����𕪊�
						if (IsGoal) { *IsGoal = false; }
						if (NowIndex == this->m_TargetPathPlanningIndex) {											// ���ݏ���Ă���n�_���ړ����Ԓn�_�̒n�_�̏ꍇ�͎��̒��Ԓn�_�����肷�鏈�����s��
							PathplanningUnit::GetNextUnit(this->m_UnitArray, &NowIndex);
							// ���̒��Ԓn�_�����肷��܂ł�蒼���Ȃ��ƃG���[�ɂȂ邩��
							this->m_TargetPathPlanningIndex = NowIndex;
						}
						// �ړ����������肷��A�ړ������͌��݂̍��W���璆�Ԓn�_�̒n�_�̒��S���W�Ɍ���������
						return BackGround->GetFloorData(this->m_TargetPathPlanningIndex)->GetTileCenterPos();
					}
					else {
						if (IsGoal) {
							float Len = 0.5f;
							*IsGoal = (this->m_GoalPosition - NowPosition).sqrMagnitude() < (Len * Len);
						}
						// �����͖ڕW���W
						return this->m_GoalPosition;
					}
				}
			public:
				auto	UpdatePath(const Vector2DX& StartPos, const Vector2DX& GoalPos) noexcept {
					// �w��̂Q�_�̌o�H��T������( �߂�l true:�o�H�\�z���� false:�o�H�\�z���s( �X�^�[�g�n�_�ƃS�[���n�_���q���o�H������������ ) )
					auto* BackGround = BackGroundClassBase::Instance();
					this->m_GoalPosition = GoalPos;			// �S�[���ʒu��ۑ�
					// �o�H�T���p�̒n�_�����i�[���郁�����̈���m�ہA������
					for (auto& p : this->m_UnitArray) {
						p.Reset(static_cast<int>(&p - &this->m_UnitArray.front()));
					}
					// �X�^�[�g�n�_������
					int StartIndex = BackGround->GetNearestFloors(StartPos);
					if (StartIndex == InvalidID) { return false; }
					this->m_pStartUnit = &this->m_UnitArray.at(static_cast<size_t>(StartIndex));
					// �S�[���n�_������
					int GoalIndex = BackGround->GetNearestFloors(this->m_GoalPosition);
					if (GoalIndex == InvalidID) { return false; }
					this->m_pGoalUnit = &this->m_UnitArray.at(static_cast<size_t>(GoalIndex));
					// �S�[���n�_�ɂ���n�_�ƃX�^�[�g�n�_�ɂ���n�_�������������� false ��Ԃ�
					if (GoalIndex == StartIndex) { return false; }
					// �o�H��T��
					if (!PathplanningUnit::SetStartToGoal(this->m_UnitArray, this->m_pStartUnit, this->m_pGoalUnit)) { return false; }
					this->m_TargetPathPlanningIndex = this->m_pStartUnit->GetIndex();	// �ړ��J�n���_�̈ړ����Ԓn�_�̌o�H�T�������X�^�[�g�n�_�ɂ���n�_�̏��
					return true;
				}
				void		UpdateByPatrol(int StartPoint, PlayerID MyID) noexcept {
					auto* EventParts = EventDataBase::Instance();
					// �o�H�T���p�̒n�_�����i�[���郁�����̈���m�ہA������
					for (auto& p : this->m_UnitArray) {
						p.Reset(static_cast<int>(&p - &this->m_UnitArray.front()));
					}
					// �X�^�[�g�n�_������
					this->m_pStartUnit = (PathplanningUnit*)&this->m_UnitArray.at(static_cast<size_t>(EventParts->GetPlayerSpawn().at(static_cast<size_t>(MyID)).m_Patrol.at(static_cast<size_t>(StartPoint))));
					this->m_pGoalUnit = nullptr;
					// �o�H��T��
					PathplanningUnit::SetByPatrol(this->m_UnitArray, StartPoint, MyID);
					this->m_TargetPathPlanningIndex = this->m_pStartUnit->GetIndex();	// �ړ��J�n���_�̈ړ����Ԓn�_�̌o�H�T�������X�^�[�g�n�_�ɂ���n�_�̏��
				}
			public:
				void		Init(void) noexcept {
					auto* BackGround = BackGroundClassBase::Instance();
					this->m_UnitArray.resize(static_cast<size_t>(BackGround->GetXSize() * BackGround->GetYSize()));
				}
				void		Dispose(void) noexcept {
					this->m_UnitArray.clear();
				}
			};
		private:
			std::shared_ptr<CharacterObject>		m_TargetChara{ nullptr };
			std::shared_ptr<CharacterObject>		m_MyChara{ nullptr };
		private:
			// 
			ENUM_AI_PHASE							m_Phase{ ENUM_AI_PHASE::Normal };
			InputControl							m_MyInput;
			float									m_InputRad{ 0.f };
			Vector2DX								m_LastFindPos{};
			float									m_LostTimer{ 0.f };
			float									m_TargetDistance{ 0.f };
			float									m_ShotTimer{ 0.f };
			float									m_GraphTimer{ 0.f };
			// �o�H�T��
			PathChecker								m_PathChecker;
			float									m_PathUpdateTimer{ 0.f };
		public:
			const InputControl& GetAIInput(void) const noexcept { return this->m_MyInput; }
			inline bool			IsCaution(void) const noexcept { return this->m_Phase == ENUM_AI_PHASE::Check || this->m_Phase == ENUM_AI_PHASE::Caution; }
			inline bool			IsAlert(void) const noexcept { return this->m_Phase == ENUM_AI_PHASE::Alert; }
			// 
			void			SetMyChara(const std::shared_ptr<CharacterObject>& MyChara) noexcept { this->m_MyChara = MyChara; }
			void			SetTargetCharacter(const std::shared_ptr<CharacterObject>& TargetChara) noexcept { this->m_TargetChara = TargetChara; }

			const float GetGraphAlpha() const noexcept {
				float StartTimer = 0.25f;
				float EndTimer = 0.25f;

				if (this->m_GraphTimer > 2.f - StartTimer) {
					return 1.f - (this->m_GraphTimer - (2.f - StartTimer)) / StartTimer;
				}
				else if (this->m_GraphTimer > 0.25f) {
					return 1.f;
				}
				if (this->m_GraphTimer > 0.f) {
					return (this->m_GraphTimer - (0.25f - EndTimer)) / EndTimer;
				}
				else {
					return 0.f;
				}
			}
		public:
			Impl(void) noexcept {}
			Impl(const Impl&) = delete;
			Impl(Impl&& o) = delete;
			Impl& operator=(const Impl&) = delete;
			Impl& operator=(Impl&& o) = delete;

			~Impl(void) noexcept {}
		private:
			void		SetGraphActive() noexcept { this->m_GraphTimer = 2.f; }

			auto	GetIsSeeTarget(void) const noexcept {
				if (!this->m_TargetChara) { return false; }
				if (this->m_MyChara->CanLookPlayer0()) {
					if (this->m_TargetDistance < 15.f) {
						Vector2DX Vec = GetVecByRad(this->m_MyChara->GetViewRad());
						Vector2DX vec_a; vec_a = (this->m_TargetChara->GetPosition() - this->m_MyChara->GetPosition()).normalized();
						if (-Vector2DX::Dot(vec_a, Vec) > std::cos(deg2rad(45))) {
							return true;
						}
					}
				}
				return false;
			}
			// 
			void		PatrolPoint(void) noexcept {
				auto* BackGround = BackGroundClassBase::Instance();
				auto* EventParts = EventDataBase::Instance();
				int StartIndex = BackGround->GetNearestFloors(this->m_MyChara->GetPosition());
				auto& PatList = EventParts->GetPlayerSpawn().at(static_cast<size_t>(this->m_MyChara->GetPlayerID())).m_Patrol;
				float Length = 1000000.f;
				int NearestID = InvalidID;
				for (auto& p : PatList) {
					if (StartIndex == p) {
						this->m_PathChecker.UpdateByPatrol(static_cast<int>(&p - &PatList.front()), this->m_MyChara->GetPlayerID());
						return;
					}
					float sqrLen = (BackGround->GetFloorData(p)->GetTileCenterPos() - this->m_MyChara->GetPosition()).sqrMagnitude();
					if (Length > sqrLen) {
						Length = sqrLen;
						NearestID = p;
					}
				}
				// �ڕW�l�V��ɂ��Ȃ��̂ň�ԋ߂��Ƃ���ɖ߂�悤�O���C��
				this->m_PathChecker.UpdatePath(this->m_MyChara->GetPosition(), BackGround->GetFloorData(NearestID)->GetTileCenterPos());
			}
			// 
			void		ChangeNormalPhase(void) noexcept {
				this->m_Phase = ENUM_AI_PHASE::Normal;
				this->SetGraphActive();
			}
			void		ChangeCheckPhase(void) noexcept {
				this->m_LostTimer = 0.f;
				this->m_Phase = ENUM_AI_PHASE::Check;
				this->SetGraphActive();
			}
			void		ChangeCautionPhase(void) noexcept {
				this->m_LostTimer = 10.f;
				this->m_Phase = ENUM_AI_PHASE::Caution;
				this->SetGraphActive();
			}
			void		ChangeAlertPhase(void) noexcept {
				this->m_LostTimer = 10.f;
				this->m_Phase = ENUM_AI_PHASE::Alert;
				this->SetGraphActive();
			}
		private:
			void		Update_Before(void) noexcept {
				auto* DrawParts = DXDraw::Instance();
				auto* PlayerMngr = PlayerManager::Instance();
				if (!this->m_TargetChara) { return; }
				this->m_TargetDistance = (this->m_MyChara->GetPosition() - this->m_TargetChara->GetPosition()).magnitude();
				// ������
				this->m_MyInput.ResetAllInput();
				// �߂��̖�������������
				if (this->m_Phase == ENUM_AI_PHASE::Normal) {
					for (int i = 0; i < PlayerMngr->GetPlayerNum(); i++) {
						auto& p = PlayerMngr->GetPlayer((PlayerID)i);
						if (p->GetChara()) {
							if (i != this->m_MyChara->GetPlayerID() && i != this->m_TargetChara->GetPlayerID()) {
								if (p->GetAI()->IsAlert()) {
									float Len = 10.f;
									auto Vec = p->GetChara()->GetPosition() - this->m_MyChara->GetPosition();
									if (Vec.sqrMagnitude() < Len * Len) {
										ChangeAlertPhase();
										break;
									}
								}
							}
						}
					}
				}
				// 
				this->m_PathUpdateTimer -= DrawParts->GetDeltaTime();
				if (this->m_PathUpdateTimer <= 0.f) {
					if (this->m_Phase == ENUM_AI_PHASE::Normal || this->m_Phase == ENUM_AI_PHASE::Caution) {
						PatrolPoint();
						this->m_PathUpdateTimer += 5.f;
					}
					if (this->m_Phase == ENUM_AI_PHASE::Check) {
						this->m_PathChecker.UpdatePath(this->m_MyChara->GetPosition(), this->m_LastFindPos);
						this->m_PathUpdateTimer += 1.f;
					}
					if (this->m_Phase == ENUM_AI_PHASE::Alert) {
						this->m_PathChecker.UpdatePath(this->m_MyChara->GetPosition(), this->m_TargetChara->GetPosition());
						this->m_PathUpdateTimer += 1.f;
					}
				}
			}
			void		Update_Normal(void) noexcept {
				if (!this->m_TargetChara) { return; }
				// �T��
				{
					this->m_LastFindPos = this->m_PathChecker.CalcNextPosition(this->m_MyChara->GetPosition());
					float Len = 0.1f;
					auto Vec = this->m_LastFindPos - this->m_MyChara->GetPosition();
					if (Vec.sqrMagnitude() > Len * Len) {
						this->m_MyInput.SetInputPADS(PADS::MOVE_W, Vec.y > Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_S, Vec.y < -Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_A, Vec.x < -Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_D, Vec.x > Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::WALK, true);
					}
				}
				// 
				if (GetIsSeeTarget()) {
					if (this->m_TargetDistance < 10.f) {
						ChangeAlertPhase();
					}
					else {
						ChangeCheckPhase();
					}
				}
				else if (this->m_TargetDistance < 2.f + this->m_TargetChara->GetSpeed() * 0.04f) {
					ChangeCheckPhase();
				}
				// 
				this->m_InputRad = -GetRadVec(this->m_LastFindPos - this->m_MyChara->GetPosition());
			}
			void		Update_Check(void) noexcept {
				auto* DrawParts = DXDraw::Instance();
				if (!this->m_TargetChara) { return; }
				// 
				Vector2DX GoingPoint = this->m_LastFindPos;
				// 
				this->m_LostTimer = GetMin(this->m_LostTimer + DrawParts->GetDeltaTime(), 5.f);
				if (this->m_LostTimer == 5.f) {
					// �T��
					{
						bool IsGoal = false;
						GoingPoint = this->m_PathChecker.CalcNextPosition(this->m_MyChara->GetPosition(), &IsGoal);
						float Len = 0.1f;
						auto Vec = GoingPoint - this->m_MyChara->GetPosition();
						if (Vec.sqrMagnitude() > Len * Len) {
							this->m_MyInput.SetInputPADS(PADS::MOVE_W, Vec.y > Len / 3.f);
							this->m_MyInput.SetInputPADS(PADS::MOVE_S, Vec.y < -Len / 3.f);
							this->m_MyInput.SetInputPADS(PADS::MOVE_A, Vec.x < -Len / 3.f);
							this->m_MyInput.SetInputPADS(PADS::MOVE_D, Vec.x > Len / 3.f);
							this->m_MyInput.SetInputPADS(PADS::WALK, true);
						}
						if (IsGoal) {
							ChangeNormalPhase();
						}
					}
				}
				if (GetIsSeeTarget()) {
					if (this->m_TargetDistance < 10.f) {
						ChangeAlertPhase();
					}
					this->m_LastFindPos = this->m_TargetChara->GetPosition();
				}
				else if (this->m_TargetDistance < 1.5f + this->m_TargetChara->GetSpeed() * 0.007f) {
					this->m_LastFindPos = this->m_TargetChara->GetPosition();
				}
				// 
				this->m_InputRad = -GetRadVec(GoingPoint - this->m_MyChara->GetPosition());
			}
			void		Update_Caution(void) noexcept {
				auto* DrawParts = DXDraw::Instance();
				if (!this->m_TargetChara) { return; }
				// �T��
				{
					this->m_LastFindPos = this->m_PathChecker.CalcNextPosition(this->m_MyChara->GetPosition());
					float Len = 0.1f;
					auto Vec = this->m_LastFindPos - this->m_MyChara->GetPosition();
					if (Vec.sqrMagnitude() > Len * Len) {
						this->m_MyInput.SetInputPADS(PADS::MOVE_W, Vec.y > Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_S, Vec.y < -Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_A, Vec.x < -Len / 3.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_D, Vec.x > Len / 3.f);
						// this->m_MyInput.SetInputPADS(PADS::WALK, true);
					}
				}
				// 
				if (GetIsSeeTarget()) {
					if (this->m_TargetDistance < 10.f) {
						ChangeAlertPhase();
					}
					this->m_LostTimer = 10.f;
				}
				else if (this->m_TargetDistance < 1.5f + this->m_TargetChara->GetSpeed() * 0.007f) {
					ChangeAlertPhase();
					this->m_LostTimer = 10.f;
				}
				else {
					this->m_LostTimer = GetMax(this->m_LostTimer - DrawParts->GetDeltaTime(), 0.f);
					if (this->m_LostTimer == 0.f) {
						ChangeNormalPhase();
					}
				}
				// 
				this->m_InputRad = -GetRadVec(this->m_LastFindPos - this->m_MyChara->GetPosition());
			}
			void		Update_Alert(void) noexcept {
				auto* DrawParts = DXDraw::Instance();
				if (!this->m_TargetChara) { return; }

				// �T��
				if (this->m_TargetDistance > 8.5f) {
					this->m_LastFindPos = this->m_PathChecker.CalcNextPosition(this->m_MyChara->GetPosition());

					float Len = 0.1f;
					auto Vec = this->m_LastFindPos - this->m_MyChara->GetPosition();
					if (Vec.sqrMagnitude() > Len * Len) {
						this->m_MyInput.SetInputPADS(PADS::MOVE_W, Vec.y > 0.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_S, Vec.y < 0.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_A, Vec.x < 0.f);
						this->m_MyInput.SetInputPADS(PADS::MOVE_D, Vec.x > 0.f);
					}
				}
				else {
					this->m_LastFindPos = this->m_TargetChara->GetPosition();
					if (this->m_ShotTimer == 0.f) {
						this->m_ShotTimer = 0.5f + static_cast<float>(GetRand(100)) / 100.f;
						this->m_MyInput.SetInputPADS(PADS::SHOT, true);
					}
					else {
						this->m_ShotTimer = GetMax(this->m_ShotTimer - DrawParts->GetDeltaTime(), 0.f);
					}
				}
				// 
				// 
				if (GetIsSeeTarget()) {
					this->m_LostTimer = 10.f;
				}
				else if (this->m_TargetDistance < 1.5f + this->m_TargetChara->GetSpeed() * 0.007f) {
					this->m_LostTimer = 10.f;
				}
				else {
					this->m_LostTimer = GetMax(this->m_LostTimer - DrawParts->GetDeltaTime(), 0.f);
					if (this->m_LostTimer == 0.f) {
						ChangeCautionPhase();
					}
				}
				// 
				this->m_InputRad = -GetRadVec(this->m_LastFindPos - this->m_MyChara->GetPosition());
			}
		public:
			void		Init(void) noexcept {
				auto* PlayerMngr = PlayerManager::Instance();
				this->m_PathUpdateTimer = -static_cast<float>(this->m_MyChara->GetPlayerID()) / static_cast<float>(PlayerMngr->GetPlayerNum()) * 1.f;
				this->m_PathChecker.Init();
			}
			void		Update() noexcept {
				// return;
				auto* DrawParts = DXDraw::Instance();
				this->Update_Before();
				switch (this->m_Phase) {
				case ENUM_AI_PHASE::Normal:
					this->Update_Normal();
					break;
				case ENUM_AI_PHASE::Check:
					this->Update_Check();
					break;
				case ENUM_AI_PHASE::Caution:
					this->Update_Caution();
					break;
				case ENUM_AI_PHASE::Alert:
					this->Update_Alert();
					break;
				case ENUM_AI_PHASE::Dead:
					break;
				default:
					break;
				}
				// 
				this->m_GraphTimer = GetMax(this->m_GraphTimer - DrawParts->GetDeltaTime(), 0.f);
				this->m_MyInput.SetyRad(this->m_InputRad);
			}
			void		Dispose(void) noexcept {
				this->m_PathChecker.Dispose();
				m_TargetChara.reset();
				m_MyChara.reset();
			}
		};
		// 
		AIControl::AIControl(void) noexcept {
			this->m_Param = new Impl;
		}
		AIControl::~AIControl(void) noexcept {
			if (this->m_Param) {
				delete this->m_Param;
				this->m_Param = nullptr;
			}
		}
		// 
		const InputControl& AIControl::GetAIInput(void) const noexcept { return this->GetParam()->GetAIInput(); }
		const float			AIControl::GetGraphAlpha(void) const noexcept { return this->GetParam()->GetGraphAlpha(); }
		bool			AIControl::IsCaution(void) const noexcept { return this->GetParam()->IsCaution(); }
		bool			AIControl::IsAlert(void) const noexcept { return this->GetParam()->IsAlert(); }
		// 
		void			AIControl::SetCharacter(PlayerID MyCharaID, PlayerID TargetCharaID) noexcept {
			auto* PlayerMngr = PlayerManager::Instance();
			this->GetParam()->SetMyChara(PlayerMngr->GetPlayer(MyCharaID)->GetChara());
			this->GetParam()->SetTargetCharacter(PlayerMngr->GetPlayer(TargetCharaID)->GetChara());
		}
		// 
		void			AIControl::Init(void) noexcept { this->GetParam()->Init(); }
		void			AIControl::Update(void) noexcept { this->GetParam()->Update(); }
		void			AIControl::Dispose(void) noexcept { this->GetParam()->Dispose(); }
	}
}
