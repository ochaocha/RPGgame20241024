#include	"CPU.hpp"
#include	"Player.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {
		enum class ENUM_AI_PHASE {
			Normal,		// 特に気にせず巡回
			Check,		// 何かを見つけたのでそちらによる
			Caution,	// 警戒しながら巡回
			Alert,		// 発見したので追う
			Dead,
		};
		// AI用
		class AIControl::Impl {
		private:
			// 経路探索
			class PathChecker {
			private:
				// 経路探索処理用の１地点の情報
				class PathplanningUnit {
				private:
					int						m_Index{ 0 };					// 番号
					PathplanningUnit* m_NextUnit{ nullptr };			// 経路探索で確定した経路上の一つ先の地点( 当地点が経路上に無い場合は nullptr )
					PathplanningUnit* m_PrevUnit{ nullptr };			// 経路探索で確定した経路上の一つ前の地点( 当地点が経路上に無い場合は nullptr )
					PathplanningUnit* m_ActiveNextUnit{ nullptr };	// 経路探索処理対象になっている次の地点のメモリアドレスを格納する変数
					float					m_TotalDistance{ 0.f };			// 経路探索でこの地点に到達するまでに通過した地点間の距離の合計
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
						PathplanningUnit* ActiveFirstUnit = pStart;		// 経路探索処理対象の地点としてスタート地点にある地点を登録する
						// 経路を探索してゴール地点の地点にたどり着くまでループを繰り返す
						while (true) {
							bool Goal = false;
							// 経路探索処理対象になっている地点をすべて処理
							PathplanningUnit* PUnit = ActiveFirstUnit;
							ActiveFirstUnit = nullptr;
							while (true) {
								// 地点の辺の数だけ繰り返し
								auto& Link = BackGround->GetFloorData(PUnit->GetIndex())->GetLinkIndex();
								// 逆向きに
								for (auto i = Link.rbegin(), e = Link.rend(); i != e; ++i) {
									int Index = *i;
									if (Index == InvalidID) { continue; }								// 辺に隣接する地点が無い場合は何もしない
									if (Index == pStart->GetIndex()) { continue; }				// スタート地点の地点だった場合は何もしない
									PathplanningUnit* NowUnit = (PathplanningUnit*)&UnitArray.at(static_cast<size_t>(Index));
									// Prev決定
									{
										auto trisdistance = PUnit->m_TotalDistance + (BackGround->GetFloorData(Index)->GetTileCenterPos() + BackGround->GetFloorData(PUnit->GetIndex())->GetTileCenterPos()).magnitude();
										if (NowUnit->m_TotalDistance > trisdistance) {		// 隣接する地点が既に経路探索処理が行われていて、且つより距離の長い経路となっている場合は何もしない
											NowUnit->m_TotalDistance = trisdistance;		// 隣接する地点にここに到達するまでの距離を代入する
										}
										else {
											if (NowUnit->m_PrevUnit) { continue; }
										}
										NowUnit->m_PrevUnit = PUnit;// 隣接する地点に経路情報となる自分の地点の番号を代入する
									}
									// 次のループで行う経路探索処理対象に追加する、既に追加されていたら追加しない
									{
										PathplanningUnit* PTmp = ActiveFirstUnit;
										while (true) {
											if (!PTmp) { break; }
											if (PTmp == NowUnit) { break; }// 既に追加されとる
											PTmp = PTmp->m_ActiveNextUnit;
										}
										if (!PTmp) {
											NowUnit->m_ActiveNextUnit = ActiveFirstUnit;
											ActiveFirstUnit = NowUnit;
										}
									}
									// 隣接する地点がゴール地点にある地点だったらゴールに辿り着いたフラグを立てる
									if (Index == pGoal->GetIndex()) {
										Goal = true;
									}
								}
								PUnit = PUnit->m_ActiveNextUnit;
								if (!PUnit) { break; }
							}

							if (!ActiveFirstUnit) { return false; }			// スタート地点にある地点からゴール地点にある地点に辿り着けないということなので false を返す
							if (Goal) { break; }
						}
						// ゴール地点の地点からスタート地点の地点に辿って経路上の地点に次に移動すべき地点の番号を代入する
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
					static bool		GetNextUnit(const std::vector<PathplanningUnit>& UnitArray, int* NowIndex) noexcept {// 該当ユニットの次の進路を確認
						auto* BackGround = BackGroundClassBase::Instance();
						auto* NextPtr = UnitArray.at(static_cast<size_t>(*NowIndex)).GetNextUnit();
						if (!NextPtr) { return false; }
						bool IsHit = false;
						for (const auto& Index : BackGround->GetFloorData(*NowIndex)->GetLinkIndex()) {
							// もし辺と接している地点が目標座標上に存在する地点だったら 開始座標から目標座標上まで途切れなく地点が存在する
							if (Index == NextPtr->GetIndex()) {
								IsHit = true;
								break;
							}
						}
						if (!IsHit) { return false; }
						*NowIndex = NextPtr->GetIndex();													// チェック対象を経路上の更に一つ先の地点に変更する
						return true;
					}
				};
			private:
				Vector2DX						m_GoalPosition;					// 目標位置
				std::vector<PathplanningUnit>	m_UnitArray;					// 経路探索処理で使用する全地点の情報配列が格納されたメモリ領域の先頭メモリアドレスを格納する変数
				PathplanningUnit* m_pStartUnit{ nullptr };		// 経路のスタート地点にある地点情報へのメモリアドレスを格納する変数
				PathplanningUnit* m_pGoalUnit{ nullptr };			// 経路のゴール地点にある地点情報へのメモリアドレスを格納する変数
				int								m_TargetPathPlanningIndex{ 0 };	// 次の中間地点となる経路上の地点の経路探索情報が格納されているメモリアドレスを格納する変数
			public:
				auto	CalcNextPosition(const Vector2DX& NowPosition, bool* IsGoal = nullptr) noexcept {
					auto* BackGround = BackGroundClassBase::Instance();
					int NowIndex = BackGround->GetNearestFloors(NowPosition);
					if (!this->m_pGoalUnit || NowIndex != this->m_pGoalUnit->GetIndex()) {						// 現在乗っている地点がゴール地点にある地点の場合は処理を分岐
						if (IsGoal) { *IsGoal = false; }
						if (NowIndex == this->m_TargetPathPlanningIndex) {											// 現在乗っている地点が移動中間地点の地点の場合は次の中間地点を決定する処理を行う
							PathplanningUnit::GetNextUnit(this->m_UnitArray, &NowIndex);
							// 次の中間地点が決定するまでやり直さないとエラーになるかも
							this->m_TargetPathPlanningIndex = NowIndex;
						}
						// 移動方向を決定する、移動方向は現在の座標から中間地点の地点の中心座標に向かう方向
						return BackGround->GetFloorData(this->m_TargetPathPlanningIndex)->GetTileCenterPos();
					}
					else {
						if (IsGoal) {
							float Len = 0.5f;
							*IsGoal = (this->m_GoalPosition - NowPosition).sqrMagnitude() < (Len * Len);
						}
						// 方向は目標座標
						return this->m_GoalPosition;
					}
				}
			public:
				auto	UpdatePath(const Vector2DX& StartPos, const Vector2DX& GoalPos) noexcept {
					// 指定の２点の経路を探索する( 戻り値 true:経路構築成功 false:経路構築失敗( スタート地点とゴール地点を繋ぐ経路が無かった等 ) )
					auto* BackGround = BackGroundClassBase::Instance();
					this->m_GoalPosition = GoalPos;			// ゴール位置を保存
					// 経路探索用の地点情報を格納するメモリ領域を確保、初期化
					for (auto& p : this->m_UnitArray) {
						p.Reset(static_cast<int>(&p - &this->m_UnitArray.front()));
					}
					// スタート地点を決定
					int StartIndex = BackGround->GetNearestFloors(StartPos);
					if (StartIndex == InvalidID) { return false; }
					this->m_pStartUnit = &this->m_UnitArray.at(static_cast<size_t>(StartIndex));
					// ゴール地点を決定
					int GoalIndex = BackGround->GetNearestFloors(this->m_GoalPosition);
					if (GoalIndex == InvalidID) { return false; }
					this->m_pGoalUnit = &this->m_UnitArray.at(static_cast<size_t>(GoalIndex));
					// ゴール地点にある地点とスタート地点にある地点が同じだったら false を返す
					if (GoalIndex == StartIndex) { return false; }
					// 経路を探索
					if (!PathplanningUnit::SetStartToGoal(this->m_UnitArray, this->m_pStartUnit, this->m_pGoalUnit)) { return false; }
					this->m_TargetPathPlanningIndex = this->m_pStartUnit->GetIndex();	// 移動開始時点の移動中間地点の経路探索情報もスタート地点にある地点の情報
					return true;
				}
				void		UpdateByPatrol(int StartPoint, PlayerID MyID) noexcept {
					auto* EventParts = EventDataBase::Instance();
					// 経路探索用の地点情報を格納するメモリ領域を確保、初期化
					for (auto& p : this->m_UnitArray) {
						p.Reset(static_cast<int>(&p - &this->m_UnitArray.front()));
					}
					// スタート地点を決定
					this->m_pStartUnit = (PathplanningUnit*)&this->m_UnitArray.at(static_cast<size_t>(EventParts->GetPlayerSpawn().at(static_cast<size_t>(MyID)).m_Patrol.at(static_cast<size_t>(StartPoint))));
					this->m_pGoalUnit = nullptr;
					// 経路を探索
					PathplanningUnit::SetByPatrol(this->m_UnitArray, StartPoint, MyID);
					this->m_TargetPathPlanningIndex = this->m_pStartUnit->GetIndex();	// 移動開始時点の移動中間地点の経路探索情報もスタート地点にある地点の情報
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
			// 経路探索
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
				// 目標値天井にいないので一番近いところに戻るよう軌道修正
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
				// 初期化
				this->m_MyInput.ResetAllInput();
				// 近くの味方も巻き込む
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
				// 探索
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
					// 探索
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
				// 探索
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

				// 探索
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
