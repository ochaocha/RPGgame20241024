#include	"BackGround.hpp"

const DXLIB_Sample::Sceneclass::EventDataBase* SingletonBase<DXLIB_Sample::Sceneclass::EventDataBase>::m_Singleton = nullptr;
const DXLIB_Sample::Sceneclass::BackGroundClassBase* SingletonBase<DXLIB_Sample::Sceneclass::BackGroundClassBase>::m_Singleton = nullptr;

namespace DXLIB_Sample {
	namespace Sceneclass {
		void EventDataBase::LoadData(const std::string& SoftImagePath) noexcept {
			auto* BackGround = BackGroundClassBase::Instance();
			std::array<std::pair<int, int>, 8> Dir;
			Dir.at(0) = std::make_pair(-1, 0);
			Dir.at(1) = std::make_pair(0, -1);
			Dir.at(2) = std::make_pair(1, 0);
			Dir.at(3) = std::make_pair(0, 1);

			Dir.at(4) = std::make_pair(-1, -1);
			Dir.at(5) = std::make_pair(1, -1);
			Dir.at(6) = std::make_pair(1, 1);
			Dir.at(7) = std::make_pair(-1, 1);

			int EvtImage = LoadSoftImage(SoftImagePath.c_str());
			int r, g, b;

			this->m_PlayerSpawn.resize(1);// 0はイベントに
			this->m_EventChip.clear();
			for (int x = 0; x < BackGround->GetXSize(); x++) {
				for (int y = 0; y < BackGround->GetYSize(); y++) {
					GetPixelSoftImage(EvtImage, x, BackGround->GetYSize() - 1 - y, &r, &g, &b, nullptr);
					if (r == 255 && g == 0 && b == 0) {
						PlayerPatrol tmp; tmp.m_index = BackGround->GetXYToNum(x, y);
						this->m_PlayerSpawn.emplace_back(tmp);
					}
					else if (r == 192 && g == 168) {// イベント マップ移動とか
						EventChip tmp;
						tmp.m_index = BackGround->GetXYToNum(x, y);
						tmp.m_EventID = b;// 青色部分にイベントIDを埋め込む
						tmp.m_CutSceneID = InvalidID;
						tmp.m_ActiveDelaySec = 0;
						tmp.m_WinCutSceneID = InvalidID;
						this->m_EventChip.emplace_back(tmp);
					}
				}
			}
			// 巡回ルート
			for (auto& s : this->m_PlayerSpawn) {
				if (static_cast<int>(&s - &this->m_PlayerSpawn.front()) == PlayerCharacter) { continue; }
				s.m_Patrol.clear();
				s.m_Patrol.emplace_back(s.m_index);
				while (true) {
					auto XY = BackGround->GetNumToXY(s.m_Patrol.back());
					bool isHitNext = false;
					for (auto& d : Dir) {
						int x = XY.first + d.first;
						int y = XY.second + d.second;
						if (HitPointToRectangle(x, y, 0, 0, BackGround->GetXSize() - 1, BackGround->GetYSize() - 1)) {
							int Num = BackGround->GetXYToNum(x, y);
							GetPixelSoftImage(EvtImage, x, BackGround->GetYSize() - 1 - y, &r, &g, &b, nullptr);
							if (r == 255 && g == 192 && b == 192) {
								bool isHit = false;
								for (auto& p : s.m_Patrol) {
									if (p == Num) {
										isHit = true;
										break;
									}
								}
								if (!isHit) {
									s.m_Patrol.emplace_back(Num);
									isHitNext = true;
									break;
								}
							}
						}
					}
					if (!isHitNext) {
						break;
					}
				}
			}

			DeleteSoftImage(EvtImage);
		}
		void EventDataBase::LoadEventScript(const std::string& LEFT, const std::string& RIGHT) noexcept {
			int targetID = std::stoi(LEFT.substr(0, 3));
			for (auto& e : this->m_EventChip) {
				if (e.m_EventID == targetID) {
					// イベントタイプ
					if (LEFT.find("EvtType") != std::string::npos) {
						for (int i = 0; i < static_cast<int>(EventType::Max); i++) {
							if (RIGHT.find(g_EventStr[i]) != std::string::npos) {
								e.m_EventType = (EventType)i;
								break;
							}
						}
					}
					else if (LEFT.find("EntryCutScene") != std::string::npos) {
						e.m_CutSceneID = std::stoi(RIGHT);
					}
					// 遷移用設定
					else if (LEFT.find("NextStage") != std::string::npos) {
						if (RIGHT == "None") {
							e.m_EntryID = InvalidID;
						}
						else {
							e.m_EntryID = std::stoi(RIGHT.substr(0, 3));
							e.m_MapName = RIGHT.substr(4);
						}
					}
					// カットシーン用設定
					else if (LEFT.find("CutSelect") != std::string::npos) {
						e.m_CutSceneID = std::stoi(RIGHT);
					}
					else if (LEFT.find("ActiveDelaySec") != std::string::npos) {
						e.m_ActiveDelaySec = std::stoi(RIGHT);
					}
					// 
					else if (LEFT.find("WinCut") != std::string::npos) {
						e.m_WinCutSceneID = std::stoi(RIGHT);
					}
					break;
				}
			}
		}

