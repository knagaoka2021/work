using UniRx.Toolkit;//ObjectPoolを使うのに必要
using UnityEngine;

/// <summary>
/// ParticlePlayerのオブジェクトプール
/// </summary>
public class ParticlePlayerPool : ObjectPool<ParticlePlayer> {

  private readonly ParticlePlayer _original;

  //=================================================================================
  //初期化
  //=================================================================================

  /// <summary>
  /// オリジナルを渡して初期化
  /// </summary>
  public ParticlePlayerPool(ParticlePlayer original) {
    //オリジナルは非表示に
    _original = original;
    _original.gameObject.SetActive(false);
  }
  
  //インスタンスを作る処理
  protected override ParticlePlayer CreateInstance() {
    //オリジナルを複製してインスタンス作成(オリジナルと同じ親の下に配置)
    return ParticlePlayer.Instantiate(_original, _original.transform.parent);
  }
  
  //=================================================================================
  //イベント
  //=================================================================================
  
  //プールからオブジェクトを取得する前に実行される
  protected override void OnBeforeRent(ParticlePlayer instance) {
    base.OnBeforeRent(instance);
  }

  //オブジェクトがプールに戻る前に実行される
  protected override void OnBeforeReturn(ParticlePlayer instance) {
    base.OnBeforeReturn(instance);
  }

}
