using UniRx;
using UnityEngine;
using UnityEngine.UI;
using Random = UnityEngine.Random;

/// <summary>
/// パーティクルのオブジェクトプールのサンプル
/// </summary>
public class ParticlePoolSample : MonoBehaviour {

  //元のパーティクル(Inspectorから設定)
  [SerializeField]
  private ParticlePlayer _original = null;

  //数値を入力する用のUI(Inspectorから設定)
  [SerializeField]
  private InputField _inputField = null;
  
  //オブジェクトプール
  private ParticlePlayerPool _pool;
  
  //=================================================================================
  //初期化
  //=================================================================================

  private void Awake() {
    //パーティクルを指定してオブジェクトプール作成
    _pool = new ParticlePlayerPool(_original);
  }
  
  //=================================================================================
  //再生処理(uGUIのボタンから実行)
  //=================================================================================

  /// <summary>
  /// パーティクルを再生
  /// </summary>
  public void PlayParticle() {
    //入力された回数だけ実行
    for (var i = 0; i < int.Parse(_inputField.text); i++) {
      //新たなParticlePlayerをプールから取得
      var particlePlayer = _pool.Rent();
    
      //パーティクルの位置をランダムに決定
      particlePlayer.transform.position = new Vector3(Random.Range(-8f, 8f), Random.Range(-2f, 2f), 0.0f);
    
      //パーティクルの再生を実行、再生が終わったらプールに戻す
      particlePlayer.Play(_pool.Return);
    }
  }

}