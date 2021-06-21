//==========================================-
//	�C���N���[�h
//==========================================-
#include "aplication.h"
#include "consoleBuffer.h"
#include "map.h"
#include "title.h"
#include "game.h"
#include "result.h"

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
static int iAppMode = APLICATIONMODE_NONE;
IMAGEBUFFER         imageBuffer;        // �L�����摜�̃e�L�X�g�f�[�^
CONSOLIMAGEDATA     header;    // �R���\�[���ɏo�͂���L�����摜�f�[�^
HANDLE hCOut;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
CONSOLE_SCREEN_BUFFER_INFOEX csbiBackUp;
WORD wAttr;


//==========================================-
//	�֐��̒�`
//==========================================-
//=============================
//	���[�h�擾�֐�
//�����F��
//�ߒl�Fint	���݂̃��[�h
//=============================
int GetAppMode(void)
{
	return(iAppMode);
}

//=============================
//	���[�h�ݒ�֐�
//�����Fint	iMode ����̃��[�h
//�ߒl�F��
//=============================
void SetAppMode(int iMode)
{
	//�V�[�����̏I������
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//�^�C�g��
		TitleUnInitScene();
		break;
	case APLICATIONMODE_GAME:		//�Q�[���{��
		GameUnInitScene();
		break;
	case APLICATIONMODE_RESULT:	//���U���g
		ResultUnInitScene();
		break;
	}
	//���[�h�̕ύX
	iAppMode = iMode;
	//�V�[�����̏���������
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//�^�C�g��
		TitleInitScene();
		break;
	case APLICATIONMODE_GAME:		//�Q�[���{��
		GameInitScene();
		break;
	case APLICATIONMODE_RESULT:	//���U���g
		ResultInitScene();
		break;
	}
}

//=============================
//	�A�v���P�[�V�����S�̂̏�����
//�����F��
//�ߒl�F��
//=============================
int AppInit(void)
{
    // �n���h�����擾(�o�b�N�A�b�v)
    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hCOut == INVALID_HANDLE_VALUE)
    {
        printf_s("ErrorCode = %d\n", GetLastError());
        return -1;
    }
    // �f�t�H���g�̃X�N���[���o�b�t�@����ۑ�
    if (!GetConsoleScreenBufferInfo(hCOut, &consoleInfo))
    {
        printf("ErrorCode = %d\n", GetLastError());
        return -1;
    }

    // �f�t�H���g�̐F����ۑ�
    
    if (!SetConsoleTextAttribute(hCOut, wAttr))
    {
        printf_s("ErrorCode = %d\n", GetLastError());
        return -1;
    }

    // �R���\�[�������p�ϐ�
    csbiBackUp.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    // �R���\�[�����擾
    GetConsoleScreenBufferInfoEx(hCOut, &csbiBackUp);

    // �t�H���g�T�C�Y�ύX
    CONSOLE_FONT_INFOEX font_size = { sizeof font_size };
    font_size.dwFontSize.X = 1;
    font_size.dwFontSize.Y = 1;
    SetCurrentConsoleFontEx(hCOut, 0, &font_size);

    // �E�B���h�E�T�C�Y�g��
    COORD       dwSize = { SCREEN_WIDTH,SCREEN_HEIGHT };
    SMALL_RECT consoleWindow = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
    setWindowSizeBig(hCOut, dwSize, consoleWindow);

    ConsoleBufferInit();

    MapInit();

    // �摜�f�[�^�ǂݍ���
    char        readFileName[100];//�L����
    sprintf(readFileName, "%s", DEF_HEADDER_FILE);

    imageHeadderDataLoad(readFileName);

	SetAppMode(APLICATIONMODE_TITLE);
	TitleInit();
	GameInit();
	ResultInit();

    return 0;
}

//=============================
//	�A�v���P�[�V�����S�̂̏�����
//�����F��
//�ߒl�F��
//=============================
void AppUnInit(void)
{
	TitleUnInit();
	GameUnInit();
	ResultUnInit();
    MapUnInit();

    // �̈���
    if (header.pImg != NULL)
    {
        free(header.pImg);
        header.pImg = NULL;
    }

    wAttr = consoleInfo.wAttributes;

    // �f�t�H���g�p���b�g�ݒ�
    SetConsoleScreenBufferInfoEx(hCOut, &csbiBackUp); //�����p���b�g�ɖ߂�
    SetConsoleTextAttribute(hCOut, wAttr);// �����\�������̐F�����ɖ߂��B
    SetConsoleScreenBufferSize(hCOut, consoleInfo.dwSize);// �����E�B���h�E�T�C�Y�����ɖ߂��B

}

