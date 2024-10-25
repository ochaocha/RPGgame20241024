#pragma once
#include "Dxlib.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "map.h"


typedef struct Location
{
	float rx;		//右下の座標
	float ry;		//右下の座標
	float lx;		//左上の座標
	float ly;		//左下の座標
	int w;		//幅
	int h;		//幅
};


/// <summary>
/// マップ初期化
/// </summary>
void mapInit();

/// <summary>
/// マップ描画
/// </summary>
void mapDraw();
/// <summary>
/// マップの動き
/// </summary>
void mapaupdate();
/// @brief マップエンジン

class MapData {
private:

    std::vector<std::vector<int>> map;             // マップの2次元配列

public:
    bool LoadMapdata(std::string filePath)
    {
        std::string linebuf;                       // 1行読み込みバッファ

        std::string data;                          // カンマ区切りで切り出したデータ格納用
        //ファイルオープン
        std::ifstream csvFile(filePath);
        if (csvFile.fail()) { return false; }      //読み込みに失敗したのでfalseを返します

        // ファイルからCSV読み込み

        int line = 0;

        while (getline(csvFile, linebuf))
        {
            // map配列の行を追加
            map.emplace_back();
            //カンマ区切りで読みやすいように istringstream型に変換
            std::istringstream iStream(linebuf);

            // カンマ区切り単位でdataに文字列格納
            while (getline(iStream, data, ','))
            {
                // 文字列データを数値に変換して、map[line][]の末尾に追加
                map[line].emplace_back(stoi(data));
            }
            line++;
        }
        csvFile.close();                     //一応クローズします

        return true;                         //最後まで実行できたのでtrueを返します
    }
public:
    //特定のチップのマップデータを取得します
    int GetMapChip(int x, int y)
    {
        if (map[y][x] >= 0xFFFF)
        {
            return map[y][x] - 0xFFFF;
        }
        return map[y][x];
    }
    //特定のチップのマップデータが壁かどうかを判別します
    bool IsWallMapChip(int x, int y) {
        return map[y][x] >= 0xFFFF;         //0xFFFF(65535)以上なら壁と判別します
    }

    //マップのXサイズを返します

    int GetMapXsize()
    {
        //仮でmap[0]~[max]まで全部同じサイズと仮定します
        return static_cast<int>(map[0].size());
    }

    //マップのYサイズを返します
    int GetMapYsize()
    {
        return static_cast<int>(map.size());
    }
};


static MapData mapData;

void MapEngine();