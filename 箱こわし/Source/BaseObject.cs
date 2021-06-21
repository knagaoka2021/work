using System;
using CommonProperty;
using UnityEngine;

public class BaseObject : MonoBehaviour {
    private Action<BaseObject> _callback;

    [SerializeField]
    private Vector3 m_force;

    [SerializeField]
    private Const.VECTOR_TYPE m_vectorType = 0;
    [SerializeField]
    private float m_speed = 0.0f;

    public void Make (Action<BaseObject> callback) {
        _callback = callback;
    }

    void FixedUpdate () {
        if (this.transform.position.y <= Const.MIN_DROPPOSITION) {
            // 失敗数更新
            Score.AddMissNum (1);
            EndBaseObject ();
        }
    }

    public void EndBaseObject () {
        // 残数更新
        Score.AddRemainingNum (-1);
        this.transform.rotation = Quaternion.identity;
        this.GetComponent<Rigidbody> ().velocity = Vector3.zero;
        this.GetComponent<Rigidbody> ().angularVelocity = Vector3.zero;
        _callback (this);
    }
    public Vector3 GetForce () {
        return Const.VECTOR_VALUE[(int) m_vectorType] * m_speed;
    }
    public Const.VECTOR_TYPE GetVectorType () {
        return m_vectorType;
    }
    public void SetVectorType (Const.VECTOR_TYPE type) {
        m_vectorType = type;
    }
    public float GetSpeed () {
        return m_speed;
    }
    public void SetSpeed (float value) {
        m_speed = value;
    }

}