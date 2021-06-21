//==========================================-
//	インクルード
//==========================================-
#include "aplication.h"
#include "consoleBuffer.h"
#include "map.h"


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
MAPDATA mapData;

//=============================
//	マップの初期化処理
//引数：無
//戻値：無
//=============================
void MapInit(void)
{
    MapFileRead("Data/Map/map.txt", &mapData);
}

//=============================
//	マップの終了処理
//引数：無
//戻値：無
//=============================
void MapUnInit(void)
{
    if (mapData.mapData != NULL)
    {
        free(mapData.mapData);
        mapData.mapData = NULL;
    }
}

//=============================
//	マップデータ取得
//引数：無
//戻値：MAPDATA　マップデータ
//=============================
MAPDATA* GeMapInfo(void)
{
    return(&mapData);
}

//=============================
//	前マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapFront(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (0 <= y - 3 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 3, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (0 <= y - 3 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 3, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (0 <= y - 3) {
            if (checkMapData(&mapData, x, y - 3, '1')) {
                ret = true;
            }
        }
        break;

    case 3:
        if (0 <= y - 2 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 2, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (0 <= y - 2 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 2, '1')) {
                ret = true;
            }
        }

        break;
    case 5:
        if (0 <= y - 2) {
            if (checkMapData(&mapData, x, y - 2, '1')) {
                ret = true;
            }
        }
        break;

    case 6:
        if (0 <= y - 1 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (0 <= y - 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (0 <= y - 1) {
            if (checkMapData(&mapData, x, y - 1, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	右マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapRight(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (0 <= y - 1 && mapData.mapWidth > x + 3) {
            if (checkMapData(&mapData, x + 3, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 3) {
            if (checkMapData(&mapData, x + 3, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (mapData.mapWidth > x + 3) {
            if (checkMapData(&mapData, x + 3, y, '1')) {
                ret = true;
            }
        }
        break;
    case 3:
        if (0 <= y - 1 && mapData.mapWidth > x + 2) {
            if (checkMapData(&mapData, x + 2, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 2) {}
        if (checkMapData(&mapData, x + 2, y + 1, '1')) {
            ret = true;
        }
        break;
    case 5:
        if (mapData.mapWidth > x + 2) {
            if (checkMapData(&mapData, x + 2, y, '1')) {
                ret = true;
            }
        }
        break;
    case 6:
        if (0 <= y - 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	左マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapLeft(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x - 3) {
            if (checkMapData(&mapData, x - 3, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (0 <= y - 1 && 0 <= x - 3) {
            if (checkMapData(&mapData, x - 3, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (0 <= x - 3) {
            if (checkMapData(&mapData, x - 3, y, '1')) {
                ret = true;
            }
        }
        break;
    case 3:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x - 2) {
            if (checkMapData(&mapData, x - 2, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (0 <= y - 1 && 0 <= x - 2) {}
        if (checkMapData(&mapData, x - 2, y - 1, '1')) {
            ret = true;
        }
        break;
    case 5:
        if (0 <= x - 2) {
            if (checkMapData(&mapData, x - 2, y, '1')) {
                ret = true;
            }
        }
        break;
    case 6:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x - 1) {
            if (checkMapData(&mapData, x - 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (0 <= y - 1 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y - 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	後マップデータ判定
//引数：no, 壁配置パターン,x 配置座標X,y 配置座標Y
//戻値：BOOL　結果
//=============================
BOOL checkMapBack(int no, int x, int y)
{
    BOOL ret = false;

    switch (no) {
    case 0:
        if (mapData.mapHight > y + 3 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 3, '1')) {
                ret = true;
            }
        }
        break;
    case 1:
        if (mapData.mapHight > y + 3 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y + 3, '1')) {
                ret = true;
            }
        }
        break;
    case 2:
        if (mapData.mapHight > y + 3) {
            if (checkMapData(&mapData, x, y + 3, '1')) {
                ret = true;
            }
        }
        break;

    case 3:
        if (mapData.mapHight > y + 2 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 2, '1')) {
                ret = true;
            }
        }
        break;
    case 4:
        if (mapData.mapHight > y + 2 && 0 <= x - 1) {}
        if (checkMapData(&mapData, x - 1, y + 2, '1')) {
            ret = true;
        }
        break;
    case 5:
        if (mapData.mapHight > y + 2) {
            if (checkMapData(&mapData, x, y + 2, '1')) {
                ret = true;
            }
        }
        break;

    case 6:
        if (mapData.mapHight > y + 1 && mapData.mapWidth > x + 1) {
            if (checkMapData(&mapData, x + 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 7:
        if (mapData.mapHight > y + 1 && 0 <= x - 1) {
            if (checkMapData(&mapData, x - 1, y + 1, '1')) {
                ret = true;
            }
        }
        break;
    case 8:
        if (mapData.mapHight > y + 1) {
            if (checkMapData(&mapData, x, y + 1, '1')) {
                ret = true;
            }
        }
        break;

    }

    return ret;
}

//=============================
//	マップ更新処理
//引数：playerプレイヤー情報
//戻値：無
//=============================
void updateMap(PLAYER* player)
{
    CONSOLIMAGEDATA* pConsolMainBuffer = GetConsolMainBufferInfo();
    CIMAGEDATA* pConsolImageDataList = GetConsolImageDataList();

    screenBufferClear(pConsolMainBuffer, 0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (checkMapDirection((3 * i) + j, player)) {
                CIMAGEDATA* pWork = (pConsolImageDataList + (3 * i) + j);
                updateBufferObject(pConsolMainBuffer, pWork, 0, 0);
            }
        }
    }

    // スクリーン描画
    SetConsoleStringAttributesBuffer(0, 0, pConsolMainBuffer->screenSizeX, pConsolMainBuffer->screenSizeY, pConsolMainBuffer->pImg);
}

//=============================
//	方向判定処理
//引数：no マップデータ,playerプレイヤー情報
//戻値：BOOL 結果
//=============================
BOOL checkMapDirection(int no, PLAYER* player)
{
    BOOL ret = false;

    switch (player->direction)
    {
    case DIRECTION_FRONT:
        ret = checkMapFront(no, player->posX, player->posY);
        break;
    case DIRECTION_RIGHT:
        ret = checkMapRight(no, player->posX, player->posY);
        break;
    case DIRECTION_BACK:
        ret = checkMapBack(no, player->posX, player->posY);
        break;
    case DIRECTION_LEFT:
        ret = checkMapLeft(no, player->posX, player->posY);
        break;
    }

    return ret;
}

//=============================
//	壁判定処理
//引数：md マップデータ,x 位置座標X,y 位置座標Y
//戻値：BOOL 結果
//=============================
BOOL checkMapWallInto(MAPDATA* md, int x, int y)
{
    return checkMapData(md, x, y, '1');
}
int MapFileRead(const char* fileName, MAPDATA* md)
{
    FILE* fp;    // ファイルポインタ
    int     fsize;  // ファイルサイズ

    char* buf;   // データ領域

    int pcount = 0;


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
        if (*(buf + i) == 0x0A || *(buf + i) == ',')
        {
            *(buf + i) = 0x00;
        }
    }

    int         mapWidth, mapHight;         // マップサイズ
    int         initPosX, initPosY;         // 初期座標
    int         goalPosX, goalPosY;         // マップゴール座標
    char* map;
    char* mapWork;

    // マップサイズ読み込み
    sscanf(buf + pcount, "%d", &mapWidth);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &mapHight);
    buffMove(buf, pcount);

    pcount++;       // 改行スキップ

    // マップ初期座標読み込み
    sscanf(buf + pcount, "%d", &initPosX);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &initPosY);
    buffMove(buf, pcount);

    pcount++;       // 改行スキップ

    // マップゴール座標読み込み
    sscanf(buf + pcount, "%d", &goalPosX);
    buffMove(buf, pcount);
    sscanf(buf + pcount, "%d", &goalPosY);
    buffMove(buf, pcount);

    pcount++;       // 改行スキップ

        // 領域確保
    map = (char*)malloc(mapWidth * mapHight * sizeof(char));
    if (map == NULL)
    {
        printf("領域確保失敗");
        return NG;
    }

    // データ変換
    for (int loopY = 0; loopY < mapHight; loopY++)
    {
        for (int loopX = 0; loopX < mapWidth; loopX++)
        {

            // アドレス計算
            mapWork = map + (loopY * mapHight) + loopX;

            sscanf(buf + pcount, "%c", mapWork);
            buffMove(buf, pcount);

        }

        pcount++;  // 改行コードスキップ
    }

    // バッファセット
    md->mapData = map;

    md->mapWidth = mapWidth;
    md->mapHight = mapHight;
    md->initPosX = initPosX;
    md->initPosY = initPosY;
    md->goalPosX = goalPosX;
    md->goalPosY = goalPosY;

    if (buf != NULL)
    {
        free(buf);
        buf = NULL;
    }


    return  OK;
}
//=============================
//	マップ判定処理
//引数：md マップデータ,x 位置座標X,y 位置座標Y,target 壁フラグ
//戻値：BOOL 結果
//=============================
BOOL checkMapData(MAPDATA* md, int x, int y, char target)
{
    BOOL ret = false;
    char* work = md->mapData;

    if (work[(y * md->mapHight) + x] == target) {
        ret = true;
    }


    return ret;
}
//=============================
//	プレイヤー移動による描画処理
//引数：md マップデータ,playerプレイヤー情報,sign 符号
//戻値：DIRECTION 方向
//=============================
PLAYER* moveDirection(MAPDATA* md, PLAYER* player, int sign)
{
    int x = player->posX;
    int y = player->posY;
    DIRECTION direction = player->direction;


    switch (direction)
    {
    case DIRECTION_FRONT:
        y = y - (1 * sign);
        break;
    case DIRECTION_RIGHT:
        x = x + (1 * sign);
        break;
    case DIRECTION_BACK:
        direction = DIRECTION_BACK;
        y = y + (1 * sign);
        break;
    case DIRECTION_LEFT:
        x = x - (1 * sign);
        break;
    }

    // 移動制限
    if (x >= md->mapWidth - 1) {
        x = md->mapWidth - 1;
    }
    if (x < 0) {
        x = 0;
    }
    if (y >= md->mapHight - 1) {
        y = md->mapHight - 1;
    }
    if (y < 0) {
        y = 0;
    }

    // 壁判定
    if (!checkMapWallInto(md, x, y))
    {
        player->posX = x;
        player->posY = y;
    }

    return player;
}
