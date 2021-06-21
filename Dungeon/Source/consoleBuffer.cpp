//==========================================-
//	�C���N���[�h
//==========================================-
#include "aplication.h"
#include "consoleBuffer.h"

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
CONSOLIMAGEDATA     consolMainBuffer;    // ���C���R���\�[���o�b�t�@
CONSOLIMAGEDATA     consolSubBuffer;    // �T�u�R���\�[���o�b�t�@
CIMAGEDATA     consolImageDataList[DEF_FILE_MAX];    // �R���\�[���ɏo�͂���摜�f�[�^���X�g
CIMAGEDATA     consolfontImageData;    // �R���\�[���ɏo�͂���e�L�X�g�t�H���g�f�[�^

//=============================
//	�R���\�[��������
//�����F��
//�ߒl�F��
//=============================
void ConsoleBufferInit(void)
{
    // ���C���X�N���[��������
    screenInit(&consolMainBuffer, 200, 200, 2);
    screenInit(&consolSubBuffer, 320, SCREEN_WIDTH, 8);

    imageDataLoad("Data/Wall/BMP_DAT_L3.txt", &consolImageDataList[0]);
    imageDataLoad("Data/Wall/BMP_DAT_R3.txt", &consolImageDataList[1]);
    imageDataLoad("Data/Wall/BMP_DAT_M2.txt", &consolImageDataList[2]);
    imageDataLoad("Data/Wall/BMP_DAT_L2.txt", &consolImageDataList[3]);
    imageDataLoad("Data/Wall/BMP_DAT_R2.txt", &consolImageDataList[4]);
    imageDataLoad("Data/Wall/BMP_DAT_M1.txt", &consolImageDataList[5]);
    imageDataLoad("Data/Wall/BMP_DAT_L1.txt", &consolImageDataList[6]);
    imageDataLoad("Data/Wall/BMP_DAT_R1.txt", &consolImageDataList[7]);
    imageDataLoad("Data/Wall/BMP_DAT_M0.txt", &consolImageDataList[8]);
    imageDataLoad("Data/Wall/BMP_DAT_M3.txt", &consolImageDataList[9]);

    imageDataLoad("Data/Font/BMP_DAT_FONT.txt", &consolfontImageData);

    //������
    screenBufferClear(&consolMainBuffer, 0);
    screenBufferClear(&consolSubBuffer, 0);

}

//=============================
//	�R���\�[���C��
//�����F��
//�ߒl�F��
//=============================
void ConsoleBufferUnInit(void)
{
    if (consolMainBuffer.pImg != NULL)
    {
        free(consolMainBuffer.pImg);
        consolMainBuffer.pImg = NULL;
    }

    if (consolSubBuffer.pImg != NULL)
    {
        free(consolSubBuffer.pImg);
        consolSubBuffer.pImg = NULL;
    }

    if (consolfontImageData.pImg != NULL)
    {
        free(consolfontImageData.pImg);
        consolfontImageData.pImg = NULL;
    }

    for (int i = 0; i < DEF_FILE_MAX; i++) {
        if (consolImageDataList[i].pImg != NULL)
        {
            free(consolImageDataList[i].pImg);
            consolImageDataList[i].pImg = NULL;
        }
    }
}

//=============================
//	�X�N���[���o�b�t�@������
//�����Fcid �R���\�[���o�b�t�@,screenW �X�N���[����,screenH �X�N���[������,colorNo �J���[�ԍ�
//�ߒl�Fint ����
//=============================
int screenInit(CONSOLIMAGEDATA* cid, int screenW, int screenH, int colorNo)
{
    CHAR_INFO* pImg;
    CHAR_INFO* pWork;

    // �X�N���[��������
    cid->imgWidth = screenW;
    cid->imgHight = screenH;

    // �X�N���[���T�C�Y�ݒ�
    cid->screenSizeX = screenW;
    cid->screenSizeY = screenH;

    // �X�N���[���o�b�t�@�T�C�Y�ݒ�
    cid->bufferSizeX = screenW + 1;
    cid->bufferSizeY = screenH + 1;

    // �̈�m��
    pImg = (CHAR_INFO*)malloc(cid->screenSizeX * cid->screenSizeY * sizeof(CHAR_INFO));
    if (pImg == NULL)
    {
        printf("�̈�m�ێ��s");
        return NG;
    }

    // �f�[�^�ϊ�
    for (int y = 0; y < cid->screenSizeY; y++)
    {
        for (int x = 0; x < cid->screenSizeX; x++)
        {
            // �A�h���X�v�Z
            pWork = pImg + (y * cid->screenSizeX) + x;

            // �摜�f�[�^�i �����ƐF �j
            pWork->Char.AsciiChar = ' ';        // �����f�[�^�Z�b�g
            pWork->Attributes = (colorNo << 4) | colorNo;   // �F  �f�[�^�Z�b�g
        }

    }

    // �o�b�t�@�Z�b�g
    cid->pImg = pImg;

    return OK;
}

