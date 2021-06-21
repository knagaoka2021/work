//=============================================================================
//
// ÉQÅ[ÉÄ [game.cpp]
// Author : í∑â™òaê^
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "Score.h"
#include "fade.h"
#include "sound.h"

static LPDIRECT3DVERTEXBUFFER9 g_pD3DBGVtxBuff = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBG = NULL;
static LPDIRECT3DTEXTURE9 g_pD3DBGTex = NULL;
static VERTEX_2D* pVtx;
static float uu = 0;
static float vv = 0;
static int flag = 0;

#define SPEED (0.01f);

HRESULT InitGame(void)
{

    PlaySound(SOUND_LABEL_BGM003, XAUDIO2_LOOP_INFINITE);

    return S_OK;
}
void UpdateGame(void)
{
    if (tringkeyboardPress(DIK_P))
    {
        SetPauseflag(true);
        PlaySound(SOUND_LABEL_SE_LOCKON, 0);
    }

    if (flag == 1 && GetFade() != FADE_NONE)
    {
        SetFade(FADE_IN);
        flag = 0;
    }

    if (GetFade() == FADE_NONE)
    {
        SetMode(MODE_RESULT);
        SetFade(FADE_OUT);
    }

}
void DrawGame(void)
{
}
void UninitGame(void)
{
    StopSound(SOUND_LABEL_BGM003);

    flag = 0;
}
void flag_on(void)
{
    flag = 1;
}