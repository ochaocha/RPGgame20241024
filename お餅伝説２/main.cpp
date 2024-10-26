#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include "mobchara.h"
#include "fight.h"
#include "fps.h"
#include"MainScreen.h"
#include "Camera.h"


int displaywidhtX = 0;
int displayhigthY = 0;

 int displaywidth2X = 1920;
 int displayhight2Y = 1080;

 int MeinscreenVertical = 1080;
 int MeinscreenHorizontal = 1920;
 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//Emiya 1-5:�ړ���@static�ł���K�v���Ȃ��Ȃ�̂ŊO���܂��傤
	

	// DxLib������
	ChangeWindowMode(TRUE);

	//�X�N���[�����������ꏊ
	//SetGraphMode(displaywidth, displayhight, 16);
	camera.Screen(displaywidhtX, displayhigthY, displaywidth2X,displayhight2Y);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	
	

	PlayerFunctionOll.PlayerInit(displaywidth2X, displayhight2Y);

	mapInit();//�}�b�v�̏�����
	
	MapEngine();
	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// �Q�[�����[�v
	//FPS������
	FPSInit();
	int Exchange = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime�v��
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;

		PlayeAttackType Playerchoicetern(); //�v���C���[�̍U���p�^�[���\����
		

		switch (Exchange)
		{
		case 0://���C���X�N���[��
			
			Mein.MeinDrawScreen(&Exchange);

			break;
			
		case 1:
			
			ClearDrawScreen();		//�}�b�v�̕`��
			//MapEngine();
			mapaupdate();			//�}�b�v�̓���
			
			

			PlayerFunctionOll.Playerappdate();		//�v���C���[�̍X�V

			
			mapDraw();	

			
						//��ʍX�V����
			
			PlayerFunctionOll.PlayerDraw();			//�v���C���[�`��
			

			break;

		}

		// �q�b�g������?����ʕ\��
		ScreenFlip();   //�X�N���[���̓��������炩�ɂ��鏈��
		{
			FPSCheck(); //FPS�v��
			FPSDraw();  //FPS�`��
			FPSWait();  //FPS�ҋ@
		}
	
	}
	PlayerFunctionOll.PlayerFinalize();	//�v���C���[�̌�n��
	WaitKey();
	DxLib_End();
	return 0;
}

