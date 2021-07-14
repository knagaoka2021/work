using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TransformCommand : ICommand {
    private Transform m_transform;
    private TransformInfo m_Info;
    private TransformInfo m_prevInfo;

    public TransformCommand (Transform transform) {
        m_transform = transform;
        m_prevInfo = new TransformInfo {
            position = transform.position,
            rotation = transform.transform.rotation,
            scale = transform.localScale
        };
    }
    public void Execute () {
        m_transform.position = m_Info.position;
        m_transform.rotation = m_Info.rotation;
        m_transform.localScale = m_Info.scale;
    }

    public void UnExecute () {
        m_transform.position = m_prevInfo.position;
        m_transform.rotation = m_prevInfo.rotation;
        m_transform.localScale = m_prevInfo.scale;
    }
    public void SetTransformInfo () {
        m_Info = new TransformInfo {
            position = m_transform.position,
            rotation = m_transform.transform.rotation,
            scale = m_transform.localScale
        };
    }

    struct TransformInfo {
        public Vector3 position;
        public Quaternion rotation;
        public Vector3 scale;
    }
}