using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class MapAreaSerial : Serialize.SerialBase<string, GameObject, MapAreaSerialPair>{

}

[System.Serializable]
public class MapAreaSerialPair : Serialize.KeyAndValue<string, GameObject>{

    public MapAreaSerialPair (string key, GameObject value) : base (key, value) {

    }
}

public class SerializableMapArea : MonoBehaviour
{
    //Inspectorに表示用
    public MapAreaSerial m_mapAreaSerial;

    void Awake () {
        #if UNITY_EDITOR
        foreach (KeyValuePair<string, GameObject> pair in m_mapAreaSerial.GetDictionary()) {
            Debug.Log ("Key : " + pair.Key + "  Value : "  + pair.Value);
        }
        #endif
    }
    public Dictionary<string, GameObject> GetDictionary(){
        return m_mapAreaSerial.GetDictionary();
    }
}