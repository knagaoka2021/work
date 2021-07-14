using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using UnityEngine;

public class CreateObjectSelect : MonoBehaviour {

    [SerializeField]
    public Dictionary<string, GameObject> m_gameObject;

    public GameObject SelectObject(string type){

        m_gameObject = GetComponent<SerializableMapArea>().GetDictionary();

        // キー項目に変換
        string key = type;
        
        // キー取得
        GameObject obj = m_gameObject[key];

        if(obj != null){
            return obj;
        }

        return new GameObject();
    }
}