//=============================================================================
//
//  [CCamera.cpp]
// Author : �����a�^
//
//=============================================================================
#include "CCamera.h"
#include "CPlayerMotion.h"
#include "CDebugproc.h"
#include "Csound.h"
#include "common.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define START_CAMERA_POSX (0.0f)						// �J���������ʒuX
#define START_CAMERA_POSY (120.0f)						// �J���������ʒuY
#define START_CAMERA_POSZ (-90.0f)						// �J���������ʒuZ
#define START_CAMERA_ANGLE (0.237f)						// �J���������A���O��
#define CAMERA_TO__CAMERAE_P (90.0f)					// ���f������J�����̈ʒu����
#define CAMERA_TO__CAMERAE_R (20.0f)					// ���f������J�����̒����_����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);
float GetView(void);
//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 g_posCameraP;							// �J�����̎��_
	D3DXVECTOR3 g_posCameraR;							// �J�����̒����_
	D3DXVECTOR3 g_vecCameraU;							// �J�����̏�����x�N�g��

	D3DXVECTOR3 g_posCameraPDest;
	D3DXVECTOR3 g_posCameraRDest;

	D3DXVECTOR3 g_rotCamera;							// �J�����̌����i��]�p�j

	D3DVIEWPORT9 g_viewport;
}Camera;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CCamera* pCameraGet = NULL;
D3DXVECTOR3 g_posCameraP;							// �J�����̎��_
D3DXVECTOR3 g_posCameraR;							// �J�����̒����_
D3DXVECTOR3 g_vecCameraU;							// �J�����̏�����x�N�g��

D3DXVECTOR3 g_posCameraPDest;
D3DXVECTOR3 g_posCameraRDest;

D3DXVECTOR3 g_posModelDest;

D3DXVECTOR3 g_rotCamera;									// �J�����̌����i��]�p�j
D3DXVECTOR3 g_modelrot;
D3DVIEWPORT9 g_viewport;

static D3DXMATRIX g_FieldmtxView;							// �r���[�}�g���b�N�X(�n��)
static D3DXMATRIX g_FieldmtxProjection;						// �v���W�F�N�V�����}�g���b�N�X(�n��)
static D3DXMATRIX g_ModelmtxView;							// �r���[�}�g���b�N�X(���f��)
static D3DXMATRIX g_ModelmtxProjection;						// �v���W�F�N�V�����}�g���b�N�X(���f��)

// ���_���璍���_�܂ł̋���
static float g_fLengXZ = 0.0f;
static float g_fLengYZ = 0.0f;
static float g_CameraSpeed = 0.1f;
static float g_fLength;										//�|���S���Ίp���̒���
static float g_fAngle;										//�|���S���Ίp���̊p�x
static float Crx,Cry,Crz = 0;
static float seed = 0;
static float g_fLeng=48.0f;
static float g_fLengZ=90;
static int g_rollspeed=0;

