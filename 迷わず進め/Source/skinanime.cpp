#include "skinanime.h"
#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOOPTIME (0.2f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �\����
//*****************************************************************************
struct HLANIMATION_DESC
{
	int uiAnimID;                 // �A�j���[�V����ID
	double fLoopTime;               // 1���[�v�̎���
	double fTrackSpeed;             // �g���b�N�X�s�[�h���ߒl
	double fShiftTime;              // �V�t�g����̂ɂ����鎞��
	double fCurWeightTime;          // ���݂̃E�F�C�g����
};
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

HLANIMATION_DESC g_animdata[100];

//=============================================================================
// �N���G�C�g
//=============================================================================
XFileMesh::XFileMesh(LPCTSTR filename, LPDIRECT3DDEVICE9 pDev) : pDevice(pDev) 
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	if (FAILED( D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pD3DXMtrlBuffer, NULL, &dwNumMaterials, &pMesh))) throw TEXT("X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
    pMeshTextures  = new LPDIRECT3DTEXTURE9[dwNumMaterials];
	for (DWORD i = 0; i < dwNumMaterials; ++i) 
	{ 
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;	
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
        pMeshTextures[i] = NULL;

		// �e�N�X�`���t�@�C������Unicode�Ή�
		TCHAR textureFilename[256] = {0};
#ifdef UNICODE
		if (d3dxMaterials[i].pTextureFilename) MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED, d3dxMaterials[i].pTextureFilename, strlen(d3dxMaterials[i].pTextureFilename), textureFilename, (sizeof textureFilename)/2);
#else
		if (d3dxMaterials[i].pTextureFilename) strcpy_s(textureFilename, d3dxMaterials[i].pTextureFilename);
#endif
		if (textureFilename != NULL && lstrlen(textureFilename) > 0)
			if(FAILED(D3DXCreateTextureFromFileEx(pDevice, textureFilename, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xff000000, 0, 0, &pMeshTextures[i]))) throw TEXT("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���");
	}
	pD3DXMtrlBuffer->Release();
}

XFileMesh::~XFileMesh() {
	SAFE_RELEASE(pMesh);
	for (DWORD i = 0; i < dwNumMaterials; ++i) {
		SAFE_RELEASE(pMeshTextures[i]);
	}
	SAFE_DELETE(pMeshTextures);
	SAFE_DELETE(pMeshMaterials);
}
//=============================================================================
// �`�揈��
//=============================================================================
void XFileMesh::Draw(D3DXVECTOR3 position, D3DXMATRIX attitude) 
{ 
	D3DXMATRIX matWorld, matTrans;
	D3DXMatrixTranslation(&matTrans, position.x, position.y, position.z);
	matWorld = attitude*matTrans;
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �|���S����Diffuse�F�̓����x���e�N�X�`���ɔ��f������
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	for (DWORD i = 0; i < dwNumMaterials; ++i) {
		pDevice->SetMaterial(&pMeshMaterials[i]);
		pDevice->SetTexture(0, pMeshTextures[i]); 
		pMesh->DrawSubset(i);
    }
}

LPD3DXMESH XFileMesh::GetMesh() { return pMesh; }


///// ���b�V���p�J���� /////
MeshCamera::MeshCamera(LPDIRECT3DDEVICE9 pDev) : pDevice(pDev) {
}

MeshCamera::~MeshCamera() {
}

void MeshCamera::Look(D3DXVECTOR3 eyePoint, D3DXVECTOR3 lookAtPoint) 
{
}

///// ���b�V���p���C�g /////
MeshLight::MeshLight(LPDIRECT3DDEVICE9 pDev) : pDevice(pDev) {
	const D3DCOLORVALUE diffuse = {1, 1, 1, 1};	// �f�B�t���[�Y
	const D3DCOLORVALUE specular = {1, 1, 1, 1};	// �X�y�L�����[
	const D3DCOLORVALUE ambient = {1, 1, 1, 1};	// �A���r�G���g
	const float range = 200.0f;							// ����
	const D3DXVECTOR3 direction(-0.1f, -1.0f, -0.1f);	// ����
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient	= ambient;
	light.Diffuse	= diffuse;
	light.Specular	= specular;
	light.Range     = range;
	light.Direction = direction;
}

MeshLight::~MeshLight() {
}

void MeshLight::Illume(D3DXVECTOR3 direction) {
	light.Direction = direction;
	pDevice->SetLight( 0, &light );
	pDevice->LightEnable( 0, TRUE );
}


///// �A�j���[�V�������b�V�� /////
HRESULT XFileAnimationMesh::MyHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame) 
{
    HRESULT hr = S_OK;
    MYFRAME *pFrame;
    *ppNewFrame = NULL;
    pFrame = new MYFRAME;
    if (!pFrame) return E_OUTOFMEMORY;
	pFrame->Name = (LPSTR)(new TCHAR[strlen(Name) + 1]);
    if (!pFrame->Name) return E_FAIL;
	strcpy_s(pFrame->Name, strlen(Name) + 1, Name);
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;
    *ppNewFrame = pFrame;
    return S_OK;
}

void XFileAnimationMesh::MyHierarchy::CreateTexture(MYMESHCONTAINER* pMeshContainer, LPDIRECT3DDEVICE9 pDevice, DWORD NumMaterials) {
	for (DWORD iMaterial = 0; iMaterial < NumMaterials; ++iMaterial) {
		if (pMeshContainer->pMaterials[iMaterial].pTextureFilename) {
			// �e�N�X�`���t�@�C������Unicode�Ή�
			TCHAR strTexturePath[MAX_PATH] = {0};
#ifdef UNICODE
			MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, pMeshContainer->pMaterials[iMaterial].pTextureFilename, strlen(pMeshContainer->pMaterials[iMaterial].pTextureFilename), strTexturePath, (sizeof strTexturePath)/2);
