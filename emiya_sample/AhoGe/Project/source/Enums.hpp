#pragma once
#include	"Header.hpp"

namespace DXLIB_Sample {
	// --------------------------------------------------------------------------------------------------
	// 定数
	// --------------------------------------------------------------------------------------------------
	// SEの設定を行う連番
	enum class SESelect {
		RunFoot = static_cast<int>(SoundSelectCommon::Num),// SoundSelectCommonの末尾からの連番
		Shot1,
		Shot2,
		Shot3,
		Hit,
		Guard,
		Bomb,
		Normal,
	};
	// BGMの設定を行う連番
	enum class BGMSelect {
		Title,
		Normal,
		Caution,
		Alert,
	};
	// オブジェクトの種別
	enum class Object2DType {
		Human,
		Bullet,
		Rocket,
		Metal,
	};
	// エフェクトの種類
	enum class EffectType {
		Damage,
		Guard,
		WallHit,
	};
	// 基本型に名前を付けて管理
	using PlayerID = char;
	using HitPoint = short;
	// 変更してもよい定数(調整値)
	constexpr float Tile_DispSize{ 48.f };// タイルを何pxで描画するか
	constexpr PlayerID PlayerCharacter{ 0 };// 操作するプレイヤー
}
