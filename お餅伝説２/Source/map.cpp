#include "DxLib.h"
#include "map.h"
#include <cmath>

// 直線と直線の判定
// 戻り値=直線が交わっているかどうか
bool GetHitLineToLine(
	const Vector2DX& Line11, const Vector2DX& Line12,
	const Vector2DX& Line21, const Vector2DX& Line22,
	Vector2DX* pReturn,		// pReturn=線同士で一番近い座標のうちLine1上のものを返すためのポインタ
	float* pLine1AcrossPer	// 交点のLine1上の割合 (交点-Line11)/(Line12-Line11)
) {
	//完全に平行な場合は除外
	if (std::fabsf(Vector2DX::Cross(Line12 - Line11, Line22 - Line21)) <= 0.001f)
	{
		return false;
	}

	//z=0としてDXLIBでの直線と直線の判定関数を実行
	SEGMENT_SEGMENT_RESULT Result;
	VECTOR Pos11 = Line11.get();
	VECTOR Pos12 = Line12.get();
	VECTOR Pos21 = Line21.get();
	VECTOR Pos22 = Line22.get();
	DxLib::Segment_Segment_Analyse(&Pos11, &Pos12, &Pos21, &Pos22, &Result);

	//戻り値を設定
	*pReturn = Result.SegA_MinDist_Pos;
	*pLine1AcrossPer = Result.SegA_MinDist_Pos1_Pos2_t;

	return Result.SegA_SegB_MinDist_Square <= 0.001f * 0.001f;
}

//直線どうしで、Line1のベクトルをLine2上でスライドしたベクトルを得る
// Line1=接触している場合、接触点とスライドした点とを代入します
// 戻り値=接触しているならTRUE
bool CalcLineToLineSlideVector
(
	const Vector2DX& Line11, const Vector2DX& Line12, Vector2DX* LineHit, Vector2DX* LineSlide,
	const Vector2DX& Line21, const Vector2DX& Line22,
	const Vector2DX& Line2Normal//Line2と垂直なベクトルで、外向きを指すものを長さ1.0としたもの
) {
	Vector2DX Return;
	float Line1AcrossPer = 0.f;
	//交差した場合
	if (GetHitLineToLine(Line11, Line12, Line21, Line22, &Return, &Line1AcrossPer))
	{
		//NormalとLine1との法線
		float NormalToLine1Dot = std::fabsf(Vector2DX::Dot(Line2Normal, Line12 - Line11));
		//接触点を代入
		*LineHit = Line11 + (Line12 - Line11) * (1.f - Line1AcrossPer);
		//交点とPlayerNowとの距離ぶん、Normal方向に移動した場所がスライド後の座標
		*LineSlide = Line12 + Line2Normal * NormalToLine1Dot * (1.f - Line1AcrossPer);
		return true;
	}
	return false;
}



#if SINGLETON//Emiya 2-1:
/// <summary>
/// シングルトンとして変数を定義
/// </summary>
const MapData* MapData::m_Singleton = nullptr;
#endif

void MapData::Init()
{
	// 画像総数  横枚数x縦枚数,横方向個数,縦方向個数,チップ1枚の横サイズ, チップ1毎の縦サイズ, 画像配列先頭ポインタ 
	LoadDivGraph("Data/map/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	LoadDivGraph("Data/map/river.png", riverImgXNum * riverImgYNum, riverImgXNum, riverImgYNum, mapChipSize, mapChipSize, riverChipImg);

}


void  MapData::Draw()
{

	for (int y = 0; y < GetMapYsize(); y++)
	{
		for (int x = 0; x < GetMapXsize(); x++)
		{
			int imgIndex = GetMapChip(x, y);              // map配列よりブロック種類を取得
			const Location& mapLoca = GetMapLoca(x, y);

			if (IsWallMapChip(x, y))
			{
				int imgHandle = riverChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

				// マップチップ幅でブロック画像を敷き詰めて描画する
				// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
				DrawGraph(static_cast<int>(mapLoca.GetMinPos().x), static_cast<int>(mapLoca.GetMinPos().y), imgHandle, TRUE);
			}
			else
			{
				int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

				// マップチップ幅でブロック画像を敷き詰めて描画する
				// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
				DrawGraph(static_cast<int>(mapLoca.GetMinPos().x), static_cast<int>(mapLoca.GetMinPos().y), imgHandle, TRUE);
			}

		}
	}
}

void MapData::Update()
{
	/*
	for (int iy = 0; iy < GetMapYsize(); iy++)
	{

		for (int ix = 0; ix < GetMapXsize(); ix++)
		{
			Maploca[iy][ix].GetMinPos().y;
			Maploca[iy][ix].GetMaxPos().y;
			Maploca[iy][ix].GetMinPos().x;
			Maploca[iy][ix].GetMaxPos().x;
			int imgIndex = map[iy][ix];            // map配列よりブロック種類を取得
			int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列か�                )code_items_cnt'indexes_ready)comment_tokenspath)client_versionversion%	safe_to_open       楸3o灘���炯y   � �ﾗｿl4�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            