#pragma once
//Emiya 1-3:�����Ɉړ�

class PlayerFunction {
	//Emiya 1-1:�����Ɉړ�
public:
	void PlayerInit(int displaywidth, int displayhight);
	void Playerappdate();
	void PlayerDraw();
	void PlayerFinalize();

};

static PlayerFunction PlayerFunctionOll;//Emiya 1-5:main.cpp�̃��[�J���ϐ��ɂ��邽�߈ړ����܂��傤

//Emiya 1-4:ScreenCamera�N���X��camera2D.h�̂悤�ȃt�@�C����p�ӂ��Ă�����Ɉړ������Ⴂ�܂��傤
class  ScreenCamera
{
public:
	
	void Screen(int displaywidthX, int displayhightY, int displaywidth2X, int displayhight2Y);


};

static ScreenCamera camera;//Emiya 1-5:main.cpp�̃��[�J���ϐ��ɂ��邽�߈ړ����܂��傤

