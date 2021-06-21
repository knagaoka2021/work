using System.Collections;
using System.Collections.Generic;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.UI;

public class FadeController : MonoBehaviour {
    private Image m_render;
    private float m_alpha = 0.0f;
    private float m_progress = 0.0f;
    private float m_start = 0.0f;
    private float m_end = 1.0f;
    private bool m_fadeFlag = false;
    [SerializeField]
    private bool isDebug = false;

    // Start is called before the first frame update
    void Start () {

        // コンポーネントをキャッシュ
        m_render = GameObject.FindWithTag ("Fade").GetComponent<Image> ();

        // alpha値を初期化
        m_render.material.SetFloat ("p_alpha", m_alpha);

        // alpha値更新
        this.UpdateAsObservable ()
            .Select (_ => m_alpha)
            .DistinctUntilChanged ()
            .Subscribe (alpha => {
                m_render.material.SetFloat ("p_alpha", m_alpha);
            }).AddTo (this);

        if (isDebug) {
            m_alpha = 1.0f;
        }

    }

    // Update is called once per frame
    void FixedUpdate () {
        if (m_fadeFlag) {
            m_progress = m_progress + Time.deltaTime;
            m_alpha = Mathf.Lerp (m_start, m_end, m_progress);
        }

        if (m_fadeFlag && m_progress > 1.0f) {
            m_fadeFlag = false;
            m_render.raycastTarget = false;
        }

    }

    public void SetFadeIn () {
        m_start = 1.0f;
        m_end = 0.0f;
        m_progress = 0.0f;
        m_fadeFlag = true;
        m_render.raycastTarget = true;
    }
    public void SetFadeOut () {
        m_start = 0.0f;
        m_end = 1.0f;
        m_progress = 0.0f;
        m_fadeFlag = true;
    }
    public bool IsFade () {
        return m_fadeFlag;
    }
}