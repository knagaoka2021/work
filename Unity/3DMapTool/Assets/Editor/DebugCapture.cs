using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DebugCapture : MonoBehaviour {
    public  RenderTexture CamTex;
    // Start is called before the first frame update
    public void Capture () {
        

        Texture2D tex = new Texture2D (CamTex.width, CamTex.height, TextureFormat.RGB24, false);
        RenderTexture.active = CamTex;
        tex.ReadPixels (new Rect (0, 0, CamTex.width, CamTex.height), 0, 0);

        if (PlayerSettings.colorSpace == ColorSpace.Linear) {
            // ガンマ補正
            var color = tex.GetPixels ();
            for (int i = 0; i < color.Length; i++) {
                color[i].r = Mathf.Pow (color[i].r, 1f / 2.2f);
                color[i].g = Mathf.Pow (color[i].g, 1f / 2.2f);
                color[i].b = Mathf.Pow (color[i].b, 1f / 2.2f);
            }
            tex.SetPixels (color);
        }

        tex.Apply ();

        byte[] bytes = tex.EncodeToPNG ();
        System.IO.File.WriteAllBytes ("Assets/Data/Texture/RenderTexture.png", bytes);
    }
}