using System.Collections;
using System.Collections.Generic;
using UniRx;
using UnityEngine;

public class SpwnObject : MonoBehaviour {

    private BaseObjectPool _pool;
    public GameObject baseObjectPrefub;

    // Start is called before the first frame update
    void Awake () {
        // 同期プール
        _pool = new BaseObjectPool ();
        _pool.baseObjectPrefub = baseObjectPrefub;
        // プール拡張設定
        _pool.PreloadAsync (50, 10).Subscribe ();

    }

    public void Spwn (Stage stage) {

        for (int i = 0; i < stage.GetXNum (); i++) {
            for (int j = 0; j < stage.GetZNum (); j++) {
                for (int k = 0; k < stage.GetYNum (); k++) {
                    BaseObject obj = _pool.Rent ();
                    obj.transform.position = new Vector3 ((i) - (stage.GetXNum () / 2) + stage.GetXOfset (),
                        (k) - (stage.GetYNum () / 2) + stage.GetYOfset (),
                        (j) - (stage.GetZNum () / 2) + stage.GetZOfset ());
                    obj.Make (_pool.Return);
                }
            }
        }
    }
    public void Spwn (WStageSave save) {
        foreach (StageSave stageObj in save.stageList) {
            BaseObject obj = _pool.Rent ();
            obj.transform.localPosition = stageObj.pos;
            obj.SetVectorType (stageObj.vectortype);
            obj.SetSpeed (stageObj.speed);

            obj.Make (_pool.Return);
        }
    }
    public void Clear () {
        GameObject[] satageObjects = GameObject.FindGameObjectsWithTag ("Explodable");
        foreach (GameObject satageobj in satageObjects) {
            satageobj.GetComponent<BaseObject> ().EndBaseObject ();

        }
    }
    public void Add () {
        BaseObject obj = _pool.Rent ();
        obj.transform.position = new Vector3 (0.0f, 1.0f, 0.0f);
        obj.GetComponent<Rigidbody> ().isKinematic = true;
        obj.Make (_pool.Return);
    }
}