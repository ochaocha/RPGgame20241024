#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "fps.h"
#include"MainScreen.h"
#include "camera2D.h"
#include <Windows.h>


int displaywidhtX = 0;
int displayhigthY = 0;

 int displaywidth2X = 1920;
 int displayhight2Y = 1080;

 int MeinscreenVertical = 1080;
 int MeinscreenHorizontal = 1920;
 


 int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	CharacterManager::Create();

#if SINGLETON
	MapData::Create();
	MapData* MapDataOllPtr = MapData::Instance();
#else
	MapData MapDataOll;
#endif
	

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
	
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);

	//MapDataOll�ϐ���������
	MapDataOllPtr->Engine();//�}�b�v�̓ǂݍ���
	MapDataOllPtr->Init();//�}�b�v�̏�����
	

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

		

		switch (Exchange)
		{
		case 0://���C���X�N���[��
			
			Mein.Draw(&Exchange);
				
			break;
			
		case 1:
			
			ClearDrawScreen();		//�}�b�v�̕`��
			//MapEngine();		//�}�b�v�̓���
			MapDataOllPtr->Update();			//�}�b�v�̓���

			//�v���C���[�̓��͏���
			{
				Input Player;

				Player.SetInput(
					CheckHitKey(KEY_INPUT_A),
					CheckHitKey(KEY_INPUT_D),
					CheckHitKey(KEY_INPUT_S),
					CheckHitKey(KEY_INPUT_W)
				);

				CharacterManager::Instance()->GetChara(0).CalcInput(Player);
			}

			//NPC�̓��͏���
			{
				Input Player;

				Player.SetInput(
					1,
					0,
					0,
					0
				);
				CharacterManager::Instance()->GetChara(1).CalcInput(Player);
			}

			CharacterManager::Instance()->Update();		//�v���C���[�̍X�V

			MapDataOllPtr->Draw();
			//��ʍX�V����
			CharacterManager::Instance()->Draw();			//�v���C���[�`��

			break;

		}

		// �q�b�g������?����ʕ\��

		FPSCtrl.Draw();		//FPS�`��
		ScreenFlip();		//����܂ŕ`�悵�����e���E�B���h�E�ɔ��f����
		FPSCtrl.FPSWait();  //�������Ԍv�����I�����A����܂ł̎��ԂƖڕWFPS�Ƃőҋ@���鎞�Ԃ�����o���ҋ@���܂�
	}
	CharacterManager::Instance()->DeleteChara();
	//WaitKey();//�I�����ɑ����ɏI���ł���悤�Ɉꉞ�R�����g�A�E�g���܂�
	DxLib_End();

	return 0;
}

