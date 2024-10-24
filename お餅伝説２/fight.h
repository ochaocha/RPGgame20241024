#pragma once

struct PlayerFight
{
	unsigned int PlayerATK=3;
	unsigned int PlaeyrHP=20;
	unsigned int PlaeyrMP=15;
	unsigned int PlaeyrMagicPower=5;
	unsigned short PlaerLevel = 1;
};
struct PlayeAttackType
{
	unsigned int EldaraFalsis=1;		//高速で氷の槍を生み出し、敵に放つ。氷の破片が散り周囲にもダメージを与える。
	unsigned int LunarisAethernal=1;	//月の力を借り、光の矢を放つ。暗闇の中でも命中精度が高い。
	unsigned int IgnisVarask=1;		//灼熱の炎を呼び出し、対象を焼き尽くす。炎は範囲に広がり続け、徐々に強くなる。
	unsigned int UmbraTenebris=1;		//闇の力を操り、敵の視界を奪う。影が広がり、敵の動きを封じ込める。
	unsigned int ZephirosExhalis=1;	//風の精霊を召喚し、強力な突風で敵を吹き飛ばす。
	unsigned int SolariaVeritas=1;		//太陽の光を集め、敵に強力な光線を放つ。光は正義の象徴であり、闇の魔物に対して特に効果的。
	unsigned int AquaMiralis=1;		// 水の精霊を召喚し、浄化の雨を降らせる。敵の力を弱め、味方を癒す力を持つ。
	unsigned int TerranOthos=1;		//大地の力を引き出し、地割れを起こす。岩や土の防御壁も形成し、プレイヤーを守る。
	unsigned int NormalAttack = 0;

};
/// <summary>
/// プレイヤーの選択攻撃手段の関数fight.hの中
/// </summary>
/// <param name=""></param>
void Playerchoicetern(struct PlayeAttackType);
/// <summary>
/// プレイヤーの攻撃手段
/// </summary>
/// <param name=""></param>
void PlayerType(struct PlayerFight);


