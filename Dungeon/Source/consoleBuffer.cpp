//==========================================-
//	インクルード
//==========================================-
#include "aplication.h"
#include "consoleBuffer.h"

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
CONSOLIMAGEDATA     consolMainBuffer;    // メインコンソールバッファ
CONSOLIMAGEDATA     consolSubBuffer;    // サブコンソールバッファ
CIMAGEDATA     consolImageDataList[DEF_FILE_MAX];    // コンソールに出力する画像データリスト
CIMAGEDATA     consolfontImageData;    // コンソールに出力するテキストフォントデータ

//=============================
//	コンソール初期化
//引数：無
//戻値：無
//=============================
void ConsoleBufferInit(void)
{
    // メインスクリーン初期化
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

    //初期化
    screenBufferClear(&consolMainBuffer, 0);
    screenBufferClear(&consolSubBuffer, 0);

}

//=============================
//	コンソール修了
//引数：無
//戻値：無
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
//	スクリーンバッファ初期化
//引数：cid コンソールバッファ,screenW スクリーン幅,screenH スクリーン高さ,colorNo カラー番号
//戻値：int 結果
//=============================
int screenInit(CONSOLIMAGEDATA* cid, int screenW, int screenH, int colorNo)
{
    CHAR_INFO* pImg;
    CHAR_INFO* pWork;

    // スクリーン初期化
    cid->imgWidth = screenW;
    cid->imgHight = screenH;

    // スクリーンサイズ設定
    cid->screenSizeX = screenW;
    cid->screenSizeY = screenH;

    // スクリーンバッファサイズ設定
    cid->bufferSizeX = screenW + 1;
    cid->bufferSizeY = screenH + 1;

    // 領域確保
    pImg = (CHAR_INFO*)malloc(cid->screenSizeX * cid->screenSizeY * sizeof(CHAR_INFO));
    if (pImg == NULL)
    {
        printf("領域確保失敗");
        return NG;
    }

    // データ変換
    for (int y = 0; y < cid->screenSizeY; y++)
    {
        for (int x = 0; x < cid->screenSizeX; x++)
        {
            // アドレス計算
            pWork = pImg + (y * cid->screenSizeX) + x;

            // 画像データ（ 文字と色 ）
            pWork->Char.AsciiChar = ' ';        // 文字データセット
            pWork->Attributes = (colorNo << 4) | colorNo;   // 色  データセット
        }

    }

    // バッファセット
    cid->pImg = pImg;

    return OK;
}

CONSOLIMAGEDATA* GetConsolMainBufferInfo(void)
{
    return(&consolMainBuffer);
}
//=============================
//	サブコンソールバッファ取得
//引数：無
//戻値：CONSOLIMAGEDATA コンソールバッファ
//=============================
CONSOLIMAGEDATA* GetConsolSubBufferInfo(void)
{
    return(&consolSubBuffer);
}

//=============================
//	画像データリスト取得
//引数：無
//戻値：CIMAGEDATA 画像テキスト
//=============================
CIMAGEDATA* GetConsolImageDataList(void)
{
    return(consolImageDataList);
}

//=============================
//	テキストフォント取得
//引数：無
//戻値：CIMAGEDATA テキストフォント
//=============================
CIMAGEDATA* GetConsolFontImageInfo(void)
{
    return(&consolfontImageData);
}
//=============================
//	スクリーンバッファクリア
//引数：cid コンソールバッファ,colorNo カラー番号
//戻値：int 結果
//=============================
int screenBufferClear(CONSOLIMAGEDATA* cid, int colorNo)
{
    CHAR_INFO* pImg;
    CHAR_INFO* pWork;

    pImg = cid->pImg;

    // データ変換
    for (int y = 0; y < cid->screenSizeY; y++)
    {
        for (int x = 0; x < cid->screenSizeX; x++)
        {
            // アドレス計算
            pWork = pImg + (y * cid->screenSizeX) + x;

            // 画像データ（ 文字と色 ）
            pWork->Attributes = (colorNo << 4) | colorNo;   // 色  データセット

        }

    }

    // バッファセット
    cid->pImg = pImg;

    return OK;
}

