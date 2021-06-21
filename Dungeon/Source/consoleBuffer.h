#ifndef __CONSOLEBUFFER_H__
#define __CONSOLEBUFFER_H__

// �R���\�[���o�b�t�@�Ǘ��p�\����
typedef struct _CONSOLIMAGEDATA
{
    int         imgWidth, imgHight;         // �摜�T�C�Y

    int         bufferSizeX, bufferSizeY;   // �R���\�[���o�b�t�@�T�C�Y
    int         screenSizeX, screenSizeY;   // �R���\�[����ʃT�C�Y

    int         paletteSize;                // �p���b�g��
    COLORREF    palette[16];                // �p���b�g�f�[�^

    CHAR_INFO* pImg;                      // �摜�f�[�^�i�����ƐF�j

} CONSOLIMAGEDATA;

typedef struct _CIMAGEDATA
{
    int         imgWidth, imgHight;         // �摜�T�C�Y

    CHAR_INFO* pImg;                      // �摜�f�[�^�i�����ƐF�j

} CIMAGEDATA;
//==========================================-
//	�v���g�^�C�v�錾
//==========================================-
//=============================
//	�X�N���[���o�b�t�@������
//�����Fcid �R���\�[���o�b�t�@,screenW �X�N���[����,screenH �X�N���[������,colorNo �J���[�ԍ�
//�ߒl�Fint ����
//=============================
int screenInit(CONSOLIMAGEDATA* cid, int screenW, int screenH, int colorNo);

//=============================
//	���C���R���\�[���o�b�t�@�擾
//�����F��
//�ߒl�FCONSOLIMAGEDATA �R���\�[���o�b�t�@
//=============================
CONSOLIMAGEDATA* GetConsolMainBufferInfo(void);

//=============================
//	�T�u�R���\�[���o�b�t�@�擾
//�����F��
//�ߒl�FCONSOLIMAGEDATA �R���\�[���o�b�t�@
//=============================
CONSOLIMAGEDATA* GetConsolSubBufferInfo(void);

//=============================
//	�摜�f�[�^���X�g�擾
//�����F��
//�ߒl�FCIMAGEDATA �摜�e�L�X�g
//=============================
CIMAGEDATA* GetConsolImageDataList(void);

//=============================
//	�e�L�X�g�t�H���g�擾
//�����F��
//�ߒl�FCIMAGEDATA �e�L�X�g�t�H���g
//=============================
CIMAGEDATA* GetConsolFontImageInfo(void);

//=============================
//	�X�N���[���o�b�t�@�N���A
//�����Fcid �R���\�[���o�b�t�@,colorNo �J���[�ԍ�
//�ߒl�Fint ����
//=============================
int screenBufferClear(CONSOLIMAGEDATA* cid, int colorNo);
//------------------------------------------------
// �R���\�[����ʕ`��
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     int size    �������ރf�[�^�T�C�Y
//                          ���R���\�[���E�B���h�E�̃o�b�t�@�T�C�Y�Ɠ����T�C�Y�ɂ��Ȃ��Ɛ���ɓ��삵�Ȃ�
//                            �ꕔ�������������ނ悤�ɂ͂ł��Ă��Ȃ�
// ��Q����     int *string �������ރf�[�^
//------------------------------------------------
int SetConsoleStringAttributesBuffer(int posX, int posY, int width, int Hight, CHAR_INFO* pImage);
//------------------------------------------------
// �R���\�[����ʕ`��
// �Ԃ�l       int         true:����  false:�G���[
// ��P����     int         posX    �������ވʒu X
// ��Q����     int         posY    �������݈ʒu Y
// ��R����     int         width   �������ޕ�
// ��S����     int         hight   �������ލ���
// ��T����     CHAR_INFO   *pImage �������ރf�[�^�̃A�h���X
//------------------------------------------------
int SetConsoleStringAttributesBuffer2(int posX, int posY, int width, int hight, CHAR_INFO* pImage);

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
int SetConsoleStringAttributesBufferCustom(int posX, int posY, int width, int hight, int windowX, int windowY, CHAR_INFO* pImage);

//=============================
//	�e�L�X�g�t�H���g�o�b�t�@�X�V
//�����Fcid �R���\�[���o�b�t�@,obj �e�L�X�g�t�H���g,x �X�V�ʒuX,y �X�V�ʒuY
//�ߒl�Fint ����
//=============================
void updateFontBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y, const char* message);

//=============================
//	�摜�f�[�^�o�b�t�@�X�V
//�����Fcid �R���\�[���o�b�t�@,obj �摜�f�[�^,x �X�V�ʒuX,y �X�V�ʒuY
//�ߒl�Fint ����
//=============================
void updateBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y);

//=============================
//	�X�N���[���o�b�t�@�X�V
//�����Fcid �R���\�[���o�b�t�@,x �X�V�ʒuX,y �X�V�ʒuY,colorAttributes �J���[�f�[�^
//�ߒl�Fint ����
//=============================
void update(CONSOLIMAGEDATA* cid, int x, int y, WORD colorAttributes);

//=============================
//	�R���\�[��������
//�����F��
//�ߒl�F��
//=============================
void ConsoleBufferInit(void);

//=============================
//	�R���\�[���C��
//�����F��
//�ߒl�F��
//=============================
void ConsoleBufferUnInit(void);

#endif//__CONSOLEBUFFER_H__