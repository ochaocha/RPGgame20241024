#include "map.h"
#include "DxLib.h"

#include <cmath>


/// <summary>
/// 2�����̊O��
/// </summary>
float			Cross(float Ax, float Ay, float Bx, float By) noexcept { return Ax * By - Ay * Bx; }

/// <summary>
/// 2�����̓���
/// </summary>
float			Dot(float Ax, float Ay, float Bx, float By) noexcept { return Ax * Bx + Ay * By; }

// �����ƒ����̔���
// �߂�l=������������Ă��邩�ǂ���
bool GetHitLineToLine
(
	float Line1X1, float Line1Y1, float Line1X2, float Line1Y2,
	float Line2X1, float Line2Y1, float Line2X2, float Line2Y2,
	float* pReturnX, float* pReturnY,		// pReturnX,pReturnY=�����m�ň�ԋ߂����W�̂���Line1��̂��̂�Ԃ����߂̃|�C���^
	float* pLine1AcrossPer					// ��_��Line1��̊��� (��_-Line11)/(Line12-Line11)
) {
	//���S�ɕ��s�ȏꍇ�͏��O
	if (std::fabsf(Cross(Line1X2 - Line1X1, Line1Y2 - Line1Y1, Line2X2 - Line2X1, Line2Y2 - Line2Y1)) <= 0.001f) {
		return false;
	}

	//z=0�Ƃ���DXLIB�ł̒����ƒ����̔���֐������s
	SEGMENT_SEGMENT_RESULT Result;
	VECTOR Pos11 = VGet(Line1X1, Line1Y1, 0.f);
	VECTOR Pos12 = VGet(Line1X2, Line1Y2, 0.f);
	VECTOR Pos21 = VGet(Line2X1, Line2Y1, 0.f);
	VECTOR Pos22 = VGet(Line2X2, Line2Y2, 0.f);
	DxLib::Segment_Segment_Analyse(&Pos11, &Pos12, &Pos21, &Pos22, &Result);

	//�߂�l��ݒ�
	*pReturnX = Result.SegA_MinDist_Pos.x;
	*pReturnY = Result.SegA_MinDist_Pos.y;
	*pLine1AcrossPer = Result.SegA_MinDist_Pos1_Pos2_t;

	return Result.SegA_SegB_MinDist_Square <= 0.001f * 0.001f;
}

//�����ǂ����ŁALine1�̃x�N�g����Line2��ŃX���C�h�����x�N�g���𓾂�
// Line1=�ڐG���Ă���ꍇ�A�ڐG�_�ƃX���C�h�����_�Ƃ������܂�
// �߂�l=�ڐG���Ă���Ȃ�TRUE
bool CalcLineToLineSlideVector(
	float Line1X1, float Line1Y1, float Line1X2, float Line1Y2, float* LineHitX, float* LineHitY, float* LineSlideX, float* LineSlideY,
	float Line2X1, float Line2Y1, float Line2X2, float Line2Y2,//
	float Line2NormalX, float Line2NormalY//Line2�Ɛ����ȃx�N�g���ŁA�O�������w�����̂𒷂�1.0�Ƃ�������
) {
	float ReturnX = 0.f;
	float ReturnY = 0.f;
	float Line1AcrossPer = 0.f;
	//���������ꍇ
	if (GetHitLineToLine(
		Line1X1, Line1Y1, Line1X2, Line1Y2,
		Line2X1, Line2Y1, Line2X2, Line2Y2,
		&ReturnX, &ReturnY, &Line1AcrossPer)
		)
	{
		//Normal��Line1�Ƃ̖@��
		float NormalToLine1Dot = std::fabsf(Dot(Line2NormalX, Line2NormalY, Line1X2 - Line1X1, Line1Y2 - Line1Y1));
		//�ڐG�_����
		*LineHitX = Line1X1 + (Line1X2 - Line1X1) * (1.f - Line1AcrossPer);
		*LineHitY = Line1Y1 + (Line1Y2 - Line1Y1) * (1.f - Line1AcrossPer);
		//��_��PlayerNow�Ƃ̋����Ԃ�ANormal�����Ɉړ������ꏊ���X���C�h��̍��W
		*LineSlideX = Line1X2 + Line2NormalX * NormalToLine1Dot * (1.f - Line1AcrossPer);
		*LineSlideY = Line1Y2 + Line2NormalY * NormalToLine1Dot * (1.f - Line1AcrossPer);
		return true;
	}
	return false;
}



