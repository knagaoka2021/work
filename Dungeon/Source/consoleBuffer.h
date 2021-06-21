#ifndef __CONSOLEBUFFER_H__
#define __CONSOLEBUFFER_H__

// コンソールバッファ管理用構造体
typedef struct _CONSOLIMAGEDATA
{
    int         imgWidth, imgHight;         // 画像サイズ

    int         bufferSizeX, bufferSizeY;   // コンソールバッファサイズ
    int         screenSizeX, screenSizeY;   // コンソール画面サイズ

    int         paletteSize;                // パレット数
    COLORREF    palette[16];                // パレットデータ

    CHAR_INFO* pImg;                      // 画像データ（文字と色）

} CONSOLIMAGEDATA;

typedef struct _CIMAGEDATA
{
    int         imgWidth, imgHight;         // 画像サイズ

    CHAR_INFO* pImg;                      // 画像データ（文字と色）

} CIMAGEDATA;
//==========================================-
//	プロトタイプ宣言
//==========================================-
//=============================
//	スクリーンバッファ初期化
//引数：cid コンソールバッファ,screenW スクリーン幅,screenH スクリーン高さ,colorNo カラー番号
//戻値：int 結果
//=============================
int screenInit(CONSOLIMAGEDATA* cid, int screenW, int screenH, int colorNo);

//=============================
//	メインコンソールバッファ取得
//引数：無
//戻値：CONSOLIMAGEDATA コンソールバッファ
//=============================
CONSOLIMAGEDATA* GetConsolMainBufferInfo(void);

//=============================
//	サブコンソールバッファ取得
//引数：無
//戻値：CONSOLIMAGEDATA コンソールバッファ
//=============================
CONSOLIMAGEDATA* GetConsolSubBufferInfo(void);

//=============================
//	画像データリスト取得
//引数：無
//戻値：CIMAGEDATA 画像テキスト
//=============================
CIMAGEDATA* GetConsolImageDataList(void);

//=============================
//	テキストフォント取得
//引数：無
//戻値：CIMAGEDATA テキストフォント
//=============================
CIMAGEDATA* GetConsolFontImageInfo(void);

//=============================
//	スクリーンバッファクリア
//引数：cid コンソールバッファ,colorNo カラー番号
//戻値：int 結果
//=============================
int screenBufferClear(CONSOLIMAGEDATA* cid, int colorNo);
//------------------------------------------------
// コンソール画面描画
// 返り値       int         true:正常  false:エラー
// 第１引数     int size    書き込むデータサイズ
//                          ※コンソールウィンドウのバッファサイズと同じサイズにしないと正常に動作しない
//                            一部分だけ書き込むようにはできていない
// 第２引数     int *string 書き込むデータ
//------------------------------------------------
int SetConsoleStringAttributesBuffer(int posX, int posY, int width, int Hight, CHAR_INFO* pImage);
//------------------------------------------------
// コンソール画面描画
// 返り値       int         true:正常  false:エラー
// 第１引数     int         posX    書き込む位置 X
// 第２引数     int         posY    書き込み位置 Y
// 第３引数     int         width   書き込む幅
// 第４引数     int         hight   書き込む高さ
// 第５引数     CHAR_INFO   *pImage 書き込むデータのアドレス
//------------------------------------------------
int SetConsoleStringAttributesBuffer2(int posX, int posY, int width, int hight, CHAR_INFO* pImage);

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
int SetConsoleStringAttributesBufferCustom(int posX, int posY, int width, int hight, int windowX, int windowY, CHAR_INFO* pImage);

//=============================
//	テキストフォントバッファ更新
//引数：cid コンソールバッファ,obj テキストフォント,x 更新位置X,y 更新位置Y
//戻値：int 結果
//=============================
void updateFontBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y, const char* message);

//=============================
//	画像データバッファ更新
//引数：cid コンソールバッファ,obj 画像データ,x 更新位置X,y 更新位置Y
//戻値：int 結果
//=============================
void updateBufferObject(CONSOLIMAGEDATA* cid, CIMAGEDATA* obj, int x, int y);

//=============================
//	スクリーンバッファ更新
//引数：cid コンソールバッファ,x 更新位置X,y 更新位置Y,colorAttributes カラーデータ
//戻値：int 結果
//=============================
void update(CONSOLIMAGEDATA* cid, int x, int y, WORD colorAttributes);

//=============================
//	コンソール初期化
//引数：無
//戻値：無
//=============================
void ConsoleBufferInit(void);

//=============================
//	コンソール修了
//引数：無
//戻値：無
//=============================
void ConsoleBufferUnInit(void);

#endif//__CONSOLEBUFFER_H__