//------------------------------------------------
// コンソール画面描画
// 返り値       int         true:正常  false:エラー
// 第１引数     int size    書き込むデータサイズ
//                          ※コンソールウィンドウのバッファサイズと同じサイズにしないと正常に動作しない
//                            一部分だけ書き込むようにはできていない
// 第２引数     int *string 書き込むデータ
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
// コンソール画面描画
// 返り値       int         true:正常  false:エラー
// 第１引数     int         posX    書き込む位置 X
// 第２引数     int         posY    書き込み位置 Y
// 第３引数     int         width   書き込む幅
// 第４引数     int         hight   書き込む高さ
// 第５引数     CHAR_INFO   *pImage 書き込むデータのアドレス
//------------------------------------------------
int SetConsoleStringAttributesBuffer(int posX, int posY, int width, int hight, CHAR_INFO* pImage)
{
    BOOL        ret;
    HANDLE      hCOut;
    COORD       pos = { (SHORT)posX, (SHORT)posY };
    COORD       datSize = { (SHORT)width,(SHORT)hight };
    SMALL_RECT  windowRect = { 0, 0, (SHORT)width - 1, (SHORT)hight - 1 };

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // コンソールバッファに文字と色の書き込み
    ret = WriteConsoleOutput(hCOut, pImage, datSize, pos, &windowRect);

    return ret;
}
//------------------------------------------------
// コンソール画面に直接書き込む処理
// 返り値       int         true:正常  false:エラー
// 第１引数     int         posX    書き込む位置 X
// 第２引数     int         posY    書き込み位置 Y
// 第３引数     int         width   書き込む幅
// 第４引数     int         hight   書き込む高さ
// 第５引数     CHAR_INFO   *pImage 書き込むデータのアドレス
//------------------------------------------------
int SetConsoleStringAttributesBuffer2(int posX, int posY, int width, int hight, CHAR_INFO* pImage)
{
    BOOL        ret;
    HANDLE      hCOut;
    COORD       pos = { 0, 0 };
    COORD       datSize = { (SHORT)width,(SHORT)hight };
    SMALL_RECT  windowRect = { (SHORT)posX, (SHORT)posY, (SHORT)(posX + width) - 1, (SHORT)(posY + hight) - 1 };

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // コンソールバッファに文字と色の書き込み
    ret = WriteConsoleOutput(hCOut, pImage, datSize, pos, &windowRect);

    return ret;
}
//------------------------------------------------
// コンソール画面に直接書き込む処理
// 返り値       int         true:正常  false:エラー
// 第１引数     int         posX    書き込む位置 X
// 第２引数     int         posY    書き込み位置 Y
// 第３引数     int         width   書き込む幅
// 第４引数     int         hight   書き込む高さ
// 第５引数     int         windowX   書き込む枠位置 X
// 第６引数     int         windowY   書き込み枠位置 Y
// 第７引数     CHAR_INFO   *pImage 書き込むデータのアドレス
//------------------------------------------------
int SetConsoleStringAttributesBufferCustom(int posX, int posY, int width, int hight, int windowX, int windowY, CHAR_INFO* pImage)
{
    BOOL        ret;
    HANDLE      hCOut;
    COORD       pos = { (SHORT)posX, (SHORT)posY };
    COORD       datSize = { (SHORT)width,(SHORT)hight };
    SMALL_RECT  windowRect = { (SHORT)windowX, (SHORT)windowY, (SHORT)(windowX + width) - 1, (SHORT)(windowY + hight) - 1 };

    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // コンソールバッファに文字と色の書き込み
    ret = WriteConsoleOutput(hCOut, pImage, datSize, pos, &windowRect);

    return ret;
}
//=============================
//	テキストフォントバッファ更新
//引数：cid コンソールバッファ,obj テキストフォント,x 更新位置X,y 更新位置Y
//戻値：int 結果
//=============================
void updateFontBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y, const char* message)
{
    // 画像データ領域
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

        // データ変換
        for (int j = loopY; j < loopY + FONT_SIZE; j++)
        {
            for (int k = loopX; k < loopX + FONT_SIZE; k++)
            {

                // アドレス計算
                pWorkObj = obj->pImg + (j * obj->imgWidth) + k;

                // 領域外制御
                if (posX >= SCREEN_WIDTH - 1) {
                    break;
                }

                // 色データ取り出し
                WORD colorAttributes = pWorkObj->Attributes;   // 色  データセット

                update(cid, posX, posY, colorAttributes);

                posX++;

            }
            posX = x;
            posY++;

            // 領域外制御
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
//	画像データバッファ更新
//引数：cid コンソールバッファ,obj 画像データ,x 更新位置X,y 更新位置Y
//戻値：int 結果
//=============================
void updateBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y)
{
    // 画像データ領域
    CHAR_INFO* pWork = NULL;
    CHAR_INFO* pWorkObj = NULL;

    int     loopX, loopY, posX = x, posY = y;

    // データ変換
    for (loopY = 0; loopY < obj->imgHight; loopY++)
    {
        for (loopX = 0; loopX < obj->imgWidth; loopX++)
        {

            // アドレス計算
            pWorkObj = obj->pImg + (loopY * obj->imgWidth) + loopX;

            // 領域外制御
            if (posX >= SCREEN_WIDTH - 1) {
                break;
            }

            // 色データ取り出し
            WORD colorAttributes = pWorkObj->Attributes;   // 色  データセット

            update(cid, posX, posY, colorAttributes);

            posX++;

        }
        posX = x;
        posY++;

        // 領域外制御
        if (posY >= SCREEN_HEIGHT - 1) {
            break;
        }
    }

}
//=============================
//	スクリーンバッファ更新
//引数：cid コンソールバッファ,x 更新位置X,y 更新位置Y,colorAttributes カラーデータ
//戻値：int 結果
//=============================
void update(CONSOLIMAGEDATA* cid, int x, int y, WORD colorAttributes)
{
    // 画像データ領域
    CHAR_INFO* pWork = NULL;
    WORD alphaAttributes = (1 << 4) | 1;

    // アドレス計算
    pWork = cid->pImg + (y * cid->screenSizeX) + x;

    if (colorAttributes != alphaAttributes)
    {
        // データ変換
        // 画像データ（ 文字と色 ）
        // pWork->Char.AsciiChar = ' ';                      // 文字データセット
        pWork->Attributes = colorAttributes;   // 色  データセット
    }
}
