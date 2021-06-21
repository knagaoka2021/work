//==========================================-
//	�C���N���[�h
//==========================================-
#include "main.h"
#include "aplication.h"
#include "consoleBuffer.h"
#include "game.h"
#include "player.h"
#include "map.h"

//==========================================-
//	�֐���`
//==========================================-
//=============================
//	�Q�[���̏������i���̂݁j
//�����F��
//�ߒl�F��
//=============================
void GameInit(void)
{
}
//=============================
//	�Q�[���̏������i�V�[���J�n���j
//�����F��
//�ߒl�F��
//=============================
void GameInitScene(void)
{
	PlayerInit();	//�v���[���[

    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolFontImageData = GetConsolFontImageInfo();

    //������
    screenBufferClear(pConsolMainBuffer, 0);
    screenBufferClear(pConsolSubBuffer, 0);

    PLAYER* pPlayer = GetPlayerInfo();

    // �X�N���[���o�b�t�@�X�V
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (checkMapFront((3 * i) + j, pPlayer->posX, pPlayer->posY)) {
                CIMAGEDATA* pWork = (pConsolImageDataList + (3 * i) + j);
                updateBufferObject(pConsolMainBuffer, pWork, 0, 0);
            }
        }
    }

    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, 0, "�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE, "���[��");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 2, "�@�@�@�@�@�J�@�@�@�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 3, "�����Ă��F���[����߂����I");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 4, "�@�@�@�J�@�@�@�@�@�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 5, "�����₵�邵�F�܂����Ƃ�");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 6, "�@�@�@�J�@�@�@�@�@�@�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 7, "�����₵�邵�F�����낢�Ƃ�");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 8, "�@�J�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 9, "�݂��₵�邵�F�����Ă�");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 10, "�@�J�@�@�J");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 11, "�Ђ���₵�邵�F�����Ă�");

    // �X�N���[���`��
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
    SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
}
//=============================
//	�Q�[���̏I�������i���̂݁j
//�����F��
//�ߒl�F��
//=============================
void GameUnInit(void)
{
	PlayerUnInit();	//�v���[���[
}
//=============================
//	�Q�[���̏I�������i�V�[���I�����j
//�����F��
//�ߒl�F��
//=============================
void GameUnInitScene(void)
{
}
//=============================
//	�Q�[���̍X�V����
//�����F��
//�ߒl�F��
//=============================
void GameUpdate(void)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolfontImageData = GetConsolFontImageInfo();
    MAPDATA* pMap = GeMapInfo();

	PlayerUpdate();	//�v���[���[

    PLAYER* pPlayer = GetPlayerInfo();

    if (INP(PK_U))    // [��]�̉�����Ԃ��擾����B
    {

        screenBufferClear(pConsolMainBuffer, 0);
        screenBufferClear(pConsolSubBuffer, 0);
        updateMap(pPlayer);

        updateFontBufferObject(pConsolSubBuffer, pConsolfontImageData, 0, 0, "�������񂵂���c4");

        SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
    }

    if (pMap->goalPosX == pPlayer->posX && pMap->goalPosY == pPlayer->posY)
    {
        SetAppMode(APLICATIONMODE_RESULT);	//�V�[���ڍs
    }
}
//=============================
//	�Q�[���̕`�揈��
//�����F��
//�ߒl�F��
//=============================
void GameDraw(void)
{

}
