#include	"BackGround.hpp"

const FPS_n2::Sceneclass::BackGroundClassBase* SingletonBase<FPS_n2::Sceneclass::BackGroundClassBase>::m_Singleton = nullptr;

namespace FPS_n2 {
	namespace Sceneclass {
		// �e��`��
		static void DrawModi(std::array<Vector2DX, 4>& Position, float Add, const GraphHandle& ShadowChip) noexcept {
			// �O�㍶�E�ɑ��点��
			float XMax = -1000000.f;
			float XMin = 1000000.f;
			float YMax = -1000000.f;
			float YMin = 1000000.f;
			int xmin = InvalidID;
			int xmax = InvalidID;
			int ymin = InvalidID;
			int ymax = InvalidID;
			for (auto& p : Position) {
				int i = static_cast<int>(&p - &Position.front());
				if (XMax <= p.x) {
					XMax = p.x;
					xmax = i;
				}
				if (XMin >= p.x) {
					XMin = p.x;
					xmin = i;
				}

				if (YMax <= p.y) {
					YMax = p.y;
					ymax = i;
				}
				if (YMin >= p.y) {
					YMin = p.y;
					ymin = i;
				}
			}
			// 
			XMax = -1000000.f;
			XMin = 1000000.f;
			YMax = -1000000.f;
			YMin = 1000000.f;
			int x2min = InvalidID;
			int x2max = InvalidID;
			int y2min = InvalidID;
			int y2max = InvalidID;
			for (auto& p : Position) {
				int i = static_cast<int>(&p - &Position.front());
				if (XMax <= p.x) {
					if (xmax != i) {
						XMax = p.x;
						x2max = i;
					}
				}
				if (XMin >= p.x) {
					if (xmin != i) {
						XMin = p.x;
						x2min = i;
					}
				}

				if (YMax <= p.y) {
					if (ymax != i) {
						YMax = p.y;
						y2max = i;
					}
				}
				if (YMin >= p.y) {
					if (ymin != i) {
						YMin = p.y;
						y2min = i;
					}
				}
			}
			Position.at(static_cast<size_t>(xmin)).x -= Add;
			Position.at(static_cast<size_t>(ymin)).y -= Add;
			Position.at(static_cast<size_t>(xmax)).x += Add;
			Position.at(static_cast<size_t>(ymax)).y += Add;

			if (x2min != InvalidID) { Position.at(static_cast<size_t>(x2min)).x -= Add; }
			if (y2min != InvalidID) { Position.at(static_cast<size_t>(y2min)).y -= Add; }
			if (x2max != InvalidID) { Position.at(static_cast<size_t>(x2max)).x += Add; }
			if (y2max != InvalidID) { Position.at(static_cast<size_t>(y2max)).y += Add; }

			DrawModiGraph(
				static_cast<int>(Position.at(0).x), static_cast<int>(Position.at(0).y),
				static_cast<int>(Position.at(1).x), static_cast<int>(Position.at(1).y),
				static_cast<int>(Position.at(2).x), static_cast<int>(Position.at(2).y),
				static_cast<int>(Position.at(3).x), static_cast<int>(Position.at(3).y),
				ShadowChip.get(), FALSE);
		}
		void BackGroundClassBase::Blick::DrawAmbientShadow(const Vector2DX& AmbientLightVec, float AmbientLightRad, const GraphHandle& ShadowChip) const noexcept {
			std::array<Vector2DX, 4> Position{ };
			for (int i = 0; i < 4; i++) {
				if (GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }
				Vector2DX postmp1, postmp2;
				Cam2DControl::Convert2DtoDisp(this->GetBoxSide(i),&postmp1);
				Cam2DControl::Convert2DtoDisp(this->GetBoxSide((i + 1) % 4),&postmp2);
				float radsub = deg2rad(90 * i);
				if (std::cos(AmbientLightRad - radsub) > 0.f) {
					Position.at(0).Set(postmp1.x + AmbientLightVec.x, postmp1.y + AmbientLightVec.y);
					Position.at(1).Set(postmp2.x + AmbientLightVec.x, postmp2.y + AmbientLightVec.y);
					Position.at(2) = postmp2;
					Position.at(3) = postmp1;
					DrawModi(Position, 3.f, ShadowChip);
				}
			}
		}
		void BackGroundClassBase::Blick::DrawPointShadow(const Vector2DX& PointLightPos, const GraphHandle& ShadowChip) const noexcept {
			std::array<Vector2DX, 4> Position{ };
			for (int i = 0; i < 4; i++) {
				if (GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }
				Vector2DX postmp1, postmp2;
				Cam2DControl::Convert2DtoDisp(this->GetBoxSide(i), &postmp1);
				Cam2DControl::Convert2DtoDisp(this->GetBoxSide((i + 1) % 4), &postmp2);
				float radsub = deg2rad(90 * i);
				if (std::cos(GetRadVec2Vec((postmp1 + postmp2) / 2.f, PointLightPos) - radsub) > 0.f) {
					auto length = 1920.f;
					float lightrad1 = GetRadVec2Vec(postmp1, PointLightPos);
					float lightrad2 = GetRadVec2Vec(postmp2, PointLightPos);

					Position.at(0).Set(postmp1.x + std::sin(lightrad1) * length, postmp1.y + std::cos(lightrad1) * length);
					Position.at(1).Set(postmp2.x + std::sin(lightrad2) * length, postmp2.y + std::cos(lightrad2) * length);
					Position.at(2) = postmp2;
					Position.at(3) = postmp1;

					DrawModi(Position, 3.f, ShadowChip);
				}
			}
		}
		// �����̊Ԃŕǂɓ������Ă��邩
		float BackGroundClassBase::CheckHideShadow(const Vector2DX& PosA, const Vector2DX& PosB, float Radius) noexcept {
			float Ret = 1.f;
			for (auto& B : this->m_CheckWallBlick) {
				// �͈͊O
				if (!Cam2DControl::Is2DPositionInDisp(B->GetTileCenterPos(), Radius)) { continue; }
				// �q�b�g���邩�ƃq�b�g�����ꍇ�̓����x���w��
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
		// �ǔ���
		bool BackGroundClassBase::CheckLinetoMap(const Vector2DX& StartPos, Vector2DX* EndPos, float Radius, bool IsPhysical) const noexcept {
			std::vector<CheckLines>				WallList;
			WallList.reserve(256);
			Vector2DX Min, Max;
			Min.x = std::min(EndPos->x, StartPos.x) - (Radius * 4);
			Min.y = std::min(EndPos->y, StartPos.y) - (Radius * 4);
			Max.x = std::max(EndPos->x, StartPos.x) + (Radius * 4);
			Max.y = std::max(EndPos->y, StartPos.y) + (Radius * 4);
			// ���̃��X�g���o��
			for (auto& B : this->m_CheckWallBlick) {
				// �͈͊O
				if (!B->CheckRectangleHit(Min, Max)) { continue; }
				// �ӂ�o�^
				for (int i = 0; i < 4; i++) {
					if (B->GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }// �ӓ��m�ŏd�Ȃ��Ă���̂Ŗ��o�^��
					WallList.resize(WallList.size() + 1);
					WallList.back().SetUp(B->GetBoxSide(i), B->GetBoxSide((i + 1) % 4), B->GetBoxSide((i + 2) % 4));
				}
			}
			// 90�x�̂ւ��݂̏ꍇ�͂Ȃ炷
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

						if (Vector2DX::Dot(*pOther1 - *pCenter1, *pOther2 - *pCenter2) < 0.f) { continue; }// �s�p�łȂ�

						if (
							(Vector2DX::Dot(w.GetNormal(), *pOther2 - *pCenter2) < 0.f) &&
							(Vector2DX::Dot(w2.GetNormal(), *pOther1 - *pCenter1) < 0.f)
							) {
							continue;
						}// �@�����������łȂ�

						// �ւ��݂����炷
						*(Vector2DX*)pCenter1 = Ct;
						*(Vector2DX*)pCenter2 = Ct;
						//*pCenter1 = (*pCenter1 + Ct) / 2.f;
						//*pCenter2 = (*pCenter2 + Ct) / 2.f;

						// �m�[�}���ĉ��Z
						w.CalcNormal();
						w2.CalcNormal();
					}
				}
			}

