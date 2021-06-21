//==========================================-
//	�C���N���[�h
//==========================================-
#include "main.h"
#include "aplication.h"
#include "title.h"

//==========================================-
//	�֐���`
//==========================================-
//=============================
//	�^�C�g���̏������i���̂݁j
//�����F��
//�ߒl�F��
//=============================
void TitleInit(void)
{
}
//=============================
//	�^�C�g���̏������i�V�[���J�n���j
//�����F��
//�ߒl�F��
//=============================
void TitleInitScene(void)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolFontImageData = GetConsolFontImageInfo();

    //������
    screenBufferClear(pConsolMainBuffer, 0);
    screenBufferClear(pConsolSubBuffer, 0);

    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE, "�@�J�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 2, "�u���񂵂��v");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 3, "�K");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 4, "�ӂꂷ����");

    // �X�N���[���`��
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
    SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
}
//=============================
//	�^�C�g���̏I�������i���̂݁j
//�����F��
//�ߒl�F��
//=============================
void TitleUnInit(void)
{
}
//=============================
//	�^�C�g���̏I�������i�V�[���I�����j
//�����F��
//�ߒl�F��
//=============================
void TitleUnInitScene(void)
{
}
//=============================
//	�^�C�g���̍X�V����
//�����F��
//�ߒl�F��
//=============================
void TitleUpdate(void)
{
	if (INP(PK_ENTER))
	{
		SetAppMode(APLICATIONMODE_GAME);	//�V�[���ڍs
	}
}
//=============================
//	�^�C�g���̕`�揈��
//�����F��
//�ߒl�F��
//=============================
void TitleDraw(void)
{
}
