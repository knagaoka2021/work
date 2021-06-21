using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyDebug : MonoBehaviour {
    public Transform target;
    public Transform childGameObject;
    public float speed = 0.1f;
    private Vector3 vec;

    // Start is called before the first frame update
    void Start () {
        target = GameObject.Find ("PlayerManager").transform;
        childGameObject = transform.Find ("Enemy").transform;
    }

    // Update is called once per frame
    void Update () {
        if (childGameObject.transform.position.y < 0.5f) {
            childGameObject.transform.rotation = Quaternion.Slerp (childGameObject.transform.rotation, Quaternion.LookRotation (target.position - childGameObject.transform.position), 0.3f);
            Vector3 pos = childGameObject.transform.forward * speed;
            pos.y = 0.0f;
            childGameObject.transform.position += pos;
        }

    }
}