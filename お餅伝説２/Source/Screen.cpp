#include "Dxlib.h"
#include "Screen.h"


constexpr int  graphX = 600, graphY = 600, graph2X = 1320, graph2Y = 1000;


/// @brief �G�t�F�N�g�̏�����
void MeinScreenChanger::ScreenInit()
{
	EfectNowIndex = 0;
	LoadDivGraph("Data/Free/startscreenfile/233.png",StartEfectNum,StartEfectXNum,StartEfectYNum, StartEfectXSize, StartEfectYSize, EfectImg);

}




/// @brief�X�^�[�g��ʂ̊֐�
/// @param change 
/// @param MeinscreenVertical 
/// @param MeinscreenHorizontal 
void MeinScreenChanger::Draw(int* screenchange)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////���ʐݒ�
	//////////////////////////////////////////////////////////////////////////////////////////
	ChangeVolumeSoundMem(50, StartSound);	
	ChangeVolumeSoundMem(100, StartDesition);	
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////�F�ݒ�Ɖ摜�ݒ�
	//////////////////////////////////////////////////////////////////////////////////////////
	
	
	//�X�^�[�g�̕����̐F
	unsigned int StatrString = GetColor(ColorHandle, ColorHandle, ColorHandle);
	//�I�v�V�����̕����̐F
	unsigned int GreenString= GetColor(0, ColorHandle, 0);
	//�X�^�[�g��ʂ̔w�i�摜�n���h��
	int startHandle = LoadGraph("Data/img/pipo-bg004b.png");
	//�X�^�[�g��ʂ̘g�摜�n���h��
	int Hnadle = LoadGraph("Data/img/R.png", TRUE);

	//�}�E�X�|�C���g�̎擾
	GetMousePoint(&mauseX, &mauseY);

	//�t�H���g�T�C�Y�̕ύX
	SetFontSize(34);

	//�X�^�[�g��ʔw�i�̕`��
	DrawExtendGraph(0, 0,1920,1080,startHandle, TRUE);
	//�X�^�[�g��ʂ̘g��`��
	DrawExtendGraph(graphX, graphY, graph2X, graph2Y, Hnadle, TRUE);

	DrawString(graphX + 285, graphY + 100, "�X�^�[�g", StatrString);

	DrawString(graphX + 270, graphY + 150, "�I�v�V����", StatrString);

	

		//�X�^�[�g�̕����͈̔͂ɓ���������if��
		if (mauseX >= graphX+285 && mauseX <= graphX + 420 && mauseY >= graphY+100&& mauseY <= graphY+ 134)
		{
			//�͈͂ɓ����ĉ����o�͂���Ă��Ȃ���
			if (ClickStartSound == false)
			{
				//�͈͂̊O�ɏo��܂ŉ���炳�Ȃ��悤�ɂ���
				ClickStartSound = true;
				//sound�Đ�
				PlaySoundMem(StartSound, DX_PLAYTYPE_BACK);
			}
			
			time += 1.0f / 3.f;

			//�͈͂ɍs�����Ƃ��ɃG�t�F�N�g�̏o��
			if (time > 1.0f)
			{
				time -= 1.0f;
				EfectNowIndex += 1;
				if (EfectNowIndex >= StartEfectXNum)
				{
					EfectNowIndex -= StartEfectXNum;
				}
			}
			DrawExtendGraph(graphX + 220, graphY + 100, graphX + 480, graphY + 160, EfectImg[EfectNowIndex], TRUE);
			//�X�^�[�g�͈̔͂ɓ���ƃG�t�F�N�g���o�͂����
			
			StatrString = GetColor(ColorHandleChange, ColorHandle, ColorHandleChange);
			DrawString(graphX + 285, graphY + 100, "�X�^�[�g", StatrString);
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{//�G�t�F�N�g�̃t���[����
				
				PlaySoundMem(StartDesition, DX_PLAYTYPE_NORMAL, TRUE);
				*screenchange = 2;
			}
		}
		else
		{
			ClickStartSound = false;
		}
	

		if (mauseX >= graphX + 270 && mauseX <= graphX + 436 && mauseY >= graphY + 150 && mauseY <= graphY + 184)
		{
			StatrString = GetColor(ColorHandleChange, ColorHandle, ColorHandleChange);
			DrawString(graphX + 270, graphY + 150, "�I�v�V����", StatrString);
			if (ClickOptionSound == false)
			{
				ClickOptionSound = true;
				PlaySoundMem(StartSound, DX_PLAYTYPE_BACK);
			}


			GreenString = GetColor(ColorHandleChange, ColorHandleChange, ColorHandleChange);
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{
				*screenchange = 1;
			}
		}
		else
		{

			ClickOptionSound =false;
		}

		
	
}


void MeinScreenChanger::Music()
{
	if (CheckSoundMem(MainScreenBgm)==0)
	{
		ChangeVolumeSoundMem(82, MainScreenBgm);
		PlaySoundMem(MainScreenBgm, DX_PLAYTYPE_LOOP, TRUE);
	}
}

