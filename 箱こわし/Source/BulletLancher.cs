using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;

public class BulletLancher : MonoBehaviour
{
    public Transform muzzle;
    public float speed = 0.1f;
    private BulletPool _pool;
    public GameObject bulletPrefub;
    public Material[] _material;


    // Start is called before the first frame update
    void Start()
    {
        // 同期プール
        _pool = new BulletPool();
        _pool.bulletPrefub = bulletPrefub;

        // プール拡張設定
        // 1フレームあたり5個生成 最大20拡張
        _pool.PreloadAsync(20,5).Subscribe();

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Z)){
                    
            // 弾丸の複製
            Bullet bullets = _pool.Rent();
            bullets.transform.position = muzzle.position;

            int value = Random.Range( 0, 2 );

            if(value == 0){
                bullets.GetComponent<Renderer>().material=_material[0];
            } else{
                bullets.GetComponent<Renderer>().material=_material[1];
            }

            Vector3 force;

            force = this.gameObject.transform.forward * speed;
            // Rigidbodyに力を加えて発射
            bullets.GetComponent<Rigidbody>().AddForce(force);
            // コールバック用にプール返却処理を渡す
            bullets.Make(_pool.Return);
            
        }
    }
    private void OnApplicationQuit()
    {
        _pool.Dispose();

    }
}
