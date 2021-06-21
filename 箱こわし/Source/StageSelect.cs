using System.Collections;
using System.Collections.Generic;
using CommonProperty;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class StageSelect : MonoBehaviour {
    private Image m_image;
    private ImageProperty m_imageProperty;
    private TextMeshProUGUI m_stageName;
    private TextMeshProUGUI m_stageView;
    private TextMeshProUGUI m_stageScore;

    // Start is called before the first frame update
    public void CreateStageSelect () {

        GetChildComponent ();

        // StreamingAssetsからAssetBundleをロードする
        var assetBundle = AssetBundle.LoadFromFile (Application.streamingAssetsPath + "/StreamingAssets/bundletex");
        // AssetBundle内のアセットにはビルド時のアセットのパス、またはファイル名、ファイル名＋拡張子でアクセスできる
        var texture = assetBundle.LoadAsset<Texture2D> ("Assets/Data/Texture/03.png");

        m_image.sprite = Sprite.Create (texture, new Rect (0, 0, texture.width, texture.height), Vector2.zero);

        assetBundle.Unload (false);

        m_imageProperty.SetimageFileName ("Assets/Data/Texture/03.png");

        m_stageName.text = "Stage01";

        m_stageView.text = "☆";

    }
    public void CreateStageSelect (StageSelectUISave save) {

        GetChildComponent ();

        string a = Application.dataPath + "/StreamingAssets/bundle_tex";

        // StreamingAssetsからAssetBundleをロードする
        var assetBundle = AssetBundle.LoadFromFile (a);
        // AssetBundle内のアセットにはビルド時のアセットのパス、またはファイル名、ファイル名＋拡張子でアクセスできる
        var texture = assetBundle.LoadAsset<Texture2D> (save.imagepath);

        if (texture != null) {
            m_image.sprite = Sprite.Create (texture, new Rect (0, 0, texture.width, texture.height), Vector2.zero);
        } else {
            Texture2D texture2D = new Texture2D(270, 270);
            m_image.sprite = Sprite.Create (texture2D, new Rect (0, 0, 270.0f, 270.0f), Vector2.zero);
        }

        assetBundle.Unload (false);

        m_imageProperty.SetimageFileName (save.imagepath);

        m_stageName.text = save.stagename;

        m_stageView.text = save.stageviewtext;

        m_stageScore.text = save.scoretext;

    }

    private void GetChildComponent () {
        Transform[] childrens = this.GetComponentsInChildren<Transform> ();

        foreach (Transform obj in childrens) {

            if (obj.name == Const.STRING_STAGEIMAGE) {
                m_image = obj.GetComponent<Image> ();
                m_imageProperty = obj.GetComponent<ImageProperty> ();

            } else if (obj.name == Const.STRING_STAGETEXT) {
                m_stageName = obj.GetComponent<TextMeshProUGUI> ();

            } else if (obj.name == Const.STRING_STAGEVIEWTEXT) {
                m_stageView = obj.GetComponent<TextMeshProUGUI> ();

            } else if (obj.name == Const.STRING_SCORETEXT) {
                m_stageScore = obj.GetComponent<TextMeshProUGUI> ();
            }

        }
    }
}