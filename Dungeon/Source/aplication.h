#ifndef __APLICATION_H__
#define __APLICATION_H__

//==========================================-
//	インクルード
//==========================================-
#include "main.h"
#include "consoleBuffer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define     DEF_HEADDER_FILE    "Data/Wall/BMP_HEADDER.txt"
#define     DEF_FILE_MAX    (16)
#define buffMove( buf, count )  count += strlen(buf+count) + 1
// デフォルト値
#define     ON     ( 1 )
#define     OFF    ( 0 )
#define     OK     ( 1 )
#define     NG     (-1 )
#define SCREEN_WIDTH  (512)
#define SCREEN_HEIGHT (256)
#define FONT_SIZE  (16)

//==========================================-
//	列挙体
//==========================================-
enum					//アプリケーションモード
{
	APLICATIONMODE_NONE = -1
	,APLICATIONMODE_TITLE		// タイトル
    ,APLICATIONMODE_GAME		// ゲーム本編
    ,APLICATIONMODE_RESULT		// リザルト
    ,APLICATIONMODE_MAX			//総数
};

// 方向定義
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

// 画像のテキストデータ管理用構造体
typedef struct _IMAGEBUFFER
{
    int     point;      // 読み出し位置
    int     datSize;    // バッファサイズ
    char* buf;       // バッファポインタ

} IMAGEBUFFER;

//==========================================-
//	プロトタイプ宣言
//==========================================-
//=============================
//	モード取得関数
//引数：無
//戻値：int		現在のモード
//=============================
int GetAppMode(void);

//=============================
//	モード設定関数
//引数：int	iMode  次回のモード
//戻値：無
//=============================
void SetAppMode(int iMode);

//=============================
//	アプリケーション全体の初期化
//引数：無
//戻値：無
//=============================
int AppInit(void);

//=============================
//	アプリケーション全体の終了処理
//引数：無
//戻値：無
//=============================
void AppUnInit(void);

//=============================
//	アプリケーション全体の更新処理
//引数：無
//戻値：無
//=============================
void AppUpdate(void);

//=============================
//	アプリケーション全体の描画処理
//引数：無
//戻値：無
//=============================
void AppDraw(void);

//=============================
//	ウィンドウサイズ拡大
//引数：hCOut ハンドル,dwSize ウィンドウバッファサイズ,consoleWindow ウィンドウサイズ
//戻値：bool 結果
//=============================
BOOL setWindowSizeBig(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow);

//=============================
//	ウィンドウサイズ縮小
//引数：hCOut ハンドル,dwSize ウィンドウバッファサイズ,consoleWindow ウィンドウサイズ
//戻値：bool 結果
//=============================
BOOL setWindowSizeSmall(HANDLE hCOut, COORD dwSize, SMALL_RECT consoleWindow);

//=============================
//	パレット書き込え処理
//引数：cr 書き込むパレットデータのアドレス
//戻値：int 結果
//=============================
int SetConsolePalatte(COLORREF* cr);

//=============================
//	画像テキストヘッダーファイル読み込み処理
//引数：readFileName ファイル名
//戻値：bool 結果
//=============================
BOOL imageHeadderDataLoad(char* readFileName);

//=============================
//	画像テキストファイル読み込み処理
//引数：readFileName ファイル名,consolImageData 画像テキスト
//戻値：bool 結果
//=============================
BOOL imageDataLoad(const char* readFileName, CIMAGEDATA* consolImageData);

//=============================
//	方向回転による描画処理
//引数：direction 方向,value 向き先
//戻値：DIRECTION 方向
//=============================
DIRECTION changeDirection(DIRECTION direction, int value);

//=============================
//	テキストフォントの描画処理
//引数：target 文字列
//戻値：VECTOR2 フォント座標
//=============================
VECTOR2 GetFont(const char* target);

//=============================
//	ファイルデータ読み込み
//引数：fileName ファイル名,ib 画像バッファ
//戻値：int　結果
//=============================
int ImageFileRead(const char* fileName, IMAGEBUFFER* ib);

//=============================
//	画像サイズ設定
//引数：ib 画像バッファ,cid 画像テキスト
//戻値：int　結果
//=============================
int ChangeImageSize(IMAGEBUFFER* ib, CIMAGEDATA* cid);

//=============================
//	画像データ設定
//引数：ib 画像バッファ,cid 画像テキスト
//戻値：int　結果
//=============================
int ChangeImage(IMAGEBUFFER* ib, CIMAGEDATA* cid);

//=============================
//	パレットデータ設定
//引数：ib 画像バッファ,cid 画像ヘッダー
//戻値：int　結果
//=============================
int ChangePalette(IMAGEBUFFER* ib, CONSOLIMAGEDATA* cid);


#endif//__APLICATION_H__