using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Stage : MonoBehaviour {
    private string m_StageName = "";
    private int m_XNum = 0;
    private int m_YNum = 0;
    private int m_ZNum = 0;
    private int m_AllNum = 0;
    private float m_XOfset = 0.0f;
    private float m_YOfset = 0.0f;
    private float m_ZOfset = 0.0f;

    public void Create (string name, int XNum, int YNum, int ZNum, float XOfset, float YOfset, float ZOfset) {
        m_StageName = name;
        m_XNum = XNum;
        m_YNum = YNum;
        m_ZNum = ZNum;
        m_AllNum = m_XNum * m_YNum * m_ZNum;
        m_XOfset = XOfset;
        m_YOfset = YOfset;
        m_ZOfset = ZOfset;
    }
    public Stage GetStage () {
        return this;
    }
    public string GetStageName () {
        return m_StageName;
    }
    public int GetXNum () {
        return m_XNum;
    }
    public int GetYNum () {
        return m_YNum;
    }
    public int GetZNum () {
        return m_ZNum;
    }
    public int GetAllNum () {
        return m_AllNum;
    }
    public float GetXOfset () {
        return m_XOfset;
    }
    public float GetYOfset () {
        return m_YOfset;
    }
    public float GetZOfset () {
        return m_ZOfset;
    }
}