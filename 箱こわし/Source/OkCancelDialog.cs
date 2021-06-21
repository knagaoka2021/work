using CommonProperty;
using UnityEngine;

public class OkCancelDialog : MonoBehaviour {
    // ダイアログを追加する親のCanvas
    [SerializeField] private Canvas parent = default;
    // 表示するダイアログ
    [SerializeField] private DialogController m_dialog = default;

    public void ShowDialog (Const.SCENE scene, string name, string message) {
        parent = GameObject.Find ("Canvas").GetComponent<Canvas> ();

        // 生成してCanvasの子要素に設定
        var dialog = Instantiate (m_dialog);
        dialog.SetScene (scene);
        dialog.SetStageName (name);
        dialog.SetMessageText (name + message);
        dialog.transform.SetParent (parent.transform, false);
        // ボタンが押されたときのイベント処理

    }
}