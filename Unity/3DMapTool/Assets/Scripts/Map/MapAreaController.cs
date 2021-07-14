using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using UnityEngine;

public class MapAreaController : MonoBehaviour {
    private int m_MapAreaBuffer = 1;
    private GameObject m_mapManager;
    private CreateObjectSelect m_objectSelect;
    private SaveController m_save;
    private MapAreaNode m_mapAreaNode;
    private WMapAreaSave m_wMapAreaSaveBuffer;

    void Start () {
        m_mapAreaNode = new MapAreaNode ();
    }

    public void dataCapture () {
        if (m_save == null) {
            m_save = GetComponent<SaveController> ();
        }
        if (m_mapManager == null) {
            m_mapManager = GameObject.Find ("MapManager");

            if (m_mapManager != null && m_objectSelect == null) {
                m_objectSelect = m_mapManager.GetComponent<CreateObjectSelect> ();
            }
        }
        Dictionary<string, string> objNameDictionary = GameObject.Find ("AreaCreatePanel").GetComponent<MapEditorManager> ().GetObjectNameDictionary();

        WMapAreaSave wMapAreaSave = new WMapAreaSave ();
        wMapAreaSave.mapAreaSaveList = new List<MapAreaSave> ();
        if (m_mapManager != null) {
            wMapAreaSave.areaMax = m_mapManager.GetComponent<MapManager> ()._areaNum;
            wMapAreaSave.areaLength = m_mapManager.GetComponent<MapManager> ()._areaLength;
            foreach (Transform child in m_mapManager.GetComponentInChildren<Transform> ()) {
                MapAreaSave mapAreaSave = new MapAreaSave ();
                mapAreaSave.areaId = child.name;
                mapAreaSave.mapAreaObjectList = new List<MapAreaObject> ();

                int id = 0;
                List<MapAreaObject> mapAreaObjList = new List<MapAreaObject> ();
                foreach (Transform childObj in child.GetComponentInChildren<Transform> ()) {
                    MapAreaObject mapAreaObj = new MapAreaObject ();

                    mapAreaObj.id = id;
                    mapAreaObj.pos = childObj.transform.localPosition;
                    mapAreaObj.rot = childObj.transform.rotation;
                    mapAreaObj.scl = childObj.transform.localScale;
                    string key = childObj.name;
                    mapAreaObj.type = objNameDictionary[key];
                    mapAreaObjList.Add (mapAreaObj);

                    id++;
                }
                mapAreaSave.mapAreaObjectList = mapAreaObjList;
                wMapAreaSave.mapAreaSaveList.Add (mapAreaSave);
            }
        }

        string json = JsonUtility.ToJson (wMapAreaSave);
        m_save.SaveMapArea (json);
        DebugLogger.Log (json);

    }
    public WMapAreaSave LoadData () {
        if (m_save == null) {
            m_save = GetComponent<SaveController> ();
        }

        m_wMapAreaSaveBuffer = m_save.LoadMapArea ();

        // ノード更新
        m_mapAreaNode.SetMapAreaNode (m_MapAreaBuffer, 0, m_wMapAreaSaveBuffer.areaMax - 1);

        return m_wMapAreaSaveBuffer;
    }
    public void PlayControlArea (bool flag) {
        if (m_mapManager == null) {
            m_mapManager = GameObject.Find ("MapManager");
        }
        if (m_mapManager != null) {
            foreach (Transform child in m_mapManager.GetComponentInChildren<Transform> ()) {
                child.GetComponent<LineDraw> ().SetMove (flag);
            }
        }
    }
    private MapAreaSave GetNextMapArea () {
        // 次のノードに切り替え
        m_mapAreaNode.AddNext ();

        // 現在のノードを取得
        int id = m_mapAreaNode.GetNext ();

        if(m_wMapAreaSaveBuffer != null){
            MapAreaSave mapArea = m_wMapAreaSaveBuffer.mapAreaSaveList[id];
            return mapArea;
        }
        
        return null;
    }
    public void Create (Transform parent) {

        if (m_mapManager == null) {
            m_mapManager = GameObject.Find ("MapManager");
        }
        if (m_mapManager != null) {
            CreateObjectSelect select = m_mapManager.GetComponent<CreateObjectSelect> ();
            MapAreaSave mapArea = GetNextMapArea ();

            if(mapArea == null){
                return;
            }

            foreach (MapAreaObject mapAreaObject in mapArea.mapAreaObjectList) {
                GameObject o = Instantiate (select.SelectObject (mapAreaObject.type));
                o.name = mapAreaObject.type;
                o.transform.position = parent.TransformPoint(mapAreaObject.pos);
                o.transform.rotation = mapAreaObject.rot;
                o.transform.localScale = mapAreaObject.scl;
                o.transform.parent = parent;
            }
        }
    }
    public void SetMapAreaBuffer(int buffer){
        m_MapAreaBuffer = buffer;
    }
    public int GetMapAreaBuffer(){
        return m_MapAreaBuffer;
    }
}