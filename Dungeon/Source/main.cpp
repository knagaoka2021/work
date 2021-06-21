//============================================================================*
// [CScreen.h] Win32 console メイン
// @version 1.0
// @author kazuma.nagaoka
//============================================================================*
#include "main.h"
#include "aplication.h"


//=============================================================================
// メイン関数
//=============================================================================
int main(int argc, char* argv[])
{

    int iTime, iTimeOld;

    timeBeginPeriod(1);
    iTimeOld = timeGetTime();

    //初期化
    AppInit();

    //メインループ
    while (1)
    {
        iTime = timeGetTime();
        //フレームレートの固定化
        if (FRAMERATE <= iTime - iTimeOld)
        {
            iTimeOld = iTime;

            //更新処理
            AppUpdate();
            //描画処理
            AppDraw();

        }

    }
    //終了処理
    AppUnInit();
    //タイマーの精度を元に戻す
    timeEndPeriod(1);

    return 0;
}