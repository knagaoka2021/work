using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ImageProperty : MonoBehaviour {
    [SerializeField]
    private string m_imageFileName = "";

    public string GetImageFileName () {
        return m_imageFileName;

    }
    public void SetimageFileName (string value) {
        m_imageFileName = value;
    }

}