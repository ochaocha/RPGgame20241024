#pragma once
#include "collision.h"
#include <vector>
#include "Source/Algorithm/Vector2D.h"




class OllCharacterData {


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



	int drawOffsetX;								//�v���C���[�̑傫����ς���ϐ�
	int drawOffsetY;

	int charaimg[animPatternNum * animTypeNum];		//�L�����N�^�[�̃A�j���[�V�����v�f��

	float time = 0.f;
public:
	/// @brief �v���C���[�̓ǂݍ���
	/// @param displaywidth 
	/// @param displayhight 
	void PlayerInit(int displaywidth, int displayhight);
	/// @brief �v���C���[�̈ړ�
	void Playerappdate();
	/// @brief �v���C���\�A�j���[�V������`��
	void PlayerDraw();
	/// @brief �v���C���[�̌�n��
	void PlayerFinalize();

};