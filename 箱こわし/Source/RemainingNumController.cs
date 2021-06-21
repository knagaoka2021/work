using System;
using CommonProperty;
using TMPro;
using UniRx;
using UniRx.Triggers;
using UnityEngine;
using UnityEngine.UI;

public class RemainingNumController : MonoBehaviour {

    void Awake(){
        // 残数初期化
        Score.InitRemainingNum();
    }

    // Start is called before the first frame update
    void Start () {

        TextMeshProUGUI textPro = this.GetComponent<TextMeshProUGUI> ();

        // ステージオブジェクト数更新
        this.UpdateAsObservable ()
            .Select (_ => Score.GetRemainingNum ())
            .DistinctUntilChanged ()
            .Subscribe (remainingNum => {
                textPro.SetText(Const.STRING_REMAINING + Score.GetRemainingNum ());
            }).AddTo(this);
    }
}