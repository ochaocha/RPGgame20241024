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
	float rx;		//�E���̍��W
	float ry;		//�E���̍��W
	float lx;		//����̍��W
	float ly;		//�����̍��W
	int w;		//��
	int h;		//��
};



class MapData {

    const int mapChipSize = 40;  // �}�b�v�`�b�v�P�̑傫��

    static const int mapXNum = 100;     // �}�b�v�̉������`�b�v��

    static const int mapYNum = 100;     // �}�b�v�̏c�����`�b�v��

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
private:
    void mapInit();
    void mapDraw();
    void MapEngine();


    std::vector<std::vector<int>> Map;   // �}�b�v��2�����z��

public:
    
    bool LoadMapdata(std::string filePath)
    {
        std::string linebuf;                       // 1�s�ǂݍ��݃o�b�t�@

        std::string data;                          // �J���}��؂�Ő؂�o�����f�[�^�i�[�p
        //�t�@�C���I�[�v��
        std::ifstream csvFile(filePath);
        if (csvFile.fail()) { return false; }      //�ǂݍ��݂Ɏ��s�����̂�false��Ԃ��܂�

        // �t�@�C������CSV�ǂݍ���

         int line = 0;

        while (getline(csvFile, linebuf))
        {
            // map�z��̍s��ǉ�
            Map.emplace_back();
            //�J���}��؂�œǂ݂₷���悤�� istringstream�^�ɕϊ�
            std::istringstream iStream(linebuf);

            // �J���}��؂�P�ʂ�data�ɕ�����i�[
            while (getline(iStream, data, ','))
            {
                // ������f�[�^�𐔒l�ɕϊ����āAmap[line][]�̖����ɒǉ�
                Map[line].emplace_back(stoi(data));
            }
            line++;
        }
        csvFile.close();                     //�ꉞ�N���[�Y���܂�

        return true;                         //�Ō�܂Ŏ��s�ł����̂�true��Ԃ��܂�
    }
public:
    //����̃`�b�v�̃}�b�v�f�[�^���擾���܂�
    int GetMapChip(int x, int y)
    {
        if (Map[y][x] >= 0xFFFF)
        {
            return Map[y][x] - 0xFFFF;
        }
        return Map[y][x];
    }
    //����̃`�b�v�̃}�b�v�f�[�^���ǂ��ǂ����𔻕ʂ��܂�
    bool IsWallMapChip(int x, int y) {
        return Map[y][x] >= 0xFFFF;         //0xFFFF(65535)�ȏ�Ȃ�ǂƔ��ʂ��܂�
    }

    //�}�b�v��X�T�C�Y��Ԃ��܂�

    int GetMapXsize()
    {
        //����map[0]~[max]�܂őS�������T�C�Y�Ɖ��肵�܂�
        return static_cast<int>(Map[0].size());
    }

    //�}�b�v��Y�T�C�Y��Ԃ��܂�
    int GetMapYsize()
    {
        return static_cast<int>(Map.size());
    }
};