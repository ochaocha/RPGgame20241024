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
	//�X�^�[�g�̕����̐F
	unsigned int StatrString = GetColor(ColorHandle, ColorHandle, ColorHandle);

	//�I�v�V�����̕����̐F
	unsigned int OptionString = GetColor(ColorHandle, ColorHandle, ColorHandle);

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


		//�X�^�[�g�̕����͈̔͂ɓ���������if��

		if (mauseX >= graphX+285 && mauseX <= graphX + 420 && mauseY >= graphY+100&& mauseY <= graphY+ 134)
		{
			ChangeVolumeSoundMem(50, StartSound);
			PlaySoundMem(StartSound, DX_PLAYTYPE_NORMAL, TRUE);
			//�G�t�F�N�g�̕`��
		
			time += 1.0f / 10.f;				
			
			//�͈͂ɍs�����Ƃ��ɃG�t�F�N�g�̏o��
			if (time > 1.0f) {							
				time -= 1.0f;							
				EfectNowIndex += 1;						
				if (EfectNowIndex >= StartEfectXNum)
				{
					EfectNowIndex -= StartEfectXNum;		
				}
			}
			
			//�X�^�[�g�͈̔͂ɓ���ƃG�t�F�N�g���o�͂����
			DrawExtendGraph(graphX + 220, graphY + 100, graphX + 480, graphY + 160, EfectImg[EfectNowIndex],TRUE);
			StatrString = GetColor(ColorHandleChange, ColorHandleChange, ColorHandleChange);

			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{
				
				

				*screenchange = 2;
			}
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0)
			{
	
			}
		}
		if (mauseX >= graphX + 270 && mauseX <= graphX + 436 && mauseY >= graphY + 150 && mauseY <= graphY + 184)
		{
			OptionString = GetColor(ColorHandleChange, ColorHandleChange, ColorHandleChange);
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{
				*screenchange = 1;
			}
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0)
			{

			}
		}
		
		DrawString(graphX + 285, graphY + 100, "�X�^�[�g", StatrString);

		DrawString(graphX + 270, graphY + 150, "�I�v�V����", OptionString);

}



void BattleScreenChanger::Scene()
{
	DrawExtendGraph(0, 0, 1920, 1080, BattleScreenHandle, TRUE);
	
}

