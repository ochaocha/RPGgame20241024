#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include "mobchara.h"
#include "fight.h"
#include "fps.h"

const int displaywidth = 1920;
const int displayhight = 1080;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	
	// DxLib������
	ChangeWindowMode(TRUE);
	SetGraphMode(displaywidth, displayhight, 16);
	mobcharaimg();
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	
	PlayerInit(displaywidth,displayhight);
	mapInit();//�}�b�v�̏�����
	
	MapEngine();
	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();
	// �Q�[�����[�v
	//FPS������
	FPSInit();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime�v��
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;

		PlayeAttackType Playerchoicetern(); //�v���C���[�̍U���p�^�[���\����


		switch (int Exchange = 1)
		{
		case 0://���C���X�N���[��

		case 1:
			
			ClearDrawScreen();		//�}�b�v�̕`��
			//MapEngine();
			mapaupdate();			//�}�b�v�̓���
			
					//

			Playerappdate();		//�v���C���[�̍X�V

			
			mapDraw();	

			
						//��ʍX�V����
			
			PlayerDraw();			//�v���C���[�`��
			
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
	PlayerFinalize();	//�v���C���[�̌�n��
	WaitKey();
	DxLib_End();
	return 0;
}

//�e�X�g