#else
			strcpy_s(strTexturePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
#endif
			if(FAILED( D3DXCreateTextureFromFile(pDevice, strTexturePath, &pMeshContainer->ppTextures[iMaterial]))) pMeshContainer->ppTextures[iMaterial] = NULL;
			pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
		}
	}
}

void XFileAnimationMesh::MyHierarchy::SetDefaultMaterial(MYMESHCONTAINER* pMeshContainer) {
	pMeshContainer->pMaterials[0].pTextureFilename = NULL;
	memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
	pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
	pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
	pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
	pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
}

HRESULT XFileAnimationMesh::MyHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData, CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppMeshContainer) {
    MYMESHCONTAINER *pMeshContainer = NULL;
    INT iFacesAmount;
    LPDIRECT3DDEVICE9 pDevice = NULL;
    LPD3DXMESH pMesh = NULL;
    *ppMeshContainer = NULL;
	DWORD dwBoneAmt=0;
    pMesh = pMeshData->pMesh;
    pMeshContainer = new MYMESHCONTAINER;
    if (!pMeshContainer) return E_OUTOFMEMORY;
    ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	pMeshContainer->Name = (LPSTR)(new TCHAR[strlen(Name) + 1]);	
    if (!pMeshContainer->Name) return E_FAIL;
	strcpy_s((char*)pMeshContainer->Name, strlen(Name) + 1, Name);
    pMesh->GetDevice(&pDevice);
    iFacesAmount = pMesh->GetNumFaces();  
	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[iFacesAmount*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL)) return E_FAIL;
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)*pMeshContainer->NumMaterials);
    if (NumMaterials > 0) {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)*NumMaterials);
		CreateTexture(pMeshContainer, pDevice, NumMaterials);
    }
    else SetDefaultMaterial(pMeshContainer);
	if (pSkinInfo) {			//���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
		pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();
		dwBoneAmt = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneAmt];
		for (DWORD i = 0; i < dwBoneAmt; ++i) memcpy(&pMeshContainer->pBoneOffsetMatrices[i], pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(pMesh, NULL, pMeshContainer->pAdjacency, NULL, NULL, NULL, &pMeshContainer->dwWeight, &pMeshContainer->dwBoneAmt, &pMeshContainer->pBoneBuffer, &pMeshContainer->MeshData.pMesh))) return E_FAIL;
	}
	*ppMeshContainer = pMeshContainer;
	SAFE_RELEASE(pDevice);
    return S_OK;
}

HRESULT XFileAnimationMesh::MyHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) {
    SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
    return S_OK; 
}

HRESULT XFileAnimationMesh::MyHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase) {
    DWORD iMaterial;
    MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
    SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
    SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
    SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);
    if (pMeshContainer->ppTextures) for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; ++iMaterial) SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
    SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneBuffer);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);
    SAFE_DELETE(pMeshContainer);
    return S_OK;
}
///////////////////////////////////////////////////////
XFileAnimationMesh::XFileAnimationMesh(LPCTSTR filename, LPDIRECT3DDEVICE9 pDev) : pDevice(pDev) {
	if (FAILED(D3DXLoadMeshHierarchyFromX(filename, D3DXMESH_MANAGED, pDevice, &cHierarchy, NULL, &pFrameRoot, &pAnimController))) throw TEXT("X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");
	for(DWORD i = 0; i < pAnimController->GetNumAnimationSets(); ++i) {	//�A�j���[�V�����g���b�N�𓾂�
		D3DXTRACK_DESC TrackDesc;
		ZeroMemory(&TrackDesc,sizeof(TrackDesc));
		TrackDesc.Weight = 1;
		TrackDesc.Speed = 1;		TrackDesc.Enable = 1;
		pAnimController->SetTrackDesc(i, &TrackDesc);
		pAnimController->GetAnimationSet(i, &pAnimSet[i]);
		pAnimController->SetTrackAnimationSet(i, pAnimSet[i]);
	}
	AllocateAllBoneMatrices(pFrameRoot);
	m_MotionNumber = 0;
	m_lastMotionNumber = 0;
	///////////////////////////////////////////////////////////////
	for(DWORD i=0;i<pAnimController->GetNumAnimationSets();i++)
	{
		SetLoopTime(i, LOOPTIME);
		SetShiftTime(i,0.0);
	}
	resetMotion = FALSE;
	ChangeMotion(0);
}

