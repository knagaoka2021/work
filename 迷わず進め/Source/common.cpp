//=============================================================================
// 自作関数など
//  [common.cpp]
// Author : 長岡和真
//
//=============================================================================

#include "common.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 m_pD3DTex = NULL;
bool g_Debug = FALSE;
//=============================================================================
// 向き調整
//=============================================================================
float rotCheck(float rot)
{
	if(D3DX_PI<rot)
	{
		rot-=D3DX_PI*2;
	}
	if(-D3DX_PI>=rot)
	{
		rot+=D3DX_PI*2;
	}

	return rot;

}
//=============================================================================
// 2Dベクトル外積
//=============================================================================
D3DXVECTOR3* commonCross(D3DXVECTOR3* pOut, const D3DXVECTOR3* p0, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2)
{
	D3DXVECTOR3 u = *p1 - *p0;
	D3DXVECTOR3 v = *p2 - *p0;

	D3DXVec3Cross(pOut, &u, &v);
	D3DXVec3Normalize(pOut, pOut);

	return pOut;
}
//=============================================================================
// 正規化？
//=============================================================================
float Invers(float rot)
{
	if(D3DX_PI<rot)
	{
		rot-=D3DX_PI*2;
	}
	if(-D3DX_PI>rot)
	{
		rot+=D3DX_PI*2;
	}

	return rot;
}
//=============================================================================
// 限界領域判定
//=============================================================================
bool MarginalDomainWallHitCheck(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vecLine,vecToTarget;
	D3DXVECTOR3 posWall[4] ={D3DXVECTOR3(-WALL_SIZE,0.0f,WALL_SIZE),D3DXVECTOR3(WALL_SIZE,0.0f,WALL_SIZE),D3DXVECTOR3(WALL_SIZE,0.0f,-WALL_SIZE),D3DXVECTOR3(-WALL_SIZE,0.0f,-WALL_SIZE)};
	int m_work[4]={1,2,3,0};
	float cross=0.0f;


	for(int i=0;i<4;i++)
	{
		vecLine = (posWall[m_work[i]]-posWall[i]);

		vecToTarget = (pos - posWall[i]);

		cross = vecToTarget.x*vecLine.z - vecToTarget.z*vecLine.x;

		// 壁にあたったら
		if( cross < 0 )
		{
			return TRUE;
		}
	}
	return FALSE;

}
//=============================================================================
// 球と球の当たり判定
//=============================================================================
bool CollsionSphereChek(D3DXVECTOR3 objA,D3DXVECTOR3 objB,float rA,float rB)
{
	if((objB.x-objA.x)*(objB.x-objA.x)+(objB.y-objA.y)*(objB.y-objA.y)+(objB.z-objA.z)*(objB.z-objA.z)<=(rA-rB)*(rA-rB))
	 {
		 return TRUE;
	 }
	 return FALSE;
}
//=============================================================================
// 2点から角度取得
//=============================================================================
float GetMyShipAngle(D3DXVECTOR3 objA, D3DXVECTOR3 objB)
{
	return atan2f(objA.x-objB.x, objA.z-objB.z)/D3DX_PI/2;
}
//座標変換//////////////////////////////////////////////////////////////////////////
void Vector3Transform( D3DXVECTOR3 *Out, D3DXVECTOR3 *Vector, D3DXMATRIX *Matrix )
{
	Out->x = Matrix->_11 * Vector->x + Matrix->_21 * Vector->y + Matrix->_31 * Vector->z + Matrix->_41;
	Out->y = Matrix->_12 * Vector->x + Matrix->_22 * Vector->y + Matrix->_32 * Vector->z + Matrix->_42;
	Out->z = Matrix->_13 * Vector->x + Matrix->_23 * Vector->y + Matrix->_33 * Vector->z + Matrix->_43;
}
//=============================================================================
// 線形保管
//=============================================================================
float lerp(float a,float b,float per)
{
	return a*per + b*(1.0f-per);
}
//=============================================================================
// 線形保管
//=============================================================================
D3DXVECTOR3 lerpVec3(D3DXVECTOR3 a,D3DXVECTOR3 b,float per)
{
	D3DXVECTOR3 w_Vec3;

	w_Vec3.x = a.x*per + b.x*(1.0f-per);
	w_Vec3.y = a.y*per + b.y*(1.0f-per);
	w_Vec3.z = a.z*per + b.z*(1.0f-per);

	return w_Vec3;
}

