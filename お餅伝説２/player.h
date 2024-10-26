#pragma once
//Emiya 1-3:ここに移動

class PlayerFunction {
	//Emiya 1-1:ここに移動
public:
	void PlayerInit(int displaywidth, int displayhight);
	void Playerappdate();
	void PlayerDraw();
	void PlayerFinalize();

};

static PlayerFunction PlayerFunctionOll;//Emiya 1-5:main.cppのローカル変数にするため移動しましょう

//Emiya 1-4:ScreenCameraクラスはcamera2D.hのようなファイルを用意してそちらに移動しちゃいましょう
class  ScreenCamera
{
public:
	
	void Screen(int displaywidthX, int displayhightY, int displaywidth2X, int displayhight2Y);


};

static ScreenCamera camera;//Emiya 1-5:main.cppのローカル変数にするため移動しましょう

