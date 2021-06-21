using System;
using CommonProperty;
using UniRx;
using UniRx.Triggers;
using UnityEngine;

public class BreakObject : MonoBehaviour {
    private Action<BreakObject> _callback;

    private float start = 1.0f;

    private float goal = 0.0f;

    // 変化量
    private float speed = 0.5f;

    // 現在の変化量
    private float _progress = 0.0f;

    private bool uflag = false;

    private Rigidbody m_rigidbody;

    public void Make (Action<BreakObject> callback) {
        _callback = callback;
    }
    // Start is called before the first frame update
    void Start () {
        m_rigidbody = this.GetComponent<Rigidbody> ();
        // FixedUpdateのコールバックストリーム
        // フラグが有効な場合、実行する
        this.FixedUpdateAsObservable ().Where (_ => uflag).Subscribe (_ => ScaleAnimation ()).AddTo (this);
    }
    void FixedUpdate () {
        if (this.transform.position.y <= Const.MIN_DROPPOSITION) {
            EndObject ();
        }
    }
    public void AddForce (Vector3 force) {
        Rigidbody rb = this.GetComponent<Rigidbody> ();
        rb.AddForce (force, ForceMode.Impulse);
    }

    // イベント処理
    public void SetEvent () {
        // 2秒後に実行
        Observable.Timer (TimeSpan.FromSeconds (2.0)).Subscribe (_ => SetEndEvent2 ()).AddTo (this);
    }
    // イベント処理
    private void SetEndEvent2 () {
        uflag = true;

        this.FixedUpdateAsObservable ().Where (_ => _progress > 0.9f).Take (1).Subscribe (_ => {
            m_rigidbody.collisionDetectionMode = CollisionDetectionMode.Discrete;
            m_rigidbody.isKinematic = true;
        }).AddTo (this);

        // 3秒後に実行
        Observable.Timer (TimeSpan.FromSeconds (3.0)).Subscribe (_ => {
            EndObject ();
        }).AddTo (this);
    }
    // 縮小アニメーション
    private void ScaleAnimation () {
        _progress = _progress + speed * Time.deltaTime;
        float scale = Mathf.Lerp (start, goal, _progress);
        this.transform.localScale = new Vector3 (scale, scale, scale);
    }
    // オブジェクト返却処理
    public void EndObject () {
        _progress = 0.0f;
        uflag = false;
        m_rigidbody.isKinematic = false;
        m_rigidbody.collisionDetectionMode = CollisionDetectionMode.Continuous;
        // addfourceを初期化
        m_rigidbody.velocity = Vector3.zero;
        m_rigidbody.angularVelocity = Vector3.zero;
        this.transform.localScale = Vector3.one;
        _callback (this);
    }
}