using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using TMPro;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.SceneManagement;

public class LoadScene : MonoBehaviour {

    private FadeController fadeController;
    private string m_stageName;

    void Start () {
        fadeController = GetComponent<FadeController> ();
    }
    void Update(){
        if (Input.GetKey(KeyCode.Escape)){
            Application.Quit();
         }       
    }
    public void onClick () {

        fadeController.SetFadeIn ();

        // fade完了したタイミングでシーン遷移
        this.UpdateAsObservable ()
            .Select (_ => fadeController.IsFade ())
            .Where (isfade => !isfade)
            .Take (1)
            .Subscribe (_ => {
                // イベントに登録
                SceneManager.sceneLoaded += GameSceneLoaded;
                // シーン切り替え
                SceneManager.LoadScene ("Game");
            }).AddTo (this);
    }
    private void GameSceneLoaded (Scene next, LoadSceneMode mode) {
        // シーン切り替え後のスクリプトを取得
        var stageManager = GameObject.FindWithTag ("Manager").GetComponent<StageController> ();
        var fadeController = GameObject.FindWithTag ("Manager").GetComponent<FadeController> ();

        stageManager.PassValue (m_stageName);

        fadeController.SetFadeOut ();

        // イベントから削除
        SceneManager.sceneLoaded -= GameSceneLoaded;
    }
    private void StageSelectSceneLoaded (Scene next, LoadSceneMode mode) {
        // シーン切り替え後のスクリプトを取得
        var fadeController = GameObject.FindWithTag ("Manager").GetComponent<FadeController> ();
        fadeController.SetFadeOut ();

        var stageSelectController = GameObject.FindWithTag ("Manager").GetComponent<StageSelectController> ();
        stageSelectController.SendData (m_stageName, Score.GetScore () + Score.GetScoreBuffer ());

        // イベントから削除
        SceneManager.sceneLoaded -= StageSelectSceneLoaded;
    }
    public void StageSelect (string name) {

        fadeController.SetFadeIn ();

        m_stageName = name;

        // fade完了したタイミングでシーン遷移
        this.UpdateAsObservable ()
            .Select (_ => fadeController.IsFade ())
            .Where (isfade => !isfade)
            .Take (1)
            .Subscribe (_ => {
                // イベントに登録
                SceneManager.sceneLoaded += GameSceneLoaded;
                // シーン切り替え
                SceneManager.LoadScene ("Game");
            }).AddTo (this);
    }
    public void Title () {

        fadeController.SetFadeIn ();

        var stageController = GameObject.FindWithTag ("Manager").GetComponent<StageController> ();

        if (stageController != null) {
            m_stageName = stageController.GetStageName ();
        }

        // fade完了したタイミングでシーン遷移
        this.UpdateAsObservable ()
            .Select (_ => fadeController.IsFade ())
            .Where (isfade => !isfade)
            .Take (1)
            .Subscribe (_ => {
                // イベントに登録
                SceneManager.sceneLoaded += StageSelectSceneLoaded;
                // シーン切り替え
                SceneManager.LoadScene ("StageSelect");
            }).AddTo (this);
    }
}