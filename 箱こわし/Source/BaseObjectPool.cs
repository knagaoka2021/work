using System.Collections;
using System.Collections.Generic;
using UniRx.Toolkit;
using UnityEngine;

public class BaseObjectPool : ObjectPool<BaseObject> {

    public GameObject baseObjectPrefub;

    // オブジェクトの追加生成時
    protected override BaseObject CreateInstance () {
        return GameObject.Instantiate (baseObjectPrefub).GetComponent<BaseObject> ();
    }
    protected override void OnBeforeRent (BaseObject instance) {

        //baseではinstance.gameObject.SetActive(true)を実行している
        base.OnBeforeRent (instance);
    }
    // オブジェクトの返却時
    protected override void OnBeforeReturn (BaseObject instance) {
        //baseではinstance.gameObject.SetActive(false)を実行している
        base.OnBeforeReturn (instance);

    }
}