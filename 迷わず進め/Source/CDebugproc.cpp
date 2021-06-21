//=============================================================================
//
// デバッグ表示処理 [debugproc.cpp]
// Author : 長岡 和真
//
//=============================================================================
#include <stdio.h>
#include "CDebugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void Put_Char( int c );
static void Format_D( char* buf, int val );				// %d
static void Format_F( char* buf, double val );			// %f
static void Format_X( char* buf, int val );				// %x
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXFONT			g_pD3DXDebugFont = NULL;		// フォントへのポインタ
char g_aStrDebug[1024];
char workbuf[1024];

CDebugproc::CDebugproc(void)
{
}
CDebugproc::~CDebugproc(void)
{
}
//=============================================================================
// デバッグ表示処理の初期化
//=============================================================================
HRESULT CDebugproc::InitDebugProc(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	if(FAILED(D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal",&g_pD3DXDebugFont)))
	{
		return E_FAIL;
	}

	memset(g_aStrDebug,NULL,sizeof(g_aStrDebug));

	return S_OK;
}

//=============================================================================
// デバッグ表示処理の終了処理
//=============================================================================
void CDebugproc::UninitDebugProc(void)
{
	if(g_pD3DXDebugFont != NULL)
	{// 情報表示用フォントの開放
		g_pD3DXDebugFont->Release();
		g_pD3DXDebugFont = NULL;
	}
}

//=============================================================================
// デバッグ表示処理の更新処理
//=============================================================================
void CDebugproc::UpdateDebugProc(void)
{
}

//=============================================================================
// デバッグ表示処理の描画処理
//=============================================================================
void CDebugproc::DrawDebugProc(int x,int y)
{

	RECT rect = {x, y, SCREEN_WIDTH, SCREEN_HEIGHT};

	// テキスト描画
	g_pD3DXDebugFont->DrawText(NULL, g_aStrDebug, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	memset(g_aStrDebug,NULL,sizeof(g_aStrDebug));
}

void Put_Char( int c )
{
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';

	strcat(g_aStrDebug,buf);
	//wsprintf(g_aStrDebug,"AAA");
}




// Helper function for %x format in Print() function.
static void Format_X( char* buf, int val )
{
    int i;

    for ( i = 28; i >= 0; i -= 4 )
	{
		int x = (val >> i) & 0xf;

		*buf++ = "0123456789abcdef"[x];
    }
    *buf = '\0';
}
// Helper function for %x format in Print() function.
static void Format_F( char* buf, double val )
{
    char stack[16];
	char stack2[16];
    int top = 0;
	static int work=0;
    bool mynus;
    float  uval;
	double  wo=0;
	int  wor;
	int zxc=0;



    if ( val < 0 ) 
	{
		mynus = TRUE;
		uval = -val;
		zxc = -val;
    }
    else
	{
		mynus = FALSE;
		uval = val;
		zxc = val;
    }


	wo = uval-zxc;
	wor = wo*1000+1;
	wor /=10;
    // Convert magnitude to decimal.  We do this in order of least
    // significant to most significant digit.
    do 
	{
		int digit = zxc % 10;
		stack[top++] = digit + '0';
		zxc /= 10;

    }
    while ( zxc > 0 );

    // Add leading minus sign if negative.
		if ( mynus )
		{
			*(buf++) = '-';
		}

    // Put formatted characters in the output buffer.
    do 
	{
		*(buf++) = stack[--top];
	}
    while ( top > 0 );

	*(buf++) = '.';

    do 
	{
		int digit = wor % 10;
		stack2[top++] = digit + '0';
		wor /= 10;

    }
    while ( wor > 0 );

    // Put formatted characters in the output buffer.
    do 
	{
		*(buf++) = stack2[--top];
	}
    while ( top > 0 );

    // Terminate the output buffer.
    *buf = '\0';

}

// Helper for %d format in Print() function.
static void Format_D( char* buf, int val )
{
    char stack[16];
    int top = 0;
	static int work=0;
    bool mynus;
    unsigned uval;

    // Convert to sign and unsigned magnitude.
    if ( val < 0 ) 
	{
		mynus = TRUE;
		uval = -val;
    }
    else
	{
		mynus = FALSE;
		uval = val;
    }

    // Convert magnitude to decimal.  We do this in order of least
    // significant to most significant digit.
    do 
	{
		int digit = uval % 10;
		stack[top++] = digit + '0';
		uval /= 10;
    }
    while ( uval > 0 );

    // Add leading minus sign if negative.
		if ( mynus )
		{
			*(buf++) = '-';
		}

    // Put formatted characters in the output buffer.
    do 
	{
		*(buf++) = stack[--top];
	}
    while ( top > 0 );

    // Terminate the output buffer.
    *buf = '\0';
}
//=============================================================================
// デバッグ表示の登録
//=============================================================================
void CDebugproc::PrintDebugProc(const char *fmt,...)
{

    va_list args;
    int ival;
	double ival2;
    const char* sval;

    va_start( args, fmt );

    while ( *fmt != '\0' ) 
	{
		switch ( *fmt )
		{
			case '%':
			++fmt;
			switch ( *fmt )
			{
				case 'd':
				ival = va_arg( args, int );
				//g_aStrDebug = ival;
				Format_D( workbuf , ival );
				strcat(g_aStrDebug,workbuf);

				//Print_String( buf );
				break;
				case 'f':
				ival2 = va_arg( args, double );
				Format_F( workbuf, ival2 );
				strcat(g_aStrDebug,workbuf);
				//Print_String( buf );
				break;

				case 'x':
				ival = va_arg( args, int );
				Format_X( workbuf, ival );
				strcat(g_aStrDebug,workbuf);
				//Print_String( buf );
				break;

				case 's':
				sval = va_arg( args, const char* );
				//Print_String( sval );
				break;

				case 'c':
				ival = va_arg( args, int );
				//Put_Char( ival & 0xff );
				break;

				default:
				//Put_Char( *fmt );
				break;
			}
				break;

			default:
				Put_Char( *fmt );
				//wsprintf(g_aStrDebug,fmt,args);
				break;
		}

		++fmt;
    }
    va_end( args );






  //va_start( arglist, fmt );

  //vprintf(fmt,arglist);

  //  va_end( arglist );

	//wsprintf(g_aStrDebug, fmt);
}
