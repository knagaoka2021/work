#ifndef __MAP_H__
#define __MAP_H__

//==========================================-
//	�C���N���[�h
//==========================================-
#include "player.h"

typedef struct _MAPDATA
{
    int         mapWidth, mapHight;         // �}�b�v�T�C�Y
    int         initPosX, initPosY;         // �������W
    int         goalPosX, goalPosY;         // �}�b�v�S�[�����W
    char* mapData;

} MAPDATA;
//==========================================-
//	�v���g�^�C�v�錾
//==========================================-

//=============================
//	�}�b�v�̏���������
//�����F��
//�ߒl�F��
//=============================
void MapInit(void);

//=============================
//	�}�b�v�̏I������
//�����F��
//�ߒl�F��
//=============================
void MapUnInit(void);

//=============================
//	�}�b�v�f�[�^�擾
//�����F��
//�ߒl�FMAPDATA�@�}�b�v�f�[�^
//=============================
MAPDATA* GeMapInfo(void);

//=============================
//	�O�}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapFront(int no, int x, int y);

//=============================
//	�E�}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapRight(int no, int x, int y);
//=============================
//	���}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapLeft(int no, int x, int y);

//=============================
//	��}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapBack(int no, int x, int y);

//=============================
//	�}�b�v�X�V����
//�����Fplayer�v���C���[���
//�ߒl�F��
//=============================
void updateMap(PLAYER* player);

//=============================
//	�������菈��
//�����Fno �}�b�v�f�[�^,player�v���C���[���
//�ߒl�FBOOL ����
//=============================
BOOL checkMapDirection(int no, PLAYER* player);
//=============================
//	�v���C���[�ړ��ɂ��`�揈��
//�����Fmd �}�b�v�f�[�^,player�v���C���[���,sign ����
//�ߒl�FDIRECTION ����
//=============================
PLAYER* moveDirection(MAPDATA* md, PLAYER* player, int sign);

//=============================
//	�ǔ��菈��
//�����Fmd �}�b�v�f�[�^,x �ʒu���WX,y �ʒu���WY
//�ߒl�FBOOL ����
//=============================
BOOL checkMapWallInto(MAPDATA* md, int x, int y);

int MapFileRead(const char* fileName, MAPDATA* md);
//=============================
//	�}�b�v���菈��
//�����Fmd �}�b�v�f�[�^,x �ʒu���WX,y �ʒu���WY,target �ǃt���O
//�ߒl�FBOOL ����
//=============================
BOOL checkMapData(MAPDATA* md, int x, int y, char target);

#endif//__MAP_H__