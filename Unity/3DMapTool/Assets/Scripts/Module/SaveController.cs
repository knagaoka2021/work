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
    public InputField m_saveFileName;
    [SerializeField]
    public InputField m_loadFileName;

    void Awake () {
#if UNITY_STANDALONE_WIN
        m_filePath = Application.dataPath + "/StreamingAssets/WindowsBundle";
#elif UNITY_ANDROID
        m_filePath = Application.streamingAssetsPath + "/AndoroidBundle";
#endif
    }

    public void Save (string json, string filePath) {

        byte[] arr = System.Text.Encoding.UTF8.GetBytes (json);

        // AES暗号化
        byte[] arrEncrypted = AesEncrypt (arr, Const.INT_AESKEYSIZE, Const.INT_AESBLOCKSIZE, Const.STRING_AESVECTOR, Const.STRING_AESKEY);

        // ファイル書き込み
        System.IO.File.WriteAllBytes (filePath, arrEncrypted);
    }

    public void SaveMapArea (String json) {
        string filePath = "";
        if (m_saveFileName.text.Equals ("")) {
            filePath = m_filePath + SettingPath.STRING_MAPAREA_FILEPATH;
        } else {
            filePath = m_filePath + "/" + m_saveFileName.text;
        }

        Save (json, filePath);
    }
    public WMapAreaSave LoadMapArea () {
        string filePath = "";
        if (m_loadFileName.text.Equals ("")) {
            filePath = m_filePath + SettingPath.STRING_MAPAREA_FILEPATH;
        } else {
            filePath = m_filePath + "/" + m_loadFileName.text;
        }

        if (File.Exists (filePath)) {

            string decryptStr = AesEncryptLoad (filePath);

            if (!decryptStr.Equals ("")) {
                return JsonUtility.FromJson<WMapAreaSave> (decryptStr);
            }
        }
        return new WMapAreaSave ();

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
        string filePath = "";
#if UNITY_STANDALONE_WIN
        filePath = Application.dataPath + "/stageScore";
#elif UNITY_ANDROID
        filePath = Application.persistentDataPath + "/stageScore";
#endif

        if (!File.Exists (filePath)) {
            Save ("", filePath);
        }
    }
}