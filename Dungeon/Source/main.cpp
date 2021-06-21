//============================================================================*
// [CScreen.h] Win32 console ���C��
// @version 1.0
// @author kazuma.nagaoka
//============================================================================*
#include "main.h"
#include "aplication.h"


//=============================================================================
// ���C���֐�
//=============================================================================
int main(int argc, char* argv[])
{

    int iTime, iTimeOld;

    timeBeginPeriod(1);
    iTimeOld = timeGetTime();

    //������
    AppInit();

    //���C�����[�v
    while (1)
    {
        iTime = timeGetTime();
        //�t���[�����[�g�̌Œ艻
        if (FRAMERATE <= iTime - iTimeOld)
        {
            iTimeOld = iTime;

            //�X�V����
            AppUpdate();
            //�`�揈��
            AppDraw();

        }

    }
    //�I������
    AppUnInit();
    //�^�C�}�[�̐��x�����ɖ߂�
    timeEndPeriod(1);

    return 0;
}