//=============================
//	�A�v���P�[�V�����S�̂̏�����
//�����F��
//�ߒl�F��
//=============================
void AppUpdate(void)
{
	//���[�h���̍X�V����
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//�^�C�g��
		TitleUpdate();
		break;
	case APLICATIONMODE_GAME:		//�Q�[���{��
		GameUpdate();
		break;
	case APLICATIONMODE_RESULT:	//���U���g
		ResultUpdate();
		break;
	}


    // �L�[���[�h�o�b�t�@�N���A
    INPCLEAR();

}

//=============================
//	�A�v���P�[�V�����S�̂̏�����
//�����F��
//�ߒl�F��
//=============================
void AppDraw(void)
{
	//���[�h���̕`�揈��
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//�^�C�g��
		TitleDraw();
		break;
	case APLICATIONMODE_GAME:		//�Q�[���{��
		GameDraw();
		break;
	case APLICATIONMODE_RESULT:	//���U���g
		ResultDraw();
		break;
	}
}

// �E�B���h�E�T�C�Y�g��
BOOL setWindowSizeBig(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow)
{
    BOOL result = false;

    // �E�B���h�E�o�b�t�@�T�C�Y�ύX
    SetConsoleScreenBufferSize(hCOut, dwSize);

    // �E�B���h�E�T�C�Y�ύX
    result = SetConsoleWindowInfo(hCOut, TRUE, &consoleWindow);

    return result;
}
// �E�B���h�E�T�C�Y�k��
BOOL setWindowSizeSmall(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow)
{
    BOOL result = false;

    // �E�B���h�E�T�C�Y�ύX
    result = SetConsoleWindowInfo(hCOut, TRUE, &consoleWindow);

    // �E�B���h�E�o�b�t�@�T�C�Y�ύX
    SetConsoleScreenBufferSize(hCOut, dwSize);

    return result;
}


// �p���b�g������������
//------------------------------------------------
// �R���\�[����ʂ̃p���b�g������������
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     COLORREF *c �������ރp���b�g�f�[�^�̃A�h���X
//------------------------------------------------
int SetConsolePalatte(COLORREF* cr)
{

    BOOL        ret;
    HANDLE      hCOut;

    // �E�B���h�E�n���h���擾
    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // �R���\�[�������p�ϐ�
    CONSOLE_SCREEN_BUFFER_INFOEX csbi;
    csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    // �R���\�[�����擾
    ret = GetConsoleScreenBufferInfoEx(hCOut, &csbi);

    // �I���W�i���p���b�g��������
    memcpy(&csbi.ColorTable[0], cr, sizeof(COLORREF) * 16);

    // �I���W�i���p���b�g�ݒ�
    ret = SetConsoleScreenBufferInfoEx(hCOut, &csbi);

    return ret;
}
//=============================
//	�摜�e�L�X�g�w�b�_�[�t�@�C���ǂݍ��ݏ���
//�����FreadFileName �t�@�C����
//�ߒl�Fbool ����
//=============================
BOOL imageHeadderDataLoad(char* readFileName)
{
    BOOL        ret;

    // �t�@�C���f�[�^�ǂݍ���
    ret = ImageFileRead(readFileName, &imageBuffer);
    if (ret == NG)
    {
        // �G���[����
        printf("�t�@�C���ǂݍ��݃G���[\n");
        getchar();
        exit(0);
    }

    // �p���b�g�f�[�^�ϊ�
    ret = ChangePalette(&imageBuffer, &header);
    if (ret == NG)
    {
        // �G���[����
        printf("�p���b�g�ϊ��G���[\n");
        getchar();
        exit(0);
    }

    // �p���b�g�f�[�^�Z�b�g
    ret = SetConsolePalatte(&header.palette[0]);
    if (ret == false)
    {
        // �G���[����
        printf("�p���b�g�ݒ�G���[\n");
        getchar();
        exit(0);
    }

    if (imageBuffer.buf != NULL)
    {
        free(imageBuffer.buf);
        imageBuffer.buf = NULL;
        imageBuffer.datSize = 0;
        imageBuffer.point = 0;
    }

    return true;
}
//=============================
//	�摜�e�L�X�g�t�@�C���ǂݍ��ݏ���
//�����FreadFileName �t�@�C����,consolImageData �摜�e�L�X�g
//�ߒl�Fbool ����
//=============================
BOOL imageDataLoad(const char* readFileName, CIMAGEDATA* consolImageData)
{
    BOOL        ret;

    // �t�@�C���f�[�^�ǂݍ���
    ret = ImageFileRead(readFileName, &imageBuffer);
    if (ret == NG)
    {
        // �G���[����
        printf("�t�@�C���ǂݍ��݃G���[\n");
        getchar();
        exit(0);
    }

    // �摜�T�C�Y�ݒ�
    ret = ChangeImageSize(&imageBuffer, consolImageData);
    if (ret == NG)
    {
        // �G���[����
        printf("�摜�T�C�Y�ݒ�G���[\n");
        getchar();
        exit(0);
    }

    // �f�[�^�ϊ�
    ret = ChangeImage(&imageBuffer, consolImageData);
    if (ret == NG)
    {
        printf("�摜�f�[�^�ϊ������G���[\n");
        getchar();
        exit(0);
    }

    if (imageBuffer.buf != NULL)
    {
        free(imageBuffer.buf);
        imageBuffer.buf = NULL;
        imageBuffer.datSize = 0;
        imageBuffer.point = 0;
    }

    return true;
}
//=============================
//	�t�@�C���f�[�^�ǂݍ���
//�����FfileName �t�@�C����,ib �摜�o�b�t�@
//�ߒl�Fint�@����
//=============================
int ImageFileRead(const char* fileName, IMAGEBUFFER* ib)
{
    FILE* fp;    // �t�@�C���|�C���^
    int     fsize;  // �t�@�C���T�C�Y

    char* buf;   // �f�[�^�̈�


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
        if (*(buf + i) == 0x0A
            || *(buf + i) == ',')
        {
            *(buf + i) = 0x00;
        }
    }


    // �f�[�^�Z�b�g
    ib->datSize = fsize;    // �o�b�t�@�T�C�Y�ݒ�
    ib->buf = buf;      // �f�[�^�̈�ݒ�


    return  OK;
}