CONSOLIMAGEDATA* GetConsolMainBufferInfo(void)
{
    return(&consolMainBuffer);
}
//=============================
//	�T�u�R���\�[���o�b�t�@�擾
//�����F��
//�ߒl�FCONSOLIMAGEDATA �R���\�[���o�b�t�@
//=============================
CONSOLIMAGEDATA* GetConsolSubBufferInfo(void)
{
    return(&consolSubBuffer);
}

//=============================
//	�摜�f�[�^���X�g�擾
//�����F��
//�ߒl�FCIMAGEDATA �摜�e�L�X�g
//=============================
CIMAGEDATA* GetConsolImageDataList(void)
{
    return(consolImageDataList);
}

//=============================
//	�e�L�X�g�t�H���g�擾
//�����F��
//�ߒl�FCIMAGEDATA �e�L�X�g�t�H���g
//=============================
CIMAGEDATA* GetConsolFontImageInfo(void)
{
    return(&consolfontImageData);
}
//=============================
//	�X�N���[���o�b�t�@�N���A
//�����Fcid �R���\�[���o�b�t�@,colorNo �J���[�ԍ�
//�ߒl�Fint ����
//=============================
int screenBufferClear(CONSOLIMAGEDATA* cid, int colorNo)
{
    CHAR_INFO* pImg;
    CHAR_INFO* pWork;

    pImg = cid->pImg;

    // �f�[�^�ϊ�
    for (int y = 0; y < cid->screenSizeY; y++)
    {
        for (int x = 0; x < cid->screenSizeX; x++)
        {
            // �A�h���X�v�Z
            pWork = pImg + (y * cid->screenSizeX) + x;

            // �摜�f�[�^�i �����ƐF �j
            pWork->Attributes = (colorNo << 4) | colorNo;   // �F  �f�[�^�Z�b�g

        }

    }

    // �o�b�t�@�Z�b�g
    cid->pImg = pImg;

    return OK;
}

//------------------------------------------------
// �R���\�[����ʕ`��
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     int size    �������ރf�[�^�T�C�Y
//                          ���R���\�[���E�B���h�E�̃o�b�t�@�T�C�Y�Ɠ����T�C�Y�ɂ��Ȃ��Ɛ���ɓ��삵�Ȃ�
//                            �ꕔ�������������ނ悤�ɂ͂ł��Ă��Ȃ�
// ��Q����     int *string �������ރf�[�^
//------------------------------------------------
int SetConsoleStringBuffer(int size, char* string)
{
    BOOL        ret;
    DWORD       count;
    HANDLE      hCOut;

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    ret = WriteConsole(hCOut, string, size, &count, NULL);

    return ret;
}
//------------------------------------------------
// �R���\�[����ʕ`��
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     int         posX    �������ވʒu X
// ��Q����     int         posY    �������݈ʒu Y
// ��R����     int         width   �������ޕ�
// ��S����     int         hight   �������ލ���
// ��T����     CHAR_INFO   *pImage �������ރf�[�^�̃A�h���X
//------------------------------------------------
int SetConsoleStringAttributesBuffer(int posX, int posY, int width, int hight, CHAR_INFO* pImage)
{
    BOOL        ret;
    HANDLE      hCOut;
    COORD       pos = { (SHORT)posX, (SHORT)posY };
    COORD       datSize = { (SHORT)width,(SHORT)hight };
    SMALL_RECT  windowRect = { 0, 0, (SHORT)width - 1, (SHORT)hight - 1 };

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // �R���\�[���o�b�t�@�ɕ����ƐF�̏�������
    ret = WriteConsoleOutput(hCOut, pImage, datSize, pos, &windowRect);

    return ret;
}
//------------------------------------------------
// �R���\�[����ʂɒ��ڏ������ޏ���
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     int         posX    �������ވʒu X
// ��Q����     int         posY    �������݈ʒu Y
// ��R����     int         width   �������ޕ�
// ��S����     int         hight   �������ލ���
// ��T����     CHAR_INFO   *pImage �������ރf�[�^�̃A�h���X
//------------------------------------------------
int SetConsoleStringAttributesBuffer2(int posX, int posY, int width, int hight, CHAR_INFO* pImage)
{
    BOOL        ret;
    HANDLE      hCOut;
    COORD       pos = { 0, 0 };
    COORD       datSize = { (SHORT)width,(SHORT)hight };
    SMALL_RECT  windowRect = { (SHORT)posX, (SHORT)posY, (SHORT)(posX + width) - 1, (SHORT)(posY + hight) - 1 };

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // �R���\�[���o�b�t�@�ɕ����ƐF�̏�������
    ret = WriteConsoleOutput(hCOut, pImage, datSize, pos, &windowRect);

    return ret;
}
//------------------------------------------------
// �R���\�[����ʂɒ��ڏ������ޏ���
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     int         posX    �������ވʒu X
// ��Q����     int         posY    �������݈ʒu Y
// ��R����     int         width   �������ޕ�
// ��S����     int         hight   �������ލ���
// ��T����     int         windowX   �������ޘg�ʒu X
// ��U����     int         windowY   �������ݘg�ʒu Y
// ��V����     CHAR_INFO   *pImage �������ރf�[�^�̃A�h���X
//------------------------------------------------
int SetConsoleStringAttributesBufferCustom(int posX, int posY, int width, int hight, int windowX, int windowY, CHAR_INFO* pImage)
{
    BOOL        ret;
    HANDLE      hCOut;
    COORD       pos = { (SHORT)posX, (SHORT)posY };
    COORD       datSize = { (SHORT)width,(SHORT)hight };
    SMALL_RECT  windowRect = { (SHORT)windowX, (SHORT)windowY, (SHORT)(windowX + width) - 1, (SHORT)(windowY + hight) - 1 };

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // �R���\�[���o�b�t�@�ɕ����ƐF�̏�������
    ret = WriteConsoleOutput(hCOut, pImage, datSize, pos, &windowRect);

    return ret;
}
//=============================
//	�e�L�X�g�t�H���g�o�b�t�@�X�V
//�����Fcid �R���\�[���o�b�t�@,obj �e�L�X�g�t�H���g,x �X�V�ʒuX,y �X�V�ʒuY
//�ߒl�Fint ����
//=============================
void updateFontBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y, const char* message)
{
    // �摜�f�[�^�̈�
    CHAR_INFO* pWork = NULL;
    CHAR_INFO* pWorkObj = NULL;

    int     loopX, loopY, posX = x, posY = y;

    int count = strlen(message);
    int shiftNum = 0;

    for (int i = 0; i < count; i += 2)
    {
        VECTOR2 vec = GetFont(message + i);
        loopX = vec.x;
        loopY = vec.y;

        // �f�[�^�ϊ�
        for (int j = loopY; j < loopY + FONT_SIZE; j++)
        {
            for (int k = loopX; k < loopX + FONT_SIZE; k++)
            {

                // �A�h���X�v�Z
                pWorkObj = obj->pImg + (j * obj->imgWidth) + k;

                // �̈�O����
                if (posX >= SCREEN_WIDTH - 1) {
                    break;
                }

                // �F�f�[�^���o��
                WORD colorAttributes = pWorkObj->Attributes;   // �F  �f�[�^�Z�b�g

                update(cid, posX, posY, colorAttributes);

                posX++;

            }
            posX = x;
            posY++;

            // �̈�O����
            if (posY >= SCREEN_HEIGHT - 1) {
                break;
            }
        }
        shiftNum++;

        x = shiftNum * FONT_SIZE;
        posX = x;
        posY = y;

    }

}

