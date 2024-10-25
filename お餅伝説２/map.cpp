#include "map.h"
#include "DxLib.h"



const int mapChipSize = 40;  // マップチップ１個の大きさ

const int mapXNum = 100;     // マップの横方向チップ数

const int mapYNum = 100;     // マップの縦方向チップ数

const int objectXNum = 50;

const int objectYNum = 50;

const int mapImgXNum = 8;    // マップチップ画像の横方向チップ数

const int mapImgYNum = 11;   // マップチップ画像の縦方向チップ数

const int riverImgXNum = 1;	 //海のマップチップ画像の横方向チップ数

const int riverImgYNum = 5;	 //海のマップチップ画像の縦方向チップ数

unsigned char map[mapYNum][mapXNum];
Location Maploca[mapYNum][mapXNum];

int mapChipImg[88]; // 画像ハンドル配列

int riverChipImg[5];

int offsetX = -2000;
int offsetY = -2000;







void mapInit()
{
	for (int i = 0; i > mapYNum; i++)
	{
		for (int j = 0; i > mapXNum; j++)
		{
			map[j][i] = 0;
		}
	}

	// 画像総数  横枚数x縦枚数,横方向個数,縦方向個数,チップ1枚の横サイズ, チップ1毎の縦サイズ, 画像配列先頭ポインタ 
	LoadDivGraph("map/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	LoadDivGraph("map/river.png", riverImgXNum * riverImgYNum, riverImgXNum, riverImgYNum,mapChipSize, mapChipSize, riverChipImg);

	for (int iy = 0; iy < mapYNum; iy++)
	{

		for (int ix = 0; ix < mapXNum; ix++)
		{

			Maploca[iy][ix].ly = iy * mapChipSize+offsetY;			    //マップチップの左上Y座標

			Maploca[iy][ix].ry = Maploca[iy][ix].ly + mapChipSize;      //マップチップの右下座標

			Maploca[iy][ix].lx = ix * mapChipSize+offsetX;				//マップチップの左上X座標

			Maploca[iy][ix].rx = Maploca[iy][ix].lx + mapChipSize;		//マップチップの右下座標
		}
	}

}


void mapDraw()
{
	
		for (int y = 0; y < mapData.GetMapYsize(); y++)
		{
			for (int x = 0; x < mapData.GetMapXsize(); x++)
			{
				if (mapData.IsWallMapChip(x,y))
				{
					int imgIndex = mapData.GetMapChip(x, y);              // map配列よりブロック種類を取得

					int imgHandle = riverChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

					// マップチップ幅でブロック画像を敷き詰めて描画する
					// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				else
				{
					int imgIndex = mapData.GetMapChip(x, y);              // map配列よりブロック種類を取得

					int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

					// マップチップ幅でブロック画像を敷き詰めて描画する
					// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				
			}
		}
}

void mapaupdate()
{
	/*
	for (int iy = 0; iy < mapYNum; iy++)
	{

		for (int ix = 0; ix < mapXNum; ix++)
		{
			Maploca[iy][ix].ly;
			Maploca[iy][ix].ry;
			Maploca[iy][ix].lx;
			Maploca[iy][ix].rx;
			int imgIndex = map[iy][ix];            // map配列よりブロック種類を取得
			int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

			// マップチップ幅でブロック画像を敷き詰めて描画する
			// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
			//if(Maploca[iy][ix])
			DrawGraph(Maploca[iy][ix].lx, Maploca[iy][ix].ly, imgHandle, TRUE);
		}
	}
	//*/
}

void MapEngine()
{
	std::string filePath = "mapTest.csv";      // CSVファイル名

	if (mapData.LoadMapdata(filePath) == false) {
		std::cout << "ファイルのオープンに失敗" << std::endl;
	}

	// マップにデータが格納されたか表示
	for (int ypos = 0; ypos < mapData.GetMapYsize(); ypos++)
	{
		for (int xpos = 0; xpos < mapData.GetMapXsize(); xpos++)
		{
			if (mapData.IsWallMapChip(xpos, ypos)) {
				//壁として四角で出力
				std::cout << "■" << ",";
			}
			else {
				std::cout << mapData.GetMapChip(xpos, ypos) << ",";
			}
		}
		std::cout << std::endl;
	}
}

