#include "map.h"
#include "DxLib.h"

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
	if (std::fabsf(Vector2DX::Cross(Line12 - Line11, Line22 - Line21)) <= 0.001f) {
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
bool CalcLineToLineSlideVector(
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
				int imgIndex = GetMapChip(x, y);              // map配列よりブロック種類を取得
				const Location& mapLoca = GetMapLoca(x, y);

				if (IsWallMapChip(x,y))
				{
					int imgHandle = riverChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

					// マップチップ幅でブロック画像を敷き詰めて描画する
					// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
					DrawGraph(mapLoca.lx, mapLoca.ly, imgHandle, TRUE);
				}
				else
				{
					int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

					// マップチップ幅でブロック画像を敷き詰めて描画する
					// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
					DrawGraph(mapLoca.lx, mapLoca.ly, imgHandle, TRUE);
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

//線分とマップチップのうち壁判定があるものとの当たり判定＋当たった後の移動処理
//PlayerPrev :  移動前の座標
//pPlayerNow :  移動後の座標を渡す(内部で移動処理を経た値を反映します)
//PlayerSize :  キャラの判定サイズの中央からのサイズ値
bool MapData::CalcVectorSlideOnWallChips(const Vector2DX& PlayerPrev, Vector2DX* pPlayerNow, const Vector2DX& PlayerMinSize, const Vector2DX& PlayerMaxSize)
{
	constexpr int XSEL = 0;//xySelでXを選択している事を示す定数

	bool IsHitAnyWall = false;

	//今回は矩形と矩形どうしですので、簡易な演算とするためにX方向、Y方向で別々にしています。
	for (int xySel = 0; xySel < 2; xySel++)
	{
		//移動後の座標をローカル変数として保持します
		Vector2DX PlayerNow = *pPlayerNow;
		//(XYそれぞれ別々にするため、計算しない方をPrevにして各方向のみのベクトルにします)
		if (xySel == XSEL) {
			PlayerNow.y = PlayerPrev.y;
		}
		else {
			PlayerNow.x = PlayerPrev.x;
		}
		//このマップチップ上での演算結果を判断するためのローカル変数
		float PlayerVectorLength = (PlayerNow - PlayerPrev).sqrMagnitude();//ヒットするまでに進んだ距離(最初にヒットするまでは ヒットするまでに進むはずだった距離)
		bool IsHitLine = false;//このループ上で何かのマップチップに当たったか
		//マップチップに当たった場合の結果を代入するための変数
		Vector2DX ResultNow = PlayerNow;
		//全てのマップチップに対して、当たっているかと当たった場合のベクトル変異を演算します(全部やる必要はないので、ブレゼンハムのアルゴリズムなどを用いて最適化できます)
		for (int ypos = 0; ypos < GetMapYsize(); ypos++)
		{
			for (int xpos = 0; xpos < GetMapXsize(); xpos++)
			{
				//壁チップの場合
				if (IsWallMapChip(xpos, ypos))
				{
					//そのマップのデータを取得します
					const Location& mapLoca = GetMapLoca(xpos, ypos);
					//チップの矩形の辺と線分との交点のうちPlayerPrevに一番近い方を得る
					//左上→右上の判定
					if (!IsWallMapChip(xpos, ypos - 1))//この辺に面したマップチップが壁なら、判断しなくてもよいものとして省きます
					{
						//一時変数を用意します
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//この辺に移動ベクトルが当たった際のずりベクトルを計算します。
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.lx + PlayerMinSize.x, mapLoca.ly + PlayerMinSize.y),
							Vector2DX::vget(mapLoca.rx + PlayerMaxSize.x, mapLoca.ly + PlayerMinSize.y), Vector2DX::down()))
						{
							//ずり計算後、ずる直前に辺にヒットした座標まで進んでいた長さを取ります
							float Length = (HitPos - PlayerPrev).sqrMagnitude();
							//もしそれが今ある辺の中で一番短いのであれば、ヒットしたものとして更新します。
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNow = SlidePos;
							}
						}
					}
					//以下、ほとんど同じ処理が入るのでコメントを省きます
					//右上→右下の判定
					if (!IsWallMapChip(xpos + 1, ypos))
					{
						//一時変数を用意します
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//この辺に移動ベクトルが当たった際のずりベクトルを計算します。
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.rx + PlayerMaxSize.x, mapLoca.ly + PlayerMinSize.y),
							Vector2DX::vget(mapLoca.rx + PlayerMaxSize.x, mapLoca.ry + PlayerMaxSize.y), Vector2DX::right()))
						{
							float Length = (HitPos - PlayerPrev).sqrMagnitude();
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNow = SlidePos;
							}
						}
					}
					//右下→左下の判定
					if (!IsWallMapChip(xpos, ypos + 1))
					{
						//一時変数を用意します
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//この辺に移動ベクトルが当たった際のずりベクトルを計算します。
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.rx + PlayerMaxSize.x, mapLoca.ry + PlayerMaxSize.y),
							Vector2DX::vget(mapLoca.lx + PlayerMinSize.x, mapLoca.ry + PlayerMaxSize.y), Vector2DX::up()))
						{
							float Length = (HitPos - PlayerPrev).sqrMagnitude();
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNow = SlidePos;
							}
						}
					}
					//左下→左上の判定
					if (!IsWallMapChip(xpos - 1, ypos))
					{
						//一時変数を用意します
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//この辺に移動ベクトルが当たった際のずりベクトルを計算します。
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.lx + PlayerMinSize.x, mapLoca.ry + PlayerMaxSize.y),
							Vector2DX::vget(mapLoca.lx + PlayerMinSize.x, mapLoca.ly + PlayerMinSize.y), Vector2DX::left()))
						{
							float Length = (HitPos - PlayerPrev).sqrMagnitude();
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNow = SlidePos;
							}
						}
					}
				}
			}
		}
		//この矩形上ではヒットしていると判断できるので次ループの計算をヒット後のずりベクトルに対するものにします
		if (IsHitLine) {
			//何かに当たったフラグをオン
			IsHitAnyWall = true;
			//保持した演算結果を反映(XYそれぞれ別々に)
			if (xySel == XSEL) {
				pPlayerNow->x = ResultNow.x;
			}
			else {
				pPlayerNow->y = ResultNow.y;
			}
		}
	}
	//どこかしらでヒットした場合TRUE
	return IsHitAnyWall;
}
