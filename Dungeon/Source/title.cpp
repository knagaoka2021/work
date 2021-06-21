//==========================================-
//	インクルード
//==========================================-
#include "main.h"
#include "aplication.h"
#include "title.h"

//==========================================-
//	関数定義
//==========================================-
//=============================
//	タイトルの初期化（一回のみ）
//引数：無
//戻値：無
//=============================
void TitleInit(void)
{
}
//=============================
//	タイトルの初期化（シーン開始時）
//引数：無
//戻値：無
//=============================
void TitleInitScene(void)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CONSOLIMAGEDATA* pConsolSubBuffer = GetConsolSubBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();
    CIMAGEDATA* pConsolFontImageData = GetConsolFontImageInfo();

    //初期化
    screenBufferClear(pConsolMainBuffer, 0);
    screenBufferClear(pConsolSubBuffer, 0);

    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE, "　゛　゛");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 2, "「たんしよん」");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 3, "゜");
    updateFontBufferObject(pConsolSubBuffer, pConsolFontImageData, 0, FONT_SIZE * 4, "ふれすえんた");

    // スクリーン描画
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
    SetConsoleStringAttributesBufferCustom(0, 0, pConsolSubBuffer->screenSizeX, pConsolSubBuffer->screenSizeY, 200 + 1, 0, pConsolSubBuffer->pImg);
}
//=============================
//	タイトルの終了処理（一回のみ）
//引数：無
//戻値：無
//=============================
void TitleUnInit(void)
{
}
//=============================
//	タイトルの終了処理（シーン終了時）
//引数：無
//戻値：無
//=============================
void TitleUnInitScene(void)
{
}
//=============================
//	タイトルの更新処理
//引数：無
//戻値：無
//=============================
void TitleUpdate(void)
{
	if (INP(PK_ENTER))
	{
		SetAppMode(APLICATIONMODE_GAME);	//シーン移行
	}
}
//=============================
//	タイトルの描画処理
//引数：無
//戻値：無
//=============================
void TitleDraw(void)
{
}
