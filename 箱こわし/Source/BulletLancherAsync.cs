using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;

public class BulletLancherAsync : MonoBehaviour
{
    public Transform muzzle;
    public float speed = 0.1f;

    private BulletPoolAsync _poolAsync;
    public GameObject bulletPrefub;


    // Start is called before the first frame update
    void Start()
    {
        // 非同期プール
        _poolAsync = new BulletPoolAsync(bulletPrefub,this.transform);
        _poolAsync.RentAsync().Subscribe(bullet => 
        {
            bullet.SetAsyncFlag(true);
            bullet.OutdebugLog().Subscribe(__ => _poolAsync.Return(bullet));
        });
    }

    // Update is called once per frame
    void Update()
    {
    }
    private void OnApplicationQuit()
    {
        _poolAsync.Dispose();

    }
}
