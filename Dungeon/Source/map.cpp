//==========================================-
//	�C���N���[�h
//==========================================-
#include "aplication.h"
#include "consoleBuffer.h"
#include "map.h"


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
MAPDATA mapData;

//=============================
//	�}�b�v�̏���������
//�����F��
//�ߒl�F��
//=============================
void MapInit(void)
{
    MapFileRead("Data/Map/map.txt", &mapData);
}

//=============================
//	�}�b�v�̏I������
//�����F��
//�ߒl�F��
//=============================
void MapUnInit(void)
{
    if (mapData.mapData != NULL)
    {
        free(mapData.mapData);
        mapData.mapData = NULL;
    }
}

//=============================
//	�}�b�v�f�[�^�擾
//�����F��
//�ߒl�FMAPDATA�@�}�b�v�f�[�^
//=============================
MAPDATA* GeMapInfo(void)
{
    return(&mapData);
}

//=============================
//	�O�}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapFront(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (0 <= y - 3 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 3, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (0 <= y - 3 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 3, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (0 <= y - 3) {
            if (checkMapData(&mapData, x, y - 3, '1')) {
                ret = true;
            }
        }
        break;

    case 3:
        if (0 <= y - 2 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 2, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (0 <= y - 2 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 2, '1')) {
                ret = true;
            }
        }

        break;
    case 5:
        if (0 <= y - 2) {
            if (checkMapData(&mapData, x, y - 2, '1')) {
                ret = true;
            }
        }
        break;

    case 6:
        if (0 <= y - 1 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (0 <= y - 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (0 <= y - 1) {
            if (checkMapData(&mapData, x, y - 1, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	�E�}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapRight(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (0 <= y - 1 && mapData.mapWidth > x + 3) {
            if (checkMapData(&mapData, x + 3, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 3) {
            if (checkMapData(&mapData, x + 3, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (mapData.mapWidth > x + 3) {
            if (checkMapData(&mapData, x + 3, y, '1')) {
                ret = true;
            }
        }
        break;
    case 3:
        if (0 <= y - 1 && mapData.mapWidth > x + 2) {
            if (checkMapData(&mapData, x + 2, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 2) {}
        if (checkMapData(&mapData, x + 2, y + 1, '1')) {
            ret = true;
        }
        break;
    case 5:
        if (mapData.mapWidth > x + 2) {
            if (checkMapData(&mapData, x + 2, y, '1')) {
                ret = true;
            }
        }
        break;
    case 6:
        if (0 <= y - 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	���}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapLeft(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x - 3) {
            if (checkMapData(&mapData, x - 3, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (0 <= y - 1 && 0 <= x - 3) {
            if (checkMapData(&mapData, x - 3, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (0 <= x - 3) {
            if (checkMapData(&mapData, x - 3, y, '1')) {
                ret = true;
            }
        }
        break;
    case 3:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x - 2) {
            if (checkMapData(&mapData, x - 2, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (0 <= y - 1 && 0 <= x - 2) {}
        if (checkMapData(&mapData, x - 2, y - 1, '1')) {
            ret = true;
        }
        break;
    case 5:
        if (0 <= x - 2) {
            if (checkMapData(&mapData, x - 2, y, '1')) {
                ret = true;
            }
        }
        break;
    case 6:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x - 1) {
            if (checkMapData(&mapData, x - 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (0 <= y - 1 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	��}�b�v�f�[�^����
//�����Fno, �ǔz�u�p�^�[��,x �z�u���WX,y �z�u���WY
//�ߒl�FBOOL�@����
//=============================
BOOL checkMapBack(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (mapData.mapHight > y + 3 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 3, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (mapData.mapHight > y + 3 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y + 3, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (mapData.mapHight > y + 3) {
            if (checkMapData(&mapData, x, y + 3, '1')) {
                ret = true;
            }
        }
        break;

    case 3:
        if (mapData.mapHight > y + 2 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 2, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (mapData.mapHight > y + 2 && 0 <= x - 1) {}
        if (checkMapData(&mapData, x - 1, y + 2, '1')) {
            ret = true;
        }
        break;
    case 5:
        if (mapData.mapHight > y + 2) {
            if (checkMapData(&mapData, x, y + 2, '1')) {
                ret = true;
            }
        }
        break;

    case 6:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (mapData.mapHight > y + 1 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (mapData.mapHight > y + 1) {
            if (checkMapData(&mapData, x, y + 1, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	�}�b�v�X�V����
//�����Fplayer�v���C���[���
//�ߒl�F��
//=============================
void updateMap(PLAYER* player)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();

    screenBufferClear(pConsolMainBuffer, 0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (checkMapDirection((3 * i) + j, player)) {
                CIMAGEDATA* pWork = (pConsolImageDataList + (3 * i) + j);
                updateBufferObject(pConsolMainBuffer, pWork, 0, 0);
            }
        }
    }

    // �X�N���[���`��
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
}

//=============================
//	�������菈��
//�����Fno �}�b�v�f�[�^,player�v���C���[���
//�ߒl�FBOOL ����
//=============================
BOOL checkMapDirection(int no, PLAYER* player)
{
    BOOL ret = false;

    switch (player->direction)
    {
    case DIRECTION_FRONT:
        ret = checkMapFront(no, player->posX, player->posY);
        break;
    case DIRECTION_RIGHT:
        ret = checkMapRight(no, player->posX, player->posY);
        break;
    case DIRECTION_BACK:
        ret = checkMapBack(no, player->posX, player->posY);
        break;
    case DIRECTION_LEFT:
        ret = checkMapLeft(no, player->posX, player->posY);
        break;
    }

    return ret;
}

//=============================
//	�ǔ��菈��
//�����Fmd �}�b�v�f�[�^,x �ʒu���WX,y �ʒu���WY
//�ߒl�FBOOL ����
//=============================
BOOL checkMapWallInto(MAPDATA* md, int x, int y)
{
    return checkMapData(md, x, y, '1');
}
int MapFileRead(const char* fileName, MAPDATA* md)
{
    FILE* fp;    // �t�@�C���|�C���^
    int     fsize;  // �t�@�C���T�C�Y

    char* buf;   // �f�[�^�̈�

    int pcount = 0;


    // �t�@�C���I�[�v��
    fp = fopen(fileName, "rt");
    if (fp == NULL)
    {
        return  NG;
    }


    // �t�@�C���T�C�Y�擾
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    // �f�[�^�̈�m��
    buf = (char*)malloc(fsize);


    // �S�f�[�^�ǂݍ���
    fsize = fread(buf, sizeof(char), fsize, fp);

    // �t�@�C���N���[�Y
    fclose(fp);


    // ���s�R�[�h�� , �𕶎���̋�؂�ɕϊ�
    for (int i = 0; i < fsize; i++)
    {
        if (*(buf + i) == 0x0A || *(buf + i) == ',')
        {
            *(buf + i) = 0x00;
        }
    }

    int         mapWidth, mapHight;         // �}�b�v�T�C�Y
    int         initPosX, initPosY;         // �������W
    int         goalPosX, goalPosY;         // �}�b�v�S�[�����W
    char* map;
    char* mapWork;

    // �}�b�v�T�C�Y�ǂݍ���
    sscanf(buf + pcount, "%d", &mapWidth);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &mapHight);
    buffMove(buf, pcount);

    pcount++;       // ���s�X�L�b�v

    // �}�b�v�������W�ǂݍ���
    sscanf(buf + pcount, "%d", &initPosX);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &initPosY);
    buffMove(buf, pcount);

    pcount++;       // ���s�X�L�b�v

    // �}�b�v�S�[�����W�ǂݍ���
    sscanf(buf + pcount, "%d", &goalPosX);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &goalPosY);
    buffMove(buf, pcount);

    pcount++;       // ���s�X�L�b�v

        // �̈�m��
    map = (char*)malloc(mapWidth * mapHight * sizeof(char));
    if (map == NULL)
    {
        printf("�̈�m�ێ��s");
        return NG;
    }

    // �f�[�^�ϊ�
    for (int loopY = 0; loopY < mapHight; loopY++)
    {
        for (int loopX = 0; loopX < mapWidth; loopX++)
        {

            // �A�h���X�v�Z
            mapWork = map + (loopY * mapHight) + loopX;

            sscanf(buf + pcount, "%c", mapWork);
            buffMove(buf, pcount);

        }

        pcount++;  // ���s�R�[�h�X�L�b�v
    }

    // �o�b�t�@�Z�b�g
    md->mapData = map;

    md->mapWidth = mapWidth;
    md->mapHight = mapHight;
    md->initPosX = initPosX;
    md->initPosY = initPosY;
    md->goalPosX = goalPosX;
    md->goalPosY = goalPosY;

    if (buf != NULL)
    {
        free(buf);
        buf = NULL;
    }


    return  OK;
}
//=============================
//	�}�b�v���菈��
//�����Fmd �}�b�v�f�[�^,x �ʒu���WX,y �ʒu���WY,target �ǃt���O
//�ߒl�FBOOL ����
//=============================
BOOL checkMapData(MAPDATA* md, int x, int y, char target)
{
    BOOL ret = false;
    char* work = md->mapData;

    if (work[(y * md->mapHight) + x] == target) {
        ret = true;
    }


    return ret;
}
//=============================
//	�v���C���[�ړ��ɂ��`�揈��
//�����Fmd �}�b�v�f�[�^,player�v���C���[���,sign ����
//�ߒl�FDIRECTION ����
//=============================
PLAYER* moveDirection(MAPDATA* md, PLAYER* player, int sign)
{
    int x = player->posX;
    int y = player->posY;
    DIRECTION direction = player->direction;


    switch (direction)
    {
    case DIRECTION_FRONT:
        y = y - (1 * sign);
        break;
    case DIRECTION_RIGHT:
        x = x + (1 * sign);
        break;
    case DIRECTION_BACK:
        direction = DIRECTION_BACK;
        y = y + (1 * sign);
        break;
    case DIRECTION_LEFT:
        x = x - (1 * sign);
        break;
    }

    // �ړ�����
    if (x >= md->mapWidth - 1) {
        x = md->mapWidth - 1;
    }
    if (x < 0) {
        x = 0;
    }
    if (y >= md->mapHight - 1) {
        y = md->mapHight - 1;
    }
    if (y < 0) {
        y = 0;
    }

    // �ǔ���
    if (!checkMapWallInto(md, x, y))
    {
        player->posX = x;
        player->posY = y;
    }

    return player;
}
