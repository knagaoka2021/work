using System;
using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.UI;

public class TitleController : MonoBehaviour {
    private Stage m_stage;
    private GameTextController c1;
    private GameTextController c2;
    private int m_clearNum = 0;
    private string m_stageName = "default";

    void Awake () {
        // 遊ぶステージ情報を設定する
    }
    // Start is called before the first frame update
    void Start () {
        // ステージオブジェクトをキャッシュ
        m_stage = GetComponent<Stage> ();
        CreateStage ();
        // ステージオブジェクト数がクリア基準を満たした場合
        this.UpdateAsObservable ()
            .Select (_ => Score.GetRemainingNum ())
            .Where (remainingNum => remainingNum == 0)
            .Take (1)
            .Subscribe (_ => {
                Observable.Timer (TimeSpan.FromSeconds (3.0)).Subscribe (__ => {
                    LoadScene loadScene = GameObject.FindWithTag ("Manager").GetComponent<LoadScene> ();
                    loadScene.Title ();
                }).AddTo (this);
            });
        GetComponent<SaveController>().SaveFile();
    }

    public void CreateStage () {
        // ステージオブジェクトを生成
        m_stage.Create ("title", 1, 1, 1, 0.0f, 1.0f, 0.0f);

        // 全生成数を設定
        Score.SetRemainingNum (m_stage.GetAllNum ());

        // ステージクリア条件数設定
        m_clearNum = m_stage.GetAllNum ();

        // ステージ生成
        SpwnObject spwn = GetComponent<SpwnObject> ();
        spwn.Spwn (m_stage);

    }
    public void PassValue (string stageName) {
        m_stageName = stageName;
    }
    public string GetStageName () {
        return m_stageName;
    }
}