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


/// <summary>
/// �}�b�v������
/// </summary>
void mapInit();

/// <summary>
/// �}�b�v�`��
/// </summary>
void mapDraw();
/// <summary>
/// �}�b�v�̓���
/// </summary>
void mapaupdate();
/// @brief �}�b�v�G���W��

class MapData {
private:

    std::vector<std::vector<int>> map;             // �}�b�v��2�����z��

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
            map.emplace_back();
            //�J���}��؂�œǂ݂₷���悤�� istringstream�^�ɕϊ�
            std::istringstream iStream(linebuf);

            // �J���}��؂�P�ʂ�data�ɕ�����i�[
            while (getline(iStream, data, ','))
            {
                // ������f�[�^�𐔒l�ɕϊ����āAmap[line][]�̖����ɒǉ�
                map[line].emplace_back(stoi(data));
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
        if (map[y][x] >= 0xFFFF)
        {
            return map[y][x] - 0xFFFF;
        }
        return map[y][x];
    }
    //����̃`�b�v�̃}�b�v�f�[�^���ǂ��ǂ����𔻕ʂ��܂�
    bool IsWallMapChip(int x, int y) {
        return map[y][x] >= 0xFFFF;         //0xFFFF(65535)�ȏ�Ȃ�ǂƔ��ʂ��܂�
    }

    //�}�b�v��X�T�C�Y��Ԃ��܂�

    int GetMapXsize()
    {
        //����map[0]~[max]�܂őS�������T�C�Y�Ɖ��肵�܂�
        return static_cast<int>(map[0].size());
    }

    //�}�b�v��Y�T�C�Y��Ԃ��܂�
    int GetMapYsize()
    {
        return static_cast<int>(map.size());
    }
};


static MapData mapData;

void MapEngine();