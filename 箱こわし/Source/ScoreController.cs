using System;
using CommonProperty;
using TMPro;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.UI;

public class ScoreController : MonoBehaviour {

    private char[] m_characters = new char[Const.MAX_SCORE_STRINGNUM + Const.MAX_SCORE_NUM];

    void Awake(){
        // スコア初期化
        Score.InitScore ();
        Score.InitRemainingNum();
    }

    // Start is called before the first frame update
    void Start () {

        TextMeshProUGUI textPro = this.GetComponent<TextMeshProUGUI> ();

        // スコア更新
        this.UpdateAsObservable ()
            .Select (_ => Score.GetScore ())
            .DistinctUntilChanged ()
            .Subscribe (score => {
                UpdateScore (score);
                textPro.SetCharArray (m_characters, 0, m_characters.Length);
            }).AddTo(this);
        
        // スコアバッファ分のスコアを加算
        Observable.Timer (TimeSpan.FromSeconds (0), TimeSpan.FromSeconds (Const.DOUBLE_COUNT_SPEED))
            .Select (_ => Score.GetScoreBuffer ())
            .Where(scoreBuffer => scoreBuffer > 0)
            .Subscribe (scoreBuffer => {
                Score.AddScoreBuffer(-1);
                Score.AddScore(1);
            }).AddTo(this);
        // Observable.EveryUpdate ()
        //     .Select (_ => Score.GetScore ()).DistinctUntilChanged ()
        //     .Subscribe (score => txt.text = "SCORE:" + Score.GetScore ());

        // @デバッグ用
        // Observable.Timer (TimeSpan.FromSeconds (0), TimeSpan.FromSeconds (1))
        //     .TakeWhile (_ => Score.GetScore () < Const.MAX_SCORE)
        //     .Subscribe (_ => {
        //         Score.AddScore (1);
        //     }).AddTo (this);
        // @デバッグ用
    }

    // スコア更新
    private void UpdateScore (int score) {
        // 文字列部分の格納
        for (int i = 0; i < Const.CHAR_SCORE.Length; i++) {
            m_characters[i] = Const.CHAR_SCORE[i];
        }
        // スコア部分の格納
        for (int i = m_characters.Length - 1; i >= Const.CHAR_SCORE.Length; i--) {
            m_characters[i] = (char) ((score % 10) + Const.CHAR_ZERO);
            score /= 10;
        }
    }
}