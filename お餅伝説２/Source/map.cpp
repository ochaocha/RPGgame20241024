#include "DxLib.h"
#include "map.h"
#include <cmath>

// ’¼ü‚Æ’¼ü‚Ì”»’è
// –ß‚è’l=’¼ü‚ªŒğ‚í‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
bool GetHitLineToLine(
	const Vector2DX& Line11, const Vector2DX& Line12,
	const Vector2DX& Line21, const Vector2DX& Line22,
	Vector2DX* pReturn,		// pReturn=ü“¯m‚Åˆê”Ô‹ß‚¢À•W‚Ì‚¤‚¿Line1ã‚Ì‚à‚Ì‚ğ•Ô‚·‚½‚ß‚Ìƒ|ƒCƒ“ƒ^
	float* pLine1AcrossPer	// Œğ“_‚ÌLine1ã‚ÌŠ„‡ (Œğ“_-Line11)/(Line12-Line11)
) {
	//Š®‘S‚É•½s‚Èê‡‚ÍœŠO
	if (std::fabsf(Vector2DX::Cross(Line12 - Line11, Line22 - Line21)) <= 0.001f)
	{
		return false;
	}

	//z=0‚Æ‚µ‚ÄDXLIB‚Å‚Ì’¼ü‚Æ’¼ü‚Ì”»’èŠÖ”‚ğÀs
	SEGMENT_SEGMENT_RESULT Result;
	VECTOR Pos11 = Line11.get();
	VECTOR Pos12 = Line12.get();
	VECTOR Pos21 = Line21.get();
	VECTOR Pos22 = Line22.get();
	DxLib::Segment_Segment_Analyse(&Pos11, &Pos12, &Pos21, &Pos22, &Result);

	//–ß‚è’l‚ğİ’è
	*pReturn = Result.SegA_MinDist_Pos;
	*pLine1AcrossPer = Result.SegA_MinDist_Pos1_Pos2_t;

	return Result.SegA_SegB_MinDist_Square <= 0.001f * 0.001f;
}

//’¼ü‚Ç‚¤‚µ‚ÅALine1‚ÌƒxƒNƒgƒ‹‚ğLine2ã‚ÅƒXƒ‰ƒCƒh‚µ‚½ƒxƒNƒgƒ‹‚ğ“¾‚é
// Line1=ÚG‚µ‚Ä‚¢‚éê‡AÚG“_‚ÆƒXƒ‰ƒCƒh‚µ‚½“_‚Æ‚ğ‘ã“ü‚µ‚Ü‚·
// –ß‚è’l=ÚG‚µ‚Ä‚¢‚é‚È‚çTRUE
bool CalcLineToLineSlideVector
(
	const Vector2DX& Line11, const Vector2DX& Line12, Vector2DX* LineHit, Vector2DX* LineSlide,
	const Vector2DX& Line21, const Vector2DX& Line22,
	const Vector2DX& Line2Normal//Line2‚Æ‚’¼‚ÈƒxƒNƒgƒ‹‚ÅAŠOŒü‚«‚ğw‚·‚à‚Ì‚ğ’·‚³1.0‚Æ‚µ‚½‚à‚Ì
) {
	Vector2DX Return;
	float Line1AcrossPer = 0.f;
	//Œğ·‚µ‚½ê‡
	if (GetHitLineToLine(Line11, Line12, Line21, Line22, &Return, &Line1AcrossPer))
	{
		//Normal‚ÆLine1‚Æ‚Ì–@ü
		float NormalToLine1Dot = std::fabsf(Vector2DX::Dot(Line2Normal, Line12 - Line11));
		//ÚG“_‚ğ‘ã“ü
		*LineHit = Line11 + (Line12 - Line11) * (1.f - Line1AcrossPer);
		//Œğ“_‚ÆPlayerNow‚Æ‚Ì‹——£‚Ô‚ñANormal•ûŒü‚ÉˆÚ“®‚µ‚½êŠ‚ªƒXƒ‰ƒCƒhŒã‚ÌÀ•W
		*LineSlide = Line12 + Line2Normal * NormalToLine1Dot * (1.f - Line1AcrossPer);
		return true;
	}
	return false;
}