//=============================================================================
// デバッグモード　切り替え
//=============================================================================
void SetDebug(bool value)
{
	g_Debug = value;
}

//=============================================================================
// 線形保管
//=============================================================================
bool GetDebug(void)
{
	return g_Debug;
}

//=============================================================================
// OBBとOBBの当たり判定
//=============================================================================
int CheckCollision(
	const D3DXVECTOR3 *pMin1, const D3DXVECTOR3 *pMax1, const D3DXMATRIX  *pMat1,
	const D3DXVECTOR3 *pMin2, const D3DXVECTOR3 *pMax2, const D3DXMATRIX  *pMat2)
{
	D3DXMATRIX ident; // 単位行列(NULL対策)

	if( pMat1 == NULL || pMat2 == NULL ) {
		D3DXMatrixIdentity( &ident );
		if( pMat1 == NULL ) pMat1 = &ident;
		if( pMat2 == NULL ) pMat2 = &ident;
	}

	D3DXVECTOR3 axis; // 判定軸
	D3DXMATRIX inv1, inv2; // 各軸の逆行列
	D3DXMATRIX AtoB, BtoA; // ローカル座標同士の変換行列

	// OBB の中心と各軸の長さ
	D3DXVECTOR3 obb_center1, obb_center2;
	D3DXVECTOR3 obb_length1, obb_length2;
	obb_center1.x = (pMin1->x + pMax1->x) / 2.0f;
	obb_center1.y = (pMin1->y + pMax1->y) / 2.0f;
	obb_center1.z = (pMin1->z + pMax1->z) / 2.0f;
	obb_center2.x = (pMin2->x + pMax2->x) / 2.0f;
	obb_center2.y = (pMin2->y + pMax2->y) / 2.0f;
	obb_center2.z = (pMin2->z + pMax2->z) / 2.0f;
	D3DXVec3Subtract( &obb_length1, pMax1, &obb_center1 );
	D3DXVec3Subtract( &obb_length2, pMax2, &obb_center2 );

	// A ローカル座標への座標系変換行列を求める
	D3DXMatrixInverse( &inv1, NULL, pMat1 );
	D3DXMatrixMultiply( &BtoA, pMat2, &inv1 );

	// A ローカル座標のB 中央の絶対位置
	D3DXVECTOR3 tB;
	D3DXVec3TransformCoord( &tB, &obb_center2, &BtoA );

	// A ローカル座標のB の長さ
	D3DXVECTOR3 lenB[3];
	for( int i = 0; i < 3; i++ ) {
		lenB[i] = obb_center2;
		if( i == 0 ) lenB[i].x += obb_length2.x;
		if( i == 1 ) lenB[i].y += obb_length2.y;
		if( i == 2 ) lenB[i].z += obb_length2.z;
		D3DXVec3TransformCoord( &lenB[i], &lenB[i], &BtoA );
		D3DXVec3Subtract( &lenB[i], &lenB[i], &tB );
	}

	// 絶対位置から相対位置への変換
	D3DXVec3Subtract( &tB, &tB, &obb_center1 );

	// A の軸 -- これはA の座標系で処理する
	for( int i = 0; i < 3; i++ )
	{
		float ra, rb, rc;
#if 1
		ra = obb_length1[i];
		rb = fabs( lenB[0][i] ) + fabs( lenB[1][i] ) + fabs( lenB[2][i] );
		rc = fabs( tB[i] );
#else
		// 不効率だけど、アルゴリズムに忠実に従った方法
		axis.x = (i == 0 ? 1.0f : 0.0f);
		axis.y = (i == 1 ? 1.0f : 0.0f);
		axis.z = (i == 2 ? 1.0f : 0.0f);
		ra = D3DXVec3Dot( &axis, &obb_length1 ); // 正と正の内積は正
		rb = fabs( D3DXVec3Dot( &axis, &lenB[0] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenB[1] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenB[2] ) );
		rc = fabs( D3DXVec3Dot( &axis, &tB ) );
#endif
		if( ra + rb < rc ) {
			return i;
		}
	}

	// B ローカル座標への座標系変換行列を求める
	D3DXMatrixInverse( &inv2, NULL, pMat2 );
	D3DXMatrixMultiply( &AtoB, pMat1, &inv2 );

	// B ローカル座標のA 中央の絶対位置
	D3DXVECTOR3 tA;
	D3DXVec3TransformCoord( &tA, &obb_center1, &AtoB );

	// B ローカル座標のA の長さ
	D3DXVECTOR3 lenA[3];
	for( int i = 0; i < 3; i++ ) {
		lenA[i] = obb_center1;
		if( i == 0 ) lenA[i].x += obb_length1.x;
		if( i == 1 ) lenA[i].y += obb_length1.y;
		if( i == 2 ) lenA[i].z += obb_length1.z;
		D3DXVec3TransformCoord( &lenA[i], &lenA[i], &AtoB );
		D3DXVec3Subtract( &lenA[i], &lenA[i], &tA );
	}

	// 絶対位置から相対位置への変換
	D3DXVec3Subtract( &tA, &tA, &obb_center2 );

	// B の軸 -- これはB の座標系で処理する
	for( int i = 0; i < 3; i++ ) {
		float ra, rb, rc;
#if 1
		ra = fabs( lenA[0][i] ) + fabs( lenA[1][i] ) + fabs( lenA[2][i] );
		rb = obb_length2[i];
		rc = fabs( tA[i] );
#else
		// 不効率だけど、アルゴリズムに忠実に従った方法
		axis.x = (i == 0 ? 1.0f : 0.0f);
		axis.y = (i == 1 ? 1.0f : 0.0f);
		axis.z = (i == 2 ? 1.0f : 0.0f);

		ra = fabs( D3DXVec3Dot( &axis, &lenA[0] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenA[1] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenA[2] ) );
		rb = D3DXVec3Dot( &axis, &obb_length2 ); // 正と正の内積は正
		rc = fabs( D3DXVec3Dot( &axis, &tA ) );
#endif
		if( ra + rb < rc ) {
			return i+3;
		}
	}

	// A, B の軸に垂直な軸 -- これはA の座標系で処理する
	D3DXVECTOR3 axisA, axisB[3];
	D3DXVECTOR3 zero( 0.0f, 0.0f, 0.0f );
	D3DXVec3TransformCoord( &zero, &zero, &BtoA );

	for( int i = 0; i < 3; i++ ) {
		float ra, rb, rc;
		axisA.x = (i == 0 ? 1.0f : 0.0f);
		axisA.y = (i == 1 ? 1.0f : 0.0f);
		axisA.z = (i == 2 ? 1.0f : 0.0f);
		for( int j = 0; j < 3; j++ ) {
			if( i == 0 ) {
				// 軸ベクトルを求める
				axisB[j].x = (j == 0 ? 1.0f : 0.0f);
				axisB[j].y = (j == 1 ? 1.0f : 0.0f);
				axisB[j].z = (j == 2 ? 1.0f : 0.0f);
				D3DXVec3TransformCoord( &axisB[j], &axisB[j], &BtoA );
				D3DXVec3Subtract( &axisB[j], &axisB[j], &zero );
			}

			// 判定軸はA とB の軸の外積
			D3DXVec3Cross( &axis, &axisA, &axisB[j] );

			ra = fabs( axis[0] * obb_length1[0] ) +
				 fabs( axis[1] * obb_length1[1] ) +
				 fabs( axis[2] * obb_length1[2] );
			rb = fabs( D3DXVec3Dot( &axis, &lenB[0] ) ) +
				 fabs( D3DXVec3Dot( &axis, &lenB[1] ) ) +
				 fabs( D3DXVec3Dot( &axis, &lenB[2] ) );
			rc = fabs( D3DXVec3Dot( &axis, &tB ) );
			if( ra + rb < rc ) {
				return i*3+j+6;
			}
		}
	}

	return -1; // 接触している
}
void CreateTex(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	D3DXCreateTextureFromFile(m_pD3DDevice,fileName,&m_pD3DTex);
}
LPDIRECT3DTEXTURE9 GetTexture(void)
{
	return m_pD3DTex;
}
void TexUninit(void)
{
	if(m_pD3DTex != NULL)
	{
		m_pD3DTex->Release();
		m_pD3DTex =NULL;
	}
}
void RotationAreaCheck(D3DXVECTOR3 *camera,D3DXVECTOR3 *model)
{
	bool plusflag,minusflag;
	D3DXVECTOR3 a = *camera;
	D3DXVECTOR3 b = *model;

	plusflag = FALSE;
	minusflag = FALSE;

	if(a.y<0&&b.y>0)
	{
		minusflag = TRUE;
	}
	if(a.y>0&&b.y<0)
	{
		plusflag = TRUE;
	}


	if(minusflag)
	{
		if((-a.y)+b.y>D3DX_PI)
		{
			a.y = -a.y;
		}
	}
	if(plusflag)
	{
		if((-b.y)+a.y>D3DX_PI)
		{
			a.y = -a.y;
		}
	}
	camera->y = a.y;
	model->y = b.y;
}
//	|sv|*|sv|
float Vct3LengthPow2( const D3DXVECTOR3 &sv )
{
	return( ( sv.x * sv.x ) + ( sv.y * sv.y ) + ( sv.z * sv.z ) );
}

