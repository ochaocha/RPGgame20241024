#pragma once
#include	"../../Header.hpp"
#include	"../../sub.hpp"

namespace DXLIB_Sample {
	namespace Sceneclass {

		enum class EventType {
			Entry,
			CutScene,
			Boss,
			Max,
		};
		static const char* g_EventStr[static_cast<int>(EventType::Max)] = {
			"Entry",
			"CutScene",
			"Boss",
		};

		class EventDataBase : public SingletonBase<EventDataBase> {
		private:
			friend class SingletonBase<EventDataBase>;
		private:
			EventDataBase(void) {}
			EventDataBase(const EventDataBase&) = delete;
			EventDataBase(EventDataBase&& o) = delete;
			EventDataBase& operator=(const EventDataBase&) = delete;
			EventDataBase& operator=(EventDataBase&& o) = delete;
		private:
			// CPU�ɏ��񂳂��郋�[�g
			struct PlayerPatrol {
				int					m_index{};
				std::vector<int>	m_Patrol{};
			};
			// �C�x���g�`�b�v
			struct EventChip {
				int					m_index{};
				int					m_EventID{ 0 };
				EventType			m_EventType{};
				// �J�ڗp�ݒ�
				std::string			m_MapName;// �s��}�b�v
				int					m_EntryID{};// �s��}�b�v�̃C�x���gID
				// �J�b�g�V�[���p�ݒ�
				int					m_CutSceneID{};// �J�b�g�V�[��ID
				int					m_ActiveDelaySec{};
				// 
				int					m_WinCutSceneID{};// �J�b�g�V�[��ID
			};
		private:
			std::vector<PlayerPatrol>			m_PlayerSpawn;
			std::vector<EventChip>				m_EventChip;
		public:
			const auto& GetPlayerSpawn(void) const noexcept { return this->m_PlayerSpawn; }
			const auto& GetEventChip(void) const noexcept { return this->m_EventChip; }
		public:
			void			LoadData(const std::string& SoftImagePath) noexcept;
			void			LoadEventScript(const std::string& LEFT, const std::string& RIGHT) noexcept;
		};

