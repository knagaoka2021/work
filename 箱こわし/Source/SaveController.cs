using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using CommonProperty;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class SaveController : MonoBehaviour {
    [SerializeField]
    private string m_filePath;
    [SerializeField]
    private WStageSelectUISave m_stageSelectUI;
    [SerializeField]
    private WStageSave m_stage;
    [SerializeField]
    private bool isDebug = false;
    [SerializeField]
    public InputField m_saveFileName;
    [SerializeField]
    public InputField m_loadFileName;
    [SerializeField]
    private WStageScoreSave m_stageScore;

    void Awake () {
        m_filePath = Application.dataPath + "/StreamingAssets";
        m_stageSelectUI = new WStageSelectUISave ();
        m_stageSelectUI.stageSelectUIList = new List<StageSelectUISave> ();
        m_stageScore = new WStageScoreSave ();
        m_stageScore.stageScoreList = new List<StageScore> ();
    }

    public void Save (string json, string filePath) {

        byte[] arr = System.Text.Encoding.UTF8.GetBytes (json);

        // AES暗号化
        byte[] arrEncrypted = AesEncrypt (arr, Const.INT_AESKEYSIZE, Const.INT_AESBLOCKSIZE, Const.STRING_AESVECTOR, Const.STRING_AESKEY);

        // ファイル書き込み
        System.IO.File.WriteAllBytes (filePath, arrEncrypted);
    }

    public void SaveStageUI () {
        // 
        m_stageSelectUI = this.GetComponent<StageSelectController> ().GetStageSelectUI (m_stageSelectUI);
        // 
        string json = JsonUtility.ToJson (m_stageSelectUI);

        Save (json, m_filePath + SettingPath.STRING_STAGESELECTUI_FILEPATH);
    }

    public void LoadStageUI () {

        if (File.Exists (m_filePath + SettingPath.STRING_STAGESELECTUI_FILEPATH)) {
            m_stageSelectUI.stageSelectUIList.Clear ();

            if (isDebug) {
                // 
                this.GetComponent<StageSelectController> ().Debug_ClearStageUI ();
            }

            string decryptStr = AesEncryptLoad (m_filePath + SettingPath.STRING_STAGESELECTUI_FILEPATH);

            m_stageSelectUI = JsonUtility.FromJson<WStageSelectUISave> (decryptStr);

            this.GetComponent<StageSelectController> ().Create (m_stageSelectUI);
        } else {
            m_stageSelectUI.stageSelectUIList.Clear ();
            var assetBundle = AssetBundle.LoadFromFile (m_filePath + "/bundle_load");
            var selectUI = assetBundle.LoadAsset<TextAsset> ("Assets/Data/Load/StageSelectUISave.bytes");

            byte[] arrRead = selectUI.bytes;

            string decryptStr = AesEncryptLoad (arrRead);

            m_stageSelectUI = JsonUtility.FromJson<WStageSelectUISave> (decryptStr);

            this.GetComponent<StageSelectController> ().Create (m_stageSelectUI);

            assetBundle.Unload (false);
        }

    }
    public void SaveStage () {
        string filePath = "";
        if (m_saveFileName.text.Equals ("")) {
            filePath = m_filePath + SettingPath.STRING_STAGE_FILEPATH;
        } else {
            filePath = m_filePath + "/" + m_saveFileName.text;
        }

        m_stage = this.GetComponent<StageController> ().GetStage (m_stage);

        string json = JsonUtility.ToJson (m_stage);

        Save (json, filePath);
    }

    public void LoadStage (string stageName) {

        string filePath = m_filePath + "/" + stageName;
        if (isDebug) {
            // 
            this.GetComponent<StageController> ().Debug_ClearStage ();
            if (m_loadFileName.text.Equals ("")) {
                filePath = m_filePath + SettingPath.STRING_STAGE_FILEPATH;
            } else {
                filePath = m_filePath + "/" + m_loadFileName.text;
            }
        }

        if (File.Exists (filePath)) {
            // 
            m_stage.stageList.Clear ();

            string decryptStr = AesEncryptLoad (filePath);

            m_stage = JsonUtility.FromJson<WStageSave> (decryptStr);

            // 
            this.GetComponent<StageController> ().CreateStage (m_stage);
        } else {

            m_stage.stageList.Clear ();

            var assetBundle = AssetBundle.LoadFromFile (m_filePath + "/bundle_load");
            var stage = assetBundle.LoadAsset<TextAsset> ("Assets/Data/Load/" + stageName + ".bytes");

            byte[] arrRead = stage.bytes;

            string decryptStr = AesEncryptLoad (arrRead);

            m_stage = JsonUtility.FromJson<WStageSave> (decryptStr);

            this.GetComponent<StageController> ().CreateStage (m_stage);

            assetBundle.Unload (false);

        }
    }
    public void SaveStageScore () {
        string filePath = m_filePath + "/" + "stageScore";
        if (File.Exists (filePath)) {
            m_stageScore.stageScoreList.Clear();
            // 
            m_stageScore = this.GetComponent<StageSelectController> ().GetStageScore (m_stageScore);
            // 
            string json = JsonUtility.ToJson (m_stageScore);

            Save (json, filePath);
        }

    }

    public void LoadStageScore () {
        string filePath = m_filePath + "/" + "stageScore";

        if (File.Exists (filePath)) {

            string decryptStr = AesEncryptLoad (filePath);

            if (!decryptStr.Equals ("")) {
                m_stageScore = JsonUtility.FromJson<WStageScoreSave> (decryptStr);

                this.GetComponent<StageSelectController> ().SetScoreData (m_stageScore);
            }
        }
    }
    /// <summary>
    /// AES復号読み込み
    /// </summary>
    private string AesEncryptLoad (string filePath) {
        // AES設定値
        //===================================
        int aesKeySize = 128;
        int aesBlockSize = 128;
        string aesIv = "1234567890123456";
        string aesKey = "1234567890123456";
        //===================================

        // ファイル読み込み
        byte[] arrRead = System.IO.File.ReadAllBytes (filePath);

        // 復号化
        byte[] arrDecrypt = AesDecrypt (arrRead, aesKeySize, aesBlockSize, aesIv, aesKey);

        // byte配列を文字列に変換
        string decryptStr = System.Text.Encoding.UTF8.GetString (arrDecrypt);

        return decryptStr;

    }

    /// <summary>
    /// AES復号読み込み
    /// </summary>
    private string AesEncryptLoad (byte[] arrRead) {
        // AES設定値
        //===================================
        int aesKeySize = 128;
        int aesBlockSize = 128;
        string aesIv = "1234567890123456";
        string aesKey = "1234567890123456";
        //===================================

        // 復号化
        byte[] arrDecrypt = AesDecrypt (arrRead, aesKeySize, aesBlockSize, aesIv, aesKey);

        // byte配列を文字列に変換
        string decryptStr = System.Text.Encoding.UTF8.GetString (arrDecrypt);

        return decryptStr;

    }

    /// <summary>
    /// AES暗号化
    /// </summary>
    public byte[] AesEncrypt (byte[] byteText, int aesKeySize, int aesBlockSize, string aesIv, string aesKey) {
        // AESマネージャー取得
        var aes = GetAesManager (aesKeySize, aesBlockSize, aesIv, aesKey);
        // 暗号化
        byte[] encryptText = aes.CreateEncryptor ().TransformFinalBlock (byteText, 0, byteText.Length);

        return encryptText;
    }

    /// <summary>
    /// AES復号化
    /// </summary>
    public byte[] AesDecrypt (byte[] byteText, int aesKeySize, int aesBlockSize, string aesIv, string aesKey) {
        // AESマネージャー取得
        var aes = GetAesManager (aesKeySize, aesBlockSize, aesIv, aesKey);
        // 復号化
        byte[] decryptText = aes.CreateDecryptor ().TransformFinalBlock (byteText, 0, byteText.Length);

        return decryptText;
    }

    /// <summary>
    /// AesManagedを取得
    /// </summary>
    /// <param name="keySize">暗号化鍵の長さ</param>
    /// <param name="blockSize">ブロックサイズ</param>
    /// <param name="iv">初期化ベクトル(半角X文字（8bit * X = [keySize]bit))</param>
    /// <param name="key">暗号化鍵 (半X文字（8bit * X文字 = [keySize]bit）)</param>
    private AesManaged GetAesManager (int keySize, int blockSize, string iv, string key) {
        AesManaged aes = new AesManaged ();
        aes.KeySize = keySize;
        aes.BlockSize = blockSize;
        aes.Mode = CipherMode.CBC;
        aes.IV = Encoding.UTF8.GetBytes (iv);
        aes.Key = Encoding.UTF8.GetBytes (key);
        aes.Padding = PaddingMode.PKCS7;
        return aes;
    }
    public void SaveFile () {

        string filePath = m_filePath + "/" + "stageScore";
        Debug.Log (filePath);
        if (!File.Exists (filePath)) {
            Save ("", filePath);
        }
    }
}