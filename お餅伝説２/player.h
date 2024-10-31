#pragma once
#include "collision.h"
#include <vector>
#include <string>
#include "Source/Algorithm/Vector2D.h"

//�푰 �E��
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


	Vector2DX PlayerPos;						//�v���C���[�̏������W

	int animNowType;							//�v���C���[�̔z��̏c�v�f��

	int animNowIndex;							//�v���C���[�̌��݂̃A�j���[�V����

	int animDirectionOffset;					//�A�j���[�V�����̕�����ς��邽�߂̕ϐ�

	bool AnimStopRighttFlag = false;
	bool AnimStopLeftFlag = false;
	bool AnimStopUnderFlag = false;
	bool AnimStopTopFlag = false;


	bool charaMove = false;						//�v���C���[�������Ă��邩�ǂ���

	static const int animPatternNum = 8;		// �A�j���[�V�����̃p�^�[����		

	static const int animTypeNum = 6;			// �A�j���[�V�����̎��		

	const int animSizeX = 48;					//�v���C���[�̃A�j���[�V�����T�C�Y

	const int animSizeY = 64;					//�v���C���[�̃A�j���[�V�����T�C�Y

	const int hitSizeX = 40;				
											
	const int hitSizeY = 60;				

	Input MyInput;

	int drawOffsetX;								//�v���C���[�̑傫����ς���ϐ�
	int drawOffsetY;

	int charaimg[animPatternNum * animTypeNum];		//�L�����N�^�[�̃A�j���[�V�����v�f��

	float time = 0.f;
public:
	/// @brief �v���C���[�̓ǂݍ���
	/// @param displaywidth 
	/// @param displayhight 
	void PlayerInit(int displaywidth, int displayhight);

	void CalcInput(const Input& input);

	/// @brief �v���C���[�̈ړ�
	void Playerappdate();
	/// @brief �v���C���\�A�j���[�V������`��
	void PlayerDraw();
	/// @brief �v���C���[�̌�n��
	void PlayerFinalize();

};





class CharacterManager {
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
			p.Playerappdate();		//�v���C���[�̍X�V
		}
	}

	void Draw() {
		for (auto& p : PlayerFunctionOll) {
			p.PlayerDraw();			//�v���C���[�`��
		}
	}

	void DeleteChara() {
		for (auto& p : PlayerFunctionOll) {
			p.PlayerFinalize();
		}
		PlayerFunctionOll.clear();
	}
};

