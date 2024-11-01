#pragma once
#include "Dxlib.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "map.h"

#include "Algorithm/Vector2D.h"

static const int mapChipSize = 40;  // マップチップ１個の大きさ

class Location                                  // Emiya:構造体からクラスに
{
private:                                        // Emiya:メンバ変数は外に出さないようにします
    Vector2DX MinPos;		//右下の座標        // Emiya:lx,lyを最小サイズとして変更
    Vector2DX MaxPos;		//左上の座標        // Emiya:rx,ryを最大サイズとして変更
    //int w;		//幅                        // Emiya:たぶん使ってないので消します
    //int h;		//幅                        // Emiya:たぶん使ってないので消します
    
    int graphID = 0;        //画像ID            // Emiya:追加
    bool isWall = false;    //壁かどうか        // Emiya:追加

public://上記のローカル変数を必要な情報だけ開示するメンバ関数
    const Vector2DX& GetMinPos() const { return MinPos; }
    const Vector2DX& GetMaxPos() const { return MaxPos; }
    const int GetGraphID() const { return graphID; }
    const bool IsWall() const { return isWall; }

public://メンバ関数
   
    void Setup(int xchip, int ychip, int DataByCSV) { // Emiya:貰ったデータをもとに各変数を決めるメンバ関数
        if (DataByCSV >= 0xFFFF)         //0xFFFF(65535)以上なら壁と判別します
        {
            isWall = true;
            graphID = DataByCSV - 0xFFFF;
        }
        else {
            isWall = false;
            graphID = DataByCSV;
        }

        MinPos.x = static_cast<float>(xchip * mapChipSize);			//マップチップの左上X座標
        MaxPos.x = static_cast<float>(MinPos.x + mapChipSize);		//マップチップの右下座標

        MinPos.y = static_cast<float>(ychip * mapChipSize); 	    //マップチップの左上Y座標
        MaxPos.y = static_cast<float>(MinPos.y + mapChipSize);      //マップチップの右下座標
    }
};

#define SINGLETON (TRUE)

class MapData {
#if SINGLETON//Emiya 2-1:シングルトンのインターフェースを追加します。SINGLETONのdefineをFALSE->TRUEにすると有効化されます。
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
    static MapData* Instance(void) noexcept {
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
    MapData() {}
    /// <summary>
    /// デストラクタ(シングルトンでは呼ばれません)
    /// </summary>
    /*~MapData(){}*/
private:
#endif

    const int mapImgXNum = 8;    // マップチップ画像の横方向チップ数

    const int mapImgYNum = 11;   // マップチップ画像の縦方向チップ数

    const int riverImgXNum = 1;	 //海のマップチップ画像の横方向チップ数

    const int riverImgYNum = 5;	 //海のマップチップ画像の縦方向チップ数

    std::vector<std::vector<Location>>  Maploca;

    int mapChipImg[88] = {0}; // 画像ハンドル配列

    int riverChipImg[5] = {0};//川の画像チップ
private:

public:
    /// @briefマップの読み込み  
    void Init();

    void Update();
    /// @briefマップの描画
    void Draw();
    /// @brief マップのCSVファイルから読み取り
    void Engine();

    bool LoadData(std::string filePath)
    {
        std::string linebuf;                       // 1行読み込みバッファ

        std::string data;                          // カンマ区切りで切り出したデータ格納用
        //ファイルオープン
        std::ifstream csvFile(filePath);
        if (csvFile.fail()) { return false; }      //読み込みに失敗したのでfalseを返します

        // ファイルからCSV読み込み

        int xpos = 0;
        int ypos = 0;

        while (getline(csvFile, linebuf))
        {
            // map配列の行を追加
            Maploca.emplace_back();
            //カンマ区切りで読みやすいように istringstream型に変換
            std::istringstream iStream(linebuf);

            // カンマ区切り単位でdataに文字列格納
            xpos = 0;
            while (getline(iStream, data, ','))
            {
                // 文字列データを数値に変換して、map[ypos][]の末尾に追加
                Maploca[ypos].emplace_back();

                Maploca[ypos][xpos].Setup(xpos, ypos, stoi(data));                //データの構築
                xpos++;
            }
            ypos++;
        }
        csvFile.close();                     //一応クローズします

        return true;                         //最後まで実行できたのでtrueを返します
    }
public:
    //特定のチップのマップデータを取得します
    int GetMapChip(int x, int y)
    {
        return Maploca[y][x].GetGraphID();
    }

    const Location& GetMapLoca(int x, int y) {
        return Maploca[y][x];
    }

    //特定のチップのマップデータが壁かどうかを判別します
    bool IsWallMapChip(int x, int y) {
        //指定した座標が取得できない箇所である場合、壁として判別します
        if ((x < 0) || (GetMapXsize() <= x) || (y < 0) || (GetMapYsize() < y)) {
            return true;
        }
        return Maploca[y][x].IsWall();
    }

    //マップのXサイズを返します

    int GetMapXsize()
    {
        //仮でmap[0]~[max]まで全部同じサイズと仮定します
        return static_cast<int>(Maploca[0].size());
    }

    //マップのYサイズを返します
    int GetMapYsize()
    {
        return static_cast<int>(Maploca.size());
    }

    //線分とマップチップのうち壁判定があるものとの当たり判定＋当たった後の移動処理
    bool CalcVectorSlideOnWallChips(const Vector2DX& PlayerPrev, Vector2DX* pPlayerNow, const Vector2DX& PlayerMinSize, const Vector2DX& PlayerMaxSize);

};