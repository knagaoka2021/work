using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LineDraw : MonoBehaviour {

    public float _line_weight = 0.05f;
    public float _width = 1f;
    public float _height = 1f;
    public bool _draw = true;
    public bool _hierarchy = false;
    public bool _move = true;
    public float _swapLimit = 15.0f;
    public float _resetLimit = 80.0f;
    private LineRenderer _lineRenderer;
    private Vector3[] _positions;

    public float _speed = 0.05f;

    void Start () {
        _lineRenderer = gameObject.GetComponent<LineRenderer> ();
        _lineRenderer.positionCount = 0;

        if (_draw) {

            Vector3 localpos = this.transform.localPosition;
            _positions = new Vector3[] {
                new Vector3 (-_width + localpos.x, -_height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, -_height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, -_height + localpos.y, -_width + localpos.z), // 底面
                new Vector3 (-_width + localpos.x, _height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, _height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, -_width + localpos.z), // 前面
                new Vector3 (_width + localpos.x, _height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, _width + localpos.z), // 右面
                new Vector3 (_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, -_height + localpos.y, _width + localpos.z), // 後面
                new Vector3 (-_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, _height + localpos.y, -_width + localpos.z), // 左面
            };
            _lineRenderer.material = new Material (Shader.Find ("Sprites/Default"));
            _lineRenderer.startColor = Color.green;
            _lineRenderer.endColor = Color.green;
            _lineRenderer.startWidth = _line_weight;
            _lineRenderer.endWidth = _line_weight;

            _lineRenderer.positionCount = _positions.Length;
            _lineRenderer.SetPositions (_positions);
        }
    }

    // Update is called once per frame
    void Update () {
        lineUpdate ();
    }
    public void Create (float width, float height, bool draw, bool move, float speed) {

        _width = width;
        _height = height;
        _draw = draw;
        _move = move;
        _speed = speed;
    }

    private void lineUpdate () {

        if (_draw) {
            Vector3 localpos = this.transform.localPosition;
            _positions = new Vector3[] {
                new Vector3 (-_width + localpos.x, -_height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, -_height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, -_height + localpos.y, -_width + localpos.z), // 底面
                new Vector3 (-_width + localpos.x, _height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, _height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, -_width + localpos.z), // 前面
                new Vector3 (_width + localpos.x, _height + localpos.y, -_width + localpos.z),
                new Vector3 (_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (_width + localpos.x, -_height + localpos.y, _width + localpos.z), // 右面
                new Vector3 (_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, -_height + localpos.y, _width + localpos.z), // 後面
                new Vector3 (-_width + localpos.x, _height + localpos.y, _width + localpos.z),
                new Vector3 (-_width + localpos.x, _height + localpos.y, -_width + localpos.z), // 左面
            };
            _lineRenderer.positionCount = _positions.Length;
            _lineRenderer.SetPositions (_positions);
        }
        if (_move) {
            this.transform.position -= new Vector3 (0f, 0f, _speed);
        }

    }

    public void SetMove (bool flag) {
        _move = flag;
    }
}