#include "map.h"
#include "DxLib.h"

#if SINGLETON//Emiya 2-1:
/// <summary>
/// シングルトンとして変数を定義
/// </summary>
const MapData* MapData::m_Singleton = nullptr;
#endif

void MapData::mapInit()
{
	// 画像総数  横枚数x縦枚数,横方向個数,縦方向個数,チップ1枚の横サイズ, チップ1毎の縦サイズ, 画像配列先頭ポインタ 
	LoadDivGraph("map/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	LoadDivGraph("map/river.png", riverImgXNum * riverImgYNum, riverImgXNum, riverImgYNum,mapChipSize, mapChipSize, riverChipImg);

	//Maplocaをstd::vector化したので、現状mapInit()の前にMapEngine()をする必要があります
	for (int iy = 0; iy < GetMapYsize(); iy++)
	{

		for (int ix = 0; ix < GetMapXsize(); ix++)
		{

			Maploca[iy][ix].ly = iy * mapChipSize+offsetY;			    //マップチップの左上Y座標

			Maploca[iy][ix].ry = Maploca[iy][ix].ly + mapChipSize;      //マップチップの右下座標

			Maploca[iy][ix].lx = ix * mapChipSize+offsetX;				//マップチップの左上X座標

			Maploca[iy][ix].rx = Maploca[iy][ix].lx + mapChipSize;		//マップチップの右下座標
		}
	}

}


void  MapData::mapDraw()
{
	
		for (int y = 0; y < GetMapYsize(); y++)
		{
			for (int x = 0; x < GetMapXsize(); x++)
			{
				if (IsWallMapChip(x,y))
				{
					int imgIndex =GetMapChip(x, y);              // map配列よりブロック種類を取得

					int imgHandle = riverChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

					// マップチップ幅でブロック画像を敷き詰めて描画する
					// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				else
				{
					int imgIndex = GetMapChip(x, y);              // map配列よりブロック種類を取得

					int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

					// マップチップ幅でブロック画像を敷き詰めて描画する
					// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				
			}
		}
}

void MapData::mapaupdate()
{
	/*
	for (int iy = 0; iy < GetMapYsize(); iy++)
	{

		for (int ix = 0; ix < GetMapXsize(); ix++)
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

void  MapData::MapEngine()
{
	std::string filePath = "mapTest.csv";      // CSVファイル名

	if (LoadMapdata(filePath) == false) {
		std::cout << "ファイルのオープンに失敗" << std::endl;
	}

	// マップにデータが格納されたか表示
	for (int ypos = 0; ypos < GetMapYsize(); ypos++)
	{
		for (int xpos = 0; xpos < GetMapXsize(); xpos++)
		{
			if (IsWallMapChip(xpos, ypos)) {
				//壁として四角で出力
				std::cout << "■" << ",";
			}
			else {
				std::cout << GetMapChip(xpos, ypos) << ",";
			}
		}
		std::cout << std::endl;
	}
}

