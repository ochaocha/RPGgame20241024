#pragma once
#include "DxLib.h"


//敵の名前
enum class NormalEnemyName
{
	DRAGON,
	REDDRAGON,
	GOLEM,
	MINIDRAGON,
	DRAGONBARN,
	CHIMWRA,
	DRAGONSERVANT,
	COUNT
};

//	敵のステータス
struct EnemyStutas
{
	int ATK;		//攻撃
	int DEF;		//防御
	int AGI;		//素早さ
	int MP;		    //マジックポイント
	int HP;			//ヒットポイント
	bool ALIVEENEMY;
};

struct EnemyNamberDraw
{
	int monsterX = 750;
	int monster2X = 1050;
	int monsterY = 250;
	int monster2Y = 500;
};
class AllEnemyData
{
private:
	int RandomExtraction = 0;
	
public:

	
	std::vector<NormalEnemyName> enemies;
	int normalenemy[9];	
	// 初期化
	void Init();
	// ランダムに敵を抽出して返す
	std::vector<NormalEnemyName> Rand();
	//敵によっての描画とステータスの生成	
	void Draw(EnemyNamberDraw& enemycoordinate);


	
};