		class BackGroundClassBase : public SingletonBase<BackGroundClassBase> {
		private:
			friend class SingletonBase<BackGroundClassBase>;
		private:
			class Blick {
			private:
				class BlickPalette {
					int							m_paletteNum{ InvalidID };
					float						m_ZRad{ 0.f };
				public:
					void Set(int pal, float ZRad) noexcept {
						this->m_paletteNum = pal;
						this->m_ZRad = ZRad;
					}
					void Reset() noexcept {
						this->m_paletteNum = InvalidID;
						this->m_ZRad = 0.f;
					}
				public:
					auto IsActive(void) const noexcept { return this->m_paletteNum != InvalidID; }
					const auto& GetpaletteNum(void) const noexcept { return this->m_paletteNum; }
					const auto& GetZRad(void) const noexcept { return this->m_ZRad; }
				};
			private:
				int							m_Index{};
				Vector2DX					m_Pos;
				bool						m_IsWall{ false };
				bool						m_IsCheckWall{ false };// �ǂŏ����`�F�b�N�Ώۂ��ǂ���
				std::array<BlickPalette, 5>	m_Palette{};
				std::array<Vector2DX, 4>	m_BoxSide;
				std::array<int, 8>			m_LinkIndex{};// CPU���΂ߕ������T������悤�ɂ������Ȃ�8�Ɏw��
			public:
				const auto& GetIndex(void) const noexcept { return this->m_Index; }
				const auto& GetTileCenterPos(void) const noexcept { return this->m_Pos; }
				const auto& GetIsWall(void) const noexcept { return this->m_IsWall; }
				const auto& GetIsCheckWall(void) const noexcept { return this->m_IsCheckWall; }
				const auto& GetPalette(void) const noexcept { return this->m_Palette; }
				const auto& GetBoxSide(int num) const noexcept { return this->m_BoxSide.at(static_cast<size_t>(num)); }
				const auto& GetLinkIndex(void) const noexcept { return this->m_LinkIndex; }
			public:// �R���X�g���N�^
				Blick(int x, int y, int index) noexcept {
					this->m_Pos.x = static_cast<float>(x);
					this->m_Pos.y = static_cast<float>(y);
					this->m_Index = index;
					for (auto& Lindex : this->m_LinkIndex) {
						Lindex = InvalidID;
					}
					float HalfLen = 0.5f;
					this->m_BoxSide.at(0).Set(this->m_Pos.x - HalfLen, this->m_Pos.y - HalfLen);
					this->m_BoxSide.at(1).Set(this->m_Pos.x + HalfLen, this->m_Pos.y - HalfLen);
					this->m_BoxSide.at(2).Set(this->m_Pos.x + HalfLen, this->m_Pos.y + HalfLen);
					this->m_BoxSide.at(3).Set(this->m_Pos.x - HalfLen, this->m_Pos.y + HalfLen);
				}
			public:
				void SetBasePalette(int pal, bool IsWall) noexcept {
					this->m_Palette.at(0).Set(pal, 0.f);
					for (int i = 0; i < 4; i++) {
						this->m_Palette.at(static_cast<size_t>(i + 1)).Reset();
					}
					this->m_IsWall = IsWall;
					this->m_IsCheckWall = this->m_IsWall;
				}
				void SetWallPallet(std::array<bool, 8> LURN) noexcept {
					if (!this->m_IsWall) { return; }
					int WayCount = 0;
					for (int i = 0; i < 4; i++) { if (LURN.at(static_cast<size_t>(i))) { WayCount++; } }
					switch (WayCount) {
					case 0:
						this->m_Palette.at(0).Set(0, 0.f);
						break;
					case 1:
						for (int i = 0; i < 4; i++) {
							if (LURN.at(static_cast<size_t>(i))) {
								this->m_Palette.at(0).Set(1, deg2rad(-90 * i + 90));
								break;
							}
						}
						break;
					case 2:
					{
						bool IsHit = false;
						for (int i = 0; i < 4; i++) {
							if (!LURN.at(static_cast<size_t>(i)) && !LURN.at(static_cast<size_t>((i + 1) % 4))) {
								this->m_Palette.at(0).Set(2 + 5, deg2rad(-90 * i));
								IsHit = true;
								break;
							}
						}
						if (!IsHit) {
							for (int i = 0; i < 4; i++) {
								if (!LURN.at(static_cast<size_t>(i))) {
									this->m_Palette.at(0).Set(2, deg2rad(90 * i - 90));
									break;
								}
							}
						}
					}
					break;
					case 3:
						for (int i = 0; i < 4; i++) {
							if (!LURN.at(static_cast<size_t>(i))) {
								this->m_Palette.at(0).Set(3, deg2rad(-90 * i + 90));
								break;
							}
						}
						break;
					case 4:
						this->m_Palette.at(0).Set(4, 0.f);
						this->m_IsCheckWall = false;
						break;
					default:
						break;
					}

					for (int i = 0; i < 4; i++) {
						if (LURN.at(static_cast<size_t>(i)) && LURN.at(static_cast<size_t>((i + 1) % 4)) && !LURN.at(static_cast<size_t>((i + 4) % 8))) {
							this->m_Palette.at(static_cast<size_t>(i + 1)).Set(9, deg2rad(-90 * i));
						}
					}
				}
				void SetLinkIndex(int num, int index) noexcept { this->m_LinkIndex.at(static_cast<size_t>(num)) = index; }
			public:// �����蔻��
				bool CheckPointHit(const Vector2DX& Pos) const noexcept {
					return HitPointToRectangle(
						static_cast<int>(Pos.x), static_cast<int>(Pos.y),
						static_cast<int>(this->GetBoxSide(0).x), static_cast<int>(this->GetBoxSide(0).y),
						static_cast<int>(this->GetBoxSide(2).x), static_cast<int>(this->GetBoxSide(2).y));
				}
				bool CheckRectangleHit(const Vector2DX& Min, const Vector2DX& Max) const noexcept {
					return  HitRectangleToRectangle(
						static_cast<int>(Min.x), static_cast<int>(Min.y),
						static_cast<int>(Max.x), static_cast<int>(Max.y),
						static_cast<int>(this->GetBoxSide(0).x), static_cast<int>(this->GetBoxSide(0).y),
						static_cast<int>(this->GetBoxSide(2).x), static_cast<int>(this->GetBoxSide(2).y));
				}
				bool CheckCapsuleHit(const Vector2DX& Pos1, const Vector2DX& Pos2, float Radius) const noexcept {
					for (int i = 0; i < 4; i++) {
						if (GetLinkIndex().at(static_cast<size_t>((i + 1) % 4)) == InvalidID) { continue; }
						if (GetHitCheckToCapsule(Pos1, Pos2, Radius, this->GetBoxSide(i), this->GetBoxSide((i + 1) % 4), 0.f)) {
							return true;
						}
					}
					return false;
				}
				bool CheckLineHit(const Vector2DX& Pos1, const Vector2DX& Pos2) const noexcept { return CheckCapsuleHit(Pos1, Pos2, 0.001f); }
			public:
				// ���e��`��
				void			DrawAmbientShadow(float AmbientShadowLength, float AmbientLightRad, const GraphHandle& ShadowChip) const noexcept;
				// �|�C���g�e��`��
				void			DrawPointShadow(const Vector2DX& PointLightPos, const GraphHandle& ShadowChip)  const noexcept;
			};
		private:
			int									m_Xsize{};
			int									m_Ysize{};
			std::vector<std::shared_ptr<Blick>>	m_Blick;

