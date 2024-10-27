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

#define SINGLETON (TRUE)

class MapData {
#if SINGLETON
private:
    /// <summary>
    /// シングルトンとして変数を宣言
    /// </summary>
    static const MapData* m_Singleton;
public:
    /// <summary>
    /// シングルトンを作成
    /// </summary>
    static void Create(void) noexcept { m_Singleton = new MapData(); }
    /// <summary>
    /// シングルトンのポインタを取得
    /// </summary>
    static MapData* Instance(void) noexcept 
    {
        if (m_Singleton == nullptr) {                               
            MessageBox(NULL, "Failed Instance Create", "", MB_OK);//エラーメッセージを出しています。
            exit(-1);
        }
        return const_cast<MapData*>(m_Singleton);//const抜きのポインタを渡します。
    }
private:
    /// <summary>
    /// コンストラクタ(シングルトンではprivateで宣言します
    /// )
    /// </summary>
    MapData() {
    }
    /// <summary>
    /// デストラクタ(シングルトンでは呼ばれません)
    /// </summary>
    /*~MapData(){}*/
private:
#endif
    const int mapChipSize = 40;  // マップチップ１個の大きさ

    const int mapImgXNum = 8;    // マップチップ画像の横方向チップ数

    const int mapImgYNum = 11;   // マップチップ画像の縦方向チップ数

    const int riverImgXNum = 1;	 //海のマップチップ画像の横方向チップ数

    const int riverImgYNum = 5;	 //海のマップチップ画像の縦方向チップ数

    std::vector<std::vector<Location>>  Maploca;

    int mapChipImg[88]; // 画像ハンドル配列

    int riverChipImg[5];//川の画像チップ

    int offsetX = -2000;

    int offsetY = -2000;

private:


    std::vector<std::vector<int>> Map;   // マップの2次元配列
   
public:
  

    bool CalcVectorSlideOnWallChip(float PlayerPrevX, float PlayerPrevY, float* pPlayerNowX, float* pPlayerNowY, float PlayerXminSize, float PlayerXmaxSize, float PlayerYminSize, float PlayerYmaxSize);
    /// @briefマップの読み込み  
    void mapInit();

    void mapaupdate(); 
    /// @briefマップの描画
    void mapDraw();    
    /// @brief マップのCSVファイルから読み取り
    void MapEngine();         

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
            Map.emplace_back();
            Maploca.emplace_back();
            //カンマ区切りで読みやすいように istringstream型に変換
            std::istringstream iStream(linebuf);

            // カンマ区切り単位でdataに文字列格納
            while (getline(iStream, data, ','))
            {
                // 文字列データを数値に変換して、map[line][]の末尾に追加
                Map[line].emplace_back(stoi(data));
                Maploca[line].emplace_back();
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
        if (Map[y][x] >= 0xFFFF)
        {
            return Map[y][x] - 0xFFFF;
        }
        return Map[y][x];
    }
    //特定のチップのマップデータが壁かどうかを判別します
    bool IsWallMapChip(int x, int y) {

        return Map[y][x] >= 0xFFFF;         //0xFFFF(65535)以上なら壁と判別します
    }

    //マップのXサイズを返します

    int GetMapXsize()
    {
        //仮でmap[0]~[max]まで全部同じサイズと仮定します
        return static_cast<int>(Map[0].size());
    }

    //マップのYサイズを返します
    int GetMapYsize()
    {
        return static_cast<int>(Map.size());
    }
};

class MapVector2D
{
 private:
     std::vector<std::vector<int>> GetMapLoca;

};
