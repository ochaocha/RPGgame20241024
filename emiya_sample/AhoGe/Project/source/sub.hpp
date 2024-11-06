#pragma once
#include	"Header.hpp"
// 
namespace DXLIB_Sample {
	namespace Sceneclass {
		// ---------------------------------------------------------------------------------------------
		// 
		// ---------------------------------------------------------------------------------------------
		// 2D�G�t�F�N�g
		class Effect2DControl : public SingletonBase<Effect2DControl> {
		private:
			friend class SingletonBase<Effect2DControl>;
		private:
			class GuardPos {
			private:
				Vector2DX					m_Pos{};
				float						m_Per{};
				float						m_PerMax{};
				EffectType					m_EffectType{};
			public:
				void			SetEffect(const Vector2DX& Pos2D, EffectType Type, float Per) noexcept {
					this->m_Pos = Pos2D;
					this->m_EffectType = Type;
					this->m_Per = Per;
					this->m_PerMax = Per;
				}
				const auto IsActive() const noexcept { return this->m_Per > 0.f; }
				const auto GetEffectPer() const noexcept { return this->m_Per / this->m_PerMax; }
				const auto GetEffectRotate() const noexcept { return deg2rad(360.f * this->m_Per); }
				const auto GetEffectRadius() const noexcept { return this->m_PerMax * std::pow(1.f - GetEffectPer(), 0.5f); }
				const auto& GetEffectPos() const noexcept { return this->m_Pos; }
				const auto& GetEffectType() const noexcept { return this->m_EffectType; }
			public:
				void			Initialize() noexcept {
					m_Per = 0.f;
				}
				void			Update() noexcept {
					m_Per = GetMax(m_Per - DXLib_ref::Instance()->GetDeltaTime() / 0.5f, 0.f);
				}
			};
		private:
			std::array<GuardPos, 8>		m_GuardPos;
			int							m_GuardPosNum{};
		private:
			// �R���X�g���N�^
			Effect2DControl(void) {}// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			Effect2DControl(const Effect2DControl&) = delete;
			Effect2DControl(Effect2DControl&& o) = delete;
			Effect2DControl& operator=(const Effect2DControl&) = delete;
			Effect2DControl& operator=(Effect2DControl&& o) = delete;
			// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
		public:
			//�G�t�F�N�g��z�u(�ꏊ�A��ށA�G�t�F�N�g�̎������ԂȂǂ̃p���[)
			void			SetEffect(const Vector2DX& Pos2D, EffectType Type, float Per) noexcept {
				m_GuardPos.at(m_GuardPosNum).SetEffect(Pos2D, Type, Per);
				++m_GuardPosNum %= static_cast<int>(m_GuardPos.size());
			}
		public:
			void			Initialize() noexcept {
				for (auto& g : m_GuardPos) {
					g.Initialize();
				}
				m_GuardPosNum = 0;
			}
			void			Update() noexcept {
				for (auto& g : m_GuardPos) {
					g.Update();
				}
			}
			void			Draw() noexcept;
		};
		// �L�������Z���u���[�G�t�F�N�g�̕`�揈���N���X
		class Blur2DControl {
		private:
			class BlurParts {
				Vector2DX	m_Pos;
				float		m_Time{};
				float		m_TimeMax{};
			public:
				auto IsActive(void) const noexcept { return this->m_Time > 0.f; }
				auto GetPer(void) const noexcept { return this->m_Time / this->m_TimeMax; }
				const auto& GetPos(void) const noexcept { return this->m_Pos; }
			public:
				void Set(const Vector2DX& pos, float time) noexcept {
					this->m_Pos = pos;
					this->m_TimeMax = time;
					this->m_Time = this->m_TimeMax;
				}
				void Update(void) noexcept {
					this->m_Time = GetMax(this->m_Time - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
				}
			};
		private:
			std::array<BlurParts, 60>	m_Blur{};
			int				m_BlurNow{ 0 };
		public:
			const auto& GetBlur(void) const noexcept { return this->m_Blur; }
		public:
			// �u���[��������
			void			AddBlur(float Blur, const Vector2DX& Pos, const Vector2DX& Vec) noexcept {
				int Max = static_cast<int>(GetMax(1.f, 60.f * DXLib_ref::Instance()->GetDeltaTime()));
				Vector2DX Goal = Pos - Vec * DXLib_ref::Instance()->GetDeltaTime();
				for (int i = 0; i < Max; i++) {
					this->m_Blur.at(static_cast<size_t>(this->m_BlurNow)).Set(Lerp(Goal, Pos, (static_cast<float>(i) / static_cast<float>(Max))), Blur);
					++this->m_BlurNow %= static_cast<int>(this->m_Blur.size());
				}
			}
		public:
			void			Update(void) noexcept {
				for (auto& b : this->m_Blur) {
					b.Update();
				}
			}
		};
		// �X�g�[���[�̃e�L�X�g��ID�ŊǗ�����N���X
		class StoryTextDataBase : public SingletonBase<StoryTextDataBase> {
		private:
			friend class SingletonBase<StoryTextDataBase>;
		public:
			using TextID = int;
			struct TextData {
				TextID		m_ID{ 0 };		// �Ɖ��ׂ�ID
				char		m_Str[512]{};	// �ێ����Ă����e�L�X�g
			};
		private:
			std::vector<TextData> havehandle;
		private:
			// �R���X�g���N�^
			StoryTextDataBase(void) noexcept;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			StoryTextDataBase(const StoryTextDataBase&) = delete;
			StoryTextDataBase(StoryTextDataBase&& o) = delete;
			StoryTextDataBase& operator=(const StoryTextDataBase&) = delete;
			StoryTextDataBase& operator=(StoryTextDataBase&& o) = delete;
			// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
		public:
			// �Y����ID�̃e�L�X�g���擾
			const char* Get(TextID type) noexcept {
				for (auto& s : this->havehandle) {
					if (s.m_ID == type) {
						return s.m_Str;
					}
				}
				return "";
			}
		public:
			// �ێ����Ă����e�L�X�g�̔j��
			void Dispose(void) noexcept {
				// �N���A�ɂ���
				this->havehandle.clear();
				// �m�ۂ��Ă�����������j��
				this->havehandle.shrink_to_fit();
			}
		};
		// �v���C���[��AI�̓��͂��擾���Ă����N���X
		class InputControl {
		private:
			float				m_yRad{ 0.f };	// �^�ォ�猩����]�l
			unsigned long long	m_Flags{ 0 };	// �r�b�g�t���O�Ń{�^���̉��������Ȃ����擾
			// �r�b�g�t���O�͕ϐ���2�i���Ƃ����Ƃ��̊e���̃[���C�`���t���O�Ƃ��܂� bool�����f�[�^����(�֋X��̖���)���ǂ��ł�
		public:
			//�R���X�g���N�^
			InputControl(void) noexcept {}
			//�R�s�[�R���X�g���N�^
			InputControl(const InputControl& tgt) noexcept {
				this->m_yRad = tgt.m_yRad;
				this->m_Flags = tgt.m_Flags;
			}
			//�f�X�g���N�^
			~InputControl(void) noexcept {}
		public:
			// �S�Ẵt���O�����Z�b�g
			void			ResetAllInput(void) noexcept {
				this->m_yRad = 0.f;
				this->m_Flags = 0;
			}
			// ��]�l��ݒ�
			void			SetyRad(float yRad) noexcept { this->m_yRad = yRad; }
			// ����̃{�^���̃t���O��ݒ�
			void			SetInputPADS(PADS select, bool value) noexcept {
				//true�̏ꍇ�A����n��r�b�g�V�t�g�����ʒu�̃r�b�g��_���a�ŗ��Ă�
				if (value) { this->m_Flags |= ((unsigned long long)1 << (0 + static_cast<int>(select))); }
				//false�̏ꍇ�A����n��r�b�g�V�t�g�����ʒu�̃r�b�g�̔��]�l�Ƃ̘_���ςŐ܂�
				else { this->m_Flags &= ~((unsigned long long)1 << (0 + static_cast<int>(select))); }
			}
		public:
			//��]�l���擾
			const auto& GetyRad(void) const noexcept { return this->m_yRad; }
			//����̃{�^�����͂̐����A�����Ȃ���ݒ�
			auto		GetPADSPress(PADS select) const noexcept { return (this->m_Flags & ((unsigned long long)1 << (0 + static_cast<int>(select)))) != 0; }
		};
		// 2D�J�������Z�@��ʊO�̕`�悪�������邩�ۂ��A�`�掞�̃I�t�Z�b�g�Ȃǂ̉��Z���s���܂�
		class Cam2DControl : public SingletonBase<Cam2DControl> {
		private:
			friend class SingletonBase<Cam2DControl>;
		private:
			Vector2DX					m_Pos{};					// �J�����̌��݂̍��W
		private:// �J�����V�F�C�N�n�̒ǉ�
			float						m_ShakePower{ 0.f };		// �h��̋���
			float						m_ShakeTotalTime{ 0.f };	// �h����N�����g�[�^������
			float						m_ShakeTime{ 0.f };			// �h��̌o�ߎ���(0�b�Ɍ����Č����Ă����܂�)
			Vector2DX					m_CamShake1;
			Vector2DX					m_CamShake2;				// �h����v�Z���邽�߂̕ϐ�
		private:
			// �R���X�g���N�^
			Cam2DControl(void) {}
			Cam2DControl(const Cam2DControl&) = delete;// �R�s�[���Ă͂����Ȃ��̂Œʏ�̃R���X�g���N�^�ȊO�����ׂ�delete
			Cam2DControl(Cam2DControl&& o) = delete;
			Cam2DControl& operator=(const Cam2DControl&) = delete;
			Cam2DControl& operator=(Cam2DControl&& o) = delete;
			// �f�X�g���N�^�̓V���O���g���Ȃ̂ŌĂ΂�܂���
		public:
			const Vector2DX GetCamPos(void) const noexcept { return this->m_Pos + m_CamShake2; }				// ���Ă�����W�{�h�ꂽ����
		public:
			void			SetCamPos(const Vector2DX& Pos) noexcept {
				this->m_Pos = Pos;
			}
			// �ꏊ�̐ݒ��⊮
			void			SetCamAim(const Vector2DX& Pos) noexcept {
				Easing(&this->m_Pos, Pos, 0.9f, EasingType::OutExpo);
			}
			// �h���ݒ肵�܂��B�@�h��鎞�ԂƗh��̗͂�2�_�ł�
			void			SetCamShake(float time, float power) noexcept {
				this->m_ShakeTotalTime = time;
				this->m_ShakePower = power;
				this->m_ShakeTime = this->m_ShakeTotalTime;
			}
		public:
			// ��ԍ��W�����ʏ�ւ̕ϊ�
			static Vector2DX	ConvertTiletoDisp(const Vector2DX& Pos2D) noexcept {
				auto* DrawParts = WindowSizeControl::Instance();
				auto* Cam2D = Cam2DControl::Instance();
				//��ԍ��W�ƃJ�������W�Ƃ̍��W�������擾
				auto CamOffset = Pos2D - Cam2D->GetCamPos();
				//��ʏ�̍����ɕϊ���A��������̈ړ������Ƃ��ďo��
				return Vector2DX::vget(
					static_cast<float>(DrawParts->GetScreenY(BaseScreenWidth / 2)) + Cam2DControl::GetTileToDispSize(CamOffset.x),
					static_cast<float>(DrawParts->GetScreenY(BaseScreenHeight / 2)) - Cam2DControl::GetTileToDispSize(CamOffset.y)
				);
			}
			// ��ԍ��W�n�ł̃T�C�Y�����ʏ�̃T�C�Y���擾
			static int			GetTileToDispSize(float value) noexcept {
				auto* DrawParts = WindowSizeControl::Instance();
				return DrawParts->GetScreenY(static_cast<int>(value * Tile_DispSize));
			}
		public:
			void			Update(void) noexcept {
				//�h��p���[���c�����Ă���ꍇ
				if (this->m_ShakeTotalTime > 0.f) {
					// ���͈̔͂Ń����_���Ȓn�_���擾���A���̕����ɖ��t���[��m_CamShake�𓮂���
					auto RandRange = this->m_ShakeTime / this->m_ShakeTotalTime * this->m_ShakePower;
					// 2��⊮���s�����ƂŎ��R�ȗh������o
					Easing(&this->m_CamShake1, Vector2DX::vget(GetRandf(RandRange), GetRandf(RandRange)), 0.8f, EasingType::OutExpo);
					Easing(&this->m_CamShake2, this->m_CamShake1, 0.8f, EasingType::OutExpo);
					// ���Ԍo�߂Ŏ�܂�悤�ɂ��邽�ߎ��Ԃ��v�����Č��炵�܂�
					this->m_ShakeTime = GetMax(this->m_ShakeTime - DXLib_ref::Instance()->GetDeltaTime(), 0.f);
				}
			}
		};

		static bool IsOnScreen(const Vector2DX& DispPos, int Radius) {
			auto* DrawParts = WindowSizeControl::Instance();
			// �͈͊O
			return HitPointToRectangle(
				static_cast<int>(DispPos.x), static_cast<int>(DispPos.y),
				-Radius, -Radius, DrawParts->GetScreenY(BaseScreenWidth) + Radius, DrawParts->GetScreenY(BaseScreenHeight) + Radius);
		}
	}
}