#if SINGLETON//Emiya 2-1:
/// <summary>
/// ƒVƒ“ƒOƒ‹ƒgƒ“‚Æ‚µ‚Ä•Ï”‚ğ’è‹`
/// </summary>
const MapData* MapData::m_Singleton = nullptr;
#endif

void MapData::Init()
{
	// ‰æ‘œ‘”  ‰¡–‡”xc–‡”,‰¡•ûŒüŒÂ”,c•ûŒüŒÂ”,ƒ`ƒbƒv1–‡‚Ì‰¡ƒTƒCƒY, ƒ`ƒbƒv1–ˆ‚ÌcƒTƒCƒY, ‰æ‘œ”z—ñæ“ªƒ|ƒCƒ“ƒ^ 
	LoadDivGraph("Data/map/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	LoadDivGraph("Data/map/river.png", riverImgXNum * riverImgYNum, riverImgXNum, riverImgYNum, mapChipSize, mapChipSize, riverChipImg);

}


void  MapData::Draw()
{
	for (int y = 0; y < GetMapYsize(); y++)
	{
		for (int x = 0; x < GetMapXsize(); x++)
		{
			int imgIndex = GetMapChip(x, y);              // map”z—ñ‚æ‚èƒuƒƒbƒNí—Ş‚ğæ“¾
			const Location& mapLoca = GetMapLoca(x, y);

			if (IsWallMapChip(x, y))
			{
				int imgHandle = riverChipImg[imgIndex];  // index‚ğ‚Â‚©‚Á‚Ä‰æ‘œƒnƒ“ƒhƒ‹”z—ñ‚©‚ç‰æ‘œƒnƒ“ƒhƒ‹‚ğæ“¾

				// ƒ}ƒbƒvƒ`ƒbƒv•‚ÅƒuƒƒbƒN‰æ‘œ‚ğ•~‚«‹l‚ß‚Ä•`‰æ‚·‚é
				// x‚Í0,1,2EEE‚Æ•Ï‰»‚·‚éB x * mapChipSize ‚ÌŒvZ‚Í 0,64,128,196, ... ‚ÆƒuƒƒbƒN•‚²‚Æ‚É‘‚¦‚é
				DrawGraph(static_cast<int>(mapLoca.GetMinPos().x), static_cast<int>(mapLoca.GetMinPos().y), imgHandle, TRUE);
			}
			else
			{
				int imgHandle = mapChipImg[imgIndex];  // index‚ğ‚Â‚©‚Á‚Ä‰æ‘œƒnƒ“ƒhƒ‹”z—ñ‚©‚ç‰æ‘œƒnƒ“ƒhƒ‹‚ğæ“¾

				// ƒ}ƒbƒvƒ`ƒbƒv•‚ÅƒuƒƒbƒN‰æ‘œ‚ğ•~‚«‹l‚ß‚Ä•`‰æ‚·‚é
				// x‚Í0,1,2EEE‚Æ•Ï‰»‚·‚éB x * mapChipSize ‚ÌŒvZ‚Í 0,64,128,196, ... ‚ÆƒuƒƒbƒN•‚²‚Æ‚É‘‚¦‚é
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
			int imgIndex = map[iy][ix];            // map”z—ñ‚æ‚èƒuƒƒbƒNí—Ş‚ğæ“¾
			int imgHandle = mapChipImg[imgIndex];  // index‚ğ‚Â‚©‚Á‚Ä‰æ‘œƒnƒ“ƒhƒ‹”z—ñ‚©E                )code_items_cnt'indexes_ready)comment_tokenspath)client_versionversion%	safe_to_open       ´3o“åEñàvy
   E E×¿l4E
   */
}


void MapData::Engine() {
	std::string filePath = "Data/mapTest.csv";      // CSVƒtƒ@ƒCƒ‹–¼

	if (LoadData(filePath) == false) {
		std::cout << "ƒtƒ@ƒCƒ‹‚ÌƒI[ƒvƒ“‚É¸”s" << std::endl;
	}

	// ƒ}ƒbƒv‚Éƒf[ƒ^‚ªŠi”[‚³‚ê‚½‚©•\¦
	for (int ypos = 0; ypos < GetMapYsize(); ypos++)
	{
		for (int xpos = 0; xpos < GetMapXsize(); xpos++)
		{
			if (IsWallMapChip(xpos, ypos)) {
				//•Ç‚Æ‚µ‚ÄlŠp‚Åo—Í
				std::cout << "¡" << ",";
			}
			else {
				std::cout << GetMapChip(xpos, ypos) << ",";
			}
		}
		std::cout << std::endl;
	}

}

//ü•ª‚Æƒ}ƒbƒvƒ`ƒbƒv‚Ì‚¤‚¿•Ç”»’è‚ª‚ ‚é‚à‚Ì‚Æ‚Ì“–‚½‚è”»’è{“–‚½‚Á‚½Œã‚ÌˆÚ“®ˆ—
//PlayerPrev :  ˆÚ“®‘O‚ÌÀ•W
//pPlayerNow :  ˆÚ“®Œã‚ÌÀ•W‚ğ“n‚·(“à•”‚ÅˆÚ“®ˆ—‚ğŒo‚½’l‚ğ”½‰f‚µ‚Ü‚·)
//PlayerSize :  ƒLƒƒƒ‰‚Ì”»’èƒTƒCƒY‚Ì’†‰›‚©‚ç‚ÌƒTƒCƒY’l
bool MapData::CalcVectorSlideOnWallChips(const Vector2DX& PlayerPrev, Vector2DX* pPlayerNow, const Vector2DX& PlayerMinSize, const Vector2DX& PlayerMaxSize)
{
	constexpr int XSEL = 0;//xySel‚ÅX‚ğ‘I‘ğ‚µ‚Ä‚¢‚é–‚ğ¦‚·’è”

	bool IsHitAnyWall = false;

	//¡‰ñ‚Í‹éŒ`‚Æ‹éŒ`‚Ç‚¤‚µ‚Å‚·‚Ì‚ÅAŠÈˆÕ‚È‰‰Z‚Æ‚·‚é‚½‚ß‚ÉX•ûŒüAY•ûŒü‚Å•ÊX‚É‚µ‚Ä‚¢‚Ü‚·B
	for (int xySel = 0; xySel < 2; xySel++)
	{
		//ˆÚ“®Œã‚ÌÀ•W‚ğƒ[ƒJƒ‹•Ï”‚Æ‚µ‚Ä•Û‚µ‚Ü‚·
		Vector2DX PlayerNow = *pPlayerNow;
		//(XY‚»‚ê‚¼‚ê•ÊX‚É‚·‚é‚½‚ßAŒvZ‚µ‚È‚¢•û‚ğPrev‚É‚µ‚ÄŠe•ûŒü‚Ì‚İ‚ÌƒxƒNƒgƒ‹‚É‚µ‚Ü‚·)
		if (xySel == XSEL) {
			PlayerNow.y = PlayerPrev.y;
		}
		else {
			PlayerNow.x = PlayerPrev.x;
		}
		//‚±‚Ìƒ}ƒbƒvƒ`ƒbƒvã‚Å‚Ì‰‰ZŒ‹‰Ê‚ğ”»’f‚·‚é‚½‚ß‚Ìƒ[ƒJƒ‹•Ï”
		float PlayerVectorLength = (PlayerNow - PlayerPrev).sqrMagnitude();//ƒqƒbƒg‚·‚é‚Ü‚Å‚Éi‚ñ‚¾‹——£(Å‰‚Éƒqƒbƒg‚·‚é‚Ü‚Å‚Í ƒqƒbƒg‚·‚é‚Ü‚Å‚Éi‚Ş‚Í‚¸‚¾‚Á‚½‹——£)
		bool IsHitLine = false;//‚±‚Ìƒ‹[ƒvã‚Å‰½‚©‚Ìƒ}ƒbƒvƒ`ƒbƒv‚É“–‚½‚Á‚½‚©
		//ƒ}ƒbƒvƒ`ƒbƒv‚É“–‚½‚Á‚½ê‡‚ÌŒ‹‰Ê‚ğ‘ã“ü‚·‚é‚½‚ß‚Ì•Ï”
		Vector2DX ResultNow = PlayerNow;
		//‘S‚Ä‚Ìƒ}ƒbƒvƒ`ƒbƒv‚É‘Î‚µ‚ÄA“–‚½‚Á‚Ä‚¢‚é‚©‚Æ“–‚½‚Á‚½ê‡‚ÌƒxƒNƒgƒ‹•ÏˆÙ‚ğ‰‰Z‚µ‚Ü‚·(‘S•”‚â‚é•K—v‚Í‚È‚¢‚Ì‚ÅAƒuƒŒƒ[ƒ“ƒnƒ€‚ÌƒAƒ‹ƒSƒŠƒYƒ€‚È‚Ç‚ğ—p‚¢‚ÄÅ“K‰»‚Å‚«‚Ü‚·)
		for (int ypos = 0; ypos < GetMapYsize(); ypos++)
		{
			for (int xpos = 0; xpos < GetMapXsize(); xpos++)
			{
				//•Çƒ`ƒbƒv‚Ìê‡
				if (IsWallMapChip(xpos, ypos))
				{
					//‚»‚Ìƒ}ƒbƒv‚Ìƒf[ƒ^‚ğæ“¾‚µ‚Ü‚·
					const Location& mapLoca = GetMapLoca(xpos, ypos);
					//ƒ`ƒbƒv‚Ì‹éŒ`‚Ì•Ó‚Æü•ª‚Æ‚ÌŒğ“_‚Ì‚¤‚¿PlayerPrev‚Éˆê”Ô‹ß‚¢•û‚ğ“¾‚é
					//¶ã¨‰Eã‚Ì”»’è
					if (!IsWallMapChip(xpos, ypos - 1))//‚±‚Ì•Ó‚É–Ê‚µ‚½ƒ}ƒbƒvƒ`ƒbƒv‚ª•Ç‚È‚çA”»’f‚µ‚È‚­‚Ä‚à‚æ‚¢‚à‚Ì‚Æ‚µ‚ÄÈ‚«‚Ü‚·
					{
						//ˆê•Ï”‚ğ—pˆÓ‚µ‚Ü‚·
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//‚±‚Ì•Ó‚ÉˆÚ“®ƒxƒNƒgƒ‹‚ª“–‚½‚Á‚½Û‚Ì‚¸‚èƒxƒNƒgƒ‹‚ğŒvZ‚µ‚Ü‚·B
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.GetMinPos().x + PlayerMinSize.x, mapLoca.GetMinPos().y + PlayerMinSize.y),
							Vector2DX::vget(mapLoca.GetMaxPos().x + PlayerMaxSize.x, mapLoca.GetMinPos().y + PlayerMinSize.y), Vector2DX::down()))
						{
							//‚¸‚èŒvZŒãA‚¸‚é’¼‘O‚É•Ó‚Éƒqƒbƒg‚µ‚½À•W‚Ü‚Åi‚ñ‚Å‚¢‚½’·‚³‚ğæ‚è‚Ü‚·
							float Length = (HitPos - PlayerPrev).sqrMagnitude();
							//‚à‚µ‚»‚ê‚ª¡‚ ‚é•Ó‚Ì’†‚Åˆê”Ô’Z‚¢‚Ì‚Å‚ ‚ê‚ÎAƒqƒbƒg‚µ‚½‚à‚Ì‚Æ‚µ‚ÄXV‚µ‚Ü‚·B
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNow = SlidePos;
							}
						}
					}
					//ˆÈ‰ºA‚Ù‚Æ‚ñ‚Ç“¯‚¶ˆ—‚ª“ü‚é‚Ì‚ÅƒRƒƒ“ƒg‚ğÈ‚«‚Ü‚·
					//‰Eã¨‰E‰º‚Ì”»’è
					if (!IsWallMapChip(xpos + 1, ypos))
					{
						//ˆê•Ï”‚ğ—pˆÓ‚µ‚Ü‚·
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//‚±‚Ì•Ó‚ÉˆÚ“®ƒxƒNƒgƒ‹‚ª“–‚½‚Á‚½Û‚Ì‚¸‚èƒxƒNƒgƒ‹‚ğŒvZ‚µ‚Ü‚·B
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.GetMaxPos().x + PlayerMaxSize.x, mapLoca.GetMinPos().y + PlayerMinSize.y),
							Vector2DX::vget(mapLoca.GetMaxPos().x + PlayerMaxSize.x, mapLoca.GetMaxPos().y + PlayerMaxSize.y), Vector2DX::right()))
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
					//‰E‰º¨¶‰º‚Ì”»’è
					if (!IsWallMapChip(xpos, ypos + 1))
					{
						//ˆê•Ï”‚ğ—pˆÓ‚µ‚Ü‚·
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//‚±‚Ì•Ó‚ÉˆÚ“®ƒxƒNƒgƒ‹‚ª“–‚½‚Á‚½Û‚Ì‚¸‚èƒxƒNƒgƒ‹‚ğŒvZ‚µ‚Ü‚·B
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.GetMaxPos().x + PlayerMaxSize.x, mapLoca.GetMaxPos().y + PlayerMaxSize.y),
							Vector2DX::vget(mapLoca.GetMinPos().x + PlayerMinSize.x, mapLoca.GetMaxPos().y + PlayerMaxSize.y), Vector2DX::up()))
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
					//¶‰º¨¶ã‚Ì”»’è
					if (!IsWallMapChip(xpos - 1, ypos))
					{
						//ˆê•Ï”‚ğ—pˆÓ‚µ‚Ü‚·
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//‚±‚Ì•Ó‚ÉˆÚ“®ƒxƒNƒgƒ‹‚ª“–‚½‚Á‚½Û‚Ì‚¸‚èƒxƒNƒgƒ‹‚ğŒvZ‚µ‚Ü‚·B
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.GetMinPos().x + PlayerMinSize.x, mapLoca.GetMaxPos().y + PlayerMaxSize.y),
							Vector2DX::vget(mapLoca.GetMinPos().x + PlayerMinSize.x, mapLoca.GetMinPos().y + PlayerMinSize.y), Vector2DX::left()))
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
		//‚±‚Ì‹éŒ`ã‚Å‚Íƒqƒbƒg‚µ‚Ä‚¢‚é‚Æ”»’f‚Å‚«‚é‚Ì‚ÅŸƒ‹[ƒv‚ÌŒvZ‚ğƒqƒbƒgŒã‚Ì‚¸‚èƒxƒNƒgƒ‹‚É‘Î‚·‚é‚à‚Ì‚É‚µ‚Ü‚·
		if (IsHitLine) {
			//‰½‚©‚É“–‚½‚Á‚½ƒtƒ‰ƒO‚ğƒIƒ“
			IsHitAnyWall = true;
			//•Û‚µ‚½‰‰ZŒ‹‰Ê‚ğ”½‰f(XY‚»‚ê‚¼‚ê•ÊX‚É)
			if (xySel == XSEL) {
				pPlayerNow->x = ResultNow.x;
			}
			else {
				pPlayerNow->y = ResultNow.y;
			}
		}
	}
	//‚Ç‚±‚©‚µ‚ç‚Åƒqƒbƒg‚µ‚½ê‡TRUE
	return IsHitAnyWall;
}
