#include "map.h"
#include "DxLib.h"



const int mapChipSize = 40;  // �}�b�v�`�b�v�P�̑傫��

const int mapXNum = 100;     // �}�b�v�̉������`�b�v��

const int mapYNum = 100;     // �}�b�v�̏c�����`�b�v��

const int objectXNum = 50;

const int objectYNum = 50;

const int mapImgXNum = 8;    // �}�b�v�`�b�v�摜�̉������`�b�v��

const int mapImgYNum = 11;   // �}�b�v�`�b�v�摜�̏c�����`�b�v��

const int riverImgXNum = 1;	 //�C�̃}�b�v�`�b�v�摜�̉������`�b�v��

const int riverImgYNum = 5;	 //�C�̃}�b�v�`�b�v�摜�̏c�����`�b�v��

unsigned char map[mapYNum][mapXNum];
Location Maploca[mapYNum][mapXNum];

int mapChipImg[88]; // �摜�n���h���z��

int riverChipImg[5];

int offsetX = -2000;
int offsetY = -2000;







void mapInit()
{
	for (int i = 0; i > mapYNum; i++)
	{
		for (int j = 0; i > mapXNum; j++)
		{
			map[j][i] = 0;
		}
	}

	// �摜����  ������x�c����,��������,�c������,�`�b�v1���̉��T�C�Y, �`�b�v1���̏c�T�C�Y, �摜�z��擪�|�C���^ 
	LoadDivGraph("map/map.png", mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	LoadDivGraph("map/river.png", riverImgXNum * riverImgYNum, riverImgXNum, riverImgYNum,mapChipSize, mapChipSize, riverChipImg);

	for (int iy = 0; iy < mapYNum; iy++)
	{

		for (int ix = 0; ix < mapXNum; ix++)
		{

			Maploca[iy][ix].ly = iy * mapChipSize+offsetY;			    //�}�b�v�`�b�v�̍���Y���W

			Maploca[iy][ix].ry = Maploca[iy][ix].ly + mapChipSize;      //�}�b�v�`�b�v�̉E�����W

			Maploca[iy][ix].lx = ix * mapChipSize+offsetX;				//�}�b�v�`�b�v�̍���X���W

			Maploca[iy][ix].rx = Maploca[iy][ix].lx + mapChipSize;		//�}�b�v�`�b�v�̉E�����W
		}
	}

}


void mapDraw()
{
	
		for (int y = 0; y < mapData.GetMapYsize(); y++)
		{
			for (int x = 0; x < mapData.GetMapXsize(); x++)
			{
				if (mapData.IsWallMapChip(x,y))
				{
					int imgIndex = mapData.GetMapChip(x, y);              // map�z����u���b�N��ނ��擾

					int imgHandle = riverChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

					// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
					// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				else
				{
					int imgIndex = mapData.GetMapChip(x, y);              // map�z����u���b�N��ނ��擾

					int imgHandle = mapChipImg[imgIndex];  // index�������ĉ摜�n���h���z�񂩂�摜�n���h�����擾

					// �}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
					// x��0,1,2�E�E�E�ƕω�����B x * mapChipSize �̌v�Z�� 0,64,128,196, ... �ƃu���b�N�����Ƃɑ�����
					DrawGraph(x * mapChipSize, y * mapChipSize, imgHandle, TRUE);
				}
				
			}
		}
}

void mapaupdate()
{
	/*
	for (int iy = 0; iy < mapYNum; iy++)
	{

		for (int ix = 0; ix < mapXNum; ix++)
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

void MapEngine()
{
	std::string filePath = "mapTest.csv";      // CSV�t�@�C����

	if (mapData.LoadMapdata(filePath) == false) {
		std::cout << "�t�@�C���̃I�[�v���Ɏ��s" << std::endl;
	}

	// �}�b�v�Ƀf�[�^���i�[���ꂽ���\��
	for (int ypos = 0; ypos < mapData.GetMapYsize(); ypos++)
	{
		for (int xpos = 0; xpos < mapData.GetMapXsize(); xpos++)
		{
			if (mapData.IsWallMapChip(xpos, ypos)) {
				//�ǂƂ��Ďl�p�ŏo��
				std::cout << "��" << ",";
			}
			else {
				std::cout << mapData.GetMapChip(xpos, ypos) << ",";
			}
		}
		std::cout << std::endl;
	}
}

