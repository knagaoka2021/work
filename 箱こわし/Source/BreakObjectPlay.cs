using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using UniRx;
using UnityEngine;

public class BreakObjectPlay : MonoBehaviour {
    private BreakObjectPool _pool;
    private TextObjectPool t_pool;
    public GameObject breakObjectPrefub;
    public GameObject textPrefub;
    private Camera cam;
    private AudioSource soundSe01;

    // Start is called before the first frame update
    void Start () {
        // 同期プール
        _pool = new BreakObjectPool ();
        _pool.breakObjectPrefub = breakObjectPrefub;

        t_pool = new TextObjectPool ();
        t_pool.textPrefub = textPrefub;

        // プール拡張設定
        _pool.PreloadAsync (80, 20).Subscribe ();

        t_pool.PreloadAsync (10, 5).Subscribe ();

        cam = Camera.main;

        soundSe01 = GameObject.Find ("SE").GetComponent<AudioSource> ();
    }

    // Update is called once per frame
    void Update () {
        if (Application.isEditor) {
            if (Input.GetMouseButtonDown (0)) {
                RaycastHit hit;
                if (Physics.SphereCast (cam.ScreenPointToRay (Input.mousePosition), 0.5f, out hit, 100f)) {
                    if (hit.transform.CompareTag ("Explodable")) {
                        BreakMesh (hit.transform);
                    }
                }
            }
        } else {
            if (Input.GetMouseButtonDown (0)) {
                RaycastHit hit;
                if (Physics.SphereCast (cam.ScreenPointToRay (Input.mousePosition), 0.5f, out hit, 100f)) {
                    if (hit.transform.CompareTag ("Explodable")) {
                        BreakMesh (hit.transform);
                    }
                }
            }
            if (Input.touchCount > 0) {
                Touch[] touches = Input.touches;

                for (int i = 0; i < touches.Length; i++) {
                    RaycastHit hit;
                    if (Physics.SphereCast (cam.ScreenPointToRay (touches[i].position), 0.25f, out hit, 100f)) {
                        if (hit.transform.CompareTag ("Explodable")) {
                            BreakMesh (hit.transform);
                        }
                    }
                }
            }
        }
    }

    void BreakMesh (Transform target) {
        Mesh mesh = target.GetComponent<MeshFilter> ().mesh;
        Vector3[] vertices = mesh.vertices;
        Vector3[] normals = mesh.normals;
        int[] triangles = mesh.triangles;
        Vector2[] uvs = mesh.uv;
        int index = 0;

        // remove  from original
        BaseObject t = target.GetComponent<BaseObject> ();

        // get each face
        for (int i = 0; i < triangles.Length; i += 3) {
            BreakObject obj = _pool.Rent ();
            Vector3 averageNormal = (normals[triangles[i]] + normals[triangles[i + 1]] + normals[triangles[i + 2]]).normalized;
            Vector3 s = target.GetComponent<Renderer> ().bounds.size;
            float extrudeSize = ((s.x + s.y + s.z) / 3);
            CreateMeshParts (obj, extrudeSize, target.transform.position, target.transform.rotation, target.GetComponent<Renderer> ().material, index, averageNormal, vertices[triangles[i]], vertices[triangles[i + 1]], vertices[triangles[i + 2]], uvs[triangles[i]], uvs[triangles[i + 1]], uvs[triangles[i + 2]]);
            index++;

            // イベント設定
            obj.SetEvent ();

            obj.AddForce (t.GetForce ());

            // コールバック用にプール返却処理を渡す
            obj.Make (_pool.Return);

        }

        t.EndBaseObject ();

        TextObject tobj = t_pool.Rent ();
        tobj.transform.position = target.transform.position;
        tobj.TextAnimationPlay (Const.STRING_SCORE_ANIMATION, Const.NOMAL_SCORE);
        tobj.Make (t_pool.Return);
        Score.AddScoreBuffer (Const.NOMAL_SCORE);

        soundSe01.PlayOneShot (soundSe01.clip);

    }
    void CreateMeshParts (BreakObject go, float extrudeSize, Vector3 pos, Quaternion rot, Material mat, int index, Vector3 faceNormal, Vector3 v1, Vector3 v2, Vector3 v3, Vector2 uv1, Vector2 uv2, Vector2 uv3) {

        Mesh mesh = go.GetComponent<MeshFilter> ().mesh;
        go.GetComponent<Renderer> ().material = mat;
        go.transform.position = pos;
        go.transform.rotation = rot;

        // 固定で三角錐作成
        Vector3[] vertices = new Vector3[3 * 4];
        int[] triangles = new int[3 * 4];
        Vector2[] uvs = new Vector2[3 * 4];

        // get centroid
        Vector3 v4 = (v1 + v2 + v3) / 3;
        // extend to backwards
        v4 = v4 + (-faceNormal) * extrudeSize * 0.5f;
        //v4 = v4 + (-faceNormal) * extrudeSize * 0.25f * 0.3f;

        // not shared vertices
        // orig face
        //vertices[0] = (v1);
        vertices[0] = (v1);
        vertices[1] = (v2);
        vertices[2] = (v3);
        // right face
        vertices[3] = (v1);
        vertices[4] = (v2);
        vertices[5] = (v4);
        // left face
        vertices[6] = (v1);
        vertices[7] = (v3);
        vertices[8] = (v4);
        // bottom face
        vertices[9] = (v2);
        vertices[10] = (v3);
        vertices[11] = (v4);

        // orig face
        triangles[0] = 0;
        triangles[1] = 1;
        triangles[2] = 2;
        //  right face
        triangles[3] = 5;
        triangles[4] = 4;
        triangles[5] = 3;
        //  left face
        triangles[6] = 6;
        triangles[7] = 7;
        triangles[8] = 8;
        //  bottom face
        triangles[9] = 11;
        triangles[10] = 10;
        triangles[11] = 9;

        // orig face
        uvs[0] = uv1;
        uvs[1] = uv2;
        uvs[2] = uv3; // todo

        Vector2 ze = Vector2.zero;
        // right face
        uvs[3] = ze;
        uvs[4] = ze;
        uvs[5] = ze; // todo

        // left face
        uvs[6] = ze;
        uvs[7] = ze;
        uvs[8] = ze; // todo
        // bottom face (mirror?) or custom color? or fixed from uv?
        uvs[9] = ze;
        uvs[10] = ze;
        uvs[11] = ze; // todo

        mesh.vertices = vertices;
        mesh.uv = uvs;
        mesh.triangles = triangles;

        // メッシュ作成の再計算
        mesh.RecalculateBounds ();
        mesh.RecalculateNormals ();
        // メッシュタンジェント再計算
        CalculateMeshTangents (mesh);

        // 物理演算の元を追加
        MeshCollider mc = go.GetComponent<MeshCollider> ();
        mc.sharedMesh = mesh;
        mc.convex = true;
    }

