#include "MainScreen.h"
#include "Dxlib.h"


int graphX = 600, graphY = 600, graph2X = 1320, graph2Y = 1000;


/// @briefスタート画面の関数
/// @param change 
/// @param MeinscreenVertical 
/// @param MeinscreenHorizontal 
void MeinScreen::MeinDrawScreen(int* change)
{


	
	GetMousePoint(&mauseX, &mauseY);
	SetFontSize(34);
	unsigned int Cr = GetColor(ColorHandle, ColorHandle, ColorHandle);
	int startHandle = LoadGraph("img/pipo-bg004b.png");
	

		DrawExtendGraph(0, 0,1920,1080,startHandle, TRUE);

		int Hnadle = LoadGraph("img/R.png", TRUE);

		DrawExtendGraph(graphX, graphY, graph2X, graph2Y, Hnadle, TRUE);

		
		if (mauseX >= graphX+285 && mauseX <= graphX + 420 && mauseY >= graphY+100&& mauseY <= graphY+ 134)
		{
			Cr = GetColor(ColorHnadleChange, ColorHnadleChange, ColorHnadleChange);
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)
			{
				rangeATK = false;
				*change = 1;
			}
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0)
			{
				rangeATK = true;
			}
		}
		DrawString(graphX + 285, graphY + 100, "スタート", Cr);
}


