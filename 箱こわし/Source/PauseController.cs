using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseController : MonoBehaviour {
    private bool m_isPause = false;

    public void StartPause () {
        Time.timeScale = 0f;
        m_isPause = true;
    }
    public void EndPause () {
        Time.timeScale = 1f;
        m_isPause = false;
    }
    public bool IsPause(){
        return m_isPause;
    }
}