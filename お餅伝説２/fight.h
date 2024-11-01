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
	unsigned int EldaraFalsis=1;		//�����ŕX�̑��𐶂ݏo���A�G�ɕ��B�X�̔j�Ђ��U����͂ɂ��_���[�W��^����B
	unsigned int LunarisAethernal=1;	//���̗͂��؂�A���̖����B�Èł̒��ł��������x�������B
	unsigned int IgnisVarask=1;		//�ܔM�̉����Ăяo���A�Ώۂ��Ă��s�����B���͔͈͂ɍL���葱���A���X�ɋ����Ȃ�B
	unsigned int UmbraTenebris=1;		//�ł̗͂𑀂�A�G�̎��E��D���B�e���L����A�G�̓����𕕂����߂�B
	unsigned int ZephirosExhalis=1;	//���̐�����������A���͂ȓ˕��œG�𐁂���΂��B
	unsigned int SolariaVeritas=1;		//���z�̌����W�߁A�G�ɋ��͂Ȍ�������B���͐��`�̏ے��ł���A�ł̖����ɑ΂��ē��Ɍ��ʓI�B
	unsigned int AquaMiralis=1;		// ���̐�����������A�򉻂̉J���~�点��B�G�̗͂���߁A����������͂����B
	unsigned int TerranOthos=1;		//��n�̗͂������o���A�n������N�����B���y�̖h��ǂ��`�����A�v���C���[�����B
	unsigned int NormalAttack = 0;

};
/// <summary>
/// �v���C���[�̑I���U����i�̊֐�fight.h�̒�
/// </summary>
/// <param name=""></param>
void Playerchoicetern(struct PlayeAttackType);
/// <summary>
/// �v���C���[�̍U����i
/// </summary>
/// <param name=""></param>
void PlayerType(struct PlayerFight);


