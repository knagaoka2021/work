using System;
using UnityEngine;
using UniRx;
using UniRx.Toolkit;

public class BulletPoolAsync : AsyncObjectPool<Bullet>
{
    private readonly GameObject m_bulletPrefub = null;
    private readonly Transform m_transform = null;

    public BulletPoolAsync(GameObject prefab, Transform parentTransform)
    {
        m_bulletPrefub = prefab;
        parentTransform = m_transform;
    }

    protected override IObservable<Bullet> CreateInstanceAsync()
    {
        Bullet e = GameObject.Instantiate(m_bulletPrefub).GetComponent<Bullet>();
        e.transform.SetParent(m_transform);

        return Observable.Return(e);
    }


}
