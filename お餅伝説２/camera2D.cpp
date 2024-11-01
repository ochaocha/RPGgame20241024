#include "DxLib.h"
#include "Source/camera2D.h"

void ScreenCamera::Screen(int displaywidthX, int displayhightY, int displaywidth2X, int displayhight2Y)
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		displaywidth2X++;
		displaywidthX++;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		displaywidth2X--;
		displaywidthX--;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		displayhightY++;
		displayhight2Y++;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		displayhightY--;
		displayhight2Y--;
	}
	SetGraphMode(displaywidthX + displaywidth2X, displayhightY + displayhight2Y, 16);
}