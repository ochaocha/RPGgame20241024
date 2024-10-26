#pragma once
#include "collision.h"
//Emiya 1-3:�����Ɉړ�

class PlayerFunction {
	//Emiya 1-1:�����Ɉړ�
	float px, py;								//�v���C���[�̏������W
	int animNowType;							//�v���C���[�̔z��̏c�v�f��
	int animNowIndex;							//�v���C���[�̌��݂̃A�j���[�V����

	int animDirectionOffset;					//�A�j���[�V�����̕�����ς��邽�߂̕ϐ�

	bool charaMove = false;						//�v���C���[�������Ă��邩�ǂ���

	static const int animPatternNum = 8;			// �A�j���[�V�����̃p�^�[����		//Emiya 1-2:1-1�ňړ���G���[���o��̂ŐÓI�ϐ��Ƃ��Ă��܂��܂��傤

	static const int animTypeNum = 6;			// �A�j���[�V�����̎��		//Emiya 1-2:1-1�ňړ���G���[���o��̂ŐÓI�ϐ��Ƃ��Ă��܂��܂��傤
	const int animSizeX = 48;					//�v���C���[�̃A�j���[�V�����T�C�Y
	const int animSizeY = 64;

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

//Emiya 1-4:ScreenCamera�N���X��camera2D.h�̂悤�ȃt�@�C����p�ӂ��Ă�����Ɉړ������Ⴂ�܂��傤

//Emiya 1-5:main.cpp�̃��[�J���ϐ��ɂ��邽�߈ړ����܂��傤

