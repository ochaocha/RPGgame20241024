#pragma once

class PlayerFight
{
private:
	/////////////////////////////////////////////////////////////////////
	///////					プレイヤーステータス		   		////////
	///////////////////////////////////////////////////////////////////
	unsigned int PlayerATK = 3;
	unsigned int PlaeyrHP = 20;
	unsigned int PlaeyrMP = 15;
	unsigned int PlaeyrMagicPower = 5;
	unsigned short PlaerLevel = 1;

	/////////////////////////////////////////////////////////////////////
	///////						技								/////////
	/////////////////////////////////////////////////////////////////////
	 bool EldaraFalsis=1;	
	 bool LunarisAethernal = 1;
	 bool IgnisVarask=1;		
	 bool UmbraTenebris=1;
	 bool ZephirosExhalis = 1;
	 bool SolariaVeritas=1;
	 bool AquaMiralis=1;		
	 bool TerranOthos=1;		
	 bool NormalAttack = 0;
public:
 PlayerFight();

};


