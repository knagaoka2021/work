//=============================================================================
// ����֐��Ȃ�
//  [common.cpp]
// Author : �����a�^
//
//=============================================================================

#include "common.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 m_pD3DTex = NULL;
bool g_Debug = FALSE;
//=============================================================================
// ��������
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
// 2D�x�N�g���O��
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
// ���K���H
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
// ���E�̈攻��
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

		// �ǂɂ���������
		if( cross < 0 )
		{
			return TRUE;
		}
	}
	return FALSE;

}
//=============================================================================
// ���Ƌ��̓����蔻��
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
// 2�_����p�x�擾
//=============================================================================
float GetMyShipAngle(D3DXVECTOR3 objA, D3DXVECTOR3 objB)
{
	return atan2f(objA.x-objB.x, objA.z-objB.z)/D3DX_PI/2;
}
//���W�ϊ�//////////////////////////////////////////////////////////////////////////
void Vector3Transform( D3DXVECTOR3 *Out, D3DXVECTOR3 *Vector, D3DXMATRIX *Matrix )
{
	Out->x = Matrix->_11 * Vector->x + Matrix->_21 * Vector->y + Matrix->_31 * Vector->z + Matrix->_41;
	Out->y = Matrix->_12 * Vector->x + Matrix->_22 * Vector->y + Matrix->_32 * Vector->z + Matrix->_42;
	Out->z = Matrix->_13 * Vector->x + Matrix->_23 * Vector->y + Matrix->_33 * Vector->z + Matrix->_43;
}
//=============================================================================
// ���`�ۊ�
//=============================================================================
float lerp(float a,float b,float per)
{
	return a*per + b*(1.0f-per);
}
//=============================================================================
// ���`�ۊ�
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
// �f�o�b�O���[�h�@�؂�ւ�
//=============================================================================
void SetDebug(bool value)
{
	g_Debug = value;
}

//=============================================================================
// ���`�ۊ�
//=============================================================================
bool GetDebug(void)
{
	return g_Debug;
}

