using System;
using UnityEngine;
using UniRx;

public class Bullet : MonoBehaviour
{
    private Action<Bullet> _callback;

    float seconds = 0.0f;

    private bool m_asyncFlag = false;

  public void Make(Action<Bullet> callback) {
      _callback = callback;
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(!m_asyncFlag){
            seconds += Time.deltaTime;
            if (seconds >= 5.0f)
            {
                seconds = 0.0f;
                EndBullet();
            }
        }

    }
    private void EndBullet(){
        // addfourceを初期化
        this.GetComponent<Rigidbody>().velocity = Vector3.zero;
        _callback(this);
    }
    public IObservable<Unit> OutdebugLog(){
        // 3秒後にデバッグログ出力してプールオブジェクトを返却
        return Observable.Timer(TimeSpan.FromSeconds(3.0f)).ForEachAsync(_ =>  Debug.Log("aiueo"));
    }
    public void SetAsyncFlag(bool value){
        m_asyncFlag = value;
    }
}
