using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Blast : MonoBehaviour {
    public float power = 100f;
    public float radius = 20.0f;
    public float upwardsModifier = 3.0f;

    Ray ray;
    RaycastHit hit;

    void Update () {
        if (Input.GetMouseButtonDown (0)) {
            //クリック点にRayを飛ばす
            ray = Camera.main.ScreenPointToRay (Input.mousePosition);
            if (Physics.Raycast (ray, out hit, 100f)) {
                //クリックした所から半径20mの範囲のColliderを取得
                Collider[] collider = Physics.OverlapSphere (hit.point, radius);
                foreach (Collider cube in collider) {
                    //範囲内のオブジェクトのRigidbodyに爆破の力を作用させる
                    if (cube.GetComponent<Rigidbody> ()) {
                        cube.GetComponent<Rigidbody> ().
                        AddExplosionForce (power, hit.point, radius, upwardsModifier, ForceMode.Impulse);
                    }
                }
            }
        }
    }
}