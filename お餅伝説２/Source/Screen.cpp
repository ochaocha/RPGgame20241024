#include "Dxlib.h"
#include "Screen.h"


int graphX = 600, graphY = 600, graph2X = 1320, graph2Y = 1000;


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

	GetMousePoint(&mauseX, &mauseY);

	SetFontSize(34);

		DrawExtendGraph(0, 0,1920,1080,startHandle, TRUE);

		DrawExtendGraph(graphX, graphY, graph2X, graph2Y, Hnadle, TRUE);

		if (mauseX >= graphX+285 && mauseX <= graphX + 420 && mauseY >= graphY+100&& mauseY <= graphY+ 134)
		{
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

