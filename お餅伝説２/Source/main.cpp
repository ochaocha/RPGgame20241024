#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "fps.h"
#include "Screen.h"
#include "camera2D.h"
#include <Windows.h>
#include "Battle.h"
#include "Enemy.h"

int displaywidhtX = 0;
int displayhigthY = 0;

int displaywidth2X = 1920;
int displayhight2Y = 1080;

int MeinscreenVertical = 1080;
int MeinscreenHorizontal = 1920;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	//�L�����N�^�[�̃I�u�W�F�N�g����
	CharacterManager::Create();

#if SINGLETON
	//�}�b�v�f�[�^�̃I�u�W�F�N�g����
	MapData::Create();
	//�}�b�v�f�[�^�̃V���O���g������
	MapData* MapDataOllPtr = MapData::Instance();
#else

	MapData MapDataOll;
#endif



	//�o�g����ʂ̃C���^�[�t�F�[�X
	BattleScreenChanger battlescreen;

	//�o�g���̃f�[�^�\�������L
	BattleData battledata;


	//�X�N���[���̃J�����𐶐�
	ScreenCamera camera;
	//FPS����I�u�W�F�N�g�Ƃ��ă��[�J���ϐ����쐬
	FPSControl FPSCtrl;

	// DxLib������
	ChangeWindowMode(TRUE);

	//SetGraphMode(displaywidth, displayhight, 16);
	camera.Screen(displaywidhtX, displayhigthY, displaywidth2X, displayhight2Y);


	//�G���[����
	if (DxLib_Init() == -1)
	{
		return -1;
	}


	//player�X�e�[�^�X�̃I�u�W�F�N�g����

	CharacterStatus obj;
	obj.playerstatusInit();
	CharacterStatus* playerstatus;
	playerstatus = &obj;

	OllEnemyData normalenemydata;
	
	NormalEnemyName normal;
	//NPC�̃I�u�W�F�N�g����
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);
	CharacterManager::Instance()->AddCharacter(displaywidth2X, displayhight2Y);

	//MapDataOll�ϐ���������
	MapDataOllPtr->Engine();//�}�b�v�̓ǂݍ���
	MapDataOllPtr->Init();//�}�b�v�̏�����
	//normalenemydata.Init();
	int nowCount, prevCount;

	nowCount = prevCount = GetNowCount();

	//FPS������
	FPSCtrl.Initialize();
	//�X�N���[���̕ϐ�
	int Exchange = 0;

	//�o�g���̃G���J�E���g����
	bool BattleEncounterFlag = false;
	//���C���N���X�����ԂƂ��Ē�`
	MeinScreenChanger Mein;
	battlescreen.Imginitialize();
	//�m�[�}���G�l�~�[��������
	normalenemydata.Init();
	
	//���C����ʂ̕`����͏����������Ă���
	Mein.ScreenInit();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//FPS�v�����J�n
		FPSCtrl.StartMeasureTime();
		// deltaTime�v��
		float deltaTime;

		nowCount = GetNowCount();

		deltaTime = (nowCount - prevCount) / 1000.0f;

		switch (Exchange)
		{
		case 0://�Q�[���X�^�[�g�V�[��
			if (Exchange == 0)
			{
				Mein.Music();
			}
			
			Mein.Draw(&Exchange);
			break;

		case 1://�I�v�V�����V�[��

			//��������Ƃ����ɍs���Ă��邱�Ƃ̊m�F�ς�

		case 2://���j���[�V�[��

			ClearDrawScreen();		//�}�b�v�̕`��
			//MapEngine();		//�}�b�v�̓���
			MapDataOllPtr->Update();			//�}�b�v�̓���

			//�v���C���[�̓��͏���
			{
				Input Player;

				Player.SetInput
				(
					CheckHitKey(KEY_INPUT_A),
					CheckHitKey(KEY_INPUT_D),
					CheckHitKey(KEY_INPUT_S),
					CheckHitKey(KEY_INPUT_W)
				);
				battledata.InputEncounterCheck
				(
					BattleEncounterFlag,
					&Exchange,
					CheckHitKey(KEY_INPUT_A),
					CheckHitKey(KEY_INPUT_D),
					CheckHitKey(KEY_INPUT_S),
					CheckHitKey(KEY_INPUT_W)

				);
				if (Exchange == 3)
				{

					break;
				}
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


			//�o�g���V�[��
		case 3:
			
			//�G���J�E���g�̎������m�F���܂���
			ClearDrawScreen();
			//�o�g�����̔w�i�摜
			battlescreen.Scene();
			normalenemydata.Rand();
			normalenemydata.Draw();
			//�o�g������player��`��
			battlescreen.PlayerDraw();
			//�o�g������Enemy��`��
			battlescreen.EnemyDraw();
			//�o�g������player�̃f�[�^���i�[
			battledata.StatusUI(
				playerstatus->CharacterHP,
				playerstatus->CharacterATK,
				playerstatus->CharacterMP,
				playerstatus->CharacterDEF,
				playerstatus->CharacterLUCK,
				playerstatus->CharacterAGI
			);


	
			break;
		};


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

