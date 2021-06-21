//==========================================-
//	インクルード
//==========================================-
#include "main.h"
#include "aplication.h"
#include "consoleBuffer.h"
#include "game.h"
#include "player.h"
#include "map.h"

//==========================================-
//	関数定義
//==========================================-
//=============================
//	ゲームの初期化（一回のみ）
//引数：無
//戻値：無
//=============================
void GameInit(void)
{
}
//=============================
//	ゲームの初期化（シーン開始時）
//引数：無
//戻値：無
//=============================
void GameInitScene(void)
{
	PlayerInit();	//プレーヤー

    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolFontImageData = GetConsolFontImageInfo();

    //初期化
    screenBufferClear(pConsolMainBuffer, 0);
    screenBufferClear(pConsolSubBuffer, 0);

    PLAYER* pPlayer = GetPlayerInfo();

    // スクリーンバッファ更新
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (checkMapFront((3 * i) + j, pPlayer->posX, pPlayer->posY)) {
                CIMAGEDATA* pWork = (pConsolImageDataList + (3 * i) + j);
                updateBufferObject(pConsolMainBuffer, pWork, 0, 0);
            }
        }
    }

    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, 0, "゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE, "けーむ");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 2, "　　　　　゛　　　　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 3, "もくてき：こーるをめさせ！");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 4, "　　　゛　　　　　　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 5, "うえやしるし：まえいとう");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 6, "　　　゛　　　　　　　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 7, "したやしるし：うしろいとう");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 8, "　゛　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 9, "みきやしるし：かいてん");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 10, "　゛　　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 11, "ひたりやしるし：かいてん");

    // スクリーン描画
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
    SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
}
//=============================
//	ゲームの終了処理（一回のみ）
//引数：無
//戻値：無
//=============================
void GameUnInit(void)
{
	PlayerUnInit();	//プレーヤー
}
//=============================
//	ゲームの終了処理（シーン終了時）
//引数：無
//戻値：無
//=============================
void GameUnInitScene(void)
{
}
//=============================
//	ゲームの更新処理
//引数：無
//戻値：無
//=============================
void GameUpdate(void)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolfontImageData = GetConsolFontImageInfo();
    MAPDATA* pMap = GeMapInfo();

	PlayerUpdate();	//プレーヤー

    PLAYER* pPlayer = GetPlayerInfo();

    if (INP(PK_U))    // [↓]の押下状態を取得する。
    {

        screenBufferClear(pConsolMainBuffer, 0);
        screenBufferClear(pConsolSubBuffer, 0);
        updateMap(pPlayer);

        updateFontBufferObject(pConsolSubBuffer, pConsolfontImageData, 0, 0, "こうしんしたなc4");

        SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
    }

    if (pMap->goalPosX == pPlayer->posX && pMap->goalPosY == pPlayer->posY)
    {
        SetAppMode(APLICATIONMODE_RESULT);	//シーン移行
    }
}
//=============================
//	ゲームの描画処理
//引数：無
//戻値：無
//=============================
void GameDraw(void)
{

}