		// 影を描画
		void BackGroundClassBase::Blick::DrawAmbientShadow(float AmbientShadowLength, float AmbientLightRad, const GraphHandle& ShadowChip) const noexcept {
			{
				auto* DrawParts = DXDraw::Instance();
				int R = Cam2DControl::GetTileToDispSize(0.5f + AmbientShadowLength);
				Vector2DX Pos;
				Cam2DControl::ConvertTiletoDisp(GetTileCenterPos(), &Pos);
				// 範囲外
				if (!HitPointToRectangle(
					static_cast<int>(Pos.x), static_cast<int>(Pos.y),
					-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
					return;
				}
			}
			std::array<Vector2DX, 4> Position{ };
			for (int i = 0; i < 4; i++) {
				if (GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }

				Vector2DX Pos1 = this->GetBoxSide(i);
				Vector2DX Pos2 = this->GetBoxSide((i + 1) % 4);

				float radsub = deg2rad(90 * i);
				if (std::cos(AmbientLightRad - radsub) > 0.f) {
					float lightrad1 = AmbientLightRad;
					Cam2DControl::ConvertTiletoDisp(Pos1 + GetVecByRad(lightrad1) * AmbientShadowLength, &Position.at(0));
					Cam2DControl::ConvertTiletoDisp(Pos2 + GetVecByRad(lightrad1) * AmbientShadowLength, &Position.at(1));
					Cam2DControl::ConvertTiletoDisp(Pos2, &Position.at(2));
					Cam2DControl::ConvertTiletoDisp(Pos1, &Position.at(3));
					DrawModiGraph_2D(Position, 3.f, ShadowChip);
				}
			}
		}
		void BackGroundClassBase::Blick::DrawPointShadow(const Vector2DX& PointLightPos, const GraphHandle& ShadowChip) const noexcept {
			float Radius = 255.f;
			{
				auto* DrawParts = DXDraw::Instance();
				int R = Cam2DControl::GetTileToDispSize(0.5f + Radius);
				Vector2DX Pos;
				Cam2DControl::ConvertTiletoDisp(GetTileCenterPos(), &Pos);
				// 範囲外
				if (!HitPointToRectangle(
					static_cast<int>(Pos.x), static_cast<int>(Pos.y),
					-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
					return;
				}
			}
			std::array<Vector2DX, 4> Position{ };
			for (int i = 0; i < 4; i++) {
				if (GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }

				Vector2DX Pos1 = this->GetBoxSide(i);
				Vector2DX Pos2 = this->GetBoxSide((i + 1) % 4);

				float radsub = deg2rad(90 * i);
				if (std::cos((DX_PI_F - GetRadVec((Pos1 + Pos2) / 2.f - PointLightPos)) - radsub) > 0.f) {
					float lightrad1 = DX_PI_F - GetRadVec(Pos1 - PointLightPos);
					float lightrad2 = DX_PI_F - GetRadVec(Pos2 - PointLightPos);
					Cam2DControl::ConvertTiletoDisp(Pos1 + GetVecByRad(lightrad1) * Radius, &Position.at(0));
					Cam2DControl::ConvertTiletoDisp(Pos2 + GetVecByRad(lightrad2) * Radius, &Position.at(1));
					Cam2DControl::ConvertTiletoDisp(Pos2, &Position.at(2));
					Cam2DControl::ConvertTiletoDisp(Pos1, &Position.at(3));
					DrawModiGraph_2D(Position, 3.f, ShadowChip);
				}
			}
		}
		// 線分の間で壁に当たっているか
		float BackGroundClassBase::CheckHideShadow(const Vector2DX& PosA, const Vector2DX& PosB, float Radius) noexcept {
			float Ret = 1.f;
			for (auto& B : this->m_CheckWallBlick) {
				{
					auto* DrawParts = DXDraw::Instance();
					int R = Cam2DControl::GetTileToDispSize(Radius);
					Vector2DX Pos;
					Cam2DControl::ConvertTiletoDisp(B->GetTileCenterPos(), &Pos);
					// 範囲外
					if (!HitPointToRectangle(
						static_cast<int>(Pos.x), static_cast<int>(Pos.y),
						-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
						continue;
					}
				}
				// ヒットするかとヒットした場合の透明度を指定
				float tmp = 1.f;
				if (B->CheckLineHit(PosA, PosB)) {
					tmp = 0.f;
				}
				else if (B->CheckCapsuleHit(PosA, PosB, Radius)) {
					float P = 2.f;
					for (int i = 0; i < 4; i++) {
						if (B->GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }
						float ret = (GetMinLenSegmentToSegment(B->GetBoxSide(i), B->GetBoxSide((i + 1) % 4), PosA, PosB) / Radius);
						if (ret < 1.f) {
							if (P > ret) {
								P = ret;
							}
						}
					}
					if (P <= 1.f) {
						tmp = P;
					}
				}
				if (Ret > tmp) {
					Ret = tmp;
				}
				if (Ret <= 0.f) { break; }
			}
			return Ret;
		}
		// 壁判定
		bool BackGroundClassBase::CheckLinetoMap(const Vector2DX& StartPos, Vector2DX* EndPos, float Radius, bool IsPhysical) const noexcept {
			// プレイヤーの周囲にあるステージ壁を取得する( 検出する範囲は移動距離も考慮する )
			class CheckLines {
				std::array<Vector2DX, 3>		m_Position{ };
				Vector2DX					m_Normal{};
				bool						m_Active{ true };
			public:
				void			SetActive(bool value) noexcept { this->m_Active = value; }
			public:
				const auto& GetPos(int num) const noexcept { return this->m_Position.at(static_cast<size_t>(num)); }
				const auto& GetNormal(void) const noexcept { return this->m_Normal; }
				const auto& IsActive(void) const noexcept { return this->m_Active; }
			public:
				void			CalcNormal(void) noexcept {
					Vector2DX StartP = this->m_Position.at(0);
					Vector2DX EndP = this->m_Position.at(1);
					Vector2DX In = this->m_Position.at(2);
					Vector2DX VecP = EndP - StartP;
					auto Normal = (Vector2DX::vget(VecP.y, -VecP.x) * Vector2DX::Cross(VecP, In - StartP)).normalized();
					this->m_Normal.Set(Normal.x, Normal.y);
				}
			public:
				void			SetUp(const Vector2DX& Pos0, const Vector2DX& Pos1, const Vector2DX& Pos2) noexcept {
					this->m_Active = true;
					this->m_Position.at(0) = Pos0;
					this->m_Position.at(1) = Pos1;
					this->m_Position.at(2) = Pos2;
					this->CalcNormal();
				}
			};
			std::vector<CheckLines>				WallList;
			WallList.reserve(256);
			Vector2DX Min, Max;
			Min.x = GetMin(EndPos->x, StartPos.x) - (Radius * 4);
			Min.y = GetMin(EndPos->y, StartPos.y) - (Radius * 4);
			Max.x = GetMax(EndPos->x, StartPos.x) + (Radius * 4);
			Max.y = GetMax(EndPos->y, StartPos.y) + (Radius * 4);
			// 線のリストを出す
			for (auto& B : this->m_CheckWallBlick) {
				// 範囲外
				if (!B->CheckRectangleHit(Min, Max)) { continue; }
				// 辺を登録
				for (int i = 0; i < 4; i++) {
					if (B->GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }// 辺同士で重なっているので未登録に
					WallList.resize(WallList.size() + 1);
					WallList.back().SetUp(B->GetBoxSide(i), B->GetBoxSide((i + 1) % 4), B->GetBoxSide((i + 2) % 4));
				}
			}
			// 90度のへこみの場合はならす
			for (int loop = 0; loop < 5; loop++) {
				for (auto& w : WallList) {
					for (auto& w2 : WallList) {
						if (&w == &w2) { continue; }
						const Vector2DX* pCenter1 = nullptr;
						const Vector2DX* pCenter2 = nullptr;
						const Vector2DX* pOther1 = nullptr;
						const Vector2DX* pOther2 = nullptr;
						if ((w.GetPos(0) - w2.GetPos(0)).sqrMagnitude() <= 0.00001f) {
							pCenter1 = &w.GetPos(0);
							pCenter2 = &w2.GetPos(0);
							pOther1 = &w.GetPos(1);
							pOther2 = &w2.GetPos(1);
						}
						else if ((w.GetPos(1) - w2.GetPos(0)).sqrMagnitude() <= 0.00001f) {
							pCenter1 = &w.GetPos(1);
							pCenter2 = &w2.GetPos(0);
							pOther1 = &w.GetPos(0);
							pOther2 = &w2.GetPos(1);
						}
						else if ((w.GetPos(0) - w2.GetPos(1)).sqrMagnitude() <= 0.00001f) {
							pCenter1 = &w.GetPos(0);
							pCenter2 = &w2.GetPos(1);
							pOther1 = &w.GetPos(1);
							pOther2 = &w2.GetPos(0);
						}
						else if ((w.GetPos(1) - w2.GetPos(1)).sqrMagnitude() <= 0.00001f) {
							pCenter1 = &w.GetPos(1);
							pCenter2 = &w2.GetPos(1);
							pOther1 = &w.GetPos(0);
							pOther2 = &w2.GetPos(0);
						}
						else { continue; }

						Vector2DX Ct = (*pOther1 + *pOther2) / 2.f;

						if (Vector2DX::Dot(*pOther1 - *pCenter1, *pOther2 - *pCenter2) < 0.f) { continue; }// 鋭角でない

						if (
							(Vector2DX::Dot(w.GetNormal(), *pOther2 - *pCenter2) < 0.f) &&
							(Vector2DX::Dot(w2.GetNormal(), *pOther1 - *pCenter1) < 0.f)
							) {
							continue;
						}// 法線が内向きでない

						// へこみを減らす
						*(Vector2DX*)pCenter1 = Ct;
						*(Vector2DX*)pCenter2 = Ct;
						// *pCenter1 = (*pCenter1 + Ct) / 2.f;
						// *pCenter2 = (*pCenter2 + Ct) / 2.f;

						// ノーマル再演算
						w.CalcNormal();
						w2.CalcNormal();
					}
				}
			}

			Vector2DX VecPt = *EndPos - StartPos;
			int WallCount = 0;
			for (auto& w : WallList) {
				// 外向きなのでノーヒット
				Vector2DX VecP = w.GetPos(1) - w.GetPos(0);
				float crossA = Vector2DX::Cross(VecP, VecPt);
				float crossB = Vector2DX::Cross(VecP, w.GetNormal() * -1.f);
				if (crossA * crossB < 0.f) {
					w.SetActive(false);
				}
				else {
					WallCount++;// ヒット加算
				}
			}
			if (WallCount == 0) { return false; }
			// 壁との当たり判定処理
			bool HitFlag = false;
			for (auto& w : WallList) {
				if (!w.IsActive()) { continue; }
				// 壁とプレイヤーが当たっていなかったら次のカウントへ
				if (GetHitCheckToCapsule(StartPos, *EndPos, Radius, w.GetPos(0), w.GetPos(1), 0.f)) {
					if (!IsPhysical) {
						SEGMENT_SEGMENT_RESULT Result;
						GetSegmenttoSegment(StartPos, *EndPos, w.GetPos(0), w.GetPos(1), &Result);
						*EndPos = Result.SegA_MinDist_Pos;
						*EndPos += Result.SegB_MinDist_Pos;
						*EndPos /= 2.f;
						return true;
					}
					else {
						HitFlag = true;// ここにきたら壁とプレイヤーが当たっているということなので、壁に当たったフラグを立てる
						if (VecPt.sqrMagnitude() >= 0.00001f) {
							// x軸かz軸方向に 0.001f 以上移動した場合は移動したと判定 壁に当たったら壁に遮られない移動成分分だけ移動する
							*EndPos = StartPos + Vector2DX::vget(w.GetNormal().y, -w.GetNormal().x) * Vector2DX::Cross(VecPt, w.GetNormal());
							bool IsHit = false;
							for (auto& w2 : WallList) {
								if (!w2.IsActive()) { continue; }
								if (GetHitCheckToCapsule(StartPos, *EndPos, Radius, w2.GetPos(0), w2.GetPos(1), 0.f)) {
									IsHit = true;
									break;
								}
							}
							if (!IsHit) {
								HitFlag = false;
								break;// どの壁とも当たらなかったということなので壁に当たったフラグを倒した上でループから抜ける
							}
						}
						else {
							break;
						}
					}
				}
			}
			if (!IsPhysical) {
				return false;
			}
			// 上記を経ても壁に当たっていたら壁から押し出す処理を行う
			if (HitFlag) {
				for (int i = 0; i < 8; ++i) {			// 壁からの押し出し処理を試みる最大数だけ繰り返し
					bool HitF = false;
					for (auto& w : WallList) {
						if (!w.IsActive()) { continue; }
						if (!GetHitCheckToCapsule(StartPos, *EndPos, Radius, w.GetPos(0), w.GetPos(1), 0.f)) { continue; }
						*EndPos += w.GetNormal() * 0.0015f;					// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
						bool IsHit = false;
						for (auto& w2 : WallList) {
							if (!w2.IsActive()) { continue; }
							if (GetHitCheckToCapsule(StartPos, *EndPos, Radius, w2.GetPos(0), w2.GetPos(1), 0.f)) {// 当たっていたらループを抜ける
								IsHit = true;
								break;
							}
						}
						if (!IsHit) {// 全ての壁と当たっていなかったらここでループ終了
							break;
						}
						HitF = true;
					}
					if (!HitF) {// 全部の壁で押し出しを試みる前に全ての壁壁と接触しなくなったということなのでループから抜ける
						break;
					}
				}
			}
			return HitFlag;
		}
		// 
		void BackGroundClassBase::Init(const std::string& MapPath) noexcept {
			auto* DrawParts = DXDraw::Instance();

			std::string CommonPath = "data/map/" + MapPath + "/";
			std::array<std::pair<int, int>, 8> Dir;
			Dir.at(0) = std::make_pair(-1, 0);
			Dir.at(1) = std::make_pair(0, -1);
			Dir.at(2) = std::make_pair(1, 0);
			Dir.at(3) = std::make_pair(0, 1);

			Dir.at(4) = std::make_pair(-1, -1);
			Dir.at(5) = std::make_pair(1, -1);
			Dir.at(6) = std::make_pair(1, 1);
			Dir.at(7) = std::make_pair(-1, 1);
			// 
			int ChipNum = 12;
			int r, g, b;
			// どのチップを使うかを決めるデータ
			int MapImage = LoadSoftImage((CommonPath + "col.bmp").c_str());
			GetSoftImageSize(MapImage, &this->m_Xsize, &this->m_Ysize);
			// チップデータ
			int PalImage = LoadSoftImage((CommonPath + "chp.bmp").c_str());
			GetSoftImageSize(PalImage, &ChipNum, nullptr);

			GraphHandle::LoadDiv(CommonPath + "img.bmp", ChipNum, ChipNum, 1, 32, 32, &this->m_MapChip);// 床マップチップ
			GraphHandle::LoadDiv(CommonPath + "wal.bmp", 5 * 2, 5, 2, 32, 32, &this->m_WallChip);// 壁マップチップ
			// 
			struct ChipInfo {
				std::array<unsigned char, 3> m_Color{};
				bool						m_IsWall{ false };
			};
			std::vector<ChipInfo> ChipInfoDatas;
			{
				ChipInfoDatas.resize(static_cast<size_t>(ChipNum));
				for (int x = 0; x < ChipNum; x++) {
					GetPixelSoftImage(PalImage, x, 0, &r, &g, &b, nullptr);
					ChipInfoDatas.at(static_cast<size_t>(x)).m_Color.at(0) = (unsigned char)r;
					ChipInfoDatas.at(static_cast<size_t>(x)).m_Color.at(1) = (unsigned char)g;
					ChipInfoDatas.at(static_cast<size_t>(x)).m_Color.at(2) = (unsigned char)b;
					GetPixelSoftImage(PalImage, x, 1, &r, &g, &b, nullptr);
					ChipInfoDatas.at(static_cast<size_t>(x)).m_IsWall = (r < 128);
				}
			}
			// 
			EventDataBase::Instance()->LoadData(CommonPath + "evt.bmp");
			// 
			this->m_Blick.resize(static_cast<size_t>(this->m_Xsize * this->m_Ysize));
			for (int x = 0; x < this->m_Xsize; x++) {
				for (int y = 0; y < this->m_Ysize; y++) {
					auto& B = this->m_Blick.at(static_cast<size_t>(GetXYToNum(x, y))) = std::make_shared<Blick>(x, y, GetXYToNum(x, y));
					GetPixelSoftImage(MapImage, x, this->m_Ysize - 1 - y, &r, &g, &b, nullptr);
					for (auto& p : ChipInfoDatas) {
						if ((p.m_Color.at(0) == r) && (p.m_Color.at(1) == g) && (p.m_Color.at(2) == b)) {
							// 使用する素材を設定
							B->SetBasePalette(static_cast<int>(&p - &ChipInfoDatas.front()), p.m_IsWall);
							break;
						}
					}
				}
			}
			// 
			this->m_FloorBlick.reserve(this->m_Blick.size());
			this->m_WallBlick.reserve(this->m_Blick.size());
			this->m_CheckWallBlick.reserve(this->m_Blick.size());
			// リンクと壁パレット指定の適用
			for (auto& B : this->m_Blick) {
				auto XY = GetNumToXY(static_cast<int>(&B - &this->m_Blick.front()));
				std::array<bool, 8> LURN = { true,true,true,true,true,true,true,true };
				for (int i = 0; i < 8; i++) {
					int x = XY.first + Dir.at(static_cast<size_t>(i)).first;
					int y = XY.second + Dir.at(static_cast<size_t>(i)).second;
					// 上下左右が壁か否か
					if (HitPointToRectangle(x, y, 0, 0, this->m_Xsize - 1, this->m_Ysize - 1)) {
						LURN.at(static_cast<size_t>(i)) = GetFloorData(x, y)->GetIsWall();
					}
					// リンク確認
					if ((i < static_cast<int>(B->GetLinkIndex().size())) && !LURN.at(static_cast<size_t>(i))) {
						B->SetLinkIndex(i, GetXYToNum(x, y));
					}
				}
				// 
				B->SetWallPallet(LURN);

				// ループ高速化のための保持変数
				if (!B->GetIsWall()) {
					this->m_FloorBlick.emplace_back(B);
				}
				if (B->GetIsWall()) {
					this->m_WallBlick.emplace_back(B);
				}
				if (B->GetIsCheckWall()) {
					this->m_CheckWallBlick.emplace_back(B);
				}
			}
			// 
			DeleteSoftImage(MapImage);
			DeleteSoftImage(PalImage);
			// 
			{
				FileStreamDX FileStream((CommonPath + "dat.txt").c_str());
				while (true) {
					if (FileStream.ComeEof()) { break; }
					auto ALL = FileStream.SeekLineAndGetStr();
					if (ALL == "") { continue; }
					auto LEFT = FileStreamDX::getleft(ALL);
					auto RIGHT = FileStreamDX::getright(ALL);
					if (LEFT == "Name") {
						// マップ名
						this->m_GetMapTextID = std::stoi(RIGHT);
					}
					else {
						// それ以外の記述はイベント関連とする
						EventDataBase::Instance()->LoadEventScript(LEFT, RIGHT);
					}
				}
			}
			// 
			this->m_PointShadowHandle.Make(DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), true);
			this->m_AmbientShadowHandle.Make(DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), true);
			this->m_AmbientShadowHandle.SetDraw_Screen(false);
			{
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
			}
		}
		// 
		void BackGroundClassBase::Update() noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* OptionParts = OPTION::Instance();
			if (OptionParts->GetParamBoolean(EnumSaveParam::Shadow)) {
				this->m_AmbientShadowHandle.SetDraw_Screen(false);
				{
					DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
					Object2DManager::Instance()->DrawShadow();

					for (auto& B : this->m_CheckWallBlick) {
						B->DrawAmbientShadow(this->m_AmbientShadowLength, this->m_AmbientLightRad, this->m_MapChip.at(0));
					}
				}
				GraphFilter(this->m_AmbientShadowHandle.get(), DX_GRAPH_FILTER_GAUSS, 8, 200);
			}
			this->m_PointShadowHandle.SetDraw_Screen(false);
			{
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
				for (auto& B : this->m_CheckWallBlick) {
					B->DrawPointShadow(this->m_PointLightPos, this->m_MapChip.at(0));
				}
			}
			if (OptionParts->GetParamBoolean(EnumSaveParam::Shadow)) {
				GraphFilter(this->m_PointShadowHandle.get(), DX_GRAPH_FILTER_GAUSS, 8, 200);
			}
		}
		// 
		void BackGroundClassBase::Draw(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			int R = Cam2DControl::GetTileToDispSize(0.5f);
			// 床
			for (auto& B : this->m_FloorBlick) {
				Vector2DX DispPos;
				Cam2DControl::ConvertTiletoDisp(B->GetTileCenterPos(), &DispPos);
				// 範囲外
				if (!HitPointToRectangle(
					static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
					-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
					continue;
				}
				this->m_MapChip.at(static_cast<size_t>(B->GetPalette().at(0).GetpaletteNum())).DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<float>(R * 2) / (32 - 1), B->GetPalette().at(0).GetZRad(), FALSE);
			}
			// 影
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
			auto* OptionParts = OPTION::Instance();
			if (OptionParts->GetParamBoolean(EnumSaveParam::Shadow)) {
				this->m_AmbientShadowHandle.DrawExtendGraph(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), false);
			}
			else {
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, TRUE);
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		void BackGroundClassBase::DrawFront(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			float Radius = 0.5f;
			int R = Cam2DControl::GetTileToDispSize(Radius);
			// 
			{
				Vector2DX MinPos, MaxPos;
				Cam2DControl::ConvertTiletoDisp(GetFloorData(0, 0)->GetTileCenterPos(), &MinPos);
				Cam2DControl::ConvertTiletoDisp(GetFloorData(this->m_Xsize - 1, this->m_Ysize - 1)->GetTileCenterPos(), &MaxPos);
				int xmin = static_cast<int>(MinPos.x);
				int ymin = static_cast<int>(MaxPos.y);
				int xmax = static_cast<int>(MaxPos.x);
				int ymax = static_cast<int>(MinPos.y);
				auto DrawBE = [](int p1x, int p1y, int p2x, int p2y, const unsigned int& color, int IsFill) {
					if (p2x < p1x || p2y < p1y) { return; }
					DrawBox(p1x, p1y, p2x, p2y, color, IsFill);
					};
				DrawBE(0, 0, xmin, ymax, Black, TRUE);
				DrawBE(xmin, 0, DrawParts->GetScreenY(1920), ymin, Black, TRUE);
				DrawBE(xmax, ymin, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), Black, TRUE);
				DrawBE(0, ymax, xmax, DrawParts->GetScreenY(1080), Black, TRUE);
			}
			// 壁
			for (auto& B : this->m_WallBlick) {
				Vector2DX DispPos;
				Cam2DControl::ConvertTiletoDisp(B->GetTileCenterPos(), &DispPos);
				// 範囲外
				if (!HitPointToRectangle(
					static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
					-R, -R, DrawParts->GetScreenY(1920) + R, DrawParts->GetScreenY(1080) + R)) {
					continue;
				}
				for (int i = 0; i < 5; i++) {
					if (B->GetPalette().at(static_cast<size_t>(i)).IsActive()) {
						this->m_WallChip.at(static_cast<size_t>(B->GetPalette().at(static_cast<size_t>(i)).GetpaletteNum())).DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<float>(R * 2) / (32 - 1), B->GetPalette().at(static_cast<size_t>(i)).GetZRad(), i != 0);
					}
				}
			}
		}
		// 
		void BackGroundClassBase::Dispose(void) noexcept {
			for (auto& b : this->m_FloorBlick) {
				b.reset();
			}
			this->m_FloorBlick.clear();
			for (auto& b : this->m_WallBlick) {
				b.reset();
			}
			this->m_WallBlick.clear();
			for (auto& b : this->m_CheckWallBlick) {
				b.reset();
			}
			this->m_CheckWallBlick.clear();
			for (auto& b : this->m_Blick) {
				b.reset();
			}
			this->m_Blick.clear();
			for (auto& g : this->m_MapChip) {
				g.Dispose();
			}
			this->m_MapChip.clear();
			for (auto& g : this->m_WallChip) {
				g.Dispose();
			}
			this->m_WallChip.clear();
		}
	}
}

