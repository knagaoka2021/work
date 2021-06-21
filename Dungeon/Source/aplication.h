#ifndef __APLICATION_H__
#define __APLICATION_H__

//==========================================-
//	�C���N���[�h
//==========================================-
#include "main.h"
#include "consoleBuffer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define     DEF_HEADDER_FILE    "Data/Wall/BMP_HEADDER.txt"
#define     DEF_FILE_MAX    (16)
#define buffMove( buf, count )  count += strlen(buf+count) + 1
// �f�t�H���g�l
#define     ON     ( 1 )
#define     OFF    ( 0 )
#define     OK     ( 1 )
#define     NG     (-1 )
#define SCREEN_WIDTH  (512)
#define SCREEN_HEIGHT (256)
#define FONT_SIZE  (16)

//==========================================-
//	�񋓑�
//==========================================-
enum					//�A�v���P�[�V�������[�h
{
	APLICATIONMODE_NONE = -1
	,APLICATIONMODE_TITLE		// �^�C�g��
    ,APLICATIONMODE_GAME		// �Q�[���{��
    ,APLICATIONMODE_RESULT		// ���U���g
    ,APLICATIONMODE_MAX			//����
};

// ������`
typedef enum _DIRECTION
{
    DIRECTION_FRONT,
    DIRECTION_RIGHT,
    DIRECTION_BACK,
    DIRECTION_LEFT,
    DIRECTION_MAX
} DIRECTION;


typedef struct _VECTOR2
{
    int     x;
    int     y;

} VECTOR2;

// �摜�̃e�L�X�g�f�[�^�Ǘ��p�\����
typedef struct _IMAGEBUFFER
{
    int     point;      // �ǂݏo���ʒu
    int     datSize;    // �o�b�t�@�T�C�Y
    char* buf;       // �o�b�t�@�|�C���^

} IMAGEBUFFER;

//==========================================-
//	�v���g�^�C�v�錾
//==========================================-
//=============================
//	���[�h�擾�֐�
//�����F��
//�ߒl�Fint		���݂̃��[�h
//=============================
int GetAppMode(void);

//=============================
//	���[�h�ݒ�֐�
//�����Fint	iMode  ����̃��[�h
//�ߒl�F��
//=============================
void SetAppMode(int iMode);

//=============================
//	�A�v���P�[�V�����S�̂̏�����
//�����F��
//�ߒl�F��
//=============================
int AppInit(void);

//=============================
//	�A�v���P�[�V�����S�̂̏I������
//�����F��
//�ߒl�F��
//=============================
void AppUnInit(void);

//=============================
//	�A�v���P�[�V�����S�̂̍X�V����
//�����F��
//�ߒl�F��
//=============================
void AppUpdate(void);

//=============================
//	�A�v���P�[�V�����S�̂̕`�揈��
//�����F��
//�ߒl�F��
//=============================
void AppDraw(void);

//=============================
//	�E�B���h�E�T�C�Y�g��
//�����FhCOut �n���h��,dwSize �E�B���h�E�o�b�t�@�T�C�Y,consoleWindow �E�B���h�E�T�C�Y
//�ߒl�Fbool ����
//=============================
BOOL setWindowSizeBig(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow);

//=============================
//	�E�B���h�E�T�C�Y�k��
//�����FhCOut �n���h��,dwSize �E�B���h�E�o�b�t�@�T�C�Y,consoleWindow �E�B���h�E�T�C�Y
//�ߒl�Fbool ����
//=============================
BOOL setWindowSizeSmall(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow);

//=============================
//	�p���b�g������������
//�����Fcr �������ރp���b�g�f�[�^�̃A�h���X
//�ߒl�Fint ����
//=============================
int SetConsolePalatte(COLORREF* cr);

//=============================
//	�摜�e�L�X�g�w�b�_�[�t�@�C���ǂݍ��ݏ���
//�����FreadFileName �t�@�C����
//�ߒl�Fbool ����
//=============================
BOOL imageHeadderDataLoad(char* readFileName);

//=============================
//	�摜�e�L�X�g�t�@�C���ǂݍ��ݏ���
//�����FreadFileName �t�@�C����,consolImageData �摜�e�L�X�g
//�ߒl�Fbool ����
//=============================
BOOL imageDataLoad(const char* readFileName, CIMAGEDATA* consolImageData);

//=============================
//	������]�ɂ��`�揈��
//�����Fdirection ����,value ������
//�ߒl�FDIRECTION ����
//=============================
DIRECTION changeDirection(DIRECTION direction, int value);

//=============================
//	�e�L�X�g�t�H���g�̕`�揈��
//�����Ftarget ������
//�ߒl�FVECTOR2 �t�H���g���W
//=============================
VECTOR2 GetFont(const char* target);

//=============================
//	�t�@�C���f�[�^�ǂݍ���
//�����FfileName �t�@�C����,ib �摜�o�b�t�@
//�ߒl�Fint�@����
//=============================
int ImageFileRead(const char* fileName, IMAGEBUFFER* ib);

//=============================
//	�摜�T�C�Y�ݒ�
//�����Fib �摜�o�b�t�@,cid �摜�e�L�X�g
//�ߒl�Fint�@����
//=============================
int ChangeImageSize(IMAGEBUFFER* ib, CIMAGEDATA* cid);

//=============================
//	�摜�f�[�^�ݒ�
//�����Fib �摜�o�b�t�@,cid �摜�e�L�X�g
//�ߒl�Fint�@����
//=============================
int ChangeImage(IMAGEBUFFER* ib, CIMAGEDATA* cid);

//=============================
//	�p���b�g�f�[�^�ݒ�
//�����Fib �摜�o�b�t�@,cid �摜�w�b�_�[
//�ߒl�Fint�@����
//=============================
int ChangePalette(IMAGEBUFFER* ib, CONSOLIMAGEDATA* cid);


#endif//__APLICATION_H__