			Vector2DX VecPt = *EndPos - StartPos;
			int WallCount = 0;
			for (auto& w : WallList) {
				// �O�����Ȃ̂Ńm�[�q�b�g
				Vector2DX VecP = w.GetPos(1) - w.GetPos(0);
				float crossA = Vector2DX::Cross(VecP, VecPt);
				float crossB = Vector2DX::Cross(VecP, w.GetNormal() * -1.f);
				if (crossA * crossB < 0.f) {
					w.SetActive(false);
				}
				else {
					WallCount++;// �q�b�g���Z
				}
			}
			if (WallCount == 0) { return false; }
			// �ǂƂ̓����蔻�菈��
			bool HitFlag = false;
			for (auto& w : WallList) {
				if (!w.IsActive()) { continue; }
				// �ǂƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
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
						HitFlag = true;// �����ɂ�����ǂƃv���C���[���������Ă���Ƃ������ƂȂ̂ŁA�ǂɓ��������t���O�𗧂Ă�
						if (VecPt.sqrMagnitude() >= 0.00001f) {
							// x����z�������� 0.001f �ȏ�ړ������ꍇ�͈ړ������Ɣ��� �ǂɓ���������ǂɎՂ��Ȃ��ړ������������ړ�����
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
								break;// �ǂ̕ǂƂ�������Ȃ������Ƃ������ƂȂ̂ŕǂɓ��������t���O��|������Ń��[�v���甲����
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
			// ��L���o�Ă��ǂɓ������Ă�����ǂ��牟���o���������s��
			if (HitFlag) {
				for (int i = 0; i < 8; ++i) {			// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
					bool HitF = false;
					for (auto& w : WallList) {
						if (!w.IsActive()) { continue; }
						if (!GetHitCheckToCapsule(StartPos, *EndPos, Radius, w.GetPos(0), w.GetPos(1), 0.f)) { continue; }
						*EndPos += w.GetNormal() * 0.0015f;					// �������Ă�����K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
						bool IsHit = false;
						for (auto& w2 : WallList) {
							if (!w2.IsActive()) { continue; }
							if (GetHitCheckToCapsule(StartPos, *EndPos, Radius, w2.GetPos(0), w2.GetPos(1), 0.f)) {// �������Ă����烋�[�v�𔲂���
								IsHit = true;
								break;
							}
						}
						if (!IsHit) {// �S�Ă̕ǂƓ������Ă��Ȃ������炱���Ń��[�v�I��
							break;
						}
						HitF = true;
					}
					if (!HitF) {// �S���̕ǂŉ����o�������݂�O�ɑS�Ă̕ǕǂƐڐG���Ȃ��Ȃ����Ƃ������ƂȂ̂Ń��[�v���甲����
						break;
					}
				}
			}
			return HitFlag;
		}

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
			// �}�b�v
			int MapImage = LoadSoftImage((CommonPath + "col.bmp").c_str());
			GetSoftImageSize(MapImage, &this->m_Xsize, &this->m_Ysize);
			int EvtImage = LoadSoftImage((CommonPath + "evt.bmp").c_str());
			int PalImage = LoadSoftImage((CommonPath + "chp.bmp").c_str());
			GetSoftImageSize(PalImage, &ChipNum, nullptr);
			GraphHandle::LoadDiv(CommonPath + "img.bmp", ChipNum, ChipNum, 1, 32, 32, &this->m_MapChip);// ���}�b�v�`�b�v
			GraphHandle::LoadDiv(CommonPath + "wal.bmp", 5 * 2, 5, 2, 32, 32, &this->m_WallChip);// �ǃ}�b�v�`�b�v
			// 
			struct ChipInfo {
				std::array<unsigned char,3> m_Color{};
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
			this->m_PlayerSpawn.resize(1);// 0�̓C�x���g��
			this->m_EventChip.clear();
			// 
			this->m_Blick.resize(static_cast<size_t>(this->m_Xsize* this->m_Ysize));
			for (int x = 0; x < this->m_Xsize; x++) {
				for (int y = 0; y < this->m_Ysize; y++) {
					auto& B = this->m_Blick.at(static_cast<size_t>(GetXYToNum(x, y))) = std::make_shared<Blick>(x, y, GetXYToNum(x, y));
					GetPixelSoftImage(MapImage, x, this->m_Ysize - 1 - y, &r, &g, &b, nullptr);
					for (auto& p : ChipInfoDatas) {
						if ((p.m_Color.at(0) == r) && (p.m_Color.at(1) == g) && (p.m_Color.at(2) == b)) {
							// �g�p����f�ނ�ݒ�
							B->SetBasePalette(static_cast<int>(&p - &ChipInfoDatas.front()), p.m_IsWall);
							break;
						}
					}
					GetPixelSoftImage(EvtImage, x, this->m_Ysize - 1 - y, &r, &g, &b, nullptr);
					if (r == 255 && g == 0 && b == 0) {
						PlayerPatrol tmp;tmp.m_index = GetXYToNum(x, y);
						this->m_PlayerSpawn.emplace_back(tmp);
					}
					else if (r == 192 && g == 168) {// �C�x���g �}�b�v�ړ��Ƃ�
						EventChip tmp;
						tmp.m_index = GetXYToNum(x, y);
						tmp.m_EventID = b;// �F�����ɃC�x���gID�𖄂ߍ���
						tmp.m_CutSceneID = InvalidID;
						tmp.m_ActiveDelaySec = 0;
						tmp.m_WinCutSceneID = InvalidID;
						this->m_EventChip.emplace_back(tmp);
					}
				}
			}
			// ���񃋁[�g
			for (auto& s : this->m_PlayerSpawn) {
				if (static_cast<int>(&s - &this->m_PlayerSpawn.front()) == 0) { continue; }
				s.m_Patrol.clear();
				s.m_Patrol.emplace_back(s.m_index);
				while (true) {
					auto XY = GetNumToXY(s.m_Patrol.back());
					bool isHitNext = false;
					for (auto& d : Dir) {
						int x = XY.first + d.first;
						int y = XY.second + d.second;
						if (HitPointToRectangle(x, y, 0, 0, this->m_Xsize - 1, this->m_Ysize - 1)) {
							int Num = GetXYToNum(x, y);
							GetPixelSoftImage(EvtImage, x, this->m_Ysize - 1 - y, &r, &g, &b, nullptr);
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
			this->m_FloorBlick.reserve(this->m_Blick.size());
			this->m_WallBlick.reserve(this->m_Blick.size());
			this->m_CheckWallBlick.reserve(this->m_Blick.size());
			// �����N�ƕǃp���b�g�w��̓K�p
			for (auto& B : this->m_Blick) {
				auto XY = GetNumToXY(static_cast<int>(&B - &this->m_Blick.front()));
				std::array<bool, 8> LURN = { true,true,true,true,true,true,true,true };
				for (int i = 0; i < 8; i++) {
					int x = XY.first + Dir.at(static_cast<size_t>(i)).first;
					int y = XY.second + Dir.at(static_cast<size_t>(i)).second;
					// �㉺���E���ǂ��ۂ�
					if (HitPointToRectangle(x, y, 0, 0, this->m_Xsize - 1, this->m_Ysize - 1)) {
						LURN.at(static_cast<size_t>(i)) = GetFloorData(x, y)->GetIsWall();
					}
					// �����N�m�F
					if ((i < static_cast<int>(B->GetLinkIndex().size())) && !LURN.at(static_cast<size_t>(i))) {
						B->SetLinkIndex(i, GetXYToNum(x, y));
					}
				}
				// 
				B->SetWallPallet(LURN);

				// ���[�v�������̂��߂̕ێ��ϐ�
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
			DeleteSoftImage(EvtImage);
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
						this->m_GetMapTextID = std::stoi(RIGHT);
					}
					else if (LEFT == "CamSize") {
						this->m_CamScale = std::stof(RIGHT);
					}
					else {
						int targetID = std::stoi(LEFT.substr(0, 3));
						for (auto& e : this->m_EventChip) {
							if (e.m_EventID == targetID) {
								// �C�x���g�^�C�v
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
								// �J�ڗp�ݒ�
								else if (LEFT.find("NextStage") != std::string::npos) {
									if (RIGHT == "None") {
										e.m_EntryID = InvalidID;
									}
									else {
										e.m_EntryID = std::stoi(RIGHT.substr(0, 3));
										e.m_MapName = RIGHT.substr(4);
									}
								}
								// �J�b�g�V�[���p�ݒ�
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
		void BackGroundClassBase::SetupShadow(std::function<void()> AddAmbShadow) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* OptionParts = OPTION::Instance();
			if (OptionParts->GetParamBoolean(EnumSaveParam::shadow)) {
				this->m_AmbientShadowHandle.SetDraw_Screen(false);
				{
					DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
					AddAmbShadow();
					float Radius = 0.5f + (float)(DrawParts->GetScreenY(static_cast<int>(this->m_AmbientShadowLength))) / Cam2DControl::GetTileToDispSize(1.f);
					for (auto& B : this->m_CheckWallBlick) {
						if (!Cam2DControl::Is2DPositionInDisp(B->GetTileCenterPos(), Radius)) { continue; }
						B->DrawAmbientShadow(this->m_AmbientLightVec, this->m_AmbientLightRad, this->m_MapChip.at(0));
					}
				}
				GraphFilter(this->m_AmbientShadowHandle.get(), DX_GRAPH_FILTER_GAUSS, 8, 200);
			}
			this->m_PointShadowHandle.SetDraw_Screen(false);
			{
				DrawBox(0, 0, DrawParts->GetScreenY(1920), DrawParts->GetScreenY(1080), White, true);
				float Radius = 0.5f + (float)(DrawParts->GetScreenY(255)) / Cam2DControl::GetTileToDispSize(1.f);
				for (auto& B : this->m_CheckWallBlick) {
					if (!Cam2DControl::Is2DPositionInDisp(B->GetTileCenterPos(), Radius)) { continue; }
					B->DrawPointShadow(this->m_PointLightPos, this->m_MapChip.at(0));
				}
			}
			if (OptionParts->GetParamBoolean(EnumSaveParam::shadow)) {
				GraphFilter(this->m_PointShadowHandle.get(), DX_GRAPH_FILTER_GAUSS, 8, 200);
			}
		}
		// 
		void BackGroundClassBase::Draw(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			float Radius = 0.5f;
			float Size = static_cast<float>(Cam2DControl::GetTileToDispSize(0.5f) * 2) / (32 - 1);
			// ��
			for (auto& B : this->m_FloorBlick) {
				if (!Cam2DControl::Is2DPositionInDisp(B->GetTileCenterPos(), Radius)) { continue; }
				Vector2DX DispPos;
				Cam2DControl::Convert2DtoDisp(B->GetTileCenterPos(), &DispPos);
				this->m_MapChip.at(static_cast<size_t>(B->GetPalette().at(0).GetpaletteNum())).DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), Size, B->GetPalette().at(0).GetZRad(), FALSE);
			}
			// �e
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
			auto* OptionParts = OPTION::Instance();
			if (OptionParts->GetParamBoolean(EnumSaveParam::shadow)) {
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
			float Size = static_cast<float>(Cam2DControl::GetTileToDispSize(Radius) * 2) / (32 - 1);
			// 
			{
				Vector2DX MinPos, MaxPos;
				Cam2DControl::Convert2DtoDisp(GetFloorData(0, 0)->GetTileCenterPos(), &MinPos);
				Cam2DControl::Convert2DtoDisp(GetFloorData(this->m_Xsize - 1, this->m_Ysize - 1)->GetTileCenterPos(), &MaxPos);
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
			// ��
			for (auto& B : this->m_WallBlick) {
				if (!Cam2DControl::Is2DPositionInDisp(B->GetTileCenterPos(), Radius)) { continue; }
				Vector2DX DispPos;
				Cam2DControl::Convert2DtoDisp(B->GetTileCenterPos(), &DispPos);
				for (int i = 0; i < 5; i++) {
					if (B->GetPalette().at(static_cast<size_t>(i)).IsActive()) {
						this->m_WallChip.at(static_cast<size_t>(B->GetPalette().at(static_cast<size_t>(i)).GetpaletteNum())).DrawRotaGraph(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),Size, B->GetPalette().at(static_cast<size_t>(i)).GetZRad(), i != 0);
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
	};
};