//=============================================================================
// OBB��OBB�̓����蔻��
//=============================================================================
int CheckCollision(
	const D3DXVECTOR3 *pMin1, const D3DXVECTOR3 *pMax1, const D3DXMATRIX  *pMat1,
	const D3DXVECTOR3 *pMin2, const D3DXVECTOR3 *pMax2, const D3DXMATRIX  *pMat2)
{
	D3DXMATRIX ident; // �P�ʍs��(NULL�΍�)

	if( pMat1 == NULL || pMat2 == NULL ) {
		D3DXMatrixIdentity( &ident );
		if( pMat1 == NULL ) pMat1 = &ident;
		if( pMat2 == NULL ) pMat2 = &ident;
	}

	D3DXVECTOR3 axis; // ���莲
	D3DXMATRIX inv1, inv2; // �e���̋t�s��
	D3DXMATRIX AtoB, BtoA; // ���[�J�����W���m�̕ϊ��s��

	// OBB �̒��S�Ɗe���̒���
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

	// A ���[�J�����W�ւ̍��W�n�ϊ��s������߂�
	D3DXMatrixInverse( &inv1, NULL, pMat1 );
	D3DXMatrixMultiply( &BtoA, pMat2, &inv1 );

	// A ���[�J�����W��B �����̐�Έʒu
	D3DXVECTOR3 tB;
	D3DXVec3TransformCoord( &tB, &obb_center2, &BtoA );

	// A ���[�J�����W��B �̒���
	D3DXVECTOR3 lenB[3];
	for( int i = 0; i < 3; i++ ) {
		lenB[i] = obb_center2;
		if( i == 0 ) lenB[i].x += obb_length2.x;
		if( i == 1 ) lenB[i].y += obb_length2.y;
		if( i == 2 ) lenB[i].z += obb_length2.z;
		D3DXVec3TransformCoord( &lenB[i], &lenB[i], &BtoA );
		D3DXVec3Subtract( &lenB[i], &lenB[i], &tB );
	}

	// ��Έʒu���瑊�Έʒu�ւ̕ϊ�
	D3DXVec3Subtract( &tB, &tB, &obb_center1 );

	// A �̎� -- �����A �̍��W�n�ŏ�������
	for( int i = 0; i < 3; i++ )
	{
		float ra, rb, rc;
#if 1
		ra = obb_length1[i];
		rb = fabs( lenB[0][i] ) + fabs( lenB[1][i] ) + fabs( lenB[2][i] );
		rc = fabs( tB[i] );
#else
		// �s���������ǁA�A���S���Y���ɒ����ɏ]�������@
		axis.x = (i == 0 ? 1.0f : 0.0f);
		axis.y = (i == 1 ? 1.0f : 0.0f);
		axis.z = (i == 2 ? 1.0f : 0.0f);
		ra = D3DXVec3Dot( &axis, &obb_length1 ); // ���Ɛ��̓��ς͐�
		rb = fabs( D3DXVec3Dot( &axis, &lenB[0] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenB[1] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenB[2] ) );
		rc = fabs( D3DXVec3Dot( &axis, &tB ) );
#endif
		if( ra + rb < rc ) {
			return i;
		}
	}

	// B ���[�J�����W�ւ̍��W�n�ϊ��s������߂�
	D3DXMatrixInverse( &inv2, NULL, pMat2 );
	D3DXMatrixMultiply( &AtoB, pMat1, &inv2 );

	// B ���[�J�����W��A �����̐�Έʒu
	D3DXVECTOR3 tA;
	D3DXVec3TransformCoord( &tA, &obb_center1, &AtoB );

	// B ���[�J�����W��A �̒���
	D3DXVECTOR3 lenA[3];
	for( int i = 0; i < 3; i++ ) {
		lenA[i] = obb_center1;
		if( i == 0 ) lenA[i].x += obb_length1.x;
		if( i == 1 ) lenA[i].y += obb_length1.y;
		if( i == 2 ) lenA[i].z += obb_length1.z;
		D3DXVec3TransformCoord( &lenA[i], &lenA[i], &AtoB );
		D3DXVec3Subtract( &lenA[i], &lenA[i], &tA );
	}

	// ��Έʒu���瑊�Έʒu�ւ̕ϊ�
	D3DXVec3Subtract( &tA, &tA, &obb_center2 );

	// B �̎� -- �����B �̍��W�n�ŏ�������
	for( int i = 0; i < 3; i++ ) {
		float ra, rb, rc;
#if 1
		ra = fabs( lenA[0][i] ) + fabs( lenA[1][i] ) + fabs( lenA[2][i] );
		rb = obb_length2[i];
		rc = fabs( tA[i] );
#else
		// �s���������ǁA�A���S���Y���ɒ����ɏ]�������@
		axis.x = (i == 0 ? 1.0f : 0.0f);
		axis.y = (i == 1 ? 1.0f : 0.0f);
		axis.z = (i == 2 ? 1.0f : 0.0f);

		ra = fabs( D3DXVec3Dot( &axis, &lenA[0] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenA[1] ) ) +
			 fabs( D3DXVec3Dot( &axis, &lenA[2] ) );
		rb = D3DXVec3Dot( &axis, &obb_length2 ); // ���Ɛ��̓��ς͐�
		rc = fabs( D3DXVec3Dot( &axis, &tA ) );
#endif
		if( ra + rb < rc ) {
			return i+3;
		}
	}

	// A, B �̎��ɐ����Ȏ� -- �����A �̍��W�n�ŏ�������
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
				// ���x�N�g�������߂�
				axisB[j].x = (j == 0 ? 1.0f : 0.0f);
				axisB[j].y = (j == 1 ? 1.0f : 0.0f);
				axisB[j].z = (j == 2 ? 1.0f : 0.0f);
				D3DXVec3TransformCoord( &axisB[j], &axisB[j], &BtoA );
				D3DXVec3Subtract( &axisB[j], &axisB[j], &zero );
			}

			// ���莲��A ��B �̎��̊O��
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

	return -1; // �ڐG���Ă���
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

//	ov=���K��(sv)
void Vct3Normalize( D3DXVECTOR3 &ov, const D3DXVECTOR3 &sv )
{
	float	fr = Vct3LengthRev( sv );
	Vct3MulScl( ov, sv, fr );
}

//	ov=���K��(ov)
void Vct3Normalize( D3DXVECTOR3 &osv )
{
	Vct3Normalize( osv, osv );
}

//	����(sv1�Esv2)
float Vct3Dot( const D3DXVECTOR3 &sv1, const D3DXVECTOR3 &sv2 )
{
	return( ( sv1.x * sv2.x ) + ( sv1.y * sv2.y ) + ( sv1.z * sv2.z ) );
}

//	�O��(sv1��sv2)
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
   D3DXVECTOR3 OBB::GetDirect( int elem )   // �w�莲�ԍ��̕����x�N�g�����擾
   {
	   return m_NormaDirect[elem];
   }
   FLOAT OBB::GetLen_W( int elem )          // �w�莲�����̒������擾
   {
	   return m_fLength[elem];
   }
   D3DXVECTOR3 OBB::GetPos_W()             // �ʒu���擾
   {
	   return m_Pos;
   }
