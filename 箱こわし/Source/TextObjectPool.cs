using System.Collections;
using System.Collections.Generic;
using UniRx.Toolkit;
using UnityEngine;

public class TextObjectPool : ObjectPool<TextObject> {
    public GameObject textPrefub;

    // オブジェクトの追加生成時
    protected override TextObject CreateInstance () {
        return GameObject.Instantiate (textPrefub).GetComponent<TextObject> ();
    }
    protected override void OnBeforeRent (TextObject instance) {

        //baseではinstance.gameObject.SetActive(true)を実行している
        base.OnBeforeRent (instance);
    }
    // オブジェクトの返却時
    protected override void OnBeforeReturn (TextObject instance) {
        //baseではinstance.gameObject.SetActive(false)を実行している
        base.OnBeforeReturn (instance);

    }
}