			// ���[�v�������̂��߂̕ێ��ϐ�
			std::vector<std::shared_ptr<Blick>>	m_FloorBlick;
			std::vector<std::shared_ptr<Blick>>	m_WallBlick;
			std::vector<std::shared_ptr<Blick>>	m_CheckWallBlick;

			std::vector<GraphHandle>			m_MapChip;
			std::vector<GraphHandle>			m_WallChip;
			float								m_AmbientShadowLength{ 36.f };
			float								m_AmbientLightRad = deg2rad(45);
			Vector2DX							m_PointLightPos;
			GraphHandle							m_PointShadowHandle;
			GraphHandle							m_AmbientShadowHandle;
			int									m_GetMapTextID{};
		public:
			const auto& GetMapTextID(void) const noexcept { return this->m_GetMapTextID; }
			const auto& GetShadowGraph(void) const noexcept { return this->m_PointShadowHandle; }
			const auto		GetAmbientLightVec(void) const noexcept { return GetVecByRad(this->m_AmbientLightRad) * this->m_AmbientShadowLength; }
			auto			GetXSize(void) const noexcept { return this->m_Xsize; }
			auto			GetYSize(void) const noexcept { return this->m_Ysize; }
			auto			GetXYToNum(int x, int y) const noexcept { return GetMin(x, this->m_Xsize - 1) * this->m_Ysize + GetMin(y, this->m_Ysize - 1); }
			auto			GetNumToXY(int num) const noexcept { return std::make_pair<int, int>(num / this->m_Ysize, num % this->m_Ysize); }
			const auto& GetFloorData(int num) const noexcept { return this->m_Blick.at(static_cast<size_t>(num)); }
			const auto& GetFloorData(int x, int y) const noexcept { return GetFloorData(GetXYToNum(x, y)); }
			auto			GetNearestFloors(const Vector2DX& Pos) const noexcept {
				int x = static_cast<int>(Pos.x + 0.5f);
				int y = static_cast<int>(Pos.y + 0.5f);
				return GetXYToNum(x, y);
			}
			float			CheckHideShadow(const Vector2DX& PosA, const Vector2DX& PosB, float Radius) noexcept;
			bool			CheckLinetoMap(const Vector2DX& StartPos, Vector2DX* EndPos, float Radius, bool IsPhysical) const noexcept;
		public:
			void			SetAmbientLight(float ShadowLen, float Rad) noexcept {
				this->m_AmbientShadowLength = ShadowLen;
				this->m_AmbientLightRad = Rad;
			}
			void			SetPointLight(const Vector2DX& Pos) noexcept { this->m_PointLightPos = Pos; }
		private:
			BackGroundClassBase(void) {}
			BackGroundClassBase(const BackGroundClassBase&) = delete;
			BackGroundClassBase(BackGroundClassBase&& o) = delete;
			BackGroundClassBase& operator=(const BackGroundClassBase&) = delete;
			BackGroundClassBase& operator=(BackGroundClassBase&& o) = delete;
		public:
			void			Init(const std::string& MapPath) noexcept;
			void			Update() noexcept;
			void			Draw(void) noexcept;
			void			DrawFront(void) noexcept;
			void			Dispose(void) noexcept;
		};
	}
}