    void CalculateMeshTangents (Mesh mesh) {
        //speed up math by copying the mesh arrays
        int[] triangles = mesh.triangles;
        Vector3[] vertices = mesh.vertices;
        Vector2[] uv = mesh.uv;
        Vector3[] normals = mesh.normals;

        //variable definitions
        int triangleCount = triangles.Length;
        int vertexCount = vertices.Length;

        Vector3[] tan1 = new Vector3[vertexCount];
        Vector3[] tan2 = new Vector3[vertexCount];

        Vector4[] tangents = new Vector4[vertexCount];

        for (long a = 0; a < triangleCount; a += 3) {
            long i1 = triangles[a + 0];
            long i2 = triangles[a + 1];
            long i3 = triangles[a + 2];

            Vector3 v1 = vertices[i1];
            Vector3 v2 = vertices[i2];
            Vector3 v3 = vertices[i3];

            Vector2 w1 = uv[i1];
            Vector2 w2 = uv[i2];
            Vector2 w3 = uv[i3];

            float x1 = v2.x - v1.x;
            float x2 = v3.x - v1.x;
            float y1 = v2.y - v1.y;
            float y2 = v3.y - v1.y;
            float z1 = v2.z - v1.z;
            float z2 = v3.z - v1.z;

            float s1 = w2.x - w1.x;
            float s2 = w3.x - w1.x;
            float t1 = w2.y - w1.y;
            float t2 = w3.y - w1.y;

            float r = 1.0f / (s1 * t2 - s2 * t1);

            Vector3 sdir = new Vector3 ((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
            Vector3 tdir = new Vector3 ((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

            tan1[i1] += sdir;
            tan1[i2] += sdir;
            tan1[i3] += sdir;

            tan2[i1] += tdir;
            tan2[i2] += tdir;
            tan2[i3] += tdir;
        }

        for (int a = 0; a < vertexCount; ++a) {
            Vector3 n = normals[a];
            Vector3 t = tan1[a];
            Vector3.OrthoNormalize (ref n, ref t);
            tangents[a].x = t.x;
            tangents[a].y = t.y;
            tangents[a].z = t.z;
            tangents[a].w = (Vector3.Dot (Vector3.Cross (n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f;
        }
        mesh.tangents = tangents;
    }
    public void Clear () {
        GameObject[] breakeObjects = GameObject.FindGameObjectsWithTag ("BreakObject");
        foreach (GameObject breakeobj in breakeObjects) {
            breakeobj.GetComponent<BreakObject> ().EndObject ();

        }
    }
}