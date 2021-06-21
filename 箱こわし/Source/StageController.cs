using System;
using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.UI;

public class StageController : MonoBehaviour {
    private FadeController m_fadeController;
    private Stage m_stage;
    private GameTextController m_success;
    private GameTextController m_failure;
    private GameTextController m_text;
    private int m_clearNum = 0;
    private string m_stageName = "default";

    // Start is called before the first frame update
    void Start () {
        // ステージオブジェクトをキャッシュ
        m_fadeController = GetComponent<FadeController> ();
        m_stage = GetComponent<Stage> ();
        m_success = GameObject.FindWithTag ("TextSuccess").GetComponent<GameTextController> ();
        m_success.SetText (Const.STRING_TEXTSUCCESS);
        m_failure = GameObject.FindWithTag ("TextFailure").GetComponent<GameTextController> ();
        m_failure.SetText (Const.STRING_TEXTFAILURE);
        m_text = GameObject.FindWithTag ("Text").GetComponent<GameTextController> ();
        this.GetComponent<SaveController> ().LoadStage (m_stageName);

    }

    public void CreateStage () {
        // ステージオブジェクトを生成
        m_stage.Create ("title", 1, 1, 1, 0.0f, 1.0f, 0.0f);

        // 全生成数を設定
        Score.InitScore ();
        Score.InitRemainingNum ();
        Score.SetRemainingNum (m_stage.GetAllNum ());

        // ステージクリア条件数設定
        m_clearNum = m_stage.GetAllNum ();

        // ステージ生成
        SpwnObject spwn = GetComponent<SpwnObject> ();
        spwn.Spwn (m_stage);

    }
    public void CreateStage (WStageSave save) {
        // ステージオブジェクトを生成
        int stageMax = save.stageList.Count;

        // 全生成数を設定
        Score.InitScore ();
        Score.InitRemainingNum ();
        Score.SetRemainingNum (stageMax);

        // ステージクリア条件数設定
        m_clearNum = stageMax;

        // ステージ生成
        SpwnObject spwn = GetComponent<SpwnObject> ();
        spwn.Spwn (save);

        m_text.SetText (m_stageName);
        m_text.TextAnimationPlay ();

        // ステージオブジェクト数がクリア基準を満たした場合
        this.UpdateAsObservable ()
            .Select (_ => Score.GetRemainingNum ())
            .Where (remainingNum => remainingNum == 0 || Score.GetMissNum () > 0)
            .Take (1)
            .Subscribe (_ => {
                if (!m_fadeController.IsFade ()) {
                    if (Score.GetMissNum () == 0) {
                        m_success.TextAnimationPlay ();
                    } else {
                        m_failure.TextAnimationPlay ();
                    }

                    GameObject[] buttons = GameObject.FindGameObjectsWithTag ("UI_Button");
                    foreach (GameObject button in buttons) {
                        button.GetComponent<Button> ().interactable = false;
                    }

                    Observable.Timer (TimeSpan.FromSeconds (3.0)).Subscribe (__ => {
                        OkCancelDialog dialog = GameObject.FindWithTag ("Manager").GetComponent<OkCancelDialog> ();

                        if (dialog != null) {
                            dialog.ShowDialog (Const.SCENE.GAME, "", Const.STRING_MESSAGETEXT02);
                        }
                    }).AddTo (this);
                }
            });
    }
    public void PassValue (string stageName) {
        m_stageName = stageName;
    }

    public WStageSave GetStage (WStageSave save) {
        save.stageList.Clear ();

        GameObject[] satageObjects = GameObject.FindGameObjectsWithTag ("Explodable");
        foreach (GameObject satageobj in satageObjects) {
            StageSave obj = new StageSave ();

            obj.pos = satageobj.transform.position;
            obj.vectortype = satageobj.GetComponent<BaseObject> ().GetVectorType ();
            obj.speed = satageobj.GetComponent<BaseObject> ().GetSpeed ();

            save.stageList.Add (obj);
        }

        return save;
    }
    public void Debug_ClearStage () {
        SpwnObject spwn = GetComponent<SpwnObject> ();
        spwn.Clear ();
        BreakObjectPlay play = GetComponent<BreakObjectPlay> ();
        play.Clear ();
    }
    public string GetStageName () {
        return m_stageName;
    }
}