//=============================
//	�p���b�g�f�[�^�ϊ�
//�����Fib �摜�o�b�t�@,cid �摜�w�b�_�[
//�ߒl�Fint�@����
//=============================
int ChangePalette(IMAGEBUFFER* ib, CONSOLIMAGEDATA* cid)
{
    // �p���b�g�f�[�^
    COLORREF    palette[16];

    // �p���b�g��
    int         paletteSize;

    // �F�f�[�^
    unsigned int r, g, b, a;

    // �f�[�^�o�b�t�@�i��Ɨp�j
    char* buf;
    int     pcount;     // �o�b�t�@�A�h���X�ύX�p


    // �f�[�^�Z�b�g
    buf = ib->buf;
    pcount = ib->point;


    // �p���b�g���擾
    sscanf(buf + pcount, "%d", &paletteSize);
    buffMove(buf, pcount);
    pcount++;                       // ���s�X�L�b�v


    //�p���b�g�擾
    for (int i = 0; i < paletteSize; i++)
    {
        // �ԃf�[�^�ǂݍ���
        sscanf(buf + pcount, "%d", &r);
        buffMove(buf, pcount);

        sscanf(buf + pcount, "%d", &g);
        buffMove(buf, pcount);

        sscanf(buf + pcount, "%d", &b);
        buffMove(buf, pcount);

        sscanf(buf + pcount, "%d", &a);
        buffMove(buf, pcount);

        // RGBA ����
        palette[i] = (a << 24) | (r << 16) | (g << 8) | b;

        pcount++;       // ���s�X�L�b�v
    }


    // �p���b�g�f�[�^�Z�b�g
    memcpy(cid->palette, palette, sizeof(COLORREF) * 16);


    // �p���b�g���Z�b�g
    cid->paletteSize = paletteSize;     // �p���b�g���ݒ�
    ib->point = pcount;          // �e�L�X�g�f�[�^�̎g�p�ʒu�X�V

    return  OK;
}
//=============================
//	�摜�T�C�Y�ݒ�
//�����Fib �摜�o�b�t�@,consolImageData �摜�e�L�X�g
//�ߒl�Fint�@����
//=============================
int ChangeImageSize(IMAGEBUFFER* ib, CIMAGEDATA* cid)
{
    // �o�b�t�@�f�[�^�Ǘ��ϐ�
    char* buf;
    int   pcount;

    // �摜�T�C�Y
    int xMax, yMax;

    // �o�b�t�@�f�[�^�Z�b�g
    buf = ib->buf;          // �o�b�t�@�A�h���X�Z�b�g
    pcount = ib->point;        // �o�b�t�@�g�p�ʒu�Z�b�g


    // �摜�T�C�Y�擾
    sscanf(buf + pcount, "%d", &xMax);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &yMax);
    buffMove(buf, pcount);

    pcount++;   // ���s�X�L�b�v

    // �摜�T�C�Y�ݒ�
    cid->imgWidth = xMax;
    cid->imgHight = yMax;

    ib->point = pcount;        // �o�b�t�@�g�p�ʒu�Z�b�g

    return OK;
}

