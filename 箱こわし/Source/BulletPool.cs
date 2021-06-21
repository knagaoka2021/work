using System.Collections;
using System.Collections.Generic;
using UniRx.Toolkit;
using UnityEngine;

public class BulletPool : ObjectPool<Bullet> {
    private static bool initFlag = false;
    public GameObject bulletPrefub;

    // オブジェクトの追加生成時
    protected override Bullet CreateInstance () {
        return GameObject.Instantiate (bulletPrefub).GetComponent<Bullet> ();
    }
    protected override void OnBeforeRent (Bullet instance) {
        initFlag = true;

        //baseではinstance.gameObject.SetActive(true)を実行している
        base.OnBeforeRent (instance);
    }
    // オブジェクトの返却時
    protected override void OnBeforeReturn (Bullet instance) {
        //baseではinstance.gameObject.SetActive(false)を実行している
        base.OnBeforeReturn (instance);

        if (initFlag) {
            //　爆発実行
            Collider[] collider = Physics.OverlapSphere (instance.transform.position, 5.0f);
            foreach (Collider cube in collider) {
                //範囲内のオブジェクトのRigidbodyに爆破の力を作用させる
                if (cube.GetComponent<Rigidbody> ()) {
                    cube.GetComponent<Rigidbody> ().
                    AddExplosionForce (20.0f, instance.transform.position, 5.0f, 10.0f, ForceMode.Impulse);
                }
            }
        }

    }
}