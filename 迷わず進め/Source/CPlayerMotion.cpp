//=============================================================================
//
//  [CPlayerMotion.cpp]
// Author : 長岡和真
//
//=============================================================================
#include "CRenderer.h"
#include "CPlayerMotion.h"
#include "CInput.h"
#include "common.h"
#include "CEfectContoroller.h"
#include "CDebugproc.h"
#include "CList.h"
#include "CObjModel.h"
#include "CLife.h"
#include "Csound.h"
#include "common.h"
#include "CGimmick.h"
#include "CExplotion.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMMESHFIELDX		(101)
#define NUMMESHFIELDZ		(100)
#define DRIVE				(0.8f)
#define PLAYER_HEIGHT		(25.0f)
#define PRECSION			(1.8f)
#define	RATE_MOVE_MODEL		(0.20f)	
#define	VALUE_ROTATE_MODEL	(PI * 0.05f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.20f)					// 回転慣性係数
#define GURAVITY			(0.45f)
#define CHARA_HIT_SLIDE_DISTANCE		(0.03f)	// 押し出し処理でスライドさせる距離
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 構造体
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_parts_num = 0;
float g_motionTime = 0;
float g_per = 0.0f;
float g_persave = 0.0f;
bool moveflag = false;
bool flag = false;
bool g_jumpFlag = false;
bool g_groundHit = true;
float g_ySpeed = 0.0f;
static D3DXVECTOR3 D_pos;
static float F_pos;
static bool g_moveflag = FALSE;
static float modelrotdesty = 0.0f;
static float modelrotdest = 0.0f;
static bool Inflag = FALSE;
static bool Rideflag = FALSE;
static float motionrotdesty = 0.0f;
static float motionrotdest = 0.0f;

CList *g_plistM = NULL;
static PLAYERSTATEM g_plyerState = PLAYERM_NOMAL;		// プレイヤー状態
static CCamera *g_pCamera = NULL;
CMeshField *CPlayerMotion::g_pMeshFieldDummy = NULL;
CPlayerMotion* CPlayerMotion::m_pPlayer = NULL;
MOTIONTYPE g_motiontype = MOTION_NONE;
MOTIONTYPE g_motionLastType = MOTION_WAIT;
PART g_part[PART_MAX];
OFSET g_ofset[PART_MAX];
ANM_DATA g_anim[MOTION_MAX];