XFileAnimationMesh::~XFileAnimationMesh() {
	FreeAnim(pFrameRoot);
	cHierarchy.DestroyFrame(pFrameRoot);
	SAFE_RELEASE( pAnimController);
}

void XFileAnimationMesh::FreeAnim(LPD3DXFRAME pF) {
	if (pF->pMeshContainer) cHierarchy.DestroyMeshContainer(pF->pMeshContainer);
    if (pF->pFrameSibling) FreeAnim(pF->pFrameSibling);
    if (pF->pFrameFirstChild) FreeAnim(pF->pFrameFirstChild);
}

HRESULT XFileAnimationMesh::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase) {
    MYFRAME *pFrame = NULL;
	DWORD dwBoneAmt = 0;
    MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
    if (!pMeshContainer->pSkinInfo) return S_OK;
	dwBoneAmt = pMeshContainer->pSkinInfo->GetNumBones();
    pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneAmt];
    for (DWORD i = 0; i < dwBoneAmt; ++i) {
		pFrame = (MYFRAME*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
        if (!pFrame) return E_FAIL;
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}    
    return S_OK;
}

HRESULT XFileAnimationMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrame) {
    if (pFrame->pMeshContainer) if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer))) return E_FAIL;
    if (pFrame->pFrameSibling) if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling))) return E_FAIL;
    if (pFrame->pFrameFirstChild) if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild))) return E_FAIL;
    return S_OK;
}

void XFileAnimationMesh::RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame) {
	DWORD i, k, dwBlendMatrixAmt, dwPrevBoneID;
    LPD3DXBONECOMBINATION pBoneCombination;
    UINT iMatrixIndex;
    D3DXMATRIX matStack;
	if (pMeshContainer->pSkinInfo)	// �X�L�����b�V���̏ꍇ
	{
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();
		dwPrevBoneID = UINT_MAX;

		for (i = 0; i < pMeshContainer->dwBoneAmt; ++i) 
		{
			dwBlendMatrixAmt = 0;

			for (k = 0; k < pMeshContainer->dwWeight; ++k)
			{
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					dwBlendMatrixAmt = k;
				}
					pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixAmt);
			}
			for (k = 0; k < pMeshContainer->dwWeight; ++k) 
			{
				iMatrixIndex = pBoneCombination[i].BoneId[k];

				if(iMatrixIndex != UINT_MAX)
				{
					matStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex]*(*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &matStack);
					//ArmMatrix = matStack;
				}
			}
			pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId]);
			dwPrevBoneID = pBoneCombination[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		}
	}
	else	// �ʏ탁�b�V���̏ꍇ
	{
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

void XFileAnimationMesh::DrawFrame(LPDIRECT3DDEVICE9 pDevice,LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;
	while (pMeshContainer)
	{
		RenderMeshContainer(pDevice,pMeshContainer, pFrame);
		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}
	if (pFrame->pFrameSibling) DrawFrame(pDevice,pFrame->pFrameSibling);
	if (pFrame->pFrameFirstChild) DrawFrame(pDevice,pFrame->pFrameFirstChild);
}

void XFileAnimationMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix) {
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;
    if (pParentMatrix)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
    else pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
    if (pFrame->pFrameSibling) UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    if (pFrame->pFrameFirstChild) UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
}
void XFileAnimationMesh::SearchMatrix(LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	while (pMeshContainer)
	{

		if (pMeshContainer->pSkinInfo)
		{
			for (int i = 0; i < pMeshContainer->dwBoneAmt; ++i) 
			{
				const char* aspoi = pMeshContainer->pSkinInfo->GetBoneName(i);
				if(i==0)
				{
					pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
					break;
				}

			
			}
			
		}
	}

	if (pFrame->pFrameSibling)
	{
		SearchMatrix(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild)
	{
		SearchMatrix(pFrame->pFrameFirstChild);
	}
}
bool XFileAnimationMesh::GetResetMotion(void)
{
	return resetMotion;
}
void XFileAnimationMesh::SetResetMotion(bool Motion)
{
	resetMotion = Motion;
}
D3DXMATRIX XFileAnimationMesh::GetPartsMatrix(char *partsName)
{

	if(pAnimController!=NULL)
	{
		D3DXMatrixIdentity(&ArmMatrix);
		MYFRAME* p = (MYFRAME*)D3DXFrameFind(pFrameRoot,partsName);
		ArmMatrix = p->CombinedTransformationMatrix;
	}

	return ArmMatrix;
}
void XFileAnimationMesh::Draw(D3DXVECTOR3 position, D3DXMATRIX attitude) {
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, position.x, position.y, position.z);
	D3DXMATRIX matWorld = attitude*matTrans;
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	UpdateFrameMatrices(pFrameRoot, &matWorld);
	DrawFrame(pDevice, pFrameRoot);

	
}

