#include "DXLib_ref_102_ObjectManager.hpp"


namespace DXLibRef {
	// �V���O���g���̎��Ԓ�`
	const Object2DManager* SingletonBase<Object2DManager>::m_Singleton = nullptr;
	// �쐬���ꂽ�I�u�W�F�N�g�����X�g�ɒǉ�
	void			Object2DManager::AddObject(const std::shared_ptr<Base2DObject>& NewObj) noexcept {
		// �I�u�W�F�N�g�̏�����+UniqueID�̐ݒ�
		NewObj->Init(this->m_LastUniqueID);
		this->m_LastUniqueID++;
		this->m_Object.emplace_back(NewObj);
	}
	// UniqueID�Ō����������ăI�u�W�F�N�g���擾(�Ȃ��ꍇ��nullptr)
	const std::shared_ptr<Object2DManager::Base2DObject> Object2DManager::GetObj(UniqueID ID) noexcept {
		for (auto& o : this->m_Object) {
			if (o->GetUniqueID() == ID) {
				return o;
			}
		}
		return nullptr;
	}
	// UniqueID�Ō����������ăI�u�W�F�N�g�����邩�m�F
	bool Object2DManager::HasObj(UniqueID ID) noexcept {
		for (auto& o : this->m_Object) {
			if (o->GetUniqueID() == ID) {
				return true;
			}
		}
		return false;
	}
	// �Y���̃I�u�W�F�N�g���폜
	void			Object2DManager::DelObj(const std::shared_ptr<Base2DObject>& ptr) noexcept {
		for (auto& o : this->m_Object) {
			if (o == ptr) {
				// �폜���̏������s����
				o->Dispose();
				// ���X�g����폜
				this->m_Object.erase(this->m_Object.begin() + (&o - &this->m_Object.front()));// ���Ԃ̈ێ��̂���pop_back�ł͂Ȃ�erase
				return;
			}
		}
	}
	// ���ʂ̃I�u�W�F�N�g�X�V
	void			Object2DManager::Update(void) noexcept {
		// ���ʂ̍X�V
		for (auto& o : this->m_Object) {
			if (!o->GetIsDelete()) {
				o->Update();
			}
		}
		// �I�u�W�F�N�g�̔r���`�F�b�N
		for (int i = 0, Max = static_cast<int>(this->m_Object.size()); i < Max; i++) {
			auto& o = this->m_Object.at(static_cast<size_t>(i));
			// �폜�t���O�������Ă�����
			if (o->GetIsDelete()) {
				// �폜���̏������s����
				o->Dispose();
				// ���X�g����폜
				this->m_Object.erase(this->m_Object.begin() + i);// ���Ԃ̈ێ��̂���pop_back�ł͂Ȃ�erase
				// �����������ꏊ�Ɏ��̃I�u�W�F�N�g�����Ă���̂ŁA������X���[���Ȃ��悤�Ƀ��[�v�ʒu�Ƒ��������炵�Ă���
				i--;
				Max--;
			}
		}
		// �I�u�W�F�N�g�ǂ����̏Փ˔���
		for (auto& o : this->m_Object) {
			// �I�u�W�F�N�g�ǂ����̔�����s��Ȃ��ꍇ�̓X���[
			if (!o->GetIsHitOtherObject()) { continue; }
			for (auto& ot : this->m_Object) {
				if (ot == o) { continue; }
				// ���������������牟���o��(���ʍ������Z)
				SEGMENT_SEGMENT_RESULT Result;
				GetSegmenttoSegment(o->GetPrevPos(), o->GetPos(), ot->GetPrevPos(), ot->GetPos(), &Result);// �����̓����蔻����v�Z��
				// ���ʂ����a�̍��v�����������ꍇ
				float Distance = (std::sqrt(Result.SegA_SegB_MinDist_Square)) - (o->GetSize() / 2.f + ot->GetSize() / 2.f);
				if (Distance < 0.f) {
					// Distance��0�ƂȂ�悤�ɑo���������͂��v�Z����
					{
						// ���ꂼ��̈�ԋ߂��n�_�̕��ς𒆐S�Ƃ���
						Vector2DX pos;
						pos += Result.SegA_MinDist_Pos;
						pos += Result.SegB_MinDist_Pos;
						pos /= 2.f;
						// ����������o���x�N�g�����v�Z��
						Vector2DX vec = (ot->GetPos() - pos).normalized() * Distance;
						// �o���ɗ͂�������(�d���ق������̏�ɂƂǂ܂�悤�Ɍv�Z)
						ot->SetPos(ot->GetPos() - vec * (o->GetMass() / (o->GetMass() + ot->GetMass())));
						o->SetPos(o->GetPos() + vec * (ot->GetMass() / (o->GetMass() + ot->GetMass())));
					}
					// �����������̂Ƃ��Ēʒm
					o->SetHitUniqueID(ot->GetUniqueID());
					ot->SetHitUniqueID(o->GetUniqueID());
				}
			}
		}
	}
	// �I�u�W�F�N�g�S�̂��e�`�ʂ���
	void			Object2DManager::DrawShadow(void) noexcept {
		for (auto& o : this->m_Object) {
			o->DrawShadow();
		}
	}
	// �I�u�W�F�N�g�S�̂�`�ʂ���
	void			Object2DManager::Draw(void) noexcept {
		for (auto& o : this->m_Object) {
			o->Draw();
		}
	}
	// ���ׂẴI�u�W�F�N�g���폜����
	void			Object2DManager::DeleteAll(void) noexcept {
		for (auto& o : this->m_Object) {
			if (o) {
				o->Dispose();
			}
		}
		this->m_Object.clear();
		this->m_LastUniqueID = 0;// ��U���j�[�NID�����Z�b�g
	}
};