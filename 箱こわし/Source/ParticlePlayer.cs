using System;
using UnityEngine;

public class ParticlePlayer : MonoBehaviour
{
    //再生終了時の処理
    private Action<ParticlePlayer> _callback;

  //=================================================================================
  //実行
  //=================================================================================
  public void Play(Action<ParticlePlayer> callback) {
      GetComponent<ParticleSystem>().Play();
      _callback = callback;
    }
  
  //=================================================================================
  //イベント
  //=================================================================================
  
  //パーティクルの再生が終わった時に実行
  private void OnParticleSystemStopped() {
      _callback(this);
      }

}
