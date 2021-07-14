using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapManager : MonoBehaviour {
    public int _areaNum = 0;
    public float _areaLength = 1f;
    public bool _draw = true;
    public bool _move = true;
    public Vector3 _swap = Vector3.zero;
    public Vector3 _reset = Vector3.zero;
    public float _speed = 0.05f;
    private GameObject[] _area;
    private MapAreaController m_mapAreaContoller;

    void Start () {
        if (_area == null) {
            m_mapAreaContoller = GameObject.Find ("Manager").GetComponent<MapAreaController> ();
            _area = new GameObject[_areaNum];

            _swap = new Vector3 (0f, 0f, -(_areaLength));
            _reset = new Vector3 (0f, 0f, -(_areaLength * 2) + (_areaLength * _areaNum) + (_areaLength/2));
            
            for (int i = 0; i < _areaNum; i++) {
                _area[i] = new GameObject ("Area" + i);
                _area[i].transform.position = new Vector3 (0f, 0f, -(_areaLength) + (_areaLength * i));
                _area[i].AddComponent<LineRenderer> ();
                _area[i].AddComponent<LineDraw> ();
                _area[i].GetComponent<LineDraw> ().Create (_areaLength / 2,
                    _areaLength / 2,
                    _draw,
                    _move,
                    _speed);
                _area[i].transform.parent = this.transform;

            }
        }

    }
    public void Create () {
        if (_area == null) {
            m_mapAreaContoller = GameObject.Find ("Manager").GetComponent<MapAreaController> ();
            _area = new GameObject[_areaNum];
            _swap = new Vector3 (0f, 0f, -(_areaLength));
            _reset = new Vector3 (0f, 0f, -(_areaLength * 2) + (_areaLength * _areaNum) + (_areaLength/2));

            for (int i = 0; i < _areaNum; i++) {
                _area[i] = new GameObject ("Area" + i);
                _area[i].transform.position = new Vector3 (0f, 0f, (_areaLength * i));
                _area[i].AddComponent<LineRenderer> ();
                _area[i].AddComponent<LineDraw> ();
                _area[i].GetComponent<LineDraw> ().Create (_areaLength / 2,
                    _areaLength / 2,
                    _draw,
                    _move,
                    _speed);
                _area[i].transform.parent = this.transform;

            }
        }
    }

    void Update () {
        if (_area != null) {

            for (int i = 0; i < _areaNum; i++) {
                if (swapCheck (_area[i].transform.position)) {
                    _area[i].transform.position = _reset;
                }
            }
        }

    }
    private bool swapCheck (Vector3 pos) {
        if (_swap.z > pos.z + (_areaLength / 2)) {
            return true;
        }
        return false;
    }
    public GameObject[] GetArea () {
        return _area;
    }
}