//=============================
//	�摜�f�[�^�o�b�t�@�X�V
//�����Fcid �R���\�[���o�b�t�@,obj �摜�f�[�^,x �X�V�ʒuX,y �X�V�ʒuY
//�ߒl�Fint ����
//=============================
void updateBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y)
{
    // �摜�f�[�^�̈�
    CHAR_INFO* pWork = NULL;
    CHAR_INFO* pWorkObj = NULL;

    int     loopX, loopY, posX = x, posY = y;

    // �f�[�^�ϊ�
    for (loopY = 0; loopY < obj->imgHight; loopY++)
    {
        for (loopX = 0; loopX < obj->imgWidth; loopX++)
        {

            // �A�h���X�v�Z
            pWorkObj = obj->pImg + (loopY * obj->imgWidth) + loopX;

            // �̈�O����
            if (posX >= SCREEN_WIDTH - 1) {
                break;
            }

            // �F�f�[�^���o��
            WORD colorAttributes = pWorkObj->Attributes;   // �F  �f�[�^�Z�b�g

            update(cid, posX, posY, colorAttributes);

            posX++;

        }
        posX = x;
        posY++;

        // �̈�O����
        if (posY >= SCREEN_HEIGHT - 1) {
            break;
        }
    }

}
//=============================
//	�X�N���[���o�b�t�@�X�V
//�����Fcid �R���\�[���o�b�t�@,x �X�V�ʒuX,y �X�V�ʒuY,colorAttributes �J���[�f�[�^
//�ߒl�Fint ����
//=============================
void update(CONSOLIMAGEDATA* cid, int x, int y, WORD colorAttributes)
{
    // �摜�f�[�^�̈�
    CHAR_INFO* pWork = NULL;
    WORD alphaAttributes = (1 << 4) | 1;

    // �A�h���X�v�Z
    pWork = cid->pImg + (y * cid->screenSizeX) + x;

    if (colorAttributes != alphaAttributes)
    {
        // �f�[�^�ϊ�
        // �摜�f�[�^�i �����ƐF �j
        // pWork->Char.AsciiChar = ' ';                      // �����f�[�^�Z�b�g
        pWork->Attributes = colorAttributes;   // �F  �f�[�^�Z�b�g
    }
}