static float x,y,z;
Camera g_Camera[3];
bool cameraflag = FALSE;
CCamera::CCamera(void)
{
}
CCamera::~CCamera(void)
{
}

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CCamera::InitCamera(void)
{
float Cpx,Cpy,Cpz = 0;

	for(int i=0;i<3;i++)
	{
		g_Camera[i].g_viewport.X = 0;
		g_Camera[i].g_viewport.Y  = 0;
		g_Camera[i].g_viewport.Width = SCREEN_WIDTH;
		g_Camera[i].g_viewport.Height = SCREEN_HEIGHT;
		g_Camera[i].g_viewport.MinZ = 0.0f;
		g_Camera[i].g_viewport.MaxZ = 1.0f;

		g_Camera[i].g_posCameraP = D3DXVECTOR3(START_CAMERA_POSX,START_CAMERA_POSY,START_CAMERA_POSZ);
		g_Camera[i].g_posCameraR = D3DXVECTOR3(0.0f,0.0f,1.0f);
		g_Camera[i].g_vecCameraU = D3DXVECTOR3(0.0f,1.0f,0.0f);

		Cpx = g_Camera[i].g_posCameraR.x-g_Camera[i].g_posCameraP.x;
		Cpy = g_Camera[i].g_posCameraR.y-g_Camera[i].g_posCameraP.y;
		Cpz = g_Camera[i].g_posCameraR.z-g_Camera[i].g_posCameraP.z;

		g_Camera[i].g_rotCamera.y = atan2f(Cpx,Cpz);
		g_Camera[i].g_rotCamera.x = atan2f(Cpz,Cpy);
	}
		g_Camera[2].g_viewport.X = 0;
		g_Camera[2].g_viewport.Y  = 0;
		g_Camera[2].g_viewport.Width = SCREEN_WIDTH;

	

	g_posCameraP = D3DXVECTOR3(START_CAMERA_POSX,START_CAMERA_POSY,START_CAMERA_POSZ);

	g_posCameraR = D3DXVECTOR3(0.0f,0.0f,0.0f);

	g_vecCameraU = D3DXVECTOR3(0.0f,1.0f,0.0f);


	Cpx=g_posCameraR.x-g_posCameraP.x;
	Cpy=g_posCameraR.y-g_posCameraP.y;
	Cpz=g_posCameraR.z-g_posCameraP.z;

	g_fLengXZ=sqrtf((Cpx*Cpx)+(Cpz*Cpz));
	g_fLengYZ=sqrtf((Cpy*Cpy)+(Cpz*Cpz));

	g_rotCamera.y=atan2f(Cpx,Cpz);
	g_rotCamera.x=atan2f(Cpz,Cpy);


	g_Camera[1].g_posCameraP=g_posCameraP;
	g_Camera[0].g_rotCamera.x = START_CAMERA_ANGLE;
	g_rollspeed=0;

	return S_OK;
}
//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::UninitCamera(void)
{
	g_fLeng=200;
	g_fLengZ=1000;
	pCameraGet = NULL;
}
//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::UpdateCamera(int nIndex,CPlayerMotion *p_player)
{
	if(p_player!=NULL)
	{
		g_posModelDest = p_player->GetPos();
		g_modelrot=p_player->GetRot();
	}

	if (nIndex == 0 || nIndex == 2)
	{
		//�~�܂��Ă��Ȃ��Ƃ�
		if (p_player != NULL&&!p_player->GetMoveSpeedchek())
		{

			cameraflag = TRUE;
			// ���f������J�����̈ʒu
			g_Camera[nIndex].g_posCameraPDest.x = g_posModelDest.x + sinf(g_Camera[0].g_rotCamera.y) * CAMERA_TO__CAMERAE_P;
			g_Camera[nIndex].g_posCameraPDest.y = g_posModelDest.y + g_fLeng;
			g_Camera[nIndex].g_posCameraPDest.z = g_posModelDest.z + cosf(g_Camera[0].g_rotCamera.y) * CAMERA_TO__CAMERAE_P;

			// ���f������J�����̒����_
			g_Camera[nIndex].g_posCameraRDest.x = g_posModelDest.x - sinf(g_modelrot.y) * CAMERA_TO__CAMERAE_R;
			g_Camera[nIndex].g_posCameraRDest.y = g_posModelDest.y + g_fLeng;
			g_Camera[nIndex].g_posCameraRDest.z = g_posModelDest.z - cosf(g_modelrot.y) * CAMERA_TO__CAMERAE_R;

			g_Camera[nIndex].g_posCameraP.x += (g_Camera[nIndex].g_posCameraPDest.x-g_Camera[nIndex].g_posCameraP.x);
			g_Camera[nIndex].g_posCameraP.y += (g_Camera[nIndex].g_posCameraPDest.y-g_Camera[nIndex].g_posCameraP.y);
			g_Camera[nIndex].g_posCameraP.z += (g_Camera[nIndex].g_posCameraPDest.z-g_Camera[nIndex].g_posCameraP.z);


			g_Camera[nIndex].g_posCameraR.x += (g_Camera[nIndex].g_posCameraRDest.x - g_Camera[nIndex].g_posCameraR.x);
			g_Camera[nIndex].g_posCameraR.y += (g_Camera[nIndex].g_posCameraRDest.y - g_Camera[nIndex].g_posCameraR.y);
			g_Camera[nIndex].g_posCameraR.z += (g_Camera[nIndex].g_posCameraRDest.z - g_Camera[nIndex].g_posCameraR.z);
			g_rollspeed = 0;
		}

		if (nIndex == 0 )
		{
			if (CInputPad::PresspadCheak(0, BUTTON_L) || CInputKeyboard::GetkeyboardPress(DIK_LEFTARROW))
			{
				g_Camera[nIndex].g_rotCamera.y += D3DX_PI / 180;

			}
			if (CInputPad::PresspadCheak(0, BUTTON_R) || CInputKeyboard::GetkeyboardPress(DIK_RIGHTARROW))
			{
				g_Camera[nIndex].g_rotCamera.y -= D3DX_PI / 180;
			}

			if (CInputPad::PresspadCheak(0, BUTTON_L2) || CInputKeyboard::GetkeyboardPress(DIK_UPARROW))
			{
				g_Camera[nIndex].g_rotCamera.x += D3DX_PI / 180;
			}
			if (CInputPad::PresspadCheak(0, BUTTON_R2) || CInputKeyboard::GetkeyboardPress(DIK_DOWNARROW))
			{
				g_Camera[nIndex].g_rotCamera.x -= D3DX_PI / 180;
			}
		}

		g_Camera[nIndex].g_posCameraR.y = g_posModelDest.y + g_fLeng;


		// ����
		if (g_Camera[nIndex].g_rotCamera.y < -D3DX_PI)
		{
			g_Camera[nIndex].g_rotCamera.y += D3DX_PI*2.0f;
		}
		if (g_Camera[nIndex].g_rotCamera.y > D3DX_PI)
		{
			g_Camera[nIndex].g_rotCamera.y -= D3DX_PI*2.0f;
		}

		// ����
		if (g_Camera[nIndex].g_rotCamera.x < -D3DX_PI / 4.0f + 0.6f)
		{
			g_Camera[nIndex].g_rotCamera.x = -D3DX_PI / 4.0f + 0.6f;
		}
		if (g_Camera[nIndex].g_rotCamera.x > D3DX_PI / 2.0f - 0.6f)
		{
			g_Camera[nIndex].g_rotCamera.x = D3DX_PI / 2.0f - 0.6f;
		}

		g_Camera[nIndex].g_posCameraP.x = g_Camera[nIndex].g_posCameraR.x + sinf(g_Camera[nIndex].g_rotCamera.y)*110;
		g_Camera[nIndex].g_posCameraP.y = g_Camera[nIndex].g_posCameraR.y + sinf(g_Camera[nIndex].g_rotCamera.x)*150;
		g_Camera[nIndex].g_posCameraP.z = g_Camera[nIndex].g_posCameraR.z + cosf(g_Camera[nIndex].g_rotCamera.y)*110;


	}
	g_Camera[nIndex].g_rotCamera.y = Invers(g_Camera[nIndex].g_rotCamera.y);

	if(CInputKeyboard::GetkeyboardPress(DIK_Y))
	{
		g_Camera[nIndex].g_rotCamera.z+=D3DX_PI*0.1;
	}

	if(nIndex==1)
	{

		if(CInputKeyboard::GetkeyboardPress(DIK_DOWN))
		{
			g_Camera[nIndex].g_posCameraP.x-=sinf(g_Camera[nIndex].g_rotCamera.y)*25;
			g_Camera[nIndex].g_posCameraP.z-=cosf(g_Camera[nIndex].g_rotCamera.y)*25;

		}
		if(CInputKeyboard::GetkeyboardPress(DIK_UP))
		{
			g_Camera[nIndex].g_posCameraP.x+=sinf(g_Camera[nIndex].g_rotCamera.y)*25;
			g_Camera[nIndex].g_posCameraP.z+=cosf(g_Camera[nIndex].g_rotCamera.y)*25;
		}
		if(CInputKeyboard::GetkeyboardPress(DIK_RIGHT))
		{
			g_Camera[nIndex].g_posCameraP.x+=sinf(g_Camera[nIndex].g_rotCamera.y+90*D3DX_PI/180)*25;
			g_Camera[nIndex].g_posCameraP.z+=cosf(g_Camera[nIndex].g_rotCamera.y+90*D3DX_PI/180)*25;
		}
		if(CInputKeyboard::GetkeyboardPress(DIK_LEFT))
		{
			g_Camera[nIndex].g_posCameraP.x-=sinf(g_Camera[nIndex].g_rotCamera.y+90*D3DX_PI/180)*25;
			g_Camera[nIndex].g_posCameraP.z-=cosf(g_Camera[nIndex].g_rotCamera.y+90*D3DX_PI/180)*25;
		}

		if(CInputKeyboard::GetkeyboardPress(DIK_Q))
		{
			g_Camera[nIndex].g_rotCamera.y+=D3DX_PI*0.01f;
			g_Camera[nIndex].g_posCameraP.x=g_Camera[nIndex].g_posCameraR.x-sinf(g_Camera[nIndex].g_rotCamera.y)*g_fLengXZ;
			g_Camera[nIndex].g_posCameraP.z=g_Camera[nIndex].g_posCameraR.z-cosf(g_Camera[nIndex].g_rotCamera.y)*g_fLengXZ;

		}

		if(CInputKeyboard::GetkeyboardPress(DIK_I))
		{
			g_Camera[nIndex].g_rotCamera.x+=D3DX_PI*0.01f;
			g_Camera[nIndex].g_posCameraP.x=g_Camera[nIndex].g_posCameraR.x-sinf(g_Camera[nIndex].g_rotCamera.y)*g_fLengXZ;
			g_Camera[nIndex].g_posCameraP.z=g_Camera[nIndex].g_posCameraR.z-cosf(g_Camera[nIndex].g_rotCamera.y)*g_fLengXZ;

		}
		if (CInputKeyboard::GetkeyboardPress(DIK_E))
		{
			g_Camera[nIndex].g_rotCamera.y -= D3DX_PI*0.01f;
			g_Camera[nIndex].g_posCameraP.x = g_Camera[nIndex].g_posCameraR.x - sinf(g_Camera[nIndex].g_rotCamera.y)*g_fLengXZ;
			g_Camera[nIndex].g_posCameraP.z = g_Camera[nIndex].g_posCameraR.z - cosf(g_Camera[nIndex].g_rotCamera.y)*g_fLengXZ;
		}


	}

	bool event = FALSE;

	if (nIndex == 2)
	{
		event = TRUE;

	}
	if (event)
	{
		g_Camera[nIndex].g_rotCamera.y -= D3DX_PI*0.01f;
		g_Camera[nIndex].g_posCameraP.x -= sinf(g_Camera[nIndex].g_rotCamera.y);
		g_Camera[nIndex].g_posCameraP.z -= cosf(g_Camera[nIndex].g_rotCamera.y);
		g_Camera[nIndex].g_rotCamera.x -= D3DX_PI / 180;
	}
}


//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(int nIndex,LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	// View Matrix
	D3DXMatrixIdentity(&g_FieldmtxView);															// ������	
	
	D3DXMatrixLookAtLH( &g_FieldmtxView , &g_Camera[nIndex].g_posCameraP , &g_Camera[nIndex].g_posCameraR , &g_Camera[nIndex].g_vecCameraU );


	m_pD3DDevice->SetTransform( D3DTS_VIEW , &g_FieldmtxView );

	// View Port
	m_pD3DDevice->SetViewport(&g_Camera[nIndex].g_viewport);
	
	// Projection Matrix
	D3DXMatrixIdentity(&g_FieldmtxProjection);														// ������

	D3DXMatrixPerspectiveFovLH( &g_FieldmtxProjection , D3DXToRadian(45.0f) , (float)g_Camera[nIndex].g_viewport.Width/(float)g_Camera[nIndex].g_viewport.Height , 1.0f , 8000.0f );

	m_pD3DDevice->SetTransform( D3DTS_PROJECTION , &g_FieldmtxProjection );

}


//=============================================================================
// �J�����̌������擾
//=============================================================================
D3DXVECTOR3 CCamera::GetRotCamera(void)
{
	return g_Camera[0].g_rotCamera;
}
//=============================================================================
// �J������
//=============================================================================
D3DXVECTOR3 CCamera::GetCameraLength(void)
{

	return g_Camera[1].g_posCameraR-g_Camera[1].g_posCameraP;
}
//=============================================================================
// billboard�p�J������
//=============================================================================
D3DXMATRIX CCamera::GetCameraMtxView(void)
{

	return g_FieldmtxView;
}
D3DXMATRIX CCamera::GetCameraMtxProjection(void)
{

	return g_FieldmtxProjection;
}
void CCamera::SetClass(CCamera* classtype)
{	


	pCameraGet = classtype;
}
CCamera* CCamera::GetTypeClass()
{
	return pCameraGet;
}
D3DXMATRIX GetMatrixV(void)
{
	return g_FieldmtxView;
}