#pragma once


class PlayerFunction {
public:
	void PlayerInit(int displaywidth, int displayhight);
	void Playerappdate();
	void PlayerDraw();
	void PlayerFinalize();

};

static PlayerFunction PlayerFunctionOll;

class  ScreenCamera
{
public:
	
	void Screen(int displaywidthX, int displayhightY, int displaywidth2X, int displayhight2Y);


};

static ScreenCamera camera;
