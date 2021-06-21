//==========================================-
//	�C���N���[�h
//==========================================-
#include "player.h"
#include "map.h"

//==========================================-
//	�ϐ��錾
//==========================================-
static PLAYER player;	//�v���[���[���

//=============================
//	�v���[���[���擾�֐�
//�����F��
//�ߒl�FPLAYER*		�v���[���[���|�C���^
//=============================
PLAYER* GetPlayerInfo(void)
{
	return(&player);
}

//=============================
//	�v���[���[�̏�����
//�����F��
//�ߒl�F��
//=============================
void PlayerInit(void)
{
    // �}�b�v�f�[�^�擾
    MAPDATA* pMapData = GeMapInfo();

    // �v���C���[������
    player.posX = pMapData->initPosX;
    player.posY = pMapData->initPosY;
    player.direction = DIRECTION_FRONT;
}

//=============================
//	�v���[���[�̏I������
//�����F��
//�ߒl�F��
//=============================
void PlayerUnInit(void)
{
    // �}�b�v�f�[�^�擾
    MAPDATA* pMapData = GeMapInfo();

    // �v���C���[������
    player.posX = pMapData->initPosX;
    player.posY = pMapData->initPosY;
    player.direction = DIRECTION_FRONT;
}

//=============================
//	�v���[���[�̍X�V����
//�����F��
//�ߒl�F��
//=============================
void PlayerUpdate(void)
{
    PLAYER* pPlayer = GetPlayerInfo();
    MAPDATA* pMap = GeMapInfo();

    // ���͏���
    if (INP(PK_UP))    // [��]�̉�����Ԃ��擾����B
    {
        pPlayer = moveDirection(pMap, pPlayer, 1);
        updateMap(pPlayer);

    }
    if (INP(PK_RIGHT))    // [��]�̉�����Ԃ��擾����B
    {
        pPlayer->direction = changeDirection(pPlayer->direction, 1);
        updateMap(pPlayer);
    }
    if (INP(PK_LEFT))    // [��]�̉�����Ԃ��擾����B
    {
        pPlayer->direction = changeDirection(pPlayer->direction, -1);
        updateMap(pPlayer);
    }
    if (INP(PK_DOWN))    // [��]�̉�����Ԃ��擾����B
    {
        pPlayer = moveDirection(pMap, pPlayer, -1);
        updateMap(pPlayer);
    }
}