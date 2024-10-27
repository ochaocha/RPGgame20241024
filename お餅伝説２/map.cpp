#include "map.h"
#include "DxLib.h"

#include <cmath>

// �����ƒ����̔���
// �߂�l=������������Ă��邩�ǂ���
bool GetHitLineToLine(
	const Vector2DX& Line11, const Vector2DX& Line12,
	const Vector2DX& Line21, const Vector2DX& Line22,
	Vector2DX* pReturn,		// pReturn=�����m�ň�ԋ߂����W�̂���Line1��̂��̂�Ԃ����߂̃|�C���^
	float* pLine1AcrossPer	// ��_��Line1��̊��� (��_-Line11)/(Line12-Line11)
) {
	//���S�ɕ��s�ȏꍇ�͏��O
	if (std::fabsf(Vector2DX::Cross(Line12 - Line11, Line22 - Line21)) <= 0.001f) {
		return false;
	}

	//z=0�Ƃ���DXLIB�ł̒����ƒ����̔���֐������s
	SEGMENT_SEGMENT_RESULT Result;
	VECTOR Pos11 = Line11.get();
	VECTOR Pos12 = Line12.get();
	VECTOR Pos21 = Line21.get();
	VECTOR Pos22 = Line22.get();
	DxLib::Segment_Segment_Analyse(&Pos11, &Pos12, &Pos21, &Pos22, &Result);

	//�߂�l��ݒ�
	*pReturn = Result.SegA_MinDist_Pos;
	*pLine1AcrossPer = Result.SegA_MinDist_Pos1_Pos2_t;

	return Result.SegA_SegB_MinDist_Square <= 0.001f * 0.001f;
}

//�����ǂ����ŁALine1�̃x�N�g����Line2��ŃX���C�h�����x�N�g���𓾂�
// Line1=�ڐG���Ă���ꍇ�A�ڐG�_�ƃX���C�h�����_�Ƃ������܂�
// �߂�l=�ڐG���Ă���Ȃ�TRUE
bool CalcLineToLineSlideVector(
	const Vector2DX& Line11, const Vector2DX& Line12, Vector2DX* LineHit, Vector2DX* LineSlide,
	const Vector2DX& Line21, const Vector2DX& Line22,
	const Vector2DX& Line2Normal//Line2�Ɛ����ȃx�N�g���ŁA�O�������w�����̂𒷂�1.0�Ƃ�������
) {
	Vector2DX Return;
	float Line1AcrossPer = 0.f;
	//���������ꍇ
	if (GetHitLineToLine(Line11, Line12, Line21, Line22, &Return, &Line1AcrossPer))
	{
		//Normal��Line1�Ƃ̖@��
		float NormalToLine1Dot = std::fabsf(Vector2DX::Dot(Line2Normal, Line12 - Line11));
		//�ڐG�_����
		*LineHit = Line11 + (Line12 - Line11) * (1.f - Line1AcrossPer);
		//��_��PlayerNow�Ƃ̋����Ԃ�ANormal�����Ɉړ������ꏊ���X���C�h��̍��W
		*LineSlide = Line12 + Line2Normal * NormalToLine1Dot * (1.f - Line1AcrossPer);
		return true;
	}
	return false;
}



#if SINGLETON//Emiya 2-1:
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
				const Location& mapLoca = GetMapLoca(x, y);

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
//PlayerPrev :  �ړ��O�̍��W
//pPlayerNow :  �ړ���̍��W��n��(�����ňړ��������o���l�𔽉f���܂�)
//PlayerSize :  �L�����̔���T�C�Y�̒�������̃T�C�Y�l
bool MapData::CalcVectorSlideOnWallChips(const Vector2DX& PlayerPrev, Vector2DX* pPlayerNow, const Vector2DX& PlayerMinSize, const Vector2DX& PlayerMaxSize)
{
	constexpr int XSEL = 0;//xySel��X��I�����Ă��鎖�������萔

	bool IsHitAnyWall = false;

	//����͋�`�Ƌ�`�ǂ����ł��̂ŁA�ȈՂȉ��Z�Ƃ��邽�߂�X�����AY�����ŕʁX�ɂ��Ă��܂��B
	for (int xySel = 0; xySel < 2; xySel++)
	{
		//�ړ���̍��W�����[�J���ϐ��Ƃ��ĕێ����܂�
		Vector2DX PlayerNow = *pPlayerNow;
		//(XY���ꂼ��ʁX�ɂ��邽�߁A�v�Z���Ȃ�����Prev�ɂ��Ċe�����݂̂̃x�N�g���ɂ��܂�)
		if (xySel == XSEL) {
			PlayerNow.y = PlayerPrev.y;
		}
		else {
			PlayerNow.x = PlayerPrev.x;
		}
		//���̃}�b�v�`�b�v��ł̉��Z���ʂ𔻒f���邽�߂̃��[�J���ϐ�
		float PlayerVectorLength = (PlayerNow - PlayerPrev).sqrMagnitude();//�q�b�g����܂łɐi�񂾋���(�ŏ��Ƀq�b�g����܂ł� �q�b�g����܂łɐi�ނ͂�����������)
		bool IsHitLine = false;//���̃��[�v��ŉ����̃}�b�v�`�b�v�ɓ���������
		//�}�b�v�`�b�v�ɓ��������ꍇ�̌��ʂ������邽�߂̕ϐ�
		Vector2DX ResultNow = PlayerNow;
		//�S�Ẵ}�b�v�`�b�v�ɑ΂��āA�������Ă��邩�Ɠ��������ꍇ�̃x�N�g���ψق����Z���܂�(�S�����K�v�͂Ȃ��̂ŁA�u���[���n���̃A���S���Y���Ȃǂ�p���čœK���ł��܂�)
		for (int ypos = 0; ypos < GetMapYsize(); ypos++)
		{
			for (int xpos = 0; xpos < GetMapXsize(); xpos++)
			{
				//�ǃ`�b�v�̏ꍇ
				if (IsWallMapChip(xpos, ypos))
				{
					//���̃}�b�v�̃f�[�^���擾���܂�
					const Location& mapLoca = GetMapLoca(xpos, ypos);
					//�`�b�v�̋�`�̕ӂƐ����Ƃ̌�_�̂���PlayerPrev�Ɉ�ԋ߂����𓾂�
					//���と�E��̔���
					if (!IsWallMapChip(xpos, ypos - 1))//���̕ӂɖʂ����}�b�v�`�b�v���ǂȂ�A���f���Ȃ��Ă��悢���̂Ƃ��ďȂ��܂�
					{
						//�ꎞ�ϐ���p�ӂ��܂�
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//���̕ӂɈړ��x�N�g�������������ۂ̂���x�N�g�����v�Z���܂��B
						if (CalcLineToLineSlideVector(PlayerPrev, PlayerNow, &HitPos, &SlidePos,
							Vector2DX::vget(mapLoca.lx + PlayerMinSize.x, mapLoca.ly + PlayerMinSize.y),
							Vector2DX::vget(mapLoca.rx + PlayerMaxSize.x, mapLoca.ly + PlayerMinSize.y), Vector2DX::down()))
						{
							//����v�Z��A���钼�O�ɕӂɃq�b�g�������W�܂Ői��ł������������܂�
							float Length = (HitPos - PlayerPrev).sqrMagnitude();
							//�������ꂪ������ӂ̒��ň�ԒZ���̂ł���΁A�q�b�g�������̂Ƃ��čX�V���܂��B
							if (PlayerVectorLength >= Length)
							{
								PlayerVectorLength = Length;
								IsHitLine = true;
								ResultNow = SlidePos;
							}
						}
					}
					//�ȉ��A�قƂ�Ǔ�������������̂ŃR�����g���Ȃ��܂�
					//�E�と�E���̔���
					if (!IsWallMapChip(xpos + 1, ypos))
					{
						//�ꎞ�ϐ���p�ӂ��܂�
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//���̕ӂɈړ��x�N�g�������������ۂ̂���x�N�g�����v�Z���܂��B
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
					//�E���������̔���
					if (!IsWallMapChip(xpos, ypos + 1))
					{
						//�ꎞ�ϐ���p�ӂ��܂�
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//���̕ӂɈړ��x�N�g�������������ۂ̂���x�N�g�����v�Z���܂��B
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
					//����������̔���
					if (!IsWallMapChip(xpos - 1, ypos))
					{
						//�ꎞ�ϐ���p�ӂ��܂�
						Vector2DX HitPos;
						Vector2DX SlidePos;
						//���̕ӂɈړ��x�N�g�������������ۂ̂���x�N�g�����v�Z���܂��B
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
		//���̋�`��ł̓q�b�g���Ă���Ɣ��f�ł���̂Ŏ����[�v�̌v�Z���q�b�g��̂���x�N�g���ɑ΂�����̂ɂ��܂�
		if (IsHitLine) {
			//�����ɓ��������t���O���I��
			IsHitAnyWall = true;
			//�ێ��������Z���ʂ𔽉f(XY���ꂼ��ʁX��)
			if (xySel == XSEL) {
				pPlayerNow->x = ResultNow.x;
			}
			else {
				pPlayerNow->y = ResultNow.y;
			}
		}
	}
	//�ǂ�������Ńq�b�g�����ꍇTRUE
	return IsHitAnyWall;
}
