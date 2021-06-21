//=============================================================================
//
//  [common.h]
// Author : 長岡和真
//
//=============================================================================
#ifndef	_COMMON_H
#define _COMMON_H
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RADIUS (30.0f)
#define WALL_SIZE (1800.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	FADE_NONE =0,
	FADE_IN,
	FADE_INEND,
	FADE_OUT,
	FADE_OUTEND,
	FADE_MAX
}MODE_FADE;

typedef enum
{
	MOTION_WAIT = 0,
	MOTION_RUN,
	MOTION_JAMP,
	MOTION_PANCH,
	MOTION_JUMP_PANCH,
	MOTION_DAMAGE,
	MOTION_DOWN,
	MOTION_NONE,
	MOTION_MAX,
}MOTIONTYPE;

struct OFSET
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float rotCheck(float rot);
D3DXVECTOR3* commonCross(D3DXVECTOR3* pOut, const D3DXVECTOR3* p0, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2);
float Invers(float rot);
bool MarginalDomainWallHitCheck(D3DXVECTOR3 pos);
bool CollsionSphereChek(D3DXVECTOR3 objA,D3DXVECTOR3 objB,float rA,float rB);
float GetMyShipAngle(D3DXVECTOR3 objA, D3DXVECTOR3 objB);
void Vector3Transform( D3DXVECTOR3 *Out, D3DXVECTOR3 *Vector, D3DXMATRIX *Matrix );
float lerp(float a,float b,float per);
D3DXVECTOR3 lerpVec3(D3DXVECTOR3 a,D3DXVECTOR3 b,float per);
int CheckCollision( const D3DXVECTOR3 *pMin1, const D3DXVECTOR3 *pMax1, const D3DXMATRIX *pMat1, const D3DXVECTOR3 *pMin2, const D3DXVECTOR3 *pMax2, const D3DXMATRIX  *pMat2);
void CreateTex(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName);
LPDIRECT3DTEXTURE9 GetTexture(void);
void TexUninit(void);
void RotationAreaCheck(D3DXVECTOR3 *camera,D3DXVECTOR3 *model);
float Vct3LengthPow2( const D3DXVECTOR3 &sv );
void copyBuf( unsigned sz, void *src, LPDIRECT3DVERTEXBUFFER9 buf );
D3DXMATRIX ConVertMatrix(LPD3DXMATRIX LpMatrix);
void SetDebug(bool value);
bool GetDebug(void);
int GetRandom(int min, int max);
//	|sv|
float Vct3Length( const D3DXVECTOR3 &sv );

//	1/(|sv|*|sv|)
float Vct3LengthPow2Rev( const D3DXVECTOR3 &sv );

//	1/|sv|
float Vct3LengthRev( const D3DXVECTOR3 &sv );

//	ov = sv * f
void Vct3MulScl( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv, const float f );

//	ov=正規化(sv)
void Vct3Normalize( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv );

//	ov=正規化(ov)
void Vct3Normalize( D3DXVECTOR3 &osv );

//	内積(sv1・sv2)
float Vct3Dot( const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 );

//	外積(sv1ｘsv2)
void Vct3Cross( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 );

//	ov = sv1 - sv2
void Vct3Sub( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 );
class OBB
{
public:
   D3DXVECTOR3 m_Pos;              // 位置
   D3DXVECTOR3 m_NormaDirect[3];   // 方向ベクトル
   FLOAT m_fLength[3];             // 各軸方向の長さ


   D3DXVECTOR3 GetDirect( int elem );   // 指定軸番号の方向ベクトルを取得
   FLOAT GetLen_W( int elem );          // 指定軸方向の長さを取得
   D3DXVECTOR3 GetPos_W();             // 位置を取得
};
bool ColOBBs( OBB &obb1, OBB &obb2 );
FLOAT LenSegOnSeparateAxis( D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0 );
class PLANE
{
public:
	D3DXVECTOR3 m_LocalNormal; // ローカル座標での法線ベクトル
   D3DXVECTOR3 m_Pos;         // 平面の位置
   // ローカル座標での法線ベクトルを設定
   bool SetNormal_L( D3DXVECTOR3 *Norm );
   // ローカル座標での法線ベクトルを取得
   void GetNormal_L( D3DXVECTOR3 *Norm );
   // ワールド座標での法線ベクトルを取得
   void GetNormal_W( D3DXVECTOR3 *Norm );
   // ローカル位置を設定
   void SetPos_L( FLOAT x, FLOAT y, FLOAT z);
   void SetPos_L( D3DXVECTOR3 *Pos );
   // ローカル座標位置を取得
   D3DXVECTOR3 GetPos_L();
   // ワールド座標位置を取得
   D3DXVECTOR3 GetPos_W();
};
bool OBBvsPlane( OBB &obb, PLANE &plane, FLOAT *Len=NULL );
#endif
// EOF