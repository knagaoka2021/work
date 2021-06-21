using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DrawGizmo : MonoBehaviour {
    public Transform targetTransform;
    public Vector3 targetPotision = new Vector3 (0.0f, 0.0f, 0.0f);
    public Vector3 fromPotision = new Vector3 (0.0f, 0.0f, 0.0f);
    public Vector3 toPosition = new Vector3 (0.0f, 0.0f, 0.0f);
    public float lineLength = 1.0f;
    public enum GIZMOS_TYPE {
        RAY,
        LINE,
        WIRESPHERE,
        SPHERE,
        WIRECUBE,
        CUBE,
        NONE
    }
    public GIZMOS_TYPE gizmosType = GIZMOS_TYPE.NONE;

    private void OnDrawGizmos () {

        Color preColor = Gizmos.color;
        Matrix4x4 preMatrix = Gizmos.matrix;
        Gizmos.color = Color.red;
        //Gizmos.matrix = this.transform.localToWorldMatrix;

        switch (gizmosType) {
            case GIZMOS_TYPE.RAY:
                Vector3 direction;
                if (targetTransform != null) {
                    // 対象座標への方向を設定
                    direction = (targetTransform.position - transform.position);
                    // ベクトルの長さを単一にする
                    direction = direction.normalized * lineLength;
                } else {
                    // 対象座標への方向を設定
                    direction = (targetPotision - transform.position);
                    // ベクトルの長さを単一にする
                    direction = direction.normalized * lineLength;
                }
                // 指定座標(from)から対象方向にレイを描画
                Gizmos.DrawRay (transform.position, direction);

                break;
            case GIZMOS_TYPE.LINE:
                // 指定座標(from)から指定座標(to)にラインを描画
                Gizmos.DrawLine (fromPotision, toPosition);
                break;
            case GIZMOS_TYPE.WIRESPHERE:
                Gizmos.DrawWireSphere (transform.position, 1.0f);
                break;
            case GIZMOS_TYPE.SPHERE:
                Gizmos.DrawSphere (transform.position, 1.0f);
                break;
            case GIZMOS_TYPE.WIRECUBE:
                Gizmos.DrawWireCube (transform.position, new Vector3 (1, 1, 1));
                break;
            case GIZMOS_TYPE.CUBE:
                Gizmos.DrawCube (transform.position, new Vector3 (1, 1, 1));
                break;
            default:
                break;
        }

        Gizmos.color = preColor;
        Gizmos.matrix = preMatrix;
    }
}