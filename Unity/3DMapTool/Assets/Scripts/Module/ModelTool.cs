using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ModelTool : MonoBehaviour {
    private Material material;
    private List<Vector3> x_vec3;
    private List<Vector3> y_vec3;
    private List<Vector3> z_vec3;
    private List<Vector3> all_vec3;
    private List<Vector3> m_vec3_X;
    private List<Vector3> m_vec3_Y;
    private List<Vector3> m_vec3_Z;
    private List<Vector3> m_vec3_All;
    private List<Color> m_color;
    private Color m_hoverColor = new Color (1, 0.75f, 0, 0.8f);
    private Color m_CyanColor = new Color (0f, 0.75f, 1f, 0.3f);

    private bool m_hoverX = false;
    private bool m_hoverY = false;
    private bool m_hoverZ = false;
    private bool m_hoverAll = false;
    private bool m_create = false;

    private TRANSFORME_TYPE m_transType;
    private AXIS_TYPE m_axisType;
    private UndoRedoManager m_undoRedoManager;

    private bool transformUse = false;
    private bool undoRedoUse = false;

    public enum TRANSFORME_TYPE {
        NONE,
        MOVE,
        ROTATION,
        SCALE
        } // 編集

        public enum AXIS_TYPE {
        NONE,
        X,
        Y,
        Z,
        ALL
        } // 軸

        void Start () {
        if (!m_create) {
        x_vec3 = new List<Vector3> () {
        new Vector3 (2f, 0f, 0f),
        new Vector3 (0f, 1, -1f),
        new Vector3 (0f, 1, 1f),
        new Vector3 (2f, 0f, 0f),
        new Vector3 (0f, -1, -1f),
        new Vector3 (0f, 1, -1f),
        new Vector3 (2f, 0f, 0f),
        new Vector3 (0f, -1, 1f),
        new Vector3 (0f, -1, -1f),
        new Vector3 (2f, 0f, 0f),
        new Vector3 (0f, 1, 1f),
        new Vector3 (0f, -1, 1f),
        new Vector3 (0f, -1f, 1f),
        new Vector3 (0f, 1f, -1f),
        new Vector3 (0f, -1f, -1f),
        new Vector3 (0f, -1f, 1f),
        new Vector3 (0f, 1f, 1f),
        new Vector3 (0f, 1f, -1f)
        };
        y_vec3 = new List<Vector3> () {
        new Vector3 (0f, 2f, 0f),
        new Vector3 (-1f, 0f, 1f),
        new Vector3 (1f, 0f, 1f),
        new Vector3 (0f, 2f, 0f),
        new Vector3 (-1f, 0f, -1f),
        new Vector3 (-1f, 0f, 1f),
        new Vector3 (0f, 2f, 0f),
        new Vector3 (1f, 0f, -1f),
        new Vector3 (-1f, 0f, -1f),
        new Vector3 (0f, 2f, 0f),
        new Vector3 (1f, 0f, 1f),
        new Vector3 (1f, 0f, -1f),
        new Vector3 (1f, 0f, -1f),
        new Vector3 (-1f, 0f, 1f),
        new Vector3 (-1f, 0f, -1f),
        new Vector3 (1f, 0f, -1f),
        new Vector3 (1f, 0f, 1f),
        new Vector3 (-1f, 0f, 1f)
        };
        z_vec3 = new List<Vector3> () {
        new Vector3 (0f, 0f, 2f),
        new Vector3 (1f, -1f, 0f),
        new Vector3 (1f, 1f, 0f),
        new Vector3 (0f, 0f, 2f),
        new Vector3 (-1f, -1f, 0f),
        new Vector3 (1f, -1f, 0f),
        new Vector3 (0f, 0f, 2f),
        new Vector3 (-1f, 1f, 0f),
        new Vector3 (-1f, -1f, 0f),
        new Vector3 (0f, 0f, 2f),
        new Vector3 (1f, 1f, 0f),
        new Vector3 (-1f, 1f, 0f),
        new Vector3 (-1f, 1f, 0f),
        new Vector3 (1f, -1f, 0f),
        new Vector3 (-1f, -1f, 0f),
        new Vector3 (-1f, 1f, 0f),
        new Vector3 (1f, 1f, 0f),
        new Vector3 (1f, -1f, 0f)
        };
        all_vec3 = new List<Vector3> () {
        new Vector3 (-1f, 1f, 1f),
        new Vector3 (1f, 1f, 1f),
        new Vector3 (1f, 1f, -1f),
        new Vector3 (-1f, 1f, -1f),
        new Vector3 (-1f, 1f, -1f),
        new Vector3 (-1f, -1f, -1f),
        new Vector3 (1f, -1f, -1f),
        new Vector3 (1f, 1f, -1f),
        new Vector3 (1f, -1f, -1f),
        new Vector3 (1f, -1f, 1f),
        new Vector3 (1f, 1f, 1f),
        new Vector3 (1f, 1f, -1f),
        new Vector3 (1f, -1f, 1f),
        new Vector3 (-1f, -1f, 1f),
        new Vector3 (-1f, 1f, 1f),
        new Vector3 (1f, 1f, 1f),
        new Vector3 (-1f, 1f, 1f),
        new Vector3 (-1f, 1f, -1f),
        new Vector3 (-1f, -1f, -1f),
        new Vector3 (-1f, -1f, 1f),
        new Vector3 (-1f, -1f, 1f),
        new Vector3 (1f, -1f, 1f),
        new Vector3 (1f, -1f, -1f),
        new Vector3 (-1f, -1f, -1f),
        };

        m_vec3_X = new List<Vector3> ();
        m_vec3_Y = new List<Vector3> ();
        m_vec3_Z = new List<Vector3> ();
        m_vec3_All = new List<Vector3> ();

        material = new Material (Shader.Find ("Unlit/GLDraw"));
        if (m_undoRedoManager == null) {
        m_undoRedoManager = new UndoRedoManager ();
            }

            m_transType = TRANSFORME_TYPE.NONE;
            m_axisType = AXIS_TYPE.NONE;
        }

    }
    public void Create (TRANSFORME_TYPE type) {
        x_vec3 = new List<Vector3> () {
            new Vector3 (2f, 0f, 0f),
            new Vector3 (0f, 1, -1f),
            new Vector3 (0f, 1, 1f),
            new Vector3 (2f, 0f, 0f),
            new Vector3 (0f, -1, -1f),
            new Vector3 (0f, 1, -1f),
            new Vector3 (2f, 0f, 0f),
            new Vector3 (0f, -1, 1f),
            new Vector3 (0f, -1, -1f),
            new Vector3 (2f, 0f, 0f),
            new Vector3 (0f, 1, 1f),
            new Vector3 (0f, -1, 1f),
            new Vector3 (0f, -1f, 1f),
            new Vector3 (0f, 1f, -1f),
            new Vector3 (0f, -1f, -1f),
            new Vector3 (0f, -1f, 1f),
            new Vector3 (0f, 1f, 1f),
            new Vector3 (0f, 1f, -1f)
        };
        y_vec3 = new List<Vector3> () {
            new Vector3 (0f, 2f, 0f),
            new Vector3 (-1f, 0f, 1f),
            new Vector3 (1f, 0f, 1f),
            new Vector3 (0f, 2f, 0f),
            new Vector3 (-1f, 0f, -1f),
            new Vector3 (-1f, 0f, 1f),
            new Vector3 (0f, 2f, 0f),
            new Vector3 (1f, 0f, -1f),
            new Vector3 (-1f, 0f, -1f),
            new Vector3 (0f, 2f, 0f),
            new Vector3 (1f, 0f, 1f),
            new Vector3 (1f, 0f, -1f),
            new Vector3 (1f, 0f, -1f),
            new Vector3 (-1f, 0f, 1f),
            new Vector3 (-1f, 0f, -1f),
            new Vector3 (1f, 0f, -1f),
            new Vector3 (1f, 0f, 1f),
            new Vector3 (-1f, 0f, 1f)
        };
        z_vec3 = new List<Vector3> () {
            new Vector3 (0f, 0f, 2f),
            new Vector3 (1f, -1f, 0f),
            new Vector3 (1f, 1f, 0f),
            new Vector3 (0f, 0f, 2f),
            new Vector3 (-1f, -1f, 0f),
            new Vector3 (1f, -1f, 0f),
            new Vector3 (0f, 0f, 2f),
            new Vector3 (-1f, 1f, 0f),
            new Vector3 (-1f, -1f, 0f),
            new Vector3 (0f, 0f, 2f),
            new Vector3 (1f, 1f, 0f),
            new Vector3 (-1f, 1f, 0f),
            new Vector3 (-1f, 1f, 0f),
            new Vector3 (1f, -1f, 0f),
            new Vector3 (-1f, -1f, 0f),
            new Vector3 (-1f, 1f, 0f),
            new Vector3 (1f, 1f, 0f),
            new Vector3 (1f, -1f, 0f)
        };
        all_vec3 = new List<Vector3> () {
            new Vector3 (-1f, 1f, 1f),
            new Vector3 (1f, 1f, 1f),
            new Vector3 (1f, 1f, -1f),
            new Vector3 (-1f, 1f, -1f),
            new Vector3 (-1f, 1f, -1f),
            new Vector3 (-1f, -1f, -1f),
            new Vector3 (1f, -1f, -1f),
            new Vector3 (1f, 1f, -1f),
            new Vector3 (1f, -1f, -1f),
            new Vector3 (1f, -1f, 1f),
            new Vector3 (1f, 1f, 1f),
            new Vector3 (1f, 1f, -1f),
            new Vector3 (1f, -1f, 1f),
            new Vector3 (-1f, -1f, 1f),
            new Vector3 (-1f, 1f, 1f),
            new Vector3 (1f, 1f, 1f),
            new Vector3 (-1f, 1f, 1f),
            new Vector3 (-1f, 1f, -1f),
            new Vector3 (-1f, -1f, -1f),
            new Vector3 (-1f, -1f, 1f),
            new Vector3 (-1f, -1f, 1f),
            new Vector3 (1f, -1f, 1f),
            new Vector3 (1f, -1f, -1f),
            new Vector3 (-1f, -1f, -1f),
        };

        m_vec3_X = new List<Vector3> ();
        m_vec3_Y = new List<Vector3> ();
        m_vec3_Z = new List<Vector3> ();
        m_vec3_All = new List<Vector3> ();
        m_undoRedoManager = new UndoRedoManager ();
        material = new Material (Shader.Find ("Unlit/GLDraw"));

        m_transType = type;
        m_axisType = AXIS_TYPE.NONE;
        m_create = true;
    }

    void Update () {
        Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);

        UndoRedoHandle ();
        UpdateTriangle ();
        UpdateQuad ();

        for (int i = 0; i < m_vec3_X.Count; i += 3) {
            m_hoverX = rayInterTriangle (ray, m_vec3_X[i], m_vec3_X[i + 1], m_vec3_X[i + 2]);

            if (m_hoverX) {
                m_axisType = AXIS_TYPE.X;
                break;
            }
        }
        for (int i = 0; i < m_vec3_Y.Count; i += 3) {
            m_hoverY = rayInterTriangle (ray, m_vec3_Y[i], m_vec3_Y[i + 1], m_vec3_Y[i + 2]);

            if (m_hoverY) {
                m_axisType = AXIS_TYPE.Y;
                break;
            }
        }
        for (int i = 0; i < m_vec3_Z.Count; i += 3) {
            m_hoverZ = rayInterTriangle (ray, m_vec3_Z[i], m_vec3_Z[i + 1], m_vec3_Z[i + 2]);

            if (m_hoverZ) {
                m_axisType = AXIS_TYPE.Z;
                break;
            }
        }
        if (m_transType == TRANSFORME_TYPE.SCALE) {
            for (int i = 0; i < m_vec3_All.Count; i += 4) {
                m_hoverAll = rayInterQuad (ray, m_vec3_All[i], m_vec3_All[i + 1], m_vec3_All[i + 2], m_vec3_All[i + 3]);

                if (m_hoverAll) {
                    m_axisType = AXIS_TYPE.ALL;
                    break;
                }
            }
        }
        if (!m_hoverX && !m_hoverY && !m_hoverZ && !m_hoverAll) {
            m_axisType = AXIS_TYPE.NONE;
        }

        // 3D移動回転拡大
        TransformObject ();
    }
    // 編集切り替え
    public void ChangeTransformType (TRANSFORME_TYPE type) {
        m_transType = type;
    }
    // 選択軸取得
    public bool GetAxisSelected () {
        if (m_axisType != AXIS_TYPE.NONE) {
            return true;
        }
        return false;
    }
    // 三角形描画
    private void UpdateTriangle () {
        for (int i = 0; i < x_vec3.Count; i++) {
            Vector3 vec = (x_vec3[i] * 0.25f) + this.transform.position + new Vector3 (1f, 0f, 0f);
            m_vec3_X.Add (vec);
        }
        for (int i = 0; i < y_vec3.Count; i++) {
            Vector3 vec = (y_vec3[i] * 0.25f) + this.transform.position + new Vector3 (0f, 1f, 0f);
            m_vec3_Y.Add (vec);
        }
        for (int i = 0; i < z_vec3.Count; i++) {
            Vector3 vec = (z_vec3[i] * 0.25f) + this.transform.position + new Vector3 (0f, 0f, 1f);
            m_vec3_Z.Add (vec);
        }

    }
    // 平面描画
    private void UpdateQuad () {
        for (int i = 0; i < all_vec3.Count; i++) {
            Vector3 vec = (all_vec3[i] * 0.125f) + this.transform.position;
            m_vec3_All.Add (vec);
        }
    }
    // GUI描画
    void OnRenderObject () {

        if (m_vec3_X.Count == 0) {
            return;
        }
        if (m_vec3_Y.Count == 0) {
            return;
        }
        if (m_vec3_Z.Count == 0) {
            return;
        }
        if (m_vec3_All.Count == 0) {
            return;
        }

        material.SetPass (0);

        DrawTriangle (m_vec3_X, m_hoverX ? m_hoverColor : Color.red);

        DrawTriangle (m_vec3_Y, m_hoverY ? m_hoverColor : Color.green);

        DrawTriangle (m_vec3_Z, m_hoverZ ? m_hoverColor : Color.blue);

        if (m_transType == TRANSFORME_TYPE.SCALE) {
            DrawQuad (m_vec3_All, m_hoverAll ? m_hoverColor : m_CyanColor);
        }

        m_vec3_X.Clear ();
        m_vec3_Y.Clear ();
        m_vec3_Z.Clear ();
        m_vec3_All.Clear ();
    }
    // 三角面描画
    private void DrawTriangle (List<Vector3> list, Color color) {
        if (list.Count == 0) {
            return;
        }

        GL.Begin (GL.TRIANGLES);
        GL.Color (color);

        for (int i = 0; i < list.Count; i += 3) {
            GL.Vertex (list[i]);
            GL.Vertex (list[i + 1]);
            GL.Vertex (list[i + 2]);
        }

        GL.End ();
    }
    // 平面描画
    private void DrawQuad (List<Vector3> list, Color color) {
        if (list.Count == 0) {
            return;
        }

        GL.Begin (GL.QUADS);
        GL.Color (color);

        for (int i = 0; i < list.Count; i += 4) {
            GL.Vertex (list[i]);
            GL.Vertex (list[i + 1]);
            GL.Vertex (list[i + 2]);
            GL.Vertex (list[i + 3]);
        }

        GL.End ();
    }
    // 三角形の内包判定
    private float det (Vector3 vec0, Vector3 vec1, Vector3 vec2) {
        return (vec0.x * vec1.y * vec2.z) +
            (vec0.y * vec1.z * vec2.x) +
            (vec0.z * vec1.x * vec2.y) -
            (vec0.x * vec1.z * vec2.y) -
            (vec0.y * vec1.x * vec2.z) -
            (vec0.z * vec1.y * vec2.x);
    }
    // 三角面とレイの衝突判定処理
    private bool rayInterTriangle (Ray ray, Vector3 vec0, Vector3 vec1, Vector3 vec2) {

        var inverseRay = ray.direction * (-1);
        var edge1 = vec1 - vec0;
        var edge2 = vec2 - vec0;

        var denominator = det (edge1, edge2, inverseRay);

        // レイと平面との平行チェック
        if (denominator <= 0) {
            return false;
        }

        var d = ray.origin - vec0;

        var u = det (d, edge2, inverseRay) / denominator;

        if ((u >= 0) && (u <= 1)) {
            var v = det (edge1, d, inverseRay) / denominator;

            if ((v >= 0) && (u + v <= 1)) {
                var t = det (edge1, edge2, d) / denominator;

                if (t < 0) {
                    return false;
                }

                return true;
            }
        }

        return false;
    }
    // 平面とレイの衝突判定処理
    private bool rayInterQuad (Ray ray, Vector3 vec0, Vector3 vec1, Vector3 vec2, Vector3 vec3) {

        var ret1 = rayInterTriangle (ray, vec0, vec1, vec3);

        var ret2 = rayInterTriangle (ray, vec2, vec1, vec3);

        if (ret1 || ret2) {
            return true;
        }
        return false;
    }
    private void TransformObject () {
        // 軸矢印内にカーソルが入っている && マウスクリックされている
        if (Input.GetMouseButtonDown (0)) {
            if (m_axisType != AXIS_TYPE.NONE) {
                // コルーチン処理
                StartCoroutine (TransformObject (m_transType));
            }

        }
    }

    IEnumerator TransformObject (TRANSFORME_TYPE type) {
        Vector3 axis = GetAxisDirection ();
        Vector3 planeNormal = (Camera.main.transform.position - this.transform.position).normalized;
        Vector3 projectedAxis = Vector3.ProjectOnPlane (axis, planeNormal).normalized;
        Vector3 prevMousePosition = Vector3.zero;

        ICommand transformCommand = new TransformCommand (this.transform);

        // クリックを押し続けている
        while (!Input.GetMouseButtonUp (0)) {
            Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
            Vector3 mousePosition = PlaneLineCross (this.transform.position, planeNormal, ray.origin, ray.direction);
            mousePosition = ray.origin;

            if (prevMousePosition != Vector3.zero && mousePosition != Vector3.zero) {

                // 平行移動
                if (m_transType == TRANSFORME_TYPE.MOVE) {
                    Vector3 movement = Vector3.zero;

                    float moveAmount = GetAmountTransform (mousePosition - prevMousePosition, projectedAxis) * 50f;
                    movement = axis * moveAmount;

                    this.transform.Translate (movement, Space.World);
                }
                // 拡大縮小
                else if (m_transType == TRANSFORME_TYPE.SCALE) {
                    float scaleAmount = GetAmountTransform (mousePosition - prevMousePosition, projectedAxis) * 50f;

                    Vector3 targetScaleAmount = Vector3.one;
                    targetScaleAmount = axis * scaleAmount;

                    Vector3 targetScale = this.transform.localScale + targetScaleAmount;

                    this.transform.localScale = targetScale;

                    // 回転移動
                } else if (m_transType == TRANSFORME_TYPE.ROTATION) {
                    float rotateAmount = GetAmountTransform (mousePosition - prevMousePosition, projectedAxis) * 100f;
                    Vector3 rotationAxis = axis;

                    this.transform.Rotate (rotationAxis, rotateAmount, Space.Self);
                }

            }

            prevMousePosition = mousePosition;

            yield return null;
        }

        ((TransformCommand) transformCommand).SetTransformInfo ();
        m_undoRedoManager.Regist (transformCommand);

        m_axisType = AXIS_TYPE.NONE;
        transformUse = true;
    }
    // 平面と線の距離を取得する
    private float PlaneLineDistance (Vector3 planePoint, Vector3 planeNormal, Vector3 linePoint, Vector3 lineVec) {
        float dotNumerator = Vector3.Dot ((planePoint - linePoint), planeNormal);
        float dotDenominator = Vector3.Dot (lineVec, planeNormal);

        if (dotDenominator != 0f) {
            return dotNumerator / dotDenominator;
        }

        return 0;
    }
    // 平面と線の交差点を取得する
    private Vector3 PlaneLineCross (Vector3 plane, Vector3 planeNormal, Vector3 line, Vector3 lineDirection) {
        float distance = PlaneLineDistance (plane, planeNormal, line, lineDirection);

        if (distance != 0f) {
            return line + (lineDirection * distance);
        }

        return Vector3.zero;
    }
    // 移動変化量取得
    private float GetAmountTransform (Vector3 vector, Vector3 direction) {
        direction.Normalize ();
        return Vector3.Dot (vector, direction);
    }
    // 編集対象軸取得
    private Vector3 GetAxisDirection () {

        Vector3 directionX = Vector3.right;
        Vector3 directionY = Vector3.up;
        Vector3 directionZ = Vector3.forward;

        if (m_axisType != AXIS_TYPE.NONE) {
            if (m_axisType == AXIS_TYPE.X) {
                return directionX;
            }
            if (m_axisType == AXIS_TYPE.Y) {
                return directionY;
            }
            if (m_axisType == AXIS_TYPE.Z) {
                return directionZ;
            }
            if (m_axisType == AXIS_TYPE.ALL) {
                return Vector3.one;
            }
        }

        return Vector3.zero;
    }
    private void UndoRedoHandle () {
        if (Input.GetKey (KeyCode.LeftShift)) {
            if (Input.GetKeyDown (KeyCode.Z)) {
                Undo ();
            } else if (Input.GetKeyDown (KeyCode.Y)) {
                Redo ();
            }
        }
    }
    public void Undo () {
        m_undoRedoManager.Undo ();
        undoRedoUse = true;
    }
    public void Redo () {
        m_undoRedoManager.Redo ();
        undoRedoUse = true;
    }
    public void UpdateTransform (Vector3 pos, Quaternion rot, Vector3 scl) {
        ICommand transformCommand = new TransformCommand (this.transform);

        this.transform.localPosition = pos;
        this.transform.localRotation = rot;
        this.transform.localScale = scl;

        ((TransformCommand) transformCommand).SetTransformInfo ();
        m_undoRedoManager.Regist (transformCommand);
    }
    public bool GetTransformUse () {
        var ret = transformUse;
        return ret;
    }
    public bool GetUndoRedoUse () {
        var ret = undoRedoUse;
        return ret;
    }
    public void SetTransformUse (bool flag) {
        transformUse = flag;
    }
    public void SetUndoRedoUse (bool flag) {
        undoRedoUse = flag;
    }
}