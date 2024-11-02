#pragma once
#include	"Header.hpp"

namespace FPS_n2 {
	// 定数
	namespace Sceneclass {
		enum class SESelect {
			RunFoot = static_cast<int>(SoundSelectCommon::Num),
			Shot1,
			Shot2,
			Shot3,
			Hit,
			Guard,
			Bomb,
			Normal,
			// 
		};
		enum class BGMSelect {
			Title,
			Normal,
			Caution,
			Alert,
		};
		enum class Object2DType {
			Human,
			Bullet,
			Rocket,
			Metal,
		};
		constexpr float Tile_DispSize{ 48.f };//タイルを何pxで描画するか

	};

	using PlayerID = char;
	using HitPoint = short;

	constexpr PlayerID PlayerCharacter{ 0 };//操作するプレイヤー
};
