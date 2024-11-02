#pragma once
#include	"Header.hpp"

namespace DXLIB_Sample {
	// --------------------------------------------------------------------------------------------------
	// �萔
	// --------------------------------------------------------------------------------------------------
	// SE�̐ݒ���s���A��
	enum class SESelect {
		RunFoot = static_cast<int>(SoundSelectCommon::Num),// SoundSelectCommon�̖�������̘A��
		Shot1,
		Shot2,
		Shot3,
		Hit,
		Guard,
		Bomb,
		Normal,
	};
	// BGM�̐ݒ���s���A��
	enum class BGMSelect {
		Title,
		Normal,
		Caution,
		Alert,
	};
	// �I�u�W�F�N�g�̎��
	enum class Object2DType {
		Human,
		Bullet,
		Rocket,
		Metal,
	};
	// �G�t�F�N�g�̎��
	enum class EffectType {
		Damage,
		Guard,
		WallHit,
	};
	// ��{�^�ɖ��O��t���ĊǗ�
	using PlayerID = char;
	using HitPoint = short;
	// �ύX���Ă��悢�萔(�����l)
	constexpr float Tile_DispSize{ 48.f };// �^�C������px�ŕ`�悷�邩
	constexpr PlayerID PlayerCharacter{ 0 };// ���삷��v���C���[
}
