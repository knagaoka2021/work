//==========================================-
//	インクルード
//==========================================-
#include "main.h"
#include "aplication.h"
#include "result.h"

//==========================================-
//	関数定義
//==========================================-
//=============================
//	リザルトの初期化（一回のみ）
//引数：無
//戻値：無
//=============================
void ResultInit(void)
{
}
//=============================
//	リザルトの初期化（シーン開始時）
//引数：無
//戻値：無
//=============================
void ResultInitScene(void)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolFontImageData = GetConsolFontImageInfo();

    //初期化
    screenBufferClear(pConsolMainBuffer, 0);
    screenBufferClear(pConsolSubBuffer, 0);

    updateFontBufferObject(pConsolMainBuffer, pConsolFontImageData, 0, 0, "゛");
    updateFontBufferObject(pConsolMainBuffer, pConsolFontImageData, 0, FONT_SIZE, "けーむくりあ");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, 0, "　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE, "りさると");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 2, "゜");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 3, "ふれんすえんたー");

    // スクリーン描画
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
    SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
}
//=============================
//	リザルトの終了処理（一回のみ）
//引数：無
//戻値：無
//=============================
void ResultUnInit(void)
{
}
//=============================
//	リザルトの終了処理（シーン終了時）
//引数：無
//戻値：無
//=============================
void ResultUnInitScene(void)
{
}
//=============================
//	リザルトの更新処理
//引数：無
//戻値：無
//=============================
void ResultUpdate(void)
{
	if (INP(PK_ENTER))
	{
		SetAppMode(APLICATIONMODE_TITLE);	//シーン移行
	}
}
//=============================
//	リザルトの描画処理
//引数：無
//戻値：無
//=============================
void ResultDraw(void)
{
}
