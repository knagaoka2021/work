using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using UnityEngine;

public class OkCancelDialog : MonoBehaviour {
    // ダイアログを追加する親のCanvas
    [SerializeField] private Canvas parent = default;
    // 表示するダイアログ
    [SerializeField] private DialogEditorController m_dialogEditor = default;

    public void ShowDialogEditor (GameObject gameObject, Material material, string name, string message) {
        parent = GameObject.Find ("Canvas").GetComponent<Canvas> ();

        // 生成してCanvasの子要素に設定
        var dialog = Instantiate (m_dialogEditor);
        dialog.SetMessageText (name + message);
        dialog.SetPrevObject (gameObject);
        dialog.SetPrevMaterial (material);
        dialog.transform.SetParent (parent.transform, false);

    }
    public void ShowDialogEditor (GameObject gameObject, Dictionary<string, Material> dic, string name, string message) {
        parent = GameObject.Find ("Canvas").GetComponent<Canvas> ();

        // 生成してCanvasの子要素に設定
        var dialog = Instantiate (m_dialogEditor);
        dialog.SetMessageText (name + message);
        dialog.SetPrevObject (gameObject);
        dialog.SetDictionaryMaterial(dic);
        dialog.transform.SetParent (parent.transform, false);
    }
}