// OBB v.s. OBB
bool ColOBBs( OBB &obb1, OBB &obb2 )
{
   // �e�����x�N�g���̊m��
   // �iN***:�W���������x�N�g���j
   D3DXVECTOR3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
   D3DXVECTOR3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
   D3DXVECTOR3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
   D3DXVECTOR3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
   D3DXVECTOR3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
   D3DXVECTOR3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
   D3DXVECTOR3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

   // ������ : Ae1
   FLOAT rA = D3DXVec3Length( &Ae1 );
   FLOAT rB = LenSegOnSeparateAxis( &NAe1, &Be1, &Be2, &Be3 );
   FLOAT L = fabs(D3DXVec3Dot( &Interval, &NAe1 ));
   if( L > rA + rB )
      return false; // �Փ˂��Ă��Ȃ�

   // ������ : Ae2
   rA = D3DXVec3Length( &Ae2 );
   rB = LenSegOnSeparateAxis( &NAe2, &Be1, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &NAe2 ));
   if( L > rA + rB )
   return false;

   // ������ : Ae3
   rA = D3DXVec3Length( &Ae3 );
   rB = LenSegOnSeparateAxis( &NAe3, &Be1, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &NAe3 ));
   if( L > rA + rB )
      return false;

   // ������ : Be1
   rA = LenSegOnSeparateAxis( &NBe1, &Ae1, &Ae2, &Ae3 );
   rB = D3DXVec3Length( &Be1 );
   L = fabs(D3DXVec3Dot( &Interval, &NBe1 ));
   if( L > rA + rB )
      return false;

   // ������ : Be2
   rA = LenSegOnSeparateAxis( &NBe2, &Ae1, &Ae2, &Ae3 );
   rB = D3DXVec3Length( &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &NBe2 ));
   if( L > rA + rB )
      return false;

   // ������ : Be3
   rA = LenSegOnSeparateAxis( &NBe3, &Ae1, &Ae2, &Ae3 );
   rB = D3DXVec3Length( &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &NBe3 ));
   if( L > rA + rB )
      return false;

   // ������ : C11
   D3DXVECTOR3 Cross;
   D3DXVec3Cross( &Cross, &NAe1, &NBe1 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C12
   D3DXVec3Cross( &Cross, &NAe1, &NBe2 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
      if( L > rA + rB )
   return false;

   // ������ : C13
   D3DXVec3Cross( &Cross, &NAe1, &NBe3 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae2, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C21
   D3DXVec3Cross( &Cross, &NAe2, &NBe1 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C22
   D3DXVec3Cross( &Cross, &NAe2, &NBe2 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C23
   D3DXVec3Cross( &Cross, &NAe2, &NBe3 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae3 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C31
   D3DXVec3Cross( &Cross, &NAe3, &NBe1 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
   rB = LenSegOnSeparateAxis( &Cross, &Be2, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C32
   D3DXVec3Cross( &Cross, &NAe3, &NBe2 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be3 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // ������ : C33
   D3DXVec3Cross( &Cross, &NAe3, &NBe3 );
   rA = LenSegOnSeparateAxis( &Cross, &Ae1, &Ae2 );
   rB = LenSegOnSeparateAxis( &Cross, &Be1, &Be2 );
   L = fabs(D3DXVec3Dot( &Interval, &Cross ));
   if( L > rA + rB )
      return false;

   // �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
   return true;
}


// �������ɓ��e���ꂽ���������瓊�e���������Z�o
FLOAT LenSegOnSeparateAxis( D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 )
{
   // 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
   // ������Sep�͕W��������Ă��邱��
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
   // ���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
   FLOAT r = 0.0f;          // �ߐڋ���
   D3DXVECTOR3 PlaneNormal; // ���ʂ̖@���x�N�g��
   plane.GetNormal_W( &PlaneNormal );
   int i;
   for(i=0; i<3; i++){
      D3DXVECTOR3 Direct = obb.GetDirect(i); // OBB��1�̎��x�N�g��
      r += fabs(D3DXVec3Dot( &(Direct * obb.GetLen_W(i)), &PlaneNormal ));
   }

   // ���ʂ�OBB�̋������Z�o
   D3DXVECTOR3 ObbPos = obb.GetPos_W();
   D3DXVECTOR3 PlanePos = plane.GetPos_W();
   FLOAT s = D3DXVec3Dot( &(ObbPos-PlanePos), &PlaneNormal );

   // �߂��������Z�o
   if( Len != NULL ){
   if(s>0)
      *Len = r - fabs(s);
   else
      *Len = r + fabs(s);
   }

   // �Փ˔���
   if( fabs(s)-r < 0.0f )
      return true; // �Փ˂��Ă���

   return false; // �Փ˂��Ă��Ȃ�
}
int GetRandom(int min, int max)
{

	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}