using CommonProperty;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.SceneManagement;

public class RetryLoad : MonoBehaviour {
    public string sceneName;
    private bool m_retrun = false;
    private PauseController m_pause;
    // Start is called before the first frame update
    void Start () {
        sceneName = SceneManager.GetActiveScene ().name;
        m_pause = GetComponent<PauseController>();
    }
    public void Retry () {

        LoadScene loadScene = GameObject.FindWithTag ("Manager").GetComponent<LoadScene> ();
        StageController stage = GameObject.FindWithTag ("Manager").GetComponent<StageController> ();
        loadScene.StageSelect (stage.GetStageName ());

    }
    public void Return () {

        OkCancelDialog dialog = GameObject.FindWithTag ("Manager").GetComponent<OkCancelDialog> ();

        if (dialog != null) {
            dialog.ShowDialog (Const.SCENE.GAME, "", Const.STRING_MESSAGETEXT02);
            m_retrun = true;
            m_pause.StartPause();
        }
    }
    public bool isReturn () {
        return m_retrun;
    }
    public void SetReturn (bool flag) {
        m_retrun = flag;
    }
}