//	|sv|
float Vct3Length( const D3DXVECTOR3 &sv )
{
	return( sqrtf( Vct3LengthPow2( sv ) ) );
}

//	1/(|sv|*|sv|)
float Vct3LengthPow2Rev( const D3DXVECTOR3 &sv )
{
	return( 1.f / Vct3LengthPow2( sv ) );
}

//	1/|sv|
float Vct3LengthRev( const D3DXVECTOR3 &sv )
{
	return( 1.f / Vct3Length( sv ) );
}

//	ov = sv * f
void Vct3MulScl( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv, const float f )
{
	ov.x = sv.x * f;
	ov.y = sv.y * f;
	ov.z = sv.z * f;
}

//	ov=正規化(sv)
void Vct3Normalize( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv )
{
	float	fr = Vct3LengthRev( sv );
	Vct3MulScl( ov, sv, fr );
}

//	ov=正規化(ov)
void Vct3Normalize( D3DXVECTOR3 &osv )
{
	Vct3Normalize( osv, osv );
}

//	内積(sv1・sv2)
float Vct3Dot( const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 )
{
	return( ( sv1.x * sv2.x ) + ( sv1.y * sv2.y ) + ( sv1.z * sv2.z ) );
}

//	外積(sv1ｘsv2)
void Vct3Cross( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 )
{
	ov.x = ( sv1.y * sv2.z ) - ( sv1.z * sv2.y );
	ov.y = ( sv1.z * sv2.x ) - ( sv1.x * sv2.z );
	ov.z = ( sv1.x * sv2.y ) - ( sv1.y * sv2.x );
}

