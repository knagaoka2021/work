//==========================================-
//	インクルード
//==========================================-
#include "player.h"
#include "map.h"

//==========================================-
//	変数宣言
//==========================================-
static PLAYER player;	//プレーヤー情報

//=============================
//	プレーヤー情報取得関数
//引数：無
//戻値：PLAYER*		プレーヤー情報ポインタ
//=============================
PLAYER* GetPlayerInfo(void)
{
	return(&player);
}

//=============================
//	プレーヤーの初期化
//引数：無
//戻値：無
//=============================
void PlayerInit(void)
{
    // マップデータ取得
    MAPDATA* pMapData = GeMapInfo();

    // プレイヤー初期化
    player.posX = pMapData->initPosX;
    player.posY = pMapData->initPosY;
    player.direction = DIRECTION_FRONT;
}

//=============================
//	プレーヤーの終了処理
//引数：無
//戻値：無
//=============================
void PlayerUnInit(void)
{
    // マップデータ取得
    MAPDATA* pMapData = GeMapInfo();

    // プレイヤー初期化
    player.posX = pMapData->initPosX;
    player.posY = pMapData->initPosY;
    player.direction = DIRECTION_FRONT;
}

//=============================
//	プレーヤーの更新処理
//引数：無
//戻値：無
//=============================
void PlayerUpdate(void)
{
    PLAYER* pPlayer = GetPlayerInfo();
    MAPDATA* pMap = GeMapInfo();

    // 入力処理
    if (INP(PK_UP))    // [↑]の押下状態を取得する。
    {
        pPlayer = moveDirection(pMap, pPlayer, 1);
        updateMap(pPlayer);

    }
    if (INP(PK_RIGHT))    // [→]の押下状態を取得する。
    {
        pPlayer->direction = changeDirection(pPlayer->direction, 1);
        updateMap(pPlayer);
    }
    if (INP(PK_LEFT))    // [←]の押下状態を取得する。
    {
        pPlayer->direction = changeDirection(pPlayer->direction, -1);
        updateMap(pPlayer);
    }
    if (INP(PK_DOWN))    // [↓]の押下状態を取得する。
    {
        pPlayer = moveDirection(pMap, pPlayer, -1);
        updateMap(pPlayer);
    }
}