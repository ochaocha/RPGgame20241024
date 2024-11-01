#pragma once
#include <vector>
#include <string>
#include "Source/Algorithm/Vector2D.h"

//種族 職業
enum class GROUP {
	SOULDER,
	WITCH,
};

class CharacterStatus {
	std::string Name;
	GROUP       MyGroup{};
public:
};

class Input {
	bool MoveLeft = false;
	bool MoveRight = false;
	bool MoveDown = false;
	bool MoveUp = false;
public:
	void SetInput(
		bool moveLeft,
	bool moveRight,
	bool moveDown,
	bool moveUp
	) {
		MoveLeft = moveLeft;
		MoveRight = moveRight;
		MoveDown = moveDown;
		MoveUp = moveUp;
	}
public:
	bool IsMoveLeft() { return MoveLeft; }
	bool IsMoveRight() { return MoveRight; }
	bool IsMoveDown() { return MoveDown; }
	bool IsMoveUp() { return MoveUp; }
};

class OllCharacterData {

	CharacterStatus characterStatus;

	Vector2DX PlayerVelocity;


	Vector2DX PlayerPos;						//プレイヤーの初期座標
	int animNowType=0;							//プレイヤーの配列の縦要素数
	int animNowIndex=0;							//プレイヤーの現在のアニメーション
	int animDirectionOffset=0;					//アニメーションの方向を変えるための変数
	bool AnimStopRighttFlag = false;
	bool AnimStopLeftFlag = false;
	bool AnimStopUnderFlag = false;
	bool AnimStopTopFlag = false;

	bool charaMove = false;						//プレイヤーが動いているかどうか
	static const int animPatternNum = 8;		// アニメーションのパターン数		
	static const int animTypeNum = 6;			// アニメーションの種類		
	const int animSizeX = 48;					//プレイヤーのアニメーションサイズ
	const int animSizeY = 64;					//プレイヤーのアニメーションサイ
	const int hitSizeX = 40;											
	const int hitSizeY = 60;				
	Input MyInput;
	int drawOffsetX=0;								//プレイヤーの大きさを変える変数
	int drawOffsetY=0;

	int charaimg[animPatternNum * animTypeNum] = {0};			//キャラクターのアニメーション要素数

	float time = 0.f;
public:
	/// @brief プレイヤーの読み込み
	/// @param displaywidth 
	/// @param displayhight 
	void PlayerInit(int displaywidth, int displayhight);

	void CalcInput(const Input& input);

	/// @brief プレイヤーの移動
	void Update();
	/// @brief プレイヤ―アニメーションを描画
	void PlayerDraw();
	/// @brief プレイヤーの後始末
	void PlayerFinalize();
};





class CharacterManager {
private:
	static const CharacterManager* s_Singleton;
public:
	static void Create(void) noexcept { s_Singleton = new CharacterManager(); }
	static CharacterManager* Instance(void) noexcept {
		if (s_Singleton == nullptr) { exit(-1); }
		return (CharacterManager*)s_Singleton;
	}
private:
	CharacterManager(){}
	~CharacterManager(){}
private:
	std::vector<OllCharacterData> PlayerFunctionOll;
public:
	OllCharacterData& GetChara(int ID) { return PlayerFunctionOll.at(ID); }
public:
	void AddCharacter(int FirstXPos, int FirstYPos) {
		PlayerFunctionOll.resize(PlayerFunctionOll.size() + 1);
		PlayerFunctionOll.back().PlayerInit(FirstXPos, FirstYPos);
	}

	void Update() {
		for (auto& p : PlayerFunctionOll) {
			p.Update();		//プレイヤーの更新
		}
	}

	void Draw() {
		for (auto& p : PlayerFunctionOll) {
			p.PlayerDraw();			//プレイヤー描画
		}
	}

	void DeleteChara() {
		for (auto& p : PlayerFunctionOll) {
			p.PlayerFinalize();
		}
		PlayerFunctionOll.clear();
	}
};

