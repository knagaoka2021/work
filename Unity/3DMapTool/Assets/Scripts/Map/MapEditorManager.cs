using System.Collections;
using System.Collections.Generic;
using System.Linq;
using CommonProperty;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class MapEditorManager : MonoBehaviour {
    public MapManager _mapManager;
    private MapAreaController mapAreaController;
    private CreateObjectSelect select;
    public Material m_selectMat;
    private ModelTool m_tool;
    private ModelTool.TRANSFORME_TYPE m_transType;

    private Vector3 m_prevPos = Vector3.zero;
    private Vector3 m_prevRot = Vector3.zero;
    private Vector3 m_prevScl = Vector3.zero;

    private List<string> m_areaDropValue = new List<string> ();
    private List<string> m_areaObjectDropValue = new List<string> ();
    private List<string> m_deleteReservation = new List<string> ();
    private Dictionary<string, int> m_objectDictionary = new Dictionary<string, int> ();
    private Dictionary<string, string> m_objectNameDictionary = new Dictionary<string, string> ();
    private int m_prevAreaObjectNum = 0;

    void Start () {
        SetListenerDropMapAreaObjects ();
    }

    void Update () {
        Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
        RaycastHit hit = new RaycastHit ();
        int layerMask = 1 << 8;

        string selected = GetSelectRadio ();

        if (Input.GetMouseButtonDown (0)) {
#if (UNITY_EDITOR || UNITY_STANDALONE_WIN)
            if (EventSystem.current.IsPointerOverGameObject ()) return;
#else
            if (EventSystem.current.IsPointerOverGameObject (Input.GetTouch (0).fingerId)) {
                return;
            }
#endif

            if (selected == "ADD") { // 追加処理
                Vector3 mousePosition = Input.mousePosition;
                mousePosition.z = 5f;

                GameObject obj = GameObject.Find ("MapManager");
                if (obj != null) {
                    string objName = Const.EMPTY;
                    select = obj.GetComponent<CreateObjectSelect> ();
                    GameObject prefab = select.SelectObject (GetSelectDropMapAreaObject ());
                    GameObject o = Instantiate (prefab, Camera.main.ScreenToWorldPoint (mousePosition), prefab.transform.rotation);
                    objName = StringUtils.Replace (o.name, Const.STRING_CLONE, Const.EMPTY);

                    string sortName = SortNameObject (objName);
                    o.name = sortName;
                    o.transform.parent = GetTargetChild (obj.name, GetDropMapArea ());
                    m_objectNameDictionary.Add (sortName, objName);
                    m_areaObjectDropValue.Add (sortName);
                    InitDropMapAreaObjects ();
                    SetDropMapAreaObjects (m_areaObjectDropValue);

                }
            } else if (selected == "EDIT") { // 編集処理
                if (Physics.Raycast (ray, out hit, Mathf.Infinity, layerMask)) {
                    if (m_tool != null) {
                        if (!m_tool.GetAxisSelected ()) {
                            Destroy (m_tool);
                            SetSelectObject (hit.collider.gameObject);
                        }
                    } else {
                        SetSelectObject (hit.collider.gameObject);
                    }

                }

            } else if (selected == "DELETE") { // 削除処理
                if (Physics.Raycast (ray, out hit, Mathf.Infinity, layerMask)) {
                    GameObject target = hit.collider.gameObject;
                    Renderer r = target.GetComponent<Renderer> ();
                    if (r != null) {
                        Material prevMaterial = r.material;
                        target.GetComponent<Renderer> ().material = m_selectMat;
                        OkCancelDialog dialog = this.GetComponent<OkCancelDialog> ();
                        if (dialog != null) {
                            dialog.ShowDialogEditor (target, prevMaterial, hit.collider.name, "削除しますか？");
                            m_deleteReservation.Add (target.name);

                        }

                    } else if (UnityUtils.HasChild (target)) {

                        Dictionary<string, Material> dic = new Dictionary<string, Material> ();
                        foreach (Renderer child in target.transform.GetComponentsInChildren<Renderer> ()) {
                            dic.Add (child.name, child.material);
                            child.material = m_selectMat;
                        }
                        OkCancelDialog dialog = this.GetComponent<OkCancelDialog> ();
                        if (dialog != null) {
                            dialog.ShowDialogEditor (target, dic, target.name, "削除しますか？");
                            m_deleteReservation.Add (target.name);

                        }

                    }
                }
            }
        }
        if (selected == "EDIT") { // 編集処理
            if (GetObjectSelectRadio () == "MOVE") {
                m_transType = ModelTool.TRANSFORME_TYPE.MOVE;
            } else if (GetObjectSelectRadio () == "ROTATE") {
                m_transType = ModelTool.TRANSFORME_TYPE.ROTATION;
            } else if (GetObjectSelectRadio () == "SCALE") {
                m_transType = ModelTool.TRANSFORME_TYPE.SCALE;
            }

            if (m_tool != null) {
                m_tool.ChangeTransformType (m_transType);
                if (m_tool.GetAxisSelected ()) {
                    UpdateTagetModelInfoDisp (m_tool.gameObject);
                } else {
                    if (m_tool.GetTransformUse () || m_tool.GetUndoRedoUse ()) {
                        UpdateTagetModelInfoDisp (m_tool.gameObject);
                        m_tool.SetTransformUse (false);
                        m_tool.SetUndoRedoUse (false);
                    } else {
                        UpdateTagetModelInfo (m_tool.gameObject);
                    }
                }

            }
        } else {
            if (m_tool != null) {
                if (!m_tool.GetAxisSelected ()) {
                    Destroy (m_tool);
                }
            }
        }
        UnUseAreaObjectDelete ();
    }

    public void Create () {

        // マップエリア初期化
        Clear ();

        MapManager obj = Instantiate (_mapManager);
        obj.name = "MapManager";
        obj._areaNum = GetInputInt("MapAreaNum");
        obj._areaLength = GetInputFloat("MapAreaLength");
        obj._draw = GetToggleFlag ("Draw");
        obj._move = GetToggleFlag ("Move");
        obj.transform.position = Vector3.zero;
        obj._speed = GetInputFloat("MapAreaMoveSpeed");
        obj.Create ();

        // マップエリアのドロップダウン初期化
        InitDropMapArea ();
        InitDropMapAreaObjects ();
        m_objectDictionary.Clear ();
        m_objectNameDictionary.Clear ();
        m_areaDropValue.Clear ();

        // マップエリアオブジェクト生成
        foreach (GameObject mapArea in obj.GetArea ()) {
            // ドロップダウンにマップエリア追加
            m_areaDropValue.Add (mapArea.name);
        }
        SetDropMapArea (m_areaDropValue);

        // マップエリアのドロップダウン初期化
        InitDropMapAreaObject ();
        List<string> dropObjectOptions = new List<string> ();

        Dictionary<string, GameObject> dictionary = obj.GetComponent<SerializableMapArea> ().GetDictionary ();
        foreach (string key in dictionary.Keys) {
            dropObjectOptions.Add (key);
        }
        SetDropMapAreaObject (dropObjectOptions);

    }

    public void Clear () {
        GameObject obj = GameObject.Find ("MapManager");
        if (obj != null) {
            Destroy (obj);
        }

        // UIパラメータ初期化
        m_areaDropValue.Clear ();
        m_areaObjectDropValue.Clear ();
        m_objectDictionary.Clear ();
        m_objectNameDictionary.Clear ();
        InitDropMapArea ();
        InitDropMapAreaObject ();
        InitDropMapAreaObjects ();

    }
    public void Save () {
        mapAreaController = GameObject.Find ("Manager").GetComponent<MapAreaController> ();
        mapAreaController.dataCapture ();

    }
    public void Load () {

        // マップエリア初期化
        Clear ();

        mapAreaController = GameObject.Find ("Manager").GetComponent<MapAreaController> ();
        WMapAreaSave wsave = mapAreaController.LoadData ();

        if (wsave == null) {
            return;
        }

        MapManager obj = Instantiate (_mapManager);
        obj.name = "MapManager";
        // マップエリア生成数を設定
        obj._areaNum = wsave.areaMax;

        obj._areaLength = wsave.areaLength;
        obj._draw = GetToggleFlag ("Draw");
        obj._move = GetToggleFlag ("Move");
        obj.transform.position = Vector3.zero;
        obj.Create ();

        // マップエリアのドロップダウン初期化
        InitDropMapAreaObject ();
        List<string> dropObjectOptions = new List<string> ();

        Dictionary<string, GameObject> dictionary = obj.GetComponent<SerializableMapArea> ().GetDictionary ();
        foreach (string key in dictionary.Keys) {
            dropObjectOptions.Add (key);
        }
        SetDropMapAreaObject (dropObjectOptions);

        // マップエリア生成数を設定
        SetInputMapAreaNum (obj._areaNum);

        // マップエリア枠線長を設定
        SetInputMapAreaLength (obj._areaLength);

        // マップエリアのドロップダウン初期化
        InitDropMapArea ();
        m_areaObjectDropValue.Clear ();
        m_areaDropValue.Clear ();
        // マップエリアオブジェクトのドロップダウン初期化
        InitDropMapAreaObjects ();
        m_areaObjectDropValue.Add (Const.MESSAGE_UI_01);

        select = obj.GetComponent<CreateObjectSelect> ();

        m_objectDictionary.Clear ();
        m_objectNameDictionary.Clear ();

        string objName = Const.EMPTY;
        // マップエリアオブジェクト生成
        foreach (MapAreaSave mapArea in wsave.mapAreaSaveList) {

            foreach (Transform childArea in obj.transform) {
                if (childArea.name == mapArea.areaId) {
                    foreach (MapAreaObject mapAreaObject in mapArea.mapAreaObjectList) {
                        GameObject o = Instantiate (select.SelectObject (mapAreaObject.type));
                        objName = SortNameObject (mapAreaObject.type);
                        o.name = objName;
                        o.transform.position = childArea.TransformPoint (mapAreaObject.pos);
                        o.transform.rotation = mapAreaObject.rot;
                        o.transform.localScale = mapAreaObject.scl;
                        o.transform.parent = childArea;
                        m_areaObjectDropValue.Add (objName);
                        m_objectNameDictionary.Add (objName, mapAreaObject.type);
                    }
                }
            }
            // ドロップダウンにマップエリア追加
            m_areaDropValue.Add (mapArea.areaId);
            
        }
        SetDropMapArea (m_areaDropValue);
        SetDropMapAreaObjects (m_areaObjectDropValue);

    }
    public Dictionary<string, string> GetObjectNameDictionary () {
        return m_objectNameDictionary;
    }

    private int GetInputInt (string name) {
        var result = 0;
        Transform child = GetChild (name);
        string num = "";
        if (child != null) {
            num = child.GetComponent<InputField> ().text;
            
            int.TryParse (num, out result);
        }
        return result;
    }

    private void SetInputMapAreaNum (int value) {
        Transform child = GetChild ("MapAreaNum");
        if (child != null) {
            child.GetComponent<InputField> ().text = value.ToString ();
        }
    }
    private float GetInputFloat (string name) {
        var result = 0f;
        Transform child = GetChild (name);
        string num = "";
        if (child != null) {
            num = child.GetComponent<InputField> ().text;
            
            float.TryParse (num, out result);
        }
        return result;
    }
    private void SetInputMapAreaLength (float value) {
        Transform child = GetChild ("MapAreaLength");
        if (child != null) {
            child.GetComponent<InputField> ().text = value.ToString ();
        }
    }
    private bool GetToggleFlag (string name) {
        bool result = false;
        Transform child = GetChild (name);
        if (child != null) {
            result = child.GetComponent<Toggle> ().isOn;
        }

        return result;
    }
    private void InitDropMapArea () {
        Transform child = GetChild ("AreaDrop");
        child.GetComponent<Dropdown> ().ClearOptions ();
    }
    private void SetDropMapArea (List<string> dropOptions) {
        Transform child = GetChild ("AreaDrop");
        child.GetComponent<Dropdown> ().AddOptions (dropOptions);
    }
    private string GetDropMapArea () {
        Transform child = GetChild ("AreaDrop");
        Dropdown dropDown = child.GetComponent<Dropdown> ();
        return dropDown.options[dropDown.value].text;
    }
    private void InitDropMapAreaObject () {
        Transform child = GetChild ("AreaMapObjectDrop");
        child.GetComponent<Dropdown> ().ClearOptions ();
    }
    private void SetDropMapAreaObject (List<string> dropOptions) {
        Transform child = GetChild ("AreaMapObjectDrop");
        child.GetComponent<Dropdown> ().AddOptions (dropOptions);
    }
    private string GetSelectDropMapAreaObject () {
        Transform child = GetChild ("AreaMapObjectDrop");
        Dropdown dropDown = child.GetComponent<Dropdown> ();
        return dropDown.options[dropDown.value].text;
    }
    private void SetListenerDropMapAreaObjects () {
        Transform child = GetChild ("AreaMapObjectsDrop");
        Dropdown d = child.GetComponent<Dropdown> ();
        d.onValueChanged.AddListener (delegate { ChangeTargetModel (d.options[d.value].text); });
    }
    private void ChangeTargetModel (string name) {
        if (name == Const.MESSAGE_UI_01) {
            if (m_tool != null) {
                Destroy (m_tool);
            }
            return;
        }
        SetSelectRadio ("EDIT");

        if (m_tool != null) {
            if (!m_tool.GetAxisSelected ()) {
                Destroy (m_tool);
                SetSelectObject (GameObject.Find (name));
            }
        } else {
            SetSelectObject (GameObject.Find (name));
        }

    }
    private void InitDropMapAreaObjects () {
        Transform child = GetChild ("AreaMapObjectsDrop");
        child.GetComponent<Dropdown> ().ClearOptions ();
    }
    private void SetDropMapAreaObjects (List<string> dropOptions) {
        Transform child = GetChild ("AreaMapObjectsDrop");
        child.GetComponent<Dropdown> ().AddOptions (dropOptions);
    }
    private string GetSelectRadio () {
        Transform child = GetChild ("Radio");
        ToggleGroup toggleGroup = child.GetComponent<ToggleGroup> ();
        Toggle toggle = toggleGroup.ActiveToggles ().FirstOrDefault ();
        return toggle.name;
    }
    private void SetSelectRadio (string target) {
        Transform child = GetTargetChild ("Radio", target);
        Toggle toggle = child.GetComponent<Toggle> ();
        toggle.isOn = true;
    }
    private string SortNameObject (string name) {
        string result = Const.EMPTY;
        string checkKey = Const.EMPTY;

        // オブジェクト名の存在チェック
        foreach (string key in m_objectDictionary.Keys) {
            if (key == name) {
                checkKey = name;
                break;
            }
        }

        // 存在しない場合、登録
        if (checkKey.Equals (Const.EMPTY)) {
            m_objectDictionary.Add (name, 0);
            result = name + StringUtils.Format ("{0}", 0);
        } // 存在する場合 件数をカウントして更新
        else {
            int num = m_objectDictionary[checkKey];
            num++;
            m_objectDictionary[checkKey] = num;

            result = name + StringUtils.Format ("{0}", num);
        }

        return result;
    }
    private Transform GetTargetChild (string parent, string target) {

        Transform[] children = GameObject.Find (parent).GetComponentsInChildren<Transform> ();

        foreach (Transform child in children) {
            if (child.name == target) {
                return child;
            }
        }
        return null;
    }
    private void SetSelectObject (GameObject obj) {
        UpdateTagetModelInfoDisp (obj);
        m_tool = obj.AddComponent<ModelTool> ();
        m_tool.Create (m_transType);

    }
    private string GetObjectSelectRadio () {
        string result = "";
        foreach (Transform child in GetTargetChilds (GetChild ("ObjectPanel"))) {
            if (child.name == "Radio") {
                ToggleGroup toggleGroup = child.GetComponent<ToggleGroup> ();
                Toggle toggle = toggleGroup.ActiveToggles ().FirstOrDefault ();
                result = toggle.name;
                break;
            }
        }

        return result;
    }
    public void UpdateTagetModelInfoDisp (GameObject obj) {
        float value;
        Vector3 pos = Vector3.zero;
        Vector3 rot = Vector3.zero;
        Vector3 scl = Vector3.zero;

        foreach (Transform child in GetTargetChilds (GetChild ("ObjectPanel"))) {
            if (child.name == "Target") {
                child.gameObject.GetComponent<Text> ().text = obj.name;
            }
            if (child.name == "Pos") {
                pos = obj.transform.localPosition;
                foreach (Transform grandchild in GetTargetChilds (child)) {

                    if (grandchild.name == "X") {
                        grandchild.gameObject.GetComponent<InputField> ().text = pos.x.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        pos.x = value;
                    }
                    if (grandchild.name == "Y") {
                        grandchild.gameObject.GetComponent<InputField> ().text = pos.y.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        pos.y = value;
                    }
                    if (grandchild.name == "Z") {
                        grandchild.gameObject.GetComponent<InputField> ().text = pos.z.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        pos.z = value;
                    }
                }
            }
            if (child.name == "Rot") {
                rot = obj.transform.localEulerAngles;
                foreach (Transform grandchild in GetTargetChilds (child)) {
                    if (grandchild.name == "X") {
                        grandchild.gameObject.GetComponent<InputField> ().text = rot.x.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        rot.x = value;
                    }
                    if (grandchild.name == "Y") {
                        grandchild.gameObject.GetComponent<InputField> ().text = rot.y.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        rot.y = value;
                    }
                    if (grandchild.name == "Z") {
                        grandchild.gameObject.GetComponent<InputField> ().text = rot.z.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        rot.z = value;
                    }
                }
            }
            if (child.name == "Scl") {
                scl = obj.transform.localScale;
                foreach (Transform grandchild in GetTargetChilds (child)) {
                    if (grandchild.name == "X") {
                        grandchild.gameObject.GetComponent<InputField> ().text = scl.x.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        scl.x = value;
                    }
                    if (grandchild.name == "Y") {
                        grandchild.gameObject.GetComponent<InputField> ().text = scl.y.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        scl.y = value;
                    }
                    if (grandchild.name == "Z") {
                        grandchild.gameObject.GetComponent<InputField> ().text = scl.z.ToString ();
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        scl.z = value;
                    }
                }
            }
        }
        m_prevPos = pos;
        m_prevRot = rot;
        m_prevScl = scl;

    }
    public void UpdateTagetModelInfo (GameObject obj) {
        float value = 0f;
        Vector3 pos = Vector3.zero;
        Vector3 rot = Vector3.zero;
        Vector3 scl = Vector3.zero;

        foreach (Transform child in GetTargetChilds (GetChild ("ObjectPanel"))) {
            if (child.name == "Target") {
                child.gameObject.GetComponent<Text> ().text = obj.name;
            }
            if (child.name == "Pos") {
                foreach (Transform grandchild in GetTargetChilds (child)) {
                    if (grandchild.name == "X") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        pos.x = value;
                    }
                    if (grandchild.name == "Y") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        pos.y = value;
                    }
                    if (grandchild.name == "Z") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        pos.z = value;
                    }
                }
            }
            if (child.name == "Rot") {
                foreach (Transform grandchild in GetTargetChilds (child)) {
                    if (grandchild.name == "X") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        rot.x = value;
                    }
                    if (grandchild.name == "Y") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        rot.y = value;
                    }
                    if (grandchild.name == "Z") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        rot.z = value;
                    }
                }
            }
            if (child.name == "Scl") {
                foreach (Transform grandchild in GetTargetChilds (child)) {
                    if (grandchild.name == "X") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        scl.x = value;
                    }
                    if (grandchild.name == "Y") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        scl.y = value;
                    }
                    if (grandchild.name == "Z") {
                        float.TryParse (grandchild.gameObject.GetComponent<InputField> ().text, out value);
                        scl.z = value;
                    }
                }
            }

        }

        if (m_prevPos.Equals (pos) && m_prevRot.Equals (rot) && m_prevScl.Equals (scl)) {
            return;
        }

        m_prevPos = pos;
        m_prevRot = rot;
        m_prevScl = scl;

        m_tool.UpdateTransform (pos, Quaternion.Euler (rot.x, rot.y, rot.z), scl);
    }
    public void Play () {
        mapAreaController = GameObject.Find ("Manager").GetComponent<MapAreaController> ();
        mapAreaController.PlayControlArea (true);
    }
    public void Stop () {
        mapAreaController = GameObject.Find ("Manager").GetComponent<MapAreaController> ();
        mapAreaController.PlayControlArea (false);
    }
    private Transform GetChild (string name) {
        Transform[] children = this.GetComponentsInChildren<Transform> ();

        foreach (Transform child in children) {
            if (child.name == name) {
                return child;
            }
        }
        return null;
    }
    private Transform GetTargetChild (Transform target, string name) {
        Transform[] children = target.GetComponentsInChildren<Transform> ();

        foreach (Transform child in children) {
            if (child.name == name) {
                return child;
            }
        }
        return null;
    }
    private Transform[] GetTargetChilds (Transform target) {
        Transform[] children = target.GetComponentsInChildren<Transform> ();
        return children;
    }
    private void UnUseAreaObjectDelete () {
        int count = 0;
        foreach (string area in m_areaDropValue) {
            Transform[] childrenObjects = GameObject.Find (area).GetComponentsInChildren<Transform> ();
            count += childrenObjects.Length - 1;
        }

        if (count != m_prevAreaObjectNum) {
            List<string> deleteExclusionList = new List<string> ();
            foreach (string area in m_areaDropValue) {
                Transform[] childrenObjects = GameObject.Find (area).GetComponentsInChildren<Transform> ();
                foreach (Transform obj in childrenObjects) {

                    foreach (string deleteName in m_deleteReservation) {
                        if (deleteName == obj.name) {
                            deleteExclusionList.Add (deleteName);
                        }
                    }
                }
            }
            foreach (string deleteName in m_deleteReservation) {
                bool check = true;
                foreach (string exclusion in deleteExclusionList) {
                    if (deleteName == exclusion) {
                        check = false;
                        break;
                    }
                }
                if (check) {
                    for (int i = m_areaObjectDropValue.Count - 1; i >= 0; i--) {
                        if (m_areaObjectDropValue[i] == deleteName) {
                            m_areaObjectDropValue.Remove (deleteName);
                        }
                    }
                }

            }
            InitDropMapAreaObjects ();
            SetDropMapAreaObjects (m_areaObjectDropValue);

            m_deleteReservation.Clear ();
        }

        m_prevAreaObjectNum = count;
    }
    public void Undo () {
        if (m_tool != null) {
            m_tool.Undo ();
        }
    }
    public void Redo () {
        if (m_tool != null) {
            m_tool.Redo ();
        }
    }

}