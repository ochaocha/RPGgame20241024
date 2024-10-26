#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include "mobchara.h"
#include "fight.h"
#include "fps.h"
#include"MainScreen.h"
#include "camera2D.h"


int displaywidhtX = 0;
int displayhigthY = 0;

 int displaywidth2X = 1920;
 int displayhight2Y = 1080;

 int MeinscreenVertical = 1080;
 int MeinscreenHorizontal = 1920;
 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	PlayerFunction PlayerFunctionOll;
	
	MapData MapDataOll;

	ScreenCamera camera;
	//FPS����I�u�W�F�N�g�Ƃ��ă��[�J���ϐ����쐬
	FPSControl FPSCtrl;

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

	MapDataOll.mapInit();//�}�b�v�̏�����
	
	MapDataOll.MapEngine();
	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// �Q�[�����[�v
	//FPS������
	FPSCtrl.Initialize();
	int Exchange = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		FPSCtrl.StartMeasureTime(); //FPS�v�����J�n

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
			//MapEngine();		//�}�b�v�̓���
			
			

			PlayerFunctionOll.Playerappdate();		//�v���C���[�̍X�V

			
			MapDataOll.mapDraw();

			
						//��ʍX�V����
			
			PlayerFunctionOll.PlayerDraw();			//�v���C���[�`��
			

			break;

		}

		// �q�b�g������?����ʕ\��

		FPSCtrl.Draw();		//FPS�`��
		ScreenFlip();		//����܂ŕ`�悵�����e���E�B���h�E�ɔ��f����
		FPSCtrl.FPSWait();  //�������Ԍv�����I�����A����܂ł̎��ԂƖڕWFPS�Ƃőҋ@���鎞�Ԃ�����o���ҋ@���܂�
	}
	PlayerFunctionOll.PlayerFinalize();	//�v���C���[�̌�n��
	//WaitKey();//�I�����ɑ����ɏI���ł���悤�Ɉꉞ�R�����g�A�E�g���܂�
	DxLib_End();
	return 0;
}