#if SINGLETON
/// <summary>
/// �V���O���g���Ƃ��ĕϐ����`
/// </summary>
const MapData* MapData::m_Singleton = nullptr;
#endif

void MapData::mapInit()
{
	// �摜����  ������x�c����,��������,�c������,�`�b�v1���̉��T�C�Y, �`�b�v1���̏c�T�C�Y, �摜�z��擪�|�C���^ 
	LoadDivGraph("map/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	LoadDivGraph("map/river.png", riverImgXNum * riverImgYNum, riverImgXNum, riverImgYNum,mapChipSize, mapChipSize, riverChipImg);

	//Maploca��std::vector�������̂ŁA����mapInit()�̑O��MapEngine()������K�v������܂�
	for (int iy = 0; iy < GetMapYsize(); iy++)
	{

		for (int ix = 0; ix < GetMapXsize(); ix++)
		{

			Maploca[iy][ix].ly = iy * mapChipSize+offsetY;			    //�}�b�v�`�b�v�̍���Y���W

			Maploca[iy][ix].ry = Maploca[iy][ix].ly + mapChipSize;      //�}�b�v�`�b�v�̉E�����W

			Maploca[iy][ix].lx = ix * mapChipSize+offsetX;				//�}�b�v�`�b�v�̍���X���W

			Maploca[iy][ix].rx = Maploca[iy][ix].lx + mapChipSize;		//�}�b�v�`�b�v�̉E�����W
		}
	}

}


void  MapData::mapDraw()
{
	
		for (int y = 0; y < GetMapYsize(); y++)
		{
			for (int x = 0; x < GetMapXsize(); x++)
			{
				int imgIndex = GetMapChip(x, y);              // map�z����u���b�N��ނ��擾
				const Location& mapLoca = MapVector2D::GetMapLoca(x, y);

				if (IsWallMapChip(x,y))
				{
					int imgHandle = riverChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

					// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
					// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
					DrawGraph(mapLoca.lx, mapLoca.ly, imgHandle, TRUE);
				}
				else
				{
					int imgHandle = mapChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

					// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
					// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
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
			int imgIndex = map[iy][ix];            // map�z����u���b�N��ނ��擾
			int imgHandle = mapChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

			// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
			// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
			//if(Maploca[iy][ix])
			DrawGraph(Maploca[iy][ix].lx, Maploca[iy][ix].ly, imgHandle, TRUE);
		}
	}
	//*/
}

void  MapData::MapEngine()
{
	std::string filePath = "mapTest.csv";      // CSV�t�@�C����

	if (LoadMapdata(filePath) == false) {
		std::cout << "�t�@�C���̃I�[�v���Ɏ��s" << std::endl;
	}

	// �}�b�v�Ƀf�[�^���i�[���ꂽ���\��
	for (int ypos = 0; ypos < GetMapYsize(); ypos++)
	{
		for (int xpos = 0; xpos < GetMapXsize(); xpos++)
		{
			if (IsWallMapChip(xpos, ypos)) {
				//�ǂƂ��Ďl�p�ŏo��
				std::cout << "��" << ",";
			}
			else {
				std::cout << GetMapChip(xpos, ypos) << ",";
			}
		}
		std::cout << std::endl;
	}
}

//�����ƃ}�b�v�`�b�v�̂����ǔ��肪������̂Ƃ̓����蔻��{����������̈ړ�����
//PlayerPrev :  
//pPlayerNow :  
//PlayerSize :  �L�����̔���T�C�Y�̒�������̃T�C�Y�l
bool MapData::CalcVectorSlideOnWallChip
(
	float PlayerPrevX, float PlayerPrevY, float* pPlayerNowX, float* pPlayerNowY,
	float PlayerXminSize, float PlayerXmaxSize, float PlayerYminSize, float PlayerYmaxSize
)
{
	constexpr int XSEL = 0;//xySel��X��I�����Ă��鎖�������萔

	bool IsHitAnyWall = false;

	//����͋�`�Ƌ�`�ǂ����ł��̂ŁA�ȈՂȉ��Z�Ƃ��邽�߂�X�����AY�����ŕʁX�ɂ��Ă��܂��B
	for (int xySel = 0; xySel < 2; xySel++)
	{
		//�ړ���̍��W�����[�J���ϐ��Ƃ��ĕێ����܂�
		float PlayerNowX = *pPlayerNowX;
		float PlayerNowY = *pPlayerNowY;
		//(XY���ꂼ��ʁX�ɂ��邽�߁A�v�Z���Ȃ�����Prev�ɂ��Ċe�����݂̂̃x�N�g���ɂ��܂�)
		if (xySel == XSEL) {
			PlayerNowY = PlayerPrevY;
		}
		else {
			PlayerNowX = PlayerPrevX;
		}
		//���̃}�b�v�`�b�v��ł̉��Z���ʂ𔻒f���邽�߂̃��[�J���ϐ�
		float PlayerVectorLength = std::hypotf(PlayerNowX - PlayerPrevX, PlayerNowY - PlayerPrevY);//�q�b�g����܂łɐi�񂾋���(�ŏ��Ƀq�b�g����܂ł� �q�b�g����܂łɐi�ނ͂�����������)
		bool IsHitLine = false;//���̃��[�v��ŉ����̃}�b�v�`�b�v�ɓ���������
		//�}�b�v�`�b�v�ɓ��������ꍇ�̌��ʂ������邽�߂̕ϐ�
		float ResultNowX = PlayerNowX;
		float ResultNowY = PlayerNowY;
		//�S�Ẵ}�b�v�`�b�v�ɑ΂��āA�������Ă��邩�Ɠ��������ꍇ�̃x�N�g���ψق����Z���܂�(�S�����K�v�͂Ȃ��̂ŁA�u���[���n���̃A���S���Y���Ȃǂ�p���čœK���ł��܂�)
		for (int ypos = 0; ypos < GetMapYsize(); ypos++)
		{
			for (int xpos = 0; xpos < GetMapXsize(); xpos++)
			{
				//�ǃ`�b�v�̏ꍇ
				if (IsWallMapChip(xpos, ypos))
				{
					//���̃}�b�v�̃f�[�^���擾���܂�
					const Location& mapLoca = MapVector2D::GetMapLoca(xpos, ypos);
					//�`�b�v�̋�`�̕ӂƐ����Ƃ̌�_�̂���PlayerPrev�Ɉ�ԋ߂����𓾂�
					//���と�E��̔���
					if (!IsWallMapChip(xpos, ypos - 1))//���̕ӂɖʂ����}�b�v�`�b�v���ǂȂ�A���f���Ȃ��Ă��悢���̂Ƃ��ďȂ��܂�
					{
						//�ꎞ�ϐ���p�ӂ��܂�
						float HitPosX = 0.f;
						float HitPosY = 0.f;
						float SlidePosX = 0.f;
						float SlidePosY = 0.f;
						//���̕ӂɈړ��x�N�g�������������ۂ̂���x�N�g�����v�Z���܂��B
						if (CalcLineToLineSlideVector(PlayerPrevX, PlayerPrevY, PlayerNowX, PlayerNowY, &HitPosX, &HitPosY, &SlidePosX, &SlidePosY,
							mapLoca.lx + PlayerXminSize, mapLoca.ly + PlayerYminSize, mapLoca.rx + PlayerXmaxSize, mapLoca.ly + PlayerYminSize, 0.f, -1.f))
						{
							//����v�Z��A���钼�O�ɕӂɃq�b�g�������W�܂Ői��ł������������܂�
							float Length = std::hypotf(HitPosX - PlayerPrevX, HitPosY - PlayerPrevY);
							//�������ꂪ������ӂ̒��ň�ԒZ���̂ł���΁A�q�b�g�������̂Ƃ��čX�V���܂��B
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNowX = SlidePosX;
								ResultNowY = SlidePosY;
							}
						}
					}
					//�ȉ��A�قƂ�Ǔ�������������̂ŃR�����g���Ȃ��܂�
					//�E�と�E���̔���
					if (!IsWallMapChip(xpos + 1, ypos))
					{
						float HitPosX = 0.0f;
						float HitPosY = 0.0f;
						float SlidePosX = 0.f;
						float SlidePosY = 0.f;
						if (CalcLineToLineSlideVector(PlayerPrevX, PlayerPrevY, PlayerNowX, PlayerNowY, &HitPosX, &HitPosY, &SlidePosX, &SlidePosY,
							mapLoca.rx + PlayerXmaxSize, mapLoca.ly + PlayerYminSize, mapLoca.rx + PlayerXmaxSize, mapLoca.ry + PlayerYmaxSize, 1.f, 0.f))
						{
							float Length = std::hypotf(HitPosX - PlayerPrevX, HitPosY - PlayerPrevY);
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNowX = SlidePosX;
								ResultNowY = SlidePosY;
							}
						}
					}
					//�E���������̔���
					if (!IsWallMapChip(xpos, ypos + 1))
					{
						float HitPosX = 0.0f;
						float HitPosY = 0.0f;
						float SlidePosX = 0.0f;
						float SlidePosY = 0.0f;
						if (CalcLineToLineSlideVector(PlayerPrevX, PlayerPrevY, PlayerNowX, PlayerNowY, &HitPosX, &HitPosY, &SlidePosX, &SlidePosY,
							mapLoca.rx + PlayerXmaxSize, mapLoca.ry + PlayerYmaxSize, mapLoca.lx + PlayerXminSize, mapLoca.ry + PlayerYmaxSize, 0.f, 1.f))
						{
							float Length = std::hypotf(HitPosX - PlayerPrevX, HitPosY - PlayerPrevY);
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNowX = SlidePosX;
								ResultNowY = SlidePosY;
							}
						}
					}
					//����������̔���
					if (!IsWallMapChip(xpos - 1, ypos))
					{
						float HitPosX = 0.f;
						float HitPosY = 0.f;
						float SlidePosX = 0.f;
						float SlidePosY = 0.f;
						if (CalcLineToLineSlideVector(PlayerPrevX, PlayerPrevY, PlayerNowX, PlayerNowY, &HitPosX, &HitPosY, &SlidePosX, &SlidePosY,
							mapLoca.lx + PlayerXminSize, mapLoca.ry + PlayerYmaxSize, mapLoca.lx + PlayerXminSize, mapLoca.ly + PlayerYminSize, -1.f, 0.f))
						{
							float Length = std::hypotf(HitPosX - PlayerPrevX, HitPosY - PlayerPrevY);
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNowX = SlidePosX;
								ResultNowY = SlidePosY;
							}
						}
					}
				}
			}
		}
		//���̋�`��ł̓q�b�g���Ă���Ɣ��f�ł���̂Ŏ����[�v�̌v�Z���q�b�g��̂���x�N�g���ɑ΂�����̂ɂ��܂�
		if (IsHitLine) {
			//�����ɓ��������t���O���I��
			IsHitAnyWall = true;
			//�ێ��������Z���ʂ𔽉f(XY���ꂼ��ʁX��)
			if (xySel == XSEL)
			{
				*pPlayerNowX = ResultNowX;
			}
			else {
				*pPlayerNowY = ResultNowY;
			}
		}
	}
	//�ǂ�������Ńq�b�g�����ꍇTRUE
	return IsHitAnyWall;
}
