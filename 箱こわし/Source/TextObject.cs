using System;
using CommonProperty;
using UniRx;
using UnityEngine;

public class TextObject : MonoBehaviour {
    private Action<TextObject> _callback;

    [SerializeField, Range (0, 1)]
    float m_opacity = 0.0f;

    public void Make (Action<TextObject> callback) {
        _callback = callback;
    }
    // Start is called before the first frame update
    void Start () {
        TextMesh textMesh = this.GetComponent<TextMesh> ();

        Observable.EveryUpdate ()
            .Select (_ => m_opacity)
            .DistinctUntilChanged ()
            .Subscribe (opacity => textMesh.color = new Color (textMesh.color.r, textMesh.color.g, textMesh.color.b, opacity));
    }
    public void TextAnimationPlay (string animation, int value) {
        // カウント値を設定
        TextMesh textMesh = this.GetComponent<TextMesh> ();
        textMesh.text = StringUtils.Format ("{0}{1}", Const.CHAR_PLUS, value);

        // カウントアニメーション再生
        Animator anim = this.GetComponent<Animator> ();
        anim.enabled = true;
        anim.Play (animation);

        // 1秒後に実行
        Observable.Timer (TimeSpan.FromSeconds (1.0)).Subscribe (_ => {
            EndTextObject ();
        });
    }
    private void EndTextObject () {
        Animator anim = this.GetComponent<Animator> ();
        anim.enabled = false;
        _callback (this);
    }
}