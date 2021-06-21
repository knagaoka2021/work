//==========================================-
//	インクルード
//==========================================-
#include "aplication.h"
#include "consoleBuffer.h"
#include "map.h"
#include "title.h"
#include "game.h"
#include "result.h"

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
static int iAppMode = APLICATIONMODE_NONE;
IMAGEBUFFER         imageBuffer;        // キャラ画像のテキストデータ
CONSOLIMAGEDATA     header;    // コンソールに出力するキャラ画像データ
HANDLE hCOut;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
CONSOLE_SCREEN_BUFFER_INFOEX csbiBackUp;
WORD wAttr;


//==========================================-
//	関数の定義
//==========================================-
//=============================
//	モード取得関数
//引数：無
//戻値：int	現在のモード
//=============================
int GetAppMode(void)
{
	return(iAppMode);
}

//=============================
//	モード設定関数
//引数：int	iMode 次回のモード
//戻値：無
//=============================
void SetAppMode(int iMode)
{
	//シーン毎の終了処理
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//タイトル
		TitleUnInitScene();
		break;
	case APLICATIONMODE_GAME:		//ゲーム本編
		GameUnInitScene();
		break;
	case APLICATIONMODE_RESULT:	//リザルト
		ResultUnInitScene();
		break;
	}
	//モードの変更
	iAppMode = iMode;
	//シーン毎の初期化処理
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//タイトル
		TitleInitScene();
		break;
	case APLICATIONMODE_GAME:		//ゲーム本編
		GameInitScene();
		break;
	case APLICATIONMODE_RESULT:	//リザルト
		ResultInitScene();
		break;
	}
}

//=============================
//	アプリケーション全体の初期化
//引数：無
//戻値：無
//=============================
int AppInit(void)
{
    // ハンドル情報取得(バックアップ)
    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hCOut == INVALID_HANDLE_VALUE)
    {
        printf_s("ErrorCode = %d\n", GetLastError());
        return -1;
    }
    // デフォルトのスクリーンバッファ情報を保存
    if (!GetConsoleScreenBufferInfo(hCOut, &consoleInfo))
    {
        printf("ErrorCode = %d\n", GetLastError());
        return -1;
    }

    // デフォルトの色情報を保存
    
    if (!SetConsoleTextAttribute(hCOut, wAttr))
    {
        printf_s("ErrorCode = %d\n", GetLastError());
        return -1;
    }

    // コンソール処理用変数
    csbiBackUp.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    // コンソール情報取得
    GetConsoleScreenBufferInfoEx(hCOut, &csbiBackUp);

    // フォントサイズ変更
    CONSOLE_FONT_INFOEX font_size = { sizeof font_size };
    font_size.dwFontSize.X = 1;
    font_size.dwFontSize.Y = 1;
    SetCurrentConsoleFontEx(hCOut, 0, &font_size);

    // ウィンドウサイズ拡大
    COORD       dwSize = { SCREEN_WIDTH,SCREEN_HEIGHT };
    SMALL_RECT consoleWindow = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
    setWindowSizeBig(hCOut, dwSize, consoleWindow);

    ConsoleBufferInit();

    MapInit();

    // 画像データ読み込み
    char        readFileName[100];//キャラ
    sprintf(readFileName, "%s", DEF_HEADDER_FILE);

    imageHeadderDataLoad(readFileName);

	SetAppMode(APLICATIONMODE_TITLE);
	TitleInit();
	GameInit();
	ResultInit();

    return 0;
}

//=============================
//	アプリケーション全体の初期化
//引数：無
//戻値：無
//=============================
void AppUnInit(void)
{
	TitleUnInit();
	GameUnInit();
	ResultUnInit();
    MapUnInit();

    // 領域解放
    if (header.pImg != NULL)
    {
        free(header.pImg);
        header.pImg = NULL;
    }

    wAttr = consoleInfo.wAttributes;

    // デフォルトパレット設定
    SetConsoleScreenBufferInfoEx(hCOut, &csbiBackUp); //初期パレットに戻す
    SetConsoleTextAttribute(hCOut, wAttr);// 初期表示文字の色を元に戻す。
    SetConsoleScreenBufferSize(hCOut, consoleInfo.dwSize);// 初期ウィンドウサイズを元に戻す。

}

