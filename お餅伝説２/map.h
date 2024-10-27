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

#define SINGLETON (TRUE)

class MapData {
#if SINGLETON
private:
    /// <summary>
    /// �V���O���g���Ƃ��ĕϐ���錾
    /// </summary>
    static const MapData* m_Singleton;
public:
    /// <summary>
    /// �V���O���g�����쐬
    /// </summary>
    static void Create(void) noexcept { m_Singleton = new MapData(); }
    /// <summary>
    /// �V���O���g���̃|�C���^���擾
    /// </summary>
    static MapData* Instance(void) noexcept 
    {
        if (m_Singleton == nullptr) {                               
            MessageBox(NULL, "Failed Instance Create", "", MB_OK);//�G���[���b�Z�[�W���o���Ă��܂��B
            exit(-1);
        }
        return const_cast<MapData*>(m_Singleton);//const�����̃|�C���^��n���܂��B
    }
private:
    /// <summary>
    /// �R���X�g���N�^(�V���O���g���ł�private�Ő錾���܂�
    /// )
    /// </summary>
    MapData() {
    }
    /// <summary>
    /// �f�X�g���N�^(�V���O���g���ł͌Ă΂�܂���)
    /// </summary>
    /*~MapData(){}*/
private:
#endif
    const int mapChipSize = 40;  // �}�b�v�`�b�v�P�̑傫��

    const int mapImgXNum = 8;    // �}�b�v�`�b�v�摜�̉������`�b�v��

    const int mapImgYNum = 11;   // �}�b�v�`�b�v�摜�̏c�����`�b�v��

    const int riverImgXNum = 1;	 //�C�̃}�b�v�`�b�v�摜�̉������`�b�v��

    const int riverImgYNum = 5;	 //�C�̃}�b�v�`�b�v�摜�̏c�����`�b�v��

    std::vector<std::vector<Location>>  Maploca;

    int mapChipImg[88]; // �摜�n���h���z��

    int riverChipImg[5];//��̉摜�`�b�v

    int offsetX = -2000;

    int offsetY = -2000;

private:


    std::vector<std::vector<int>> Map;   // �}�b�v��2�����z��
   
public:
  

    bool CalcVectorSlideOnWallChip(float PlayerPrevX, float PlayerPrevY, float* pPlayerNowX, float* pPlayerNowY, float PlayerXminSize, float PlayerXmaxSize, float PlayerYminSize, float PlayerYmaxSize);
    /// @brief�}�b�v�̓ǂݍ���  
    void mapInit();

    void mapaupdate(); 
    /// @brief�}�b�v�̕`��
    void mapDraw();    
    /// @brief �}�b�v��CSV�t�@�C������ǂݎ��
    void MapEngine();         

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
            Maploca.emplace_back();
            //�J���}��؂�œǂ݂₷���悤�� istringstream�^�ɕϊ�
            std::istringstream iStream(linebuf);

            // �J���}��؂�P�ʂ�data�ɕ�����i�[
            while (getline(iStream, data, ','))
            {
                // ������f�[�^�𐔒l�ɕϊ����āAmap[line][]�̖����ɒǉ�
                Map[line].emplace_back(stoi(data));
                Maploca[line].emplace_back();
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

class MapVector2D
{
 private:
     std::vector<std::vector<int>> GetMapLoca;

};