void XFileAnimationMesh::AdvanceTime(double time) 
{
	 // ���������ۂ��𔻒�
	g_animdata[m_MotionNumber].fCurWeightTime += time;
	float t = pAnimController->GetTime();
	pAnimController->SetTrackPosition(0,t);
	
	//// �ǂݍ��܂��A�j���[�V�����Z�b�g��4800�t���[����1�b
	if( g_animdata[m_MotionNumber].fCurWeightTime <= g_animdata[m_MotionNumber].fShiftTime )
	{
		// �������B�E�F�C�g���Z�o
		FLOAT Weight = g_animdata[m_MotionNumber].fCurWeightTime/g_animdata[m_MotionNumber].fShiftTime;
		// �E�F�C�g��o�^
		pAnimController->SetTrackWeight( 0, Weight );       // ���݂̃A�j���[�V����
		pAnimController->SetTrackWeight( 1, 1 - Weight );   // �O�̃A�j���[�V����
	}
	else
	{
		// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
		pAnimController->SetTrackWeight( 0, 1.0f );       // ���݂̃A�j���[�V����
		pAnimController->SetTrackEnable( 1, false );      // �O�̃A�j���[�V�����𖳌��ɂ���
	}

	pAnimController->AdvanceTime(time, NULL);
	
}
void XFileAnimationMesh::ResetTime(int motion_number)
{
 	//g_animdata[3].fCurWeightTime = 0.0;
	pAnimController->ResetTime();
}
bool XFileAnimationMesh::ChangeMotion(int motion_number) 
{
	
   // �قȂ�A�j���[�V�����ł��邩���`�F�b�N
   if( m_MotionNumber == motion_number )
      return false;


      //g_animdata[m_MotionNumber].fCurWeightTime = 0.0f;

   // ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
   D3DXTRACK_DESC TD;   // �g���b�N�̔\��
   pAnimController->GetTrackDesc( 0, &TD );

   // ���̃A�j���[�V�������g���b�N1�Ɉڍs��
   // �g���b�N�̐ݒ�l���ڍs
   pAnimController->SetTrackAnimationSet( 1, pAnimSet[m_MotionNumber] );
   pAnimController->SetTrackDesc( 1, &TD );

   // �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
   pAnimController->SetTrackAnimationSet( 0, pAnimSet[motion_number] );

   // �g���b�N0�̃X�s�[�h�̐ݒ�
   pAnimController->SetTrackSpeed( 0, g_animdata[motion_number].fTrackSpeed );

   // �g���b�N�̍���������
   pAnimController->SetTrackEnable( 0, true );
   pAnimController->SetTrackEnable( 1, true );

   // �E�F�C�g���Ԃ�������
	g_animdata[motion_number].fCurWeightTime = 0.0f;


   // ���݂̃A�j���[�V�����ԍ���؂�ւ�
   m_lastMotionNumber = m_MotionNumber;
   m_MotionNumber = motion_number;
   return true;

}
int XFileAnimationMesh::GetMotionType(void)
{
	return m_MotionNumber;
}
void XFileAnimationMesh::SetMotionType(int motionNum)
{
	m_MotionNumber = motionNum;
}
bool XFileAnimationMesh::SetLoopTime( int animID, double time )
{

   // �g���b�N�X�s�[�h���ߒl���Z�o
   FLOAT DefTime = pAnimSet[animID]->GetPeriod();
   g_animdata[animID].fLoopTime = time;
   g_animdata[animID].fTrackSpeed = DefTime / time;

   return true;
}
bool XFileAnimationMesh::SetShiftTime( int animID, double interval )
{

   // �V�t�g���Ԃ�o�^
	g_animdata[animID].fShiftTime = interval;

   return true;
}
double XFileAnimationMesh::getloop(void)
{
	float time= pAnimController->GetTime();
	if(time>1.0)
		time/=10;
	return time;
}
