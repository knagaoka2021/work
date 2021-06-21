using System.Collections;
using System.Collections.Generic;
using UniRx.Toolkit;
using UnityEngine;

public class BreakObjectPool : ObjectPool<BreakObject> {
    private static bool initFlag = false;
    public GameObject breakObjectPrefub;

    // オブジェクトの追加生成時
    protected override BreakObject CreateInstance () {
        return GameObject.Instantiate (breakObjectPrefub).GetComponent<BreakObject> ();
    }
    protected override void OnBeforeRent (BreakObject instance) {
        initFlag = true;

        //baseではinstance.gameObject.SetActive(true)を実行している
        base.OnBeforeRent (instance);
    }
    // オブジェクトの返却時
    protected override void OnBeforeReturn (BreakObject instance) {
        //baseではinstance.gameObject.SetActive(false)を実行している
        base.OnBeforeReturn (instance);

        if (initFlag) {

        }

    }
}