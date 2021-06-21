//==========================================-
//	�C���N���[�h
//==========================================-
#include "main.h"
#include "aplication.h"
#include "result.h"

//==========================================-
//	�֐���`
//==========================================-
//=============================
//	���U���g�̏������i���̂݁j
//�����F��
//�ߒl�F��
//=============================
void ResultInit(void)
{
}
//=============================
//	���U���g�̏������i�V�[���J�n���j
//�����F��
//�ߒl�F��
//=============================
void ResultInitScene(void)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolFontImageData = GetConsolFontImageInfo();

    //������
    screenBufferClear(pConsolMainBuffer, 0);
    screenBufferClear(pConsolSubBuffer, 0);

    updateFontBufferObject(pConsolMainBuffer, pConsolFontImageData, 0, 0, "�J");
    updateFontBufferObject(pConsolMainBuffer, pConsolFontImageData, 0, FONT_SIZE, "���[�ނ��肠");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, 0, "�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE, "�肳���");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 2, "�K");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 3, "�ӂ�񂷂��񂽁[");

    // �X�N���[���`��
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
    SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
}
//=============================
//	���U���g�̏I�������i���̂݁j
//�����F��
//�ߒl�F��
//=============================
void ResultUnInit(void)
{
}
//=============================
//	���U���g�̏I�������i�V�[���I�����j
//�����F��
//�ߒl�F��
//=============================
void ResultUnInitScene(void)
{
}
//=============================
//	���U���g�̍X�V����
//�����F��
//�ߒl�F��
//=============================
void ResultUpdate(void)
{
	if (INP(PK_ENTER))
	{
		SetAppMode(APLICATIONMODE_TITLE);	//�V�[���ڍs
	}
}
//=============================
//	���U���g�̕`�揈��
//�����F��
//�ߒl�F��
//=============================
void ResultDraw(void)
{
}
