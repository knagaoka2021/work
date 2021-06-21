#ifndef __MAP_H__
#define __MAP_H__

//==========================================-
//	インクルード
//==========================================-
#include "player.h"

typedef struct _MAPDATA
{
    int         mapWidth, mapHight;         // マップサイズ
    int         initPosX, initPosY;         // 初期座標
    int         goalPosX, goalPosY;         // マップゴール座標
    char* mapData;

} MAPDATA;
//==========================================-
//	プロトタイプ宣言
//==========================================-

//=============================
//	マップの初期化処理
//引数：無
//戻値：無
//=============================
void MapInit(void);

//=============================
//	マップの終了処理
//引数：無
//戻値：無
//=============================
void MapUnInit(void);

//=============================
//	マップデータ取得
//引数：無
//戻値：MAPDATA　マップデータ
//=============================
MAPDATA* GeMapInfo(void);

//=============================
//	前マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapFront(int no, int x, int y);

//=============================
//	右マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapRight(int no, int x, int y);
//=============================
//	左マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapLeft(int no, int x, int y);

//=============================
//	後マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapBack(int no, int x, int y);

//=============================
//	マップ更新処理
//引数：playerプレイヤー情報
//戻値：無
//=============================
void updateMap(PLAYER* player);

//=============================
//	方向判定処理
//引数：no マップデータ,playerプレイヤー情報
//戻値：BOOL 結果
//=============================
BOOL checkMapDirection(int no, PLAYER* player);
//=============================
//	プレイヤー移動による描画処理
//引数：md マップデータ,playerプレイヤー情報,sign 符号
//戻値：DIRECTION 方向
//=============================
PLAYER* moveDirection(MAPDATA* md, PLAYER* player, int sign);

//=============================
//	壁判定処理
//引数：md マップデータ,x 位置座標X,y 位置座標Y
//戻値：BOOL 結果
//=============================
BOOL checkMapWallInto(MAPDATA* md, int x, int y);

int MapFileRead(const char* fileName, MAPDATA* md);
//=============================
//	マップ判定処理
//引数：md マップデータ,x 位置座標X,y 位置座標Y,target 壁フラグ
//戻値：BOOL 結果
//=============================
BOOL checkMapData(MAPDATA* md, int x, int y, char target);

#endif//__MAP_H__