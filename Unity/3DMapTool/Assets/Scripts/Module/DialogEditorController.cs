using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using TMPro;
using UnityEngine;

public class DialogEditorController : MonoBehaviour {
    private GameObject m_prev;
    private Material m_prevMat;
    private Dictionary<string, Material> m_dic = new Dictionary<string, Material> ();

    void Start () { }

    // OKボタンが押されたとき
    public void OnOk () {
        Destroy (m_prev);
        Destroy (this.gameObject);
    }

    // Cancelボタンが押されたとき
    public void OnCancel () {
        if (m_prevMat != null) {
            m_prev.GetComponent<Renderer> ().material = m_prevMat;
        } else if (m_dic != null) {
            foreach (KeyValuePair<string, Material> kvp in m_dic) {
                foreach (Renderer child in m_prev.transform.GetComponentsInChildren<Renderer> ()) {
                    if (child.name == kvp.Key) {
                        child.material = kvp.Value;
                    }
                }
            }
        }

        Destroy (this.gameObject);
    }
    public void SetMessageText (string text) {
        Transform[] childrens = this.GetComponentsInChildren<Transform> ();

        foreach (Transform obj in childrens) {
            if (obj.name == Const.STRING_MESSAGETEXT) {
                obj.GetComponent<TextMeshProUGUI> ().text = text;
            }
        }
    }

    public void SetPrevObject (GameObject obj) {
        m_prev = obj;
    }
    public void SetPrevMaterial (Material mat) {
        m_prevMat = mat;
    }
    public void SetDictionaryMaterial (Dictionary<string, Material> dic) {
        m_dic = dic;
    }
}