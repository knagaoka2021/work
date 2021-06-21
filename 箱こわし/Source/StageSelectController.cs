using System.Collections;
using System.Collections.Generic;
using System.Text;
using CommonProperty;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;

public class StageSelectController : MonoBehaviour {

    PointerEventData pointer;

    [SerializeField]
    private Canvas m_parent = default;

    [SerializeField]
    private StageSelect m_stageSelectObject = default;

    private bool isReceive = false;
    private string m_receiveStageName = "";
    private int m_receiveScore = 0;
    private WStageScoreSave m_score;
    private SaveController m_saveController;

    // Start is called before the first frame update
    void Start () {
        pointer = new PointerEventData (EventSystem.current);
        m_saveController = GetComponent<SaveController> ();
        m_saveController.LoadStageScore ();
        m_saveController.LoadStageUI ();
        m_saveController.SaveStageScore ();
    }

    // Update is called once per frame
    void Update () {
        // クリックしたら
        if (Input.GetMouseButtonDown (0)) {
            List<RaycastResult> results = new List<RaycastResult> ();
            // マウスポインタの位置にレイ飛ばし、ヒットしたものを保存
            pointer.position = Input.mousePosition;
            EventSystem.current.RaycastAll (pointer, results);

            if (results.Count > 0) {
                if (results[0].gameObject.transform.CompareTag ("SelectStage")) {
                    Transform[] childrens = results[0].gameObject.GetComponentsInChildren<Transform> ();

                    string stageName = "";

                    foreach (Transform obj in childrens) {

                        if (obj.name == Const.STRING_STAGETEXT) {
                            stageName = obj.GetComponent<TextMeshProUGUI> ().text;
                        }

                    }

                    OkCancelDialog dialog = results[0].gameObject.transform.GetComponent<OkCancelDialog> ();
                    if (dialog != null) {
                        dialog.ShowDialog (Const.SCENE.STAGESELECT, stageName, Const.STRING_MESSAGETEXT01);
                    }

                }
            }
        }
    }
    public void Create (WStageSelectUISave wsave) {
        foreach (StageSelectUISave save in wsave.stageSelectUIList) {
            StageSelect stageSelectObject = Instantiate (m_stageSelectObject);

            if (m_score != null) {
                foreach (StageScore score in m_score.stageScoreList) {
                    if (save.stagename.Equals (score.stagename)) {
                        save.scoretext = score.scoretext;
                        break;
                    }
                }

            }

            if (isReceive) {
                save.scoretext = UpdateStageSelectUISave (m_receiveStageName, save);
            }
            stageSelectObject.CreateStageSelect (save);

            stageSelectObject.transform.SetParent (m_parent.transform, false);
            stageSelectObject.transform.SetSiblingIndex (0);

            RectTransform rectTransform = stageSelectObject.GetComponent<RectTransform> ();
            rectTransform.localPosition = new Vector3 (save.x_pos, save.y_pos, save.z_pos);

        }
    }
    public void Debug_AddStageUI () {
        StageSelect stageSelectObject = Instantiate (m_stageSelectObject);
        stageSelectObject.CreateStageSelect ();
        stageSelectObject.transform.SetParent (m_parent.transform, false);
        stageSelectObject.transform.SetSiblingIndex (0);
    }
    public void Debug_ClearStageUI () {
        GameObject[] stages = GameObject.FindGameObjectsWithTag ("SelectStage");
        foreach (GameObject stage in stages) {
            Destroy (stage, 0.25f);
        }
    }
    public WStageSelectUISave GetStageSelectUI (WStageSelectUISave save) {
        save.stageSelectUIList.Clear ();

        GameObject[] stages = GameObject.FindGameObjectsWithTag ("SelectStage");
        foreach (GameObject stage in stages) {
            StageSelectUISave obj = new StageSelectUISave ();

            RectTransform rectTransform = stage.GetComponent<RectTransform> ();

            obj.x_pos = rectTransform.localPosition.x;
            obj.y_pos = rectTransform.localPosition.y;
            obj.z_pos = rectTransform.localPosition.z;

            Transform[] childrens = stage.GetComponentsInChildren<Transform> ();

            foreach (Transform child in childrens) {

                if (child.name == Const.STRING_STAGEIMAGE) {
                    obj.imagepath = child.GetComponent<ImageProperty> ().GetImageFileName ();

                } else if (child.name == Const.STRING_STAGETEXT) {
                    obj.stagename = child.GetComponent<TextMeshProUGUI> ().text;

                } else if (child.name == Const.STRING_STAGEVIEWTEXT) {
                    obj.stageviewtext = child.GetComponent<TextMeshProUGUI> ().text;

                } else if (child.name == Const.STRING_SCORETEXT) {
                    obj.scoretext = child.GetComponent<TextMeshProUGUI> ().text;
                }

            }
            save.stageSelectUIList.Add (obj);
        }
        return save;
    }
    private string UpdateStageSelectUISave (string key, StageSelectUISave save) {
        string scoreText = "";

        if (save.stagename.Equals (key)) {
            scoreText = Const.STRING_SCORE +
                StringUtils.CompareInt (save.scoretext.Replace (Const.STRING_SCORE, ""), StringUtils.Format ("{0}", m_receiveScore));
            isReceive = false;
            m_receiveStageName = "";
            m_receiveScore = 0;
        } else {
            scoreText = save.scoretext;
        }
        return scoreText;
    }
    public void SendData (string stageName, int score) {
        isReceive = true;
        m_receiveStageName = stageName;
        m_receiveScore = score;
    }
    public WStageScoreSave GetStageScore (WStageScoreSave save) {

        GameObject[] stages = GameObject.FindGameObjectsWithTag ("SelectStage");
        foreach (GameObject stage in stages) {
            StageScore obj = new StageScore ();

            Transform[] childrens = stage.GetComponentsInChildren<Transform> ();

            foreach (Transform child in childrens) {

                if (child.name == Const.STRING_STAGETEXT) {
                    obj.stagename = child.GetComponent<TextMeshProUGUI> ().text;

                } else if (child.name == Const.STRING_SCORETEXT) {
                    obj.scoretext = child.GetComponent<TextMeshProUGUI> ().text;
                }

            }
            save.stageScoreList.Add (obj);
        }
        return save;
    }
    public void SetScoreData (WStageScoreSave score) {
        m_score = score;
    }
}