//=============================
//	アプリケーション全体の初期化
//引数：無
//戻値：無
//=============================
void AppUpdate(void)
{
	//モード毎の更新処理
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//タイトル
		TitleUpdate();
		break;
	case APLICATIONMODE_GAME:		//ゲーム本編
		GameUpdate();
		break;
	case APLICATIONMODE_RESULT:	//リザルト
		ResultUpdate();
		break;
	}


    // キーワードバッファクリア
    INPCLEAR();

}

//=============================
//	アプリケーション全体の初期化
//引数：無
//戻値：無
//=============================
void AppDraw(void)
{
	//モード毎の描画処理
	switch (iAppMode)
	{
	case APLICATIONMODE_TITLE:		//タイトル
		TitleDraw();
		break;
	case APLICATIONMODE_GAME:		//ゲーム本編
		GameDraw();
		break;
	case APLICATIONMODE_RESULT:	//リザルト
		ResultDraw();
		break;
	}
}

// ウィンドウサイズ拡大
BOOL setWindowSizeBig(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow)
{
    BOOL result = false;

    // ウィンドウバッファサイズ変更
    SetConsoleScreenBufferSize(hCOut, dwSize);

    // ウィンドウサイズ変更
    result = SetConsoleWindowInfo(hCOut, TRUE, &consoleWindow);

    return result;
}
// ウィンドウサイズ縮小
BOOL setWindowSizeSmall(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow)
{
    BOOL result = false;

    // ウィンドウサイズ変更
    result = SetConsoleWindowInfo(hCOut, TRUE, &consoleWindow);

    // ウィンドウバッファサイズ変更
    SetConsoleScreenBufferSize(hCOut, dwSize);

    return result;
}


