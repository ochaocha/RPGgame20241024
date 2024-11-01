#pragma once
#include "Dxlib.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "map.h"

#include "Algorithm/Vector2D.h"

static const int mapChipSize = 40;  // �}�b�v�`�b�v�P�̑傫��

class Location                                  // Emiya:�\���̂���N���X��
{
private:                                        // Emiya:�����o�ϐ��͊O�ɏo���Ȃ��悤�ɂ��܂�
    Vector2DX MinPos;		//�E���̍��W        // Emiya:lx,ly���ŏ��T�C�Y�Ƃ��ĕύX
    Vector2DX MaxPos;		//����̍��W        // Emiya:rx,ry���ő�T�C�Y�Ƃ��ĕύX
    //int w;		//��                        // Emiya:���Ԃ�g���ĂȂ��̂ŏ����܂�
    //int h;		//��                        // Emiya:���Ԃ�g���ĂȂ��̂ŏ����܂�
    
    int graphID = 0;        //�摜ID            // Emiya:�ǉ�
    bool isWall = false;    //�ǂ��ǂ���        // Emiya:�ǉ�

public://��L�̃��[�J���ϐ���K�v�ȏ�񂾂��J�����郁���o�֐�
    const Vector2DX& GetMinPos() const { return MinPos; }
    const Vector2DX& GetMaxPos() const { return MaxPos; }
    const int GetGraphID() const { return graphID; }
    const bool IsWall() const { return isWall; }

public://�����o�֐�
   
    void Setup(int xchip, int ychip, int DataByCSV) { // Emiya:������f�[�^�����ƂɊe�ϐ������߂郁���o�֐�
        if (DataByCSV >= 0xFFFF)         //0xFFFF(65535)�ȏ�Ȃ�ǂƔ��ʂ��܂�
        {
            isWall = true;
            graphID = DataByCSV - 0xFFFF;
        }
        else {
            isWall = false;
            graphID = DataByCSV;
        }

        MinPos.x = static_cast<float>(xchip * mapChipSize);			//�}�b�v�`�b�v�̍���X���W
        MaxPos.x = static_cast<float>(MinPos.x + mapChipSize);		//�}�b�v�`�b�v�̉E�����W

        MinPos.y = static_cast<float>(ychip * mapChipSize); 	    //�}�b�v�`�b�v�̍���Y���W
        MaxPos.y = static_cast<float>(MinPos.y + mapChipSize);      //�}�b�v�`�b�v�̉E�����W
    }
};

#define SINGLETON (TRUE)

class MapData {
#if SINGLETON//Emiya 2-1:�V���O���g���̃C���^�[�t�F�[�X��ǉ����܂��BSINGLETON��define��FALSE->TRUE�ɂ���ƗL��������܂��B
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
    static MapData* Instance(void) noexcept {
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
    MapData() {}
    /// <summary>
    /// �f�X�g���N�^(�V���O���g���ł͌Ă΂�܂���)
    /// </summary>
    /*~MapData(){}*/
private:
#endif

    const int mapImgXNum = 8;    // �}�b�v�`�b�v�摜�̉������`�b�v��

    const int mapImgYNum = 11;   // �}�b�v�`�b�v�摜�̏c�����`�b�v��

    const int riverImgXNum = 1;	 //�C�̃}�b�v�`�b�v�摜�̉������`�b�v��

    const int riverImgYNum = 5;	 //�C�̃}�b�v�`�b�v�摜�̏c�����`�b�v��

    std::vector<std::vector<Location>>  Maploca;

    int mapChipImg[88] = {0}; // �摜�n���h���z��

    int riverChipImg[5] = {0};//��̉摜�`�b�v
private:

public:
    /// @brief�}�b�v�̓ǂݍ���  
    void Init();

    void Update();
    /// @brief�}�b�v�̕`��
    void Draw();
    /// @brief �}�b�v��CSV�t�@�C������ǂݎ��
    void Engine();

    bool LoadData(std::string filePath)
    {
        std::string linebuf;                       // 1�s�ǂݍ��݃o�b�t�@

        std::string data;                          // �J���}��؂�Ő؂�o�����f�[�^�i�[�p
        //�t�@�C���I�[�v��
        std::ifstream csvFile(filePath);
        if (csvFile.fail()) { return false; }      //�ǂݍ��݂Ɏ��s�����̂�false��Ԃ��܂�

        // �t�@�C������CSV�ǂݍ���

        int xpos = 0;
        int ypos = 0;

        while (getline(csvFile, linebuf))
        {
            // map�z��̍s��ǉ�
            Maploca.emplace_back();
            //�J���}��؂�œǂ݂₷���悤�� istringstream�^�ɕϊ�
            std::istringstream iStream(linebuf);

            // �J���}��؂�P�ʂ�data�ɕ�����i�[
            xpos = 0;
            while (getline(iStream, data, ','))
            {
                // ������f�[�^�𐔒l�ɕϊ����āAmap[ypos][]�̖����ɒǉ�
                Maploca[ypos].emplace_back();

                Maploca[ypos][xpos].Setup(xpos, ypos, stoi(data));                //�f�[�^�̍\�z
                xpos++;
            }
            ypos++;
        }
        csvFile.close();                     //�ꉞ�N���[�Y���܂�

        return true;                         //�Ō�܂Ŏ��s�ł����̂�true��Ԃ��܂�
    }
public:
    //����̃`�b�v�̃}�b�v�f�[�^���擾���܂�
    int GetMapChip(int x, int y)
    {
        return Maploca[y][x].GetGraphID();
    }

    const Location& GetMapLoca(int x, int y) {
        return Maploca[y][x];
    }

    //����̃`�b�v�̃}�b�v�f�[�^���ǂ��ǂ����𔻕ʂ��܂�
    bool IsWallMapChip(int x, int y) {
        //�w�肵�����W���擾�ł��Ȃ��ӏ��ł���ꍇ�A�ǂƂ��Ĕ��ʂ��܂�
        if ((x < 0) || (GetMapXsize() <= x) || (y < 0) || (GetMapYsize() < y)) {
            return true;
        }
        return Maploca[y][x].IsWall();
    }

    //�}�b�v��X�T�C�Y��Ԃ��܂�

    int GetMapXsize()
    {
        //����map[0]~[max]�܂őS�������T�C�Y�Ɖ��肵�܂�
        return static_cast<int>(Maploca[0].size());
    }

    //�}�b�v��Y�T�C�Y��Ԃ��܂�
    int GetMapYsize()
    {
        return static_cast<int>(Maploca.size());
    }

    //�����ƃ}�b�v�`�b�v�̂����ǔ��肪������̂Ƃ̓����蔻��{����������̈ړ�����
    bool CalcVectorSlideOnWallChips(const Vector2DX& PlayerPrev, Vector2DX* pPlayerNow, const Vector2DX& PlayerMinSize, const Vector2DX& PlayerMaxSize);

};