//=============================
//	�摜�f�[�^�ݒ�
//�����Fib �摜�o�b�t�@,consolImageData �摜�e�L�X�g
//�ߒl�Fint�@����
//=============================
int ChangeImage(IMAGEBUFFER* ib, CIMAGEDATA* cid)
{
    // �摜�f�[�^�̈�
    CHAR_INFO* pImg;
    CHAR_INFO* pWork;

    // �o�b�t�@�f�[�^�Ǘ��ϐ�
    char* buf;
    int     pcount;

    int     colorNo;        // �J���[�ԍ�
    int     loopX, loopY;


    // �o�b�t�@�f�[�^�Z�b�g
    buf = ib->buf;          // �o�b�t�@�A�h���X�Z�b�g
    pcount = ib->point;        // �o�b�t�@�g�p�ʒu�Z�b�g


    // �̈�m��
    pImg = (CHAR_INFO*)malloc(cid->imgWidth * cid->imgHight * sizeof(CHAR_INFO));
    if (pImg == NULL)
    {
        printf("�̈�m�ێ��s");
        return NG;
    }

    // �f�[�^�ϊ�
    for (loopY = 0; loopY < cid->imgHight; loopY++)
    {
        for (loopX = 0; loopX < cid->imgWidth; loopX++)
        {
            // �F�f�[�^���o��
            sscanf(buf + pcount, "%d", &colorNo);
            buffMove(buf, pcount);

            // �A�h���X�v�Z
            pWork = pImg + (loopY * cid->imgWidth) + loopX;

            // �摜�f�[�^�i �����ƐF �j
            (pWork)->Char.AsciiChar = ' ';                      // �����f�[�^�Z�b�g
            (pWork)->Attributes = (colorNo << 4) | colorNo;   // �F  �f�[�^�Z�b�g
        }

        pcount++;  // ���s�R�[�h�X�L�b�v
    }

    // �o�b�t�@�Z�b�g
    cid->pImg = pImg;

    ib->point = pcount;        // �o�b�t�@�g�p�ʒu�Z�b�g

    return OK;
}

//=============================
//	������]�ɂ��`�揈��
//�����Fdirection ����,value ������
//�ߒl�FDIRECTION ����
//=============================
DIRECTION changeDirection(DIRECTION direction, int value)
{
    DIRECTION ret = DIRECTION_MAX;
    int directionValue = direction + value;

    if (directionValue < 0) {
        directionValue = DIRECTION_MAX - 1;
    }
    if (directionValue >= DIRECTION_MAX) {
        directionValue = 0;
    }

    switch (directionValue)
    {
    case DIRECTION_FRONT:
        direction = DIRECTION_FRONT;
        break;
    case DIRECTION_RIGHT:
        direction = DIRECTION_RIGHT;
        break;
    case DIRECTION_BACK:
        direction = DIRECTION_BACK;
        break;
    case DIRECTION_LEFT:
        direction = DIRECTION_LEFT;
        break;
    }

    return direction;
}
//=============================
//	�e�L�X�g�t�H���g�̕`�揈��
//�����Ftarget ������
//�ߒl�FVECTOR2 �t�H���g���W
//=============================
VECTOR2 GetFont(const char* target)
{
    VECTOR2 ret;

    ret.x = -1;
    ret.y = -1;

    char value[3];
    value[0] = target[0];
    value[1] = target[1];
    value[2] = '\0';

    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "�J") == 0)
    {
        ret.x = 48;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "�K") == 0)
    {
        ret.x = 64;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 48;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 64;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 0;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 16;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "��") == 0)
    {
        ret.x = 32;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "�@") == 0)
    {
        ret.x = 48;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "c4") == 0)
    {
        ret.x = 64;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "c5") == 0)
    {
        ret.x = 0;
        ret.y = 160;
        return ret;
    }
    if (strcmp(value, "c6") == 0)
    {
        ret.x = 16;
        ret.y = 160;
        return ret;
    }
    if (strcmp(value, "c7") == 0)
    {
        ret.x = 32;
        ret.y = 160;
        return ret;
    }
    if (strcmp(value, "c8") == 0)
    {
        ret.x = 48;
        ret.y = 160;
        return ret;
    }
    if (strcmp(value, "�I") == 0)
    {
        ret.x = 64;
        ret.y = 160;
        return ret;
    }
    if (strcmp(value, "�u") == 0)
    {
        ret.x = 0;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "�v") == 0)
    {
        ret.x = 16;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "�F") == 0)
    {
        ret.x = 32;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "�[") == 0)
    {
        ret.x = 48;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "cd") == 0)
    {
        ret.x = 64;
        ret.y = 176;
        return ret;
    }

    return ret;
}

