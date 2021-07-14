using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DebugMove : MonoBehaviour {
    public float speed = 1.0f;

    public bool pos = false;
    public bool pos_X = false;
    public bool pos_Y = false;
    public bool pos_Z = false;
    public bool posCurve_X = false;
    public bool posCurve_Y = false;
    public bool posCurve_Z = false;
    public bool rot = false;
    public bool rot_X = false;
    public bool rot_Y = false;
    public bool rot_Z = false;
    private Vector3 vecPos = Vector3.zero;
    private Vector3 vecRot = Vector3.zero;

    public float duration = 0.5f;
    public float magnitude = 0.5f;


    // Start is called before the first frame update
    void Start () {
        vecPos = this.transform.position;
        Quaternion quaternion = this.transform.rotation;
        vecRot = quaternion.eulerAngles;
    }

    // Update is called once per frame
    void Update () {
    }
    void FixedUpdate () {

        if (pos) {
            float add = Time.deltaTime * speed;
            if (pos_X) {
                vecPos.x += add;
            }
            if (pos_Y) {
                vecPos.y += add;
            }
            if (pos_Z) {
                vecPos.z += add;
            }
            if (posCurve_X) {
                vecPos.x = Mathf.Sin(Time.time) * add;
            }
            if (posCurve_Y) {
                vecPos.y += Mathf.Sin(Time.time) * add;
            }
            if (posCurve_Z) {
                vecPos.z += Mathf.Sin(Time.time) * add;
            }
            transform.position = vecPos;
        }
        if (rot) {
            float add = Time.deltaTime * speed;
            if (rot_X) {
                vecRot.x += add;
            }
            if (rot_Y) {
                vecRot.y += add;
            }
            if (rot_Z) {
                vecRot.z += add;
            }
            transform.rotation = Quaternion.Euler (vecRot.x, vecRot.y, vecRot.z);
        }
    }
}