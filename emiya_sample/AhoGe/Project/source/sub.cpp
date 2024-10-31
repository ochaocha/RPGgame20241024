#include	"sub.hpp"

#include	"MainScene/Player/Player.hpp"

// �V���O���g���̎��Ԓ�`
const FPS_n2::Sceneclass::Effect2DControl* SingletonBase<FPS_n2::Sceneclass::Effect2DControl>::m_Singleton = nullptr;
const FPS_n2::Sceneclass::LocalizePool* SingletonBase<FPS_n2::Sceneclass::LocalizePool>::m_Singleton = nullptr;
const FPS_n2::Sceneclass::Cam2DControl* SingletonBase<FPS_n2::Sceneclass::Cam2DControl>::m_Singleton = nullptr;

namespace FPS_n2 {
	namespace Sceneclass {
		// �R���X�g���N�^�Ńe�L�X�g�f�[�^�𐶐�
		LocalizePool::LocalizePool(void) noexcept {
			// ���炩���߈��ʖڐ����m�ۂ��Ă���
			this->havehandle.reserve(256);
			// 1�s���t�@�C������͂��A���[�J���C�YID�ɍ��v�����e�L�X�g��ۑ�����
			FileStreamDX FileStream("data/Localize/Jpn.txt");
			while (true) {
				if (FileStream.ComeEof()) { break; }
				auto ALL = FileStream.SeekLineAndGetStr();
				if (ALL == "") {
					continue;
				}
				// �C�R�[��(=)�̍���ID�A�E���e�L�X�g�Ƃ��Ď擾
				LocalizeID ID = (LocalizeID)(std::stoi(FileStreamDX::getleft(ALL)));
				std::string Text = FileStreamDX::getright(ALL);
				// �������ł�ID���o�^�ςł���΁A���s�Ƃ݂Ȃ��Ċ����e�L�X�g�̖����ɒǉ�����
				bool IsHit = false;
				for (auto& h : this->havehandle) {
					if (h.m_ID == ID) {
						std::string Before = h.m_Str;
						sprintfDx(h.m_Str, (Before + "\n" + Text).c_str());// ���s���ē����e�L�X�g�Ƃ���
						IsHit = true;
						break;
					}
				}
				// �o�^����Ă��Ȃ��̂ł���ΐV�K�Ƀn���h�������蓖�Ă�
				if (!IsHit) {
					this->havehandle.resize(this->havehandle.size() + 1);
					this->havehandle.back().m_ID = ID;
					sprintfDx(this->havehandle.back().m_Str, Text.c_str());
				}
			}
		}

		const Vector2DX Cam2DControl::GetCamPos(void) const noexcept {
			Vector2DX Shake = CameraShake::Instance()->GetCamShake();
			return this->m_Pos + Shake;
		}
		// 
		void Convert2DtoDisp(const Vector2DX& Pos2D, Vector2DX* pRet) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto& CamPos = Cam2DControl::Instance()->GetCamPos();
			pRet->x = static_cast<float>(DrawParts->GetScreenY(static_cast<int>(1920.f / 2 + (1080.f / 2.f) * (Pos2D.x - CamPos.x) * Cam2DControl::Instance()->GetCamHeight() / Base_CamScale)));
			pRet->y = static_cast<float>(DrawParts->GetScreenY(static_cast<int>(1080.f / 2 - (1080.f / 2.f) * (Pos2D.y - CamPos.y) * Cam2DControl::Instance()->GetCamHeight() / Base_CamScale)));
		}

		bool Is2DPositionInDisp(const Vector2DX& Pos2D, int Radius) noexcept {
			auto* DrawParts = DXDraw::Instance();
			Radius = Radius * 100 / DrawParts->GetScreenY(100);
			auto& CamPos = Cam2DControl::Instance()->GetCamPos();
			int x = 1920 / 2 + static_cast<int>((1080.f / 2.f) * (Pos2D.x - CamPos.x) * Cam2DControl::Instance()->GetCamHeight() / Base_CamScale);
			int y = 1080 / 2 - static_cast<int>((1080.f / 2.f) * (Pos2D.y - CamPos.y) * Cam2DControl::Instance()->GetCamHeight() / Base_CamScale);
			return HitPointToRectangle(x, y, -Radius, -Radius, 1920 + Radius, 1080 + Radius);
		}

		float GetRadVec2Vec(const Vector2DX& vec1, const Vector2DX& vec2) noexcept { return std::atan2f(vec1.x - vec2.x, vec1.y - vec2.y); }

		// ��ԏ�̃^�C�����Ƃ̃T�C�Y���擾(�^�C��value�Ԃ�)
		float Get2DSize(float value) noexcept { return (value * Tile_DispSize) * Base_CamScale / (1080.f / 2.f); }
		// ��ԏ�̃T�C�Y����^�C���������擾
		float Get2DSizetoTile(float value) noexcept { return (value * (1080.f / 2.f) / Base_CamScale) / Tile_DispSize; }
		// ��ʏ�̃^�C�����Ƃ̃T�C�Y���擾(�^�C��value�Ԃ�)
		int GetDispSize(float value) noexcept {
			auto* DrawParts = DXDraw::Instance();
			return DrawParts->GetScreenY(static_cast<int>(value * Tile_DispSize * Cam2DControl::Instance()->GetCamHeight()));
		}
		void Effect2DControl::Init() noexcept {
			for (auto& g : m_GuardPos) {
				g.m_Per = 0.f;
			}
			m_GuardPosNum = 0;
		}
		void Effect2DControl::Update() noexcept {
			auto* DrawParts = DXDraw::Instance();
			for (auto& g : m_GuardPos) {
				g.m_Per = std::max(g.m_Per - DrawParts->GetDeltaTime() / 0.5f, 0.f);
			}
		}
		void Effect2DControl::Draw() noexcept {
			float Radius = static_cast<float>(GetDispSize(5.f));
			Vector2DX DispPos;
			for (auto& g : m_GuardPos) {
				if (g.m_Per <= 0.f) { continue; }
				Convert2DtoDisp(g.m_Pos, &DispPos);
				switch (g.m_EffectType) {
				case EffectType::Damage:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * (g.m_Per / g.m_PerMax)), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Radius * g.m_PerMax * std::pow(1.f - (g.m_Per / g.m_PerMax), 0.5f), 8, GetColor(192, 0, 0), TRUE, 1.f, static_cast<double>(deg2rad(360.f * g.m_Per)));
					break;
				case EffectType::Guard:
					SetDrawBlendMode(DX_BLENDMODE_ADD, std::clamp(static_cast<int>(128.f * (g.m_Per / g.m_PerMax)), 0, 255));
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius * g.m_PerMax * std::pow(1.f - (g.m_Per / g.m_PerMax), 0.5f)), GetColor(150, 150, 255));
					break;
				case EffectType::WallHit:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * (g.m_Per / g.m_PerMax)), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Radius * g.m_PerMax * std::pow(1.f - (g.m_Per / g.m_PerMax), 0.5f), 5, GetColor(255, 192, 0), TRUE, 1.f, static_cast<double>(deg2rad(360.f * g.m_Per)));
					break;
				default:
					break;
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	};
};