CPlayerMotion *CPlayerMotion::Create(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName,CCamera *p_Camera,D3DXVECTOR3 m_pos,D3DXVECTOR3 m_rot,CMeshField *pCMeshFieldDummy,CList *plist)
{

	CPlayerMotion *pPlayer;


	pPlayer = new CPlayerMotion();


	modelrotdesty = m_rot.y;
	g_pMeshFieldDummy = pCMeshFieldDummy;
	//リストアドレス
	g_plistM = plist;
	pPlayer->Init(m_pD3DDevice,fileName);

	pPlayer->SetPos(m_pos);


	pPlayer->SetRot(m_rot);

	

	Rideflag = false;

	g_pCamera = p_Camera;

	


	return pPlayer;
}
//=============================================================================
// 初期化
//=============================================================================
HRESULT CPlayerMotion::Init(LPDIRECT3DDEVICE9 m_pD3DDevice,char *fileName)
{
	damagePoint = 1;
	m_plyerState = PLAYERM_NOMAL;
	m_list = g_plistM;
	m_pMeshFieldDummy = g_pMeshFieldDummy;

	m_pPlayerDevice = m_pD3DDevice;
	m_rotDest.y = modelrotdesty;
	modelrotdest =m_rotDest.y;

	m_pSkin = CSceneSkinMesh::Create(m_pD3DDevice,OBJTYPE_X,"",D3DXVECTOR3(0.0f,-10.0f,0.0f),D3DXVECTOR3(0,0,0));//new CSceneSkinMesh(4,OBJTYPE_NONE);
	m_pShadow = Shadow::Create(m_pD3DDevice,"data\\MODEL\\shadow000.x",D3DXVECTOR3(0.0f,10.0f,0.0f),D3DXVECTOR3(0,0,0));
	m_pObb=CSceneBoundingObj::Create(m_pD3DDevice,"data\\TEXTURE\\systemwall.jpg",D3DXVECTOR3(0.0f,28.0f,0.0f),D3DXVECTOR3(0,0,0),NULL,NULL,m_pSkin,9.0f,18.0f,9.0f,PARTS_BODY);
	m_pWeponObb=CSceneBoundingObj::Create(m_pD3DDevice,"data\\TEXTURE\\systemwall.jpg",D3DXVECTOR3(-10.0f,0.0f,0.0f),D3DXVECTOR3(0,0,0),NULL,NULL,m_pSkin,8.0f,5.0f,8.0f,PARTS_RIGHTARM);

	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_rotDest.y = motionrotdesty;
	motionrotdesty =m_rotDest.y;
	m_list = g_plistM;
	g_plistM = NULL;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CPlayerMotion::Uninit(void)
{

	m_plyerState = PLAYERM_STOP;
//---テクスチャの解放---//
	m_list = NULL;
	m_pMeshFieldDummy = NULL;
	g_pMeshFieldDummy = NULL;
	g_pCamera = NULL;
	m_pPlayer = NULL;

	if(m_pWeponObb!=NULL)
	{
		m_pWeponObb->Uninit();
		m_pWeponObb = NULL;
	}
	if(m_pObb!=NULL)
	{
		m_pObb->Uninit();
		m_pObb = NULL;
	}
	if(m_pShadow!=NULL)
	{
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	if(m_pSkin!=NULL)
	{
		m_pSkin->Uninit();
		m_pSkin = NULL;
	}
	CGame::SetPresent();

	CScene::Relese();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayerMotion::Update(void)
{
	float fDiffRotY;
	D3DXVECTOR3 rotCamera;
	// 前フレームの座標取得
	m_posDest = m_pos;


	// カメラの向きを取得
	rotCamera = g_pCamera->GetRotCamera();
	rotCamera.y = Invers(rotCamera.y);

	// カメラから垂直ベクトル生成
	m_fvecFrontY = rotCamera.y+(D3DX_PI);
	m_fvecRightY = rotCamera.y+(D3DX_PI/2.0f);

	m_fvecFrontY = Invers(m_fvecFrontY);
	m_fvecRightY = Invers(m_fvecRightY);

	if(m_pSkin!=NULL)
	{
		g_motiontype =m_pSkin->GetSkinMotion();
	}


	 //動いていたら
	if(!GetMoveSpeedchek())
	{
		g_moveflag=TRUE;
	}

	moveflag = FALSE;

	if(m_plyerState != PLAYERM_STOP&&m_plyerState != PLAYERM_DAMEGE&&m_plyerState != PLAYERM_INVINCIBLE)
	{
		// 移動処理
		if(CInputPad::PresspadCheak(0,BUTTON_UP)||CInputKeyboard::GetkeyboardPress(DIK_W)&&g_motiontype != MOTION_NONE)
		{
			m_rotDest.y = m_fvecFrontY-(D3DX_PI);
			m_xSpeed-= sinf(m_rotDest.y)*DRIVE;
			m_zSpeed-= cosf(m_rotDest.y)*DRIVE;

			moveflag=true;
		}

		if(CInputPad::PresspadCheak(0,BUTTON_DOWN)||CInputKeyboard::GetkeyboardPress(DIK_S)&&g_motiontype != MOTION_NONE)
		{
			m_rotDest.y = m_fvecFrontY;
			m_xSpeed-= sinf(m_rotDest.y)*DRIVE;
			m_zSpeed-= cosf(m_rotDest.y)*DRIVE;

			moveflag=true;
		}
		if(CInputPad::PresspadCheak(0,BUTTON_RIGHT)||CInputKeyboard::GetkeyboardPress(DIK_D)&&g_motiontype != MOTION_NONE)
		{
			m_rotDest.y = m_fvecRightY;
			m_xSpeed-= sinf(m_rotDest.y)*DRIVE;
			m_zSpeed-= cosf(m_rotDest.y)*DRIVE;

			moveflag=true;
		}
		if(CInputPad::PresspadCheak(0,BUTTON_LEFT)||CInputKeyboard::GetkeyboardPress(DIK_A)&&g_motiontype != MOTION_NONE)
		{
			m_rotDest.y = m_fvecRightY+(D3DX_PI);
			m_xSpeed-= sinf(m_rotDest.y)*DRIVE;
			m_zSpeed-= cosf(m_rotDest.y)*DRIVE;

			moveflag=true;
		}

		if (CInputPad::PresspadCheak(0, BUTTON_UL) || (CInputKeyboard::GetkeyboardPress(DIK_W) && CInputKeyboard::GetkeyboardPress(DIK_A)))
		{
			m_rotDest.y = m_rotDest.y + (D3DX_PI) / 4;
		}
		if (CInputPad::PresspadCheak(0, BUTTON_UR) || (CInputKeyboard::GetkeyboardPress(DIK_W) && CInputKeyboard::GetkeyboardPress(DIK_D)))
		{
			m_rotDest.y = m_rotDest.y - (D3DX_PI) / 4;
		}
		if (CInputPad::PresspadCheak(0, BUTTON_DL) || (CInputKeyboard::GetkeyboardPress(DIK_S) && CInputKeyboard::GetkeyboardPress(DIK_A)))
		{
			m_rotDest.y = m_rotDest.y - (D3DX_PI) / 4;
		}
		if (CInputPad::PresspadCheak(0, BUTTON_DR) || (CInputKeyboard::GetkeyboardPress(DIK_S) && CInputKeyboard::GetkeyboardPress(DIK_D)))
		{
			m_rotDest.y = m_rotDest.y + (D3DX_PI) / 4;
		}

		//攻撃処理
		if((CInputKeyboard::tringkeyboardPress(DIK_E)||CInputPad::tringpadPress(0,BUTTON_A))&&g_motiontype!=MOTION_RUN&&g_motiontype!=MOTION_PANCH)
		{
			if(!m_jumpFlag)
			{
				g_motiontype = MOTION_PANCH;
				g_motionLastType = MOTION_PANCH;
				g_motionTime = 0.0f;
				g_persave = g_per;
				g_per = 0.0f;
				g_motiontype = MOTION_PANCH;
				m_pSkin->SetSkinMotion(g_motiontype);
				m_pSkin->SkinMotionResetTime(MOTION_PANCH);
				m_pSkin->ChangeSkinMotion(MOTION_PANCH);
			}
			else
			{
				g_motiontype = MOTION_JUMP_PANCH;
				g_motionLastType = MOTION_JUMP_PANCH;
				g_motiontype = MOTION_JUMP_PANCH;
				m_pSkin->SetSkinMotion(g_motiontype);
				m_pSkin->SkinMotionResetTime(MOTION_JUMP_PANCH);
				m_pSkin->ChangeSkinMotion(MOTION_JUMP_PANCH);
			}

			PlaySound(SOUND_LABEL_SE_HIT000,0);
		
			flag=true;
		}

		//ジャンプ処理
		if((CInputKeyboard::tringkeyboardPress(DIK_SPACE)||CInputPad::tringpadPress(0,BUTTON_X))&&!m_jumpFlag && g_groundHit)
		{
			m_jumpFlag = TRUE;
			m_ySpeed =8.25f;
			g_motiontype = MOTION_JAMP;
			flag=true;
			g_groundHit = false;
			m_pSkin->SetSkinMotion(g_motiontype);
			m_pSkin->SkinMotionResetTime(MOTION_JAMP);
		}

	}

	// アクションモーションリセット
	if (!g_groundHit || g_motiontype == MOTION_PANCH || g_motiontype == MOTION_JUMP_PANCH || g_motiontype == MOTION_DAMAGE || g_motiontype == MOTION_DOWN)
	{
		if(m_pSkin!=NULL)
		{
			switch(g_motiontype)
			{
				case MOTION_PANCH:
				if(m_pSkin->Timeget()>19.0 / 60)
				{
					m_pSkin->SkinMotionResetTime(g_motiontype);
					g_motiontype = MOTION_WAIT;
					m_pSkin->SetSkinMotion(g_motiontype);
				}
				break;
				case MOTION_JUMP_PANCH:
				if(m_pSkin->Timeget()>13.5 / 60)
				{
					m_pSkin->SkinMotionResetTime(g_motiontype);
					g_motiontype = MOTION_WAIT;
					m_pSkin->SetSkinMotion(g_motiontype);
				}
				break;
				case MOTION_DAMAGE:
				if(m_pSkin->Timeget()>17.0 / 60)
				{
					
					g_motiontype = MOTION_DOWN;
					m_pSkin->SetSkinMotion(g_motiontype);
					m_pSkin->ChangeSkinMotion(MOTION_DOWN);
				}
				break;
				case MOTION_DOWN:
					g_motiontype = MOTION_DOWN;
				break;
			}

		}
	}
	else
	{
		if(m_pSkin!=NULL)
		{
			g_motiontype = MOTION_WAIT;
			m_pSkin->SetSkinMotion(g_motiontype);
		}
	}

	if(moveflag&&!g_jumpFlag&&g_motiontype != MOTION_DAMAGE&&g_groundHit)
	{
		if(m_pSkin!=NULL)
		{
			g_motiontype = MOTION_RUN;
			m_pSkin->SetSkinMotion(g_motiontype);
		}
	}

	// 重力
	if(!Rideflag)
	{
		m_ySpeed -=GURAVITY;
		g_groundHit = false;

	}

	m_rotDest.y = Invers(m_rotDest.y);

// 目的の角度までの差分
	fDiffRotY = m_rotDest.y - m_rot.y;
	if(fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if(fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}


	// 目的の角度まで慣性をかける
	m_rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	m_rot.y = Invers(m_rot.y);
	if(m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if(m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}


	m_xSpeed *= DRIVE;
	m_zSpeed *= DRIVE;

	//加速度つけたら
	m_pos.x+=m_xSpeed;
	m_pos.y+=m_ySpeed;
	m_pos.z+=m_zSpeed;

	CGimmick *pt = CGame::CTestAddrPresentAddrPresent(0);
	D3DXVECTOR3 vecLine,vecToTarget;
	float cross=0;

	if(pt&&m_pObb)
	{
		WorldPos pV[CUBE_MAX];
		TYPE_OBJ Gimmick[CUBE_MAX];

			pt->GetPosObj(&pV[0]);
			pt->GetTypeObj(&Gimmick[0]);

			for (int cube = 0; cube < CUBE_MAX; cube++)
			{

				D3DXVECTOR3 GimicVertexMin = pt->GetVertexMin();			// 最少頂点
				D3DXVECTOR3 GimicVertexMax = pt->GetVertexMax();			// 最大頂点
				D3DXVECTOR3 fGimicPos = D3DXVECTOR3(pV[cube].x, pV[cube].y, pV[cube].z);					// ギミック座標
				D3DXVECTOR3 GimicRot = pt->GetRot();						// ギミック角度
				D3DXVECTOR3 GimicRotDest = pt->GetRotDest();				// ギミック角度
				D3DXVECTOR3 pVtex[4];
				D3DXVECTOR3 offset = m_pos - fGimicPos;


				pVtex[0].x = fGimicPos.x + GimicVertexMin.x;
				pVtex[0].z = fGimicPos.z + GimicVertexMax.z;
				pVtex[1].x = fGimicPos.x + GimicVertexMax.x;
				pVtex[1].z = fGimicPos.z + GimicVertexMax.z;
				pVtex[2].x = fGimicPos.x + GimicVertexMax.x;
				pVtex[2].z = fGimicPos.z + GimicVertexMin.z;
				pVtex[3].x = fGimicPos.x + GimicVertexMin.x;
				pVtex[3].z = fGimicPos.z + GimicVertexMin.z;

				vecToTarget.x = fGimicPos.x + cos(-GimicRot.y)*offset.x + sin(-GimicRot.y)*offset.z;

				vecToTarget.z = fGimicPos.z + cos(-GimicRot.y)*offset.z + sin(-GimicRot.y)*offset.x;

				for (int i = 0; i < 4; i++)
				{
					vecLine = pVtex[(i + 1) % 4] - pVtex[i];

					vecToTarget = (m_pos - pVtex[i]);

					cross = vecToTarget.x*vecLine.z - vecToTarget.z*vecLine.x;

					// プレイヤーがギミックの範囲に入っていなかったら中断
					if (cross < 0.0f)
					{
						Inflag = FALSE;
						Rideflag = FALSE;
						break;
					}
					// プレイヤーがギミックの範囲に入っていたら
					else
					{
						Inflag = TRUE;
					}
				}

				if (Inflag == TRUE)
				{
					// ギミック当たり判定
					D3DXVECTOR3 GimickPosDest = fGimicPos;
					float GimickUpHeight = fGimicPos.y + GimicVertexMax.y;
					float GimickUpHeightDest = GimickPosDest.y + GimicVertexMax.y;
					float GimickDownHeight = fGimicPos.y + GimicVertexMin.y;
					float GimickDownHeightDest = GimickPosDest.y + GimicVertexMin.y;

					//ギミック上部からの当たり判定
					if (GimickUpHeight > m_pos.y && GimickUpHeightDest <= m_posDest.y || GimickUpHeight <= m_posDest.y && GimickUpHeight > m_pos.y)
					{
						// 上からめり込んだ
						m_pos.y = GimickUpHeight;
						m_posDest.y = GimickUpHeight;
						m_ySpeed = 0.0f;
						Rideflag = TRUE;
						if (Gimmick[cube] == 3)
						{
							SetJudg(GAME_CLEAR);
						}
					}
					//ギミック下部からの当たり判定
					else if (m_pos.y + PLAYER_HEIGHT > GimickDownHeight && m_posDest.y + PLAYER_HEIGHT <= GimickDownHeight)
					{
						// 下からめり込んだ
						m_pos.y = GimickDownHeight - PLAYER_HEIGHT;
						m_posDest.y = GimickDownHeight - PLAYER_HEIGHT;
						m_ySpeed = 0.0f;
					}

					if (Rideflag)
					{
						g_groundHit = true;
						m_jumpFlag = false;
						m_pos.y = GimickUpHeight;
					}
				}


				D3DXVECTOR3 pos;
				pos.x = pV[cube].x;
				pos.y = pV[cube].y;
				pos.z = pV[cube].z;

				D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;
				// World Matrix
				D3DXMatrixIdentity(&mtxWorld);															// 初期化

				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);

				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

				//　ギミック側面と当たったら
				if (CheckCollision(&m_pObb->GetVertexMin(), &m_pObb->GetVertexMax(), &m_pObb->GetMatrix(), &pt->GetVertexMin(), &pt->GetVertexMax(), &mtxWorld) == -1)
				{
					D3DXVECTOR3 gimicNormal = pos - m_pos;

					//反転
					gimicNormal *= -1;

					gimicNormal.y = 0;
					gimicNormal.x *= CHARA_HIT_SLIDE_DISTANCE;
					gimicNormal.z *= CHARA_HIT_SLIDE_DISTANCE;
					m_pos.x = m_pos.x + gimicNormal.x;
					m_pos.y = m_pos.y + gimicNormal.y;
					m_pos.z = m_pos.z + gimicNormal.z;
					m_xSpeed = 0;
					m_zSpeed = 0;

				}
				if (Gimmick [cube]!= 1)
				{
					if (g_motiontype == MOTION_PANCH || g_motiontype == MOTION_JUMP_PANCH)
					{
						if(CheckCollision( &m_pWeponObb->GetVertexMin(), &m_pWeponObb->GetVertexMax(), &m_pWeponObb->GetMatrix(),&pt->GetVertexMin(), &pt->GetVertexMax(), &mtxWorld)==-1)
						{
							CExplotion::Create(m_pPlayerDevice,"data\\TEXTURE\\exp.png",pos,m_rot);
							PlaySound(SOUND_LABEL_SE_EXPLOSION,0);
							pt->SetPosObj(D3DXVECTOR3(-10000.0f,-10000.0f,-10000.0f),cube);



							if (Gimmick[cube] == 2)
							{
								CExplotion::Create(m_pPlayerDevice,"data\\TEXTURE\\exp.png",m_pos,m_rot);
								PlaySound(SOUND_LABEL_SE_HIT001,0);
								m_plyerState = PLAYERM_DAMEGE;
								CLife *p = CGame::CLifeAddrPresent();
								int lifePoint = p->GetLife();
								flag=true;
								if(lifePoint>0)
								{
									lifePoint-= damagePoint;
									p->SetLife(lifePoint);
									damagePoint*=10;
								}
								g_motiontype = MOTION_DAMAGE;
								g_motionLastType = MOTION_DAMAGE;
								g_motiontype = MOTION_DAMAGE;
								m_pSkin->SetSkinMotion(g_motiontype);
								m_pSkin->SkinMotionResetTime(MOTION_DAMAGE);
								m_pSkin->ChangeSkinMotion(MOTION_DAMAGE);
							}
						}
					}
				}

			}
	}


	// 壁の当たり判定
	// 壁にあたったら
	if( MarginalDomainWallHitCheck(m_pos) )
	{
		m_xSpeed =0.0f;
		m_zSpeed =0.0f;
		m_pos.x = m_posDest.x;
		m_pos.z = m_posDest.z;
	}

	if(m_pMeshFieldDummy!=NULL)
	{
		//ジャンプ処理
		if(m_pos.y <= GetHeight(m_pos,&D3DXVECTOR3(0.0f,0.0f,0.0f),m_pMeshFieldDummy->SetMapVtx()))
		{

			// 起伏用当たり判定

			m_pos.y = GetHeight(m_pos,&D3DXVECTOR3(0.0f,0.0f,0.0f),m_pMeshFieldDummy->SetMapVtx());
			g_groundHit = true;
			m_jumpFlag=false;
			m_ySpeed=0.0f;
	
		}
	}

	if(m_plyerState==PLAYERM_DAMEGE)
	{
		m_evtTime++;
		m_pos.y+=5;
		if(m_evtTime>18)
		{
			m_plyerState = PLAYERM_INVINCIBLE;
			m_evtTime=0;
		}

	}


	if(m_plyerState==PLAYERM_INVINCIBLE)
	{
		CLife *p = CGame::CLifeAddrPresent();
		int lifePoint = p->GetLife();
		m_evtTime++;

		if(m_evtTime>120)
		{
			m_plyerState = PLAYERM_NOMAL;
			m_evtTime=0;

			if(lifePoint<=0)
			{
				m_plyerState = PLAYERM_STOP;
				PlaySound(SOUND_LABEL_SE_EXPLOSION,0);
				CExplotion::Create(m_pPlayerDevice,"data\\TEXTURE\\exp.png",m_pos,m_rot);
				SetJudg(GAME_OVER);

			}
		}

		if(lifePoint<=0)
		{
			m_flagMove = FALSE;
		}

	}


	if(m_pSkin!=NULL)
	{
		if(m_pShadow)
		{
			m_pShadow->SetPos(m_pos);
		}

		m_pObb->SetPos(D3DXVECTOR3(m_pos.x,m_pos.y,m_pos.z));
	
		m_pSkin->SetPos(m_pos);
		m_pSkin->SetRot(m_rot);
	}
	
	CPlayerMotion::SetDebugPlayerPos();
	CPlayerMotion::SetDebugFieldPos();

}
//=============================================================================
// 描画処理
//=============================================================================
void CPlayerMotion::Draw(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	if(m_pSkin!=NULL)
	{
		m_pSkin->Draw(m_pD3DDevice);
	}
}
void CPlayerMotion::SetPos(D3DXVECTOR3 set_pos)
{
	m_pos = set_pos;
}
D3DXVECTOR3 CPlayerMotion::GetPos(void)
{
	return m_pos;
}
D3DXVECTOR3 CPlayerMotion::GetPosDest(void)
{
	return m_pos;
}
bool CPlayerMotion::GetMoveSpeedchek(void)
{
	if(m_plyerState != PLAYERM_STOP&&m_posDest.x==m_pos.x&&m_posDest.z==m_pos.z&&m_posDest.y==m_pos.y&&g_moveflag==TRUE)
	{
		g_moveflag=FALSE;
		return true;
	}

		return false;
}

void CPlayerMotion::SetRot(D3DXVECTOR3 set_rot)
{
		m_rot = set_rot;
}
D3DXVECTOR3 CPlayerMotion::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CPlayerMotion::LoadTexture(LPDIRECT3DDEVICE9 m_pD3DDevice)
{

	//--------テクスチャの読み込み----------//

	D3DXCreateTextureFromFile(m_pD3DDevice,"data\\TEXTURE\\white.jpg",&m_pD3DTextureModel);
}
// デバッグモード時　プレイヤー座標観測用

void CPlayerMotion::SetDebugPlayerPos(void)
{
	D_pos = m_pos;
}
void CPlayerMotion::SetDebugFieldPos(void)
{
	if(m_pMeshFieldDummy!=NULL)
	{
		F_pos = GetHeight(m_pos,&D3DXVECTOR3(0.0f,0.0f,0.0f),m_pMeshFieldDummy->SetMapVtx());
	}
}
D3DXVECTOR3 DebugMotionPlayerPos(void)
{
	return D_pos;
}
float DebugFieldMotionPlayerPos(void)
{
	return F_pos;
}

//=============================================================================
// 起伏用当たり判定
//=============================================================================
float CPlayerMotion::GetHeight(D3DXVECTOR3 pos,D3DXVECTOR3* pNomal,VERTEX_3D * m_HitCheckMap)
{
	D3DXVECTOR3 vec0,vec1,vec2;
	bool flag = false;
	int x=0,z=0;
	float searchX =0.0f,searchZ =0.0f;
	//m_pos.z = (int)m_pos.z;

	// 座標を調整(左上を0とする)
	searchX=pos.x+2000.0f;
	searchZ=pos.z-2000.0f;

	//searchX=pos.x+200.0f;
	//searchZ=pos.z-200.0f;

	if(searchZ>0.0f||searchX<0.0f)
	{
		return 0.0f;
	}
	// 正符号に
	searchZ*=-1;

	// サーチ
	x =(int)searchX /40;
	z =(int)searchZ /40;

	// 範囲外なら終了
	if(NUMMESHFIELDX-1<=x||NUMMESHFIELDX-1<=z)
	{
		return 0.0f;
	}

	//m_HitCheckMap[z*NUMMESHFIELDX+x].vtx;					// p0
	//m_HitCheckMap[z*NUMMESHFIELDX+x+1].vtx;					// p1
	//m_HitCheckMap[(z+1)*NUMMESHFIELDX+x].vtx;				// p2
	//m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;					// p3




	vec0 = m_HitCheckMap[z*NUMMESHFIELDX+x].vtx - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x].vtx;

	vec1 = pos - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x].vtx;

	if(vec0.z*vec1.x-vec0.x*vec1.z<0)
	{
		flag=true;
	}

	vec1 = m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx -  m_HitCheckMap[z*NUMMESHFIELDX+x].vtx;

	vec2 = pos -  m_HitCheckMap[z*NUMMESHFIELDX+x].vtx;

	if(vec1.z*vec2.x-vec1.x*vec2.z<0)
	{
		flag=true;
	}

	vec2 = m_HitCheckMap[(z+1)*NUMMESHFIELDX+x].vtx - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;

	vec0 = pos - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;

	if(vec2.z*vec0.x-vec2.x*vec0.z<0)
	{
		flag=true;
	}

	//
	if(flag)
	{
		vec0 = m_HitCheckMap[z*NUMMESHFIELDX+x].vtx - m_HitCheckMap[z*NUMMESHFIELDX+x].vtx;

		vec1 = pos - m_HitCheckMap[z*NUMMESHFIELDX+x].vtx;

		if(vec0.z*vec1.x-vec0.x*vec1.z<0)
		{
			return 0.0f;
		}

		vec1 = m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;

		vec2 = pos - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;

		if( vec1.z*vec2.x-vec1.x*vec2.z<0 )
		{
			return 0.0f;
		}

		vec2 = m_HitCheckMap[z*NUMMESHFIELDX+x].vtx - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;

		vec0 = pos - m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx;

		if( vec2.z*vec0.x - vec2.x*vec0.z<0 )
		{
			return 0.0f;
		}
	}



	if(flag)
	{
		return GetHeightPolygon( m_HitCheckMap[z*NUMMESHFIELDX+x].vtx, m_HitCheckMap[z*NUMMESHFIELDX+x+1].vtx , m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx, pos, pNomal );
	}

	return GetHeightPolygon( m_HitCheckMap[z*NUMMESHFIELDX+x].vtx, m_HitCheckMap[(z+1)*NUMMESHFIELDX+x].vtx, m_HitCheckMap[(z+1)*NUMMESHFIELDX+x+1].vtx, pos, pNomal );


}
float CPlayerMotion::GetHeightPolygon(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,D3DXVECTOR3& pos,D3DXVECTOR3* pNomal)
{
	D3DXVECTOR3 vec0,vec1;

	vec0=p1-p0;
	vec1=p2-p0;

	D3DXVec3Cross(pNomal,&vec0,&vec1);
	D3DXVec3Normalize(pNomal,pNomal);

	if(pNomal->y==0.0f)
	{
		return 0.0f;
	}

	return p0.y-(pNomal->x*(pos.x-p0.x)+pNomal->z*(pos.z-p0.z))/pNomal->y;
}
float CPlayerMotion::RollCheak(float rot)
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
//範囲に入っているかのチェック
bool IncheckMotionPlayer(void)
{
	if(Inflag)
	{
		return TRUE;
	}

	return FALSE;
}

CPlayerMotion* CPlayerMotion::GetCPlayerAddress(void)
{
	return m_pPlayer;
}
CSceneBoundingObj *CPlayerMotion::GetOBB(void)
{
	return m_pWeponObb;
}
void CPlayerMotion::SetMatrix(D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}