//	ov = sv1 - sv2
void Vct3Sub( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 )
{
	ov.x = sv1.x - sv2.x;
	ov.y = sv1.y - sv2.y;
	ov.z = sv1.z - sv2.z;
}
void copyBuf( unsigned sz, void *src, LPDIRECT3DVERTEXBUFFER9 buf )
{
	void *p = 0;
	buf->Lock( 0, 0, &p, 0 );
	memcpy( p, src, sz );
	buf->Unlock();
}
   D3DXVECTOR3 OBB::GetDirect( int elem )   // 指定軸番号の方向ベクトルを取得
   {
	   return m_NormaDirect[elem];
   }
   FLOAT OBB::GetLen_W( int elem )          // 指定軸方向の長さを取得
   {
	   return m_fLength[elem];
   }
   D3DXVECTOR3 OBB::GetPos_W()             // 位置を取得
   {
	   return m_Pos;
   }
// OBB v.s. OBB
bool ColOBBs( OBB &obb1, OBB &obb2 )
{
   // 各方向ベクトルの確保
   // （N***:標準化方向ベクトル）
   D3DXVECTOR3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
   D3DXVECTOR3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
   D3DXVECTOR3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
   D3DXVECTOR3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
   D3DXVECTOR3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
   D3DXVECTOR3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
   D3DXVECTOR3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

   // 分離軸 : Ae1
   FLOAT rA = D3DXVec3Length( &Ae1 );
   FLOAT rB = LenSegOnSeparateAxis( &NAe1, &Be1, &Be2, &Be3 );
   FLOAT L = fabs(D3DXVec3Dot( &Interval, &NAe1 ));
   if( L > rA + rB )
      return false; // 衝突していない

   // 分離軸 : Ae2
   rA = D3DXVec3Length( &Ae2 );
   rB = LenSegOnSeparateAxis( &NAe2, &Be1, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &NAe2 ));
   if( L > rA + rB )
   return false;

   // 分離軸 : Ae3
   rA = D3DXVec3Length( &Ae3 );
   rB = LenSegOnSeparateAxis( &NAe3, &Be1, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &NAe3 ));
   if( L > rA + rB )
      return false;

   // 分離軸 : Be1
   rA = LenSegOnSeparateAxis( &NBe1, &Ae1, &Ae2, &Ae3 );
   rB = D3DXVec3Length( &Be1 );
   L = fabs(D3DXVec3Dot( &Interval, &NBe1 ));
   if( L > rA + rB )
      return false;

   // 分離軸 : Be2
   rA = LenSegOnSeparateAxis( &NBe2, &Ae1, &Ae2, &Ae3 );
   rB = D3DXVec3Length( &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &NBe2 ));
   if( L > rA + rB )
      return false;

   // 分離軸 : Be3
   rA = LenSegOnSeparateAxis( &NBe3, &Ae1, &Ae2, &Ae3 );
   rB = D3DXVec3Length( &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &NBe3 ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C11
   D3DXVECTOR3 Cross;
   D3DXVec3Cross( &Cross, &NAe1, &NBe1 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C12
   D3DXVec3Cross( &Cross, &NAe1, &NBe2 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
      if( L > rA + rB )
   return false;

   // 分離軸 : C13
   D3DXVec3Cross( &Cross, &NAe1, &NBe3 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C21
   D3DXVec3Cross( &Cross, &NAe2, &NBe1 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C22
   D3DXVec3Cross( &Cross, &NAe2, &NBe2 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C23
   D3DXVec3Cross( &Cross, &NAe2, &NBe3 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C31
   D3DXVec3Cross( &Cross, &NAe3, &NBe1 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
   rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C32
   D3DXVec3Cross( &Cross, &NAe3, &NBe2 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離軸 : C33
   D3DXVec3Cross( &Cross, &NAe3, &NBe3 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // 分離平面が存在しないので「衝突している」
   return true;
}


// 分離軸に投影された軸成分から投影線分長を算出
FLOAT LenSegOnSeparateAxis( D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 )
{
   // 3つの内積の絶対値の和で投影線分長を計算
   // 分離軸Sepは標準化されていること
   FLOAT r1 = fabs(D3DXVec3Dot( Sep, e1 ));
   FLOAT r2 = fabs(D3DXVec3Dot( Sep, e2 ));
   FLOAT r3 = e3 ? (fabs(D3DXVec3Dot( Sep, e3 ))) : 0;
   return r1 + r2 + r3;
}
void PLANE::GetNormal_W( D3DXVECTOR3 *Norm )
{
	Norm = &m_LocalNormal;
}
D3DXVECTOR3 PLANE::GetPos_W()
{
	return m_Pos;
}
// OBB vs Plane
bool OBBvsPlane( OBB &obb, PLANE &plane, FLOAT *Len )
{
   // 平面の法線に対するOBBの射影線の長さを算出
   FLOAT r = 0.0f;          // 近接距離
   D3DXVECTOR3 PlaneNormal; // 平面の法線ベクトル
   plane.GetNormal_W( &PlaneNormal );
   int i;
   for(i=0; i<3; i++){
      D3DXVECTOR3 Direct = obb.GetDirect(i); // OBBの1つの軸ベクトル
      r += fabs(D3DXVec3Dot( &(Direct * obb.GetLen_W(i)), &PlaneNormal ));
   }

   // 平面とOBBの距離を算出
   D3DXVECTOR3 ObbPos = obb.GetPos_W();
   D3DXVECTOR3 PlanePos = plane.GetPos_W();
   FLOAT s = D3DXVec3Dot( &(ObbPos-PlanePos), &PlaneNormal );

   // 戻し距離を算出
   if( Len != NULL ){
   if(s>0)
      *Len = r - fabs(s);
   else
      *Len = r + fabs(s);
   }

   // 衝突判定
   if( fabs(s)-r < 0.0f )
      return true; // 衝突している

   return false; // 衝突していない
}
int GetRandom(int min, int max)
{

	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}