// パレット書き込え処理
//------------------------------------------------
// コンソール画面のパレット書き換え処理
// 返り値       int         true:正常  false:エラー
// 第１引数     COLORREF *c 書き込むパレットデータのアドレス
//------------------------------------------------
int SetConsolePalatte(COLORREF* cr)
{

    BOOL        ret;
    HANDLE      hCOut;

    // ウィンドウハンドル取得
    hCOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // コンソール処理用変数
    CONSOLE_SCREEN_BUFFER_INFOEX csbi;
    csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    // コンソール情報取得
    ret = GetConsoleScreenBufferInfoEx(hCOut, &csbi);

    // オリジナルパレット書き換え
    memcpy(&csbi.ColorTable[0], cr, sizeof(COLORREF) * 16);

    // オリジナルパレット設定
    ret = SetConsoleScreenBufferInfoEx(hCOut, &csbi);

    return ret;
}
//=============================
//	画像テキストヘッダーファイル読み込み処理
//引数：readFileName ファイル名
//戻値：bool 結果
//=============================
BOOL imageHeadderDataLoad(char* readFileName)
{
    BOOL        ret;

    // ファイルデータ読み込み
    ret = ImageFileRead(readFileName, &imageBuffer);
    if (ret == NG)
    {
        // エラー処理
        printf("ファイル読み込みエラー\n");
        getchar();
        exit(0);
    }

    // パレットデータ変換
    ret = ChangePalette(&imageBuffer, &header);
    if (ret == NG)
    {
        // エラー処理
        printf("パレット変換エラー\n");
        getchar();
        exit(0);
    }

    // パレットデータセット
    ret = SetConsolePalatte(&header.palette[0]);
    if (ret == false)
    {
        // エラー処理
        printf("パレット設定エラー\n");
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
//	画像テキストファイル読み込み処理
//引数：readFileName ファイル名,consolImageData 画像テキスト
//戻値：bool 結果
//=============================
BOOL imageDataLoad(const char* readFileName, CIMAGEDATA* consolImageData)
{
    BOOL        ret;

    // ファイルデータ読み込み
    ret = ImageFileRead(readFileName, &imageBuffer);
    if (ret == NG)
    {
        // エラー処理
        printf("ファイル読み込みエラー\n");
        getchar();
        exit(0);
    }

    // 画像サイズ設定
    ret = ChangeImageSize(&imageBuffer, consolImageData);
    if (ret == NG)
    {
        // エラー処理
        printf("画像サイズ設定エラー\n");
        getchar();
        exit(0);
    }

    // データ変換
    ret = ChangeImage(&imageBuffer, consolImageData);
    if (ret == NG)
    {
        printf("画像データ変換処理エラー\n");
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
//	ファイルデータ読み込み
//引数：fileName ファイル名,ib 画像バッファ
//戻値：int　結果
//=============================
int ImageFileRead(const char* fileName, IMAGEBUFFER* ib)
{
    FILE* fp;    // ファイルポインタ
    int     fsize;  // ファイルサイズ

    char* buf;   // データ領域


    // ファイルオープン
    fp = fopen(fileName, "rt");
    if (fp == NULL)
    {
        return  NG;
    }


    // ファイルサイズ取得
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    // データ領域確保
    buf = (char*)malloc(fsize);


    // 全データ読み込み
    fsize = fread(buf, sizeof(char), fsize, fp);


    // ファイルクローズ
    fclose(fp);


    // 改行コードと , を文字列の区切りに変換
    for (int i = 0; i < fsize; i++)
    {
        if (*(buf + i) == 0x0A
            || *(buf + i) == ',')
        {
            *(buf + i) = 0x00;
        }
    }


    // データセット
    ib->datSize = fsize;    // バッファサイズ設定
    ib->buf = buf;      // データ領域設定


    return  OK;
}

//=============================
//	パレットデータ変換
//引数：ib 画像バッファ,cid 画像ヘッダー
//戻値：int　結果
//=============================
int ChangePalette(IMAGEBUFFER* ib, CONSOLIMAGEDATA* cid)
{
    // パレットデータ
    COLORREF    palette[16];

    // パレット数
    int         paletteSize;

    // 色データ
    unsigned int r, g, b, a;

    // データバッファ（作業用）
    char* buf;
    int     pcount;     // バッファアドレス変更用


    // データセット
    buf = ib->buf;
    pcount = ib->point;


    // パレット数取得
    sscanf(buf + pcount, "%d", &paletteSize);
    buffMove(buf, pcount);
    pcount++;                       // 改行スキップ


    //パレット取得
    for (int i = 0; i < paletteSize; i++)
    {
        // 赤データ読み込み
        sscanf(buf + pcount, "%d", &r);
        buffMove(buf, pcount);

        sscanf(buf + pcount, "%d", &g);
        buffMove(buf, pcount);

        sscanf(buf + pcount, "%d", &b);
        buffMove(buf, pcount);

        sscanf(buf + pcount, "%d", &a);
        buffMove(buf, pcount);

        // RGBA 合成
        palette[i] = (a << 24) | (r << 16) | (g << 8) | b;

        pcount++;       // 改行スキップ
    }


    // パレットデータセット
    memcpy(cid->palette, palette, sizeof(COLORREF) * 16);


    // パレット数セット
    cid->paletteSize = paletteSize;     // パレット数設定
    ib->point = pcount;          // テキストデータの使用位置更新

    return  OK;
}
//=============================
//	画像サイズ設定
//引数：ib 画像バッファ,consolImageData 画像テキスト
//戻値：int　結果
//=============================
int ChangeImageSize(IMAGEBUFFER* ib, CIMAGEDATA* cid)
{
    // バッファデータ管理変数
    char* buf;
    int   pcount;

    // 画像サイズ
    int xMax, yMax;

    // バッファデータセット
    buf = ib->buf;          // バッファアドレスセット
    pcount = ib->point;        // バッファ使用位置セット


    // 画像サイズ取得
    sscanf(buf + pcount, "%d", &xMax);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &yMax);
    buffMove(buf, pcount);

    pcount++;   // 改行スキップ

    // 画像サイズ設定
    cid->imgWidth = xMax;
    cid->imgHight = yMax;

    ib->point = pcount;        // バッファ使用位置セット

    return OK;
}

//=============================
//	画像データ設定
//引数：ib 画像バッファ,consolImageData 画像テキスト
//戻値：int　結果
//=============================
int ChangeImage(IMAGEBUFFER* ib, CIMAGEDATA* cid)
{
    // 画像データ領域
    CHAR_INFO* pImg;
    CHAR_INFO* pWork;

    // バッファデータ管理変数
    char* buf;
    int     pcount;

    int     colorNo;        // カラー番号
    int     loopX, loopY;


    // バッファデータセット
    buf = ib->buf;          // バッファアドレスセット
    pcount = ib->point;        // バッファ使用位置セット


    // 領域確保
    pImg = (CHAR_INFO*)malloc(cid->imgWidth * cid->imgHight * sizeof(CHAR_INFO));
    if (pImg == NULL)
    {
        printf("領域確保失敗");
        return NG;
    }

    // データ変換
    for (loopY = 0; loopY < cid->imgHight; loopY++)
    {
        for (loopX = 0; loopX < cid->imgWidth; loopX++)
        {
            // 色データ取り出し
            sscanf(buf + pcount, "%d", &colorNo);
            buffMove(buf, pcount);

            // アドレス計算
            pWork = pImg + (loopY * cid->imgWidth) + loopX;

            // 画像データ（ 文字と色 ）
            (pWork)->Char.AsciiChar = ' ';                      // 文字データセット
            (pWork)->Attributes = (colorNo << 4) | colorNo;   // 色  データセット
        }

        pcount++;  // 改行コードスキップ
    }

    // バッファセット
    cid->pImg = pImg;

    ib->point = pcount;        // バッファ使用位置セット

    return OK;
}

//=============================
//	方向回転による描画処理
//引数：direction 方向,value 向き先
//戻値：DIRECTION 方向
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
//	テキストフォントの描画処理
//引数：target 文字列
//戻値：VECTOR2 フォント座標
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

    if (strcmp(value, "あ") == 0)
    {
        ret.x = 0;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "い") == 0)
    {
        ret.x = 16;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "う") == 0)
    {
        ret.x = 32;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "え") == 0)
    {
        ret.x = 48;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "お") == 0)
    {
        ret.x = 64;
        ret.y = 0;
        return ret;
    }
    if (strcmp(value, "か") == 0)
    {
        ret.x = 0;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "き") == 0)
    {
        ret.x = 16;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "く") == 0)
    {
        ret.x = 32;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "け") == 0)
    {
        ret.x = 48;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "こ") == 0)
    {
        ret.x = 64;
        ret.y = 16;
        return ret;
    }
    if (strcmp(value, "さ") == 0)
    {
        ret.x = 0;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "し") == 0)
    {
        ret.x = 16;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "す") == 0)
    {
        ret.x = 32;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "せ") == 0)
    {
        ret.x = 48;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "そ") == 0)
    {
        ret.x = 64;
        ret.y = 32;
        return ret;
    }
    if (strcmp(value, "た") == 0)
    {
        ret.x = 0;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "ち") == 0)
    {
        ret.x = 16;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "つ") == 0)
    {
        ret.x = 32;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "て") == 0)
    {
        ret.x = 48;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "と") == 0)
    {
        ret.x = 64;
        ret.y = 48;
        return ret;
    }
    if (strcmp(value, "な") == 0)
    {
        ret.x = 0;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "に") == 0)
    {
        ret.x = 16;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "ぬ") == 0)
    {
        ret.x = 32;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "ね") == 0)
    {
        ret.x = 48;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "の") == 0)
    {
        ret.x = 64;
        ret.y = 64;
        return ret;
    }
    if (strcmp(value, "は") == 0)
    {
        ret.x = 0;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "ひ") == 0)
    {
        ret.x = 16;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "ふ") == 0)
    {
        ret.x = 32;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "へ") == 0)
    {
        ret.x = 48;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "ほ") == 0)
    {
        ret.x = 64;
        ret.y = 80;
        return ret;
    }
    if (strcmp(value, "ま") == 0)
    {
        ret.x = 0;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "み") == 0)
    {
        ret.x = 16;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "む") == 0)
    {
        ret.x = 32;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "め") == 0)
    {
        ret.x = 48;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "も") == 0)
    {
        ret.x = 64;
        ret.y = 96;
        return ret;
    }
    if (strcmp(value, "や") == 0)
    {
        ret.x = 0;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "ゆ") == 0)
    {
        ret.x = 16;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "よ") == 0)
    {
        ret.x = 32;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "゛") == 0)
    {
        ret.x = 48;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "゜") == 0)
    {
        ret.x = 64;
        ret.y = 112;
        return ret;
    }
    if (strcmp(value, "ら") == 0)
    {
        ret.x = 0;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "り") == 0)
    {
        ret.x = 16;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "る") == 0)
    {
        ret.x = 32;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "れ") == 0)
    {
        ret.x = 48;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "ろ") == 0)
    {
        ret.x = 64;
        ret.y = 128;
        return ret;
    }
    if (strcmp(value, "わ") == 0)
    {
        ret.x = 0;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "を") == 0)
    {
        ret.x = 16;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "ん") == 0)
    {
        ret.x = 32;
        ret.y = 144;
        return ret;
    }
    if (strcmp(value, "　") == 0)
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
    if (strcmp(value, "！") == 0)
    {
        ret.x = 64;
        ret.y = 160;
        return ret;
    }
    if (strcmp(value, "「") == 0)
    {
        ret.x = 0;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "」") == 0)
    {
        ret.x = 16;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "：") == 0)
    {
        ret.x = 32;
        ret.y = 176;
        return ret;
    }
    if (strcmp(value, "ー") == 0)
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

