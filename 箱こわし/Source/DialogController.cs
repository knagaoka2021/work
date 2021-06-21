using System;
using CommonProperty;
using TMPro;
using UnityEngine;

public class DialogController : MonoBehaviour {

    private LoadScene loadScene;
    private RetryLoad retry;
    private string m_stageName;
    private Const.SCENE m_scene;
    private PauseController m_pause;

    void Start () {
        loadScene = GameObject.FindWithTag ("Manager").GetComponent<LoadScene> ();
        retry = GameObject.FindWithTag ("Manager").GetComponent<RetryLoad> ();
        m_pause = GameObject.FindWithTag ("Manager").GetComponent<PauseController> ();
    }

    // OKボタンが押されたとき
    public void OnOk () {
        switch (m_scene) {
            case Const.SCENE.TITLE:
                m_scene = Const.SCENE.STAGESELECT;
                break;
            case Const.SCENE.STAGESELECT:
                loadScene.StageSelect (m_stageName);
                break;
            case Const.SCENE.GAME:
                m_pause.EndPause ();
                loadScene.Title ();
                break;
        }
        Destroy (this.gameObject);
    }

    // Cancelボタンが押されたとき
    public void OnCancel () {
        switch (m_scene) {
            case Const.SCENE.GAME:
                if (!retry.isReturn ()) {
                    retry.Retry ();
                } else {
                    retry.SetReturn (false);
                    m_pause.EndPause ();
                }

                break;
        }
        Destroy (this.gameObject);
    }
    public void SetStageName (string name) {
        m_stageName = name;
    }
    public void SetMessageText (string text) {
        Transform[] childrens = this.GetComponentsInChildren<Transform> ();

        foreach (Transform obj in childrens) {

            if (obj.name == Const.STRING_MESSAGETEXT) {
                obj.GetComponent<TextMeshProUGUI> ().text = text;
            }

        }
    }
    public void SetScene (Const.SCENE scene) {
        m_scene = scene;
    }
}