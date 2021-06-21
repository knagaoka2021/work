//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �����a�^
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define START_CAMERA_POSX (0.0f)	// �J���������ʒuX
#define START_CAMERA_POSY (35.5f)	// �J���������ʒuY
#define START_CAMERA_POSZ (-80.5f)	// �J���������ʒuZ
#define START_CAMERA_ANGLE (60)		// �J���������A���O��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

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
static float Crx, Cry, Crz = 0;
static float seed = 0;
static float g_fLeng = 200;
static float g_fLengZ = 1000;
static int g_rollspeed = 0;

Camera g_Camera[3];

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitCamera(void)
{
    float Cpx, Cpy, Cpz = 0;

    for (int i = 0; i < 3; i++)
    {
        g_Camera[i].g_viewport.X = 0;
        g_Camera[i].g_viewport.Y = 0;
        g_Camera[i].g_viewport.Width = SCREEN_WIDTH;
        g_Camera[i].g_viewport.Height = SCREEN_HEIGHT;
        g_Camera[i].g_viewport.MinZ = 0.0f;
        g_Camera[i].g_viewport.MaxZ = 1.0f;

        g_Camera[i].g_posCameraP = D3DXVECTOR3(START_CAMERA_POSX, START_CAMERA_POSY, START_CAMERA_POSZ);
        g_Camera[i].g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
        g_Camera[i].g_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        Cpx = g_Camera[i].g_posCameraR.x - g_Camera[i].g_posCameraP.x;
        Cpy = g_Camera[i].g_posCameraR.y - g_Camera[i].g_posCameraP.y;
        Cpz = g_Camera[i].g_posCameraR.z - g_Camera[i].g_posCameraP.z;

        g_Camera[i].g_rotCamera.y = atan2f(Cpx, Cpz);
        g_Camera[i].g_rotCamera.x = atan2f(Cpz, Cpy);
    }
    g_Camera[2].g_viewport.X = 0;
    g_Camera[2].g_viewport.Y = 0;
    g_Camera[2].g_viewport.Width = SCREEN_WIDTH;



    g_posCameraP = D3DXVECTOR3(START_CAMERA_POSX, START_CAMERA_POSY, START_CAMERA_POSZ);

    g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

    g_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


    Cpx = g_posCameraR.x - g_posCameraP.x;
    Cpy = g_posCameraR.y - g_posCameraP.y;
    Cpz = g_posCameraR.z - g_posCameraP.z;

    g_fLengXZ = sqrtf((Cpx * Cpx) + (Cpz * Cpz));
    g_fLengYZ = sqrtf((Cpy * Cpy) + (Cpz * Cpz));

    g_rotCamera.y = atan2f(Cpx, Cpz);
    g_rotCamera.x = atan2f(Cpz, Cpy);

    g_Camera[1].g_posCameraP.y = 180;

    g_Camera[1].g_posCameraP.z = -300;
    g_rollspeed = 0;

    //�A���O��

    return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
    g_fLeng = 200;
    g_fLengZ = 1000;
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(int nIndex)
{
    g_posModelDest = GetPos();
    g_modelrot = GetRot();

    if (nIndex == 0 || nIndex == 2)
    {

        g_Camera[nIndex].g_posCameraPDest.x = g_posModelDest.x;
        g_Camera[nIndex].g_posCameraPDest.y = g_posModelDest.y + g_fLeng;
        g_Camera[nIndex].g_posCameraPDest.z = g_posModelDest.z - g_fLengZ;

        g_Camera[nIndex].g_posCameraRDest.x = g_posModelDest.x - sinf(g_modelrot.y) * 40;
        g_Camera[nIndex].g_posCameraRDest.y = g_posModelDest.y;
        g_Camera[nIndex].g_posCameraRDest.z = g_posModelDest.z - cosf(g_modelrot.y) * 40;

        g_Camera[nIndex].g_posCameraP.x += (g_Camera[nIndex].g_posCameraPDest.x - g_Camera[nIndex].g_posCameraP.x) * 0.5f;
        g_Camera[nIndex].g_posCameraP.y += (g_Camera[nIndex].g_posCameraPDest.y - g_Camera[nIndex].g_posCameraP.y) * 0.5f;
        g_Camera[nIndex].g_posCameraP.z += (g_Camera[nIndex].g_posCameraPDest.z - g_Camera[nIndex].g_posCameraP.z) * 0.5f;

        g_Camera[nIndex].g_posCameraR.x += (g_Camera[nIndex].g_posCameraRDest.x - g_Camera[nIndex].g_posCameraR.x) * 0.5f;
        g_Camera[nIndex].g_posCameraR.y += (g_Camera[nIndex].g_posCameraRDest.y - g_Camera[nIndex].g_posCameraR.y) * 0.5f;
        g_Camera[nIndex].g_posCameraR.z += (g_Camera[nIndex].g_posCameraRDest.z - g_Camera[nIndex].g_posCameraR.z) * 0.5f;
    }


    if (nIndex == 1)
    {

        if (GetkeyboardPress(DIK_DOWN))
        {
            g_Camera[nIndex].g_posCameraP.x -= sinf(g_Camera[nIndex].g_rotCamera.y) * 2;
            g_Camera[nIndex].g_posCameraP.z -= cosf(g_Camera[nIndex].g_rotCamera.y) * 2;

        }
        if (GetkeyboardPress(DIK_UP))
        {
            g_Camera[nIndex].g_posCameraP.x += sinf(g_Camera[nIndex].g_rotCamera.y) * 2;
            g_Camera[nIndex].g_posCameraP.z += cosf(g_Camera[nIndex].g_rotCamera.y) * 2;
        }
        if (GetkeyboardPress(DIK_RIGHT))
        {
            g_Camera[nIndex].g_posCameraP.x += sinf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
            g_Camera[nIndex].g_posCameraP.z += cosf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
        }
        if (GetkeyboardPress(DIK_LEFT))
        {
            g_Camera[nIndex].g_posCameraP.x -= sinf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
            g_Camera[nIndex].g_posCameraP.z -= cosf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
        }
        if (GetkeyboardPress(DIK_Y))
        {
            g_Camera[nIndex].g_posCameraP.y += sinf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
            g_Camera[nIndex].g_posCameraP.y += cosf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
        }
        if (GetkeyboardPress(DIK_H))
        {
            g_Camera[nIndex].g_posCameraP.y -= sinf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
            g_Camera[nIndex].g_posCameraP.y -= cosf(g_Camera[nIndex].g_rotCamera.y + 90 * D3DX_PI / 180) * 2;
        }

        if (GetkeyboardPress(DIK_Z))
        {
            g_Camera[nIndex].g_rotCamera.y += D3DX_PI / 180;
            g_Camera[nIndex].g_posCameraP.x = g_Camera[nIndex].g_posCameraR.x - sinf(g_Camera[nIndex].g_rotCamera.y) * g_fLengXZ;
            g_Camera[nIndex].g_posCameraP.z = g_Camera[nIndex].g_posCameraR.z - cosf(g_Camera[nIndex].g_rotCamera.y) * g_fLengXZ;

        }

        if (GetkeyboardPress(DIK_C))
        {
            g_Camera[nIndex].g_rotCamera.y -= D3DX_PI / 180;
            g_Camera[nIndex].g_posCameraP.x = g_Camera[nIndex].g_posCameraR.x - sinf(g_Camera[nIndex].g_rotCamera.y) * g_fLengXZ;
            g_Camera[nIndex].g_posCameraP.z = g_Camera[nIndex].g_posCameraR.z - cosf(g_Camera[nIndex].g_rotCamera.y) * g_fLengXZ;
        }
        if (GetkeyboardPress(DIK_Q))
        {
            g_Camera[nIndex].g_rotCamera.y -= (D3DX_PI / 180);
        }
        else if (GetkeyboardPress(DIK_E))
        {
            g_Camera[nIndex].g_rotCamera.y += (D3DX_PI / 180);
        }

        g_Camera[nIndex].g_posCameraR.x = g_Camera[nIndex].g_posCameraP.x + sinf(g_Camera[nIndex].g_rotCamera.y) * g_fLengXZ;
        g_Camera[nIndex].g_posCameraR.z = g_Camera[nIndex].g_posCameraP.z + cosf(g_Camera[nIndex].g_rotCamera.y) * g_fLengXZ;
        g_Camera[nIndex].g_posCameraR.y = g_Camera[nIndex].g_posCameraP.y + cosf(g_Camera[nIndex].g_rotCamera.x) * g_fLengYZ;

        g_rollspeed++;
    }
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(int nIndex)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    // View Matrix
    D3DXMatrixIdentity(&g_FieldmtxView);															// ������	

    D3DXMatrixLookAtLH(&g_FieldmtxView, &g_Camera[nIndex].g_posCameraP, &g_Camera[nIndex].g_posCameraR, &g_Camera[nIndex].g_vecCameraU);


    pDevice->SetTransform(D3DTS_VIEW, &g_FieldmtxView);

    // View Port
    pDevice->SetViewport(&g_Camera[nIndex].g_viewport);

    // Projection Matrix
    D3DXMatrixIdentity(&g_FieldmtxProjection);														// ������

    D3DXMatrixPerspectiveFovLH(&g_FieldmtxProjection, D3DX_PI / 4, (float)g_Camera[nIndex].g_viewport.Width / (float)g_Camera[nIndex].g_viewport.Height, 10.0f, 1000000.0f);

    pDevice->SetTransform(D3DTS_PROJECTION, &g_FieldmtxProjection);



}

//=============================================================================
// �J�����̌������擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
    return g_rotCamera;

}
D3DXMATRIX GetMtxView(void)
{
    return g_FieldmtxView;
}
float GetView(void)
{
    return g_fLengZ;
}