#include "map.h"
#include "DxLib.h"

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
				if (IsWallMapChip(x,y))
				{
					int imgIndex =GetMapChip(x, y);              // map�z����u���b�N��ނ��擾

					int imgHandle = riverChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

					// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
					// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				else
				{
					int imgIndex = GetMapChip(x, y);              // map�z����u���b�N��ނ��擾

					int imgHandle = mapChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

					// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
					// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
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

