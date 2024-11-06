#include	"sub.hpp"

#include	"MainScene/Player/Player.hpp"

// �V���O���g���̎��Ԓ�`
const DXLIB_Sample::Sceneclass::Effect2DControl* SingletonBase<DXLIB_Sample::Sceneclass::Effect2DControl>::m_Singleton = nullptr;
const DXLIB_Sample::Sceneclass::StoryTextDataBase* SingletonBase<DXLIB_Sample::Sceneclass::StoryTextDataBase>::m_Singleton = nullptr;
const DXLIB_Sample::Sceneclass::Cam2DControl* SingletonBase<DXLIB_Sample::Sceneclass::Cam2DControl>::m_Singleton = nullptr;

namespace DXLIB_Sample {
	namespace Sceneclass {
		//
		void Effect2DControl::Draw() noexcept {
			//���a5����Ƃ��ĉ�ʏ�ł̃T�C�Y���擾
			float Radius = static_cast<float>(Cam2DControl::GetTileToDispSize(5.f));
			for (auto& g : m_GuardPos) {
				if (!g.IsActive()) { continue; }
				//�^�C�����W����\����̍��W�ɕϊ�
				Vector2DX DispPos = Cam2DControl::ConvertTiletoDisp(g.GetEffectPos());
				//���W����ɕ`��
				switch (g.GetEffectType()) {
				case EffectType::Damage:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * g.GetEffectPer()), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Radius * g.GetEffectRadius(), 8, GetColor(192, 0, 0), TRUE, 1.f, static_cast<double>(g.GetEffectRotate()));
					break;
				case EffectType::Guard:
					SetDrawBlendMode(DX_BLENDMODE_ADD, std::clamp(static_cast<int>(128.f * g.GetEffectPer()), 0, 255));
					DrawCircle(static_cast<int>(DispPos.x), static_cast<int>(DispPos.y), static_cast<int>(Radius * g.GetEffectRadius()), GetColor(150, 150, 255));
					break;
				case EffectType::WallHit:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(static_cast<int>(128.f * g.GetEffectPer()), 0, 255));
					DrawCircleAA(DispPos.x, DispPos.y, Radius * g.GetEffectRadius(), 5, GetColor(255, 192, 0), TRUE, 1.f, static_cast<double>(g.GetEffectRotate()));
					break;
				default:
					break;
				}
			}
			//��L�Ńu�����h���[�h�̐ݒ���s�����̂Ń��Z�b�g
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		//
		StoryTextDataBase::StoryTextDataBase(void) noexcept {
			// �e�L�X�g�f�[�^�𐶐�
			this->havehandle.reserve(256);					// ���炩���߈��ʖڐ����m�ۂ��Ă���
			FileStreamDX FileStream("data/StoryText.txt");	// 1�s���t�@�C������͂��A���[�J���C�YID�ɍ��v�����e�L�X�g��ۑ�����
			while (true) {
				if (FileStream.ComeEof()) { break; }		// �������肪�I������烋�[�v�𔲂���
				auto ALL = FileStream.SeekLineAndGetStr();	// 1�s�ǂ݂����ĕ�������
				if (ALL == "") { continue; }				// �󔒍s�̓X���[
				// �C�R�[��(=)�̍���ID�A�E���e�L�X�g�Ƃ��Ď擾
				TextID ID = (TextID)(std::stoi(FileStreamDX::getleft(ALL)));
				std::string Text = FileStreamDX::getright(ALL);
				// �������ł�ID���o�^�ςł���΁A���s�Ƃ݂Ȃ��Ċ����e�L�X�g�̖����ɉ��s�R�[�h�ƈꏏ�ɒǉ�����
				bool IsHit = false;
				for (auto& h : this->havehandle) {
					if (h.m_ID == ID) {
						std::string Before = h.m_Str;			// ���s���ē����e�L�X�g�ƂȂ�悤�ɑg�ݗ��Ă�
						Before += "\n";
						Before += Text;
						sprintfDx(h.m_Str, Before.c_str());		//���������
						IsHit = true;							//�o�^���ꂽ�Ƃ��ăt���O�𗧂ĂĔ�����
						break;
					}
				}
				// �o�^����Ă��Ȃ��̂ł���ΐV�K�Ƀn���h�������蓖�Ă�
				if (!IsHit) {
					this->havehandle.emplace_back();
					this->havehandle.back().m_ID = ID;							//ID��ێ�
					sprintfDx(this->havehandle.back().m_Str, Text.c_str());		//���������
				}
			}
			//FileStream�̃f�X�g���N�^�Ńe�L�X�g�f�[�^���N���[�Y
		}
	}
}
