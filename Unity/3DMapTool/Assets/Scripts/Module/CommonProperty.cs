using System;
using UnityEngine;

namespace CommonProperty {
    /// <summary>
    /// 共通定数クラス
    /// </summary>
    public static class Const {
        public const int MAX_SCORE = 99999;
        public const int MAX_SCORE_NUM = 5;
        public const int MAX_SCORE_STRINGNUM = 6; // 「SOCORE:」
        public const int NOMAL_SCORE = 50;
        public const float MIN_DROPPOSITION = -15.0f;
        public const double DOUBLE_COUNT_SPEED = 0.04;
        public const char CHAR_ZERO = '0';
        public const char CHAR_PLUS = '+';
        public const char CHAR_MINUS = '-';
        public const string EMPTY = "";
        public const string STRING_CLONE = "(Clone)";
        public const string STRING_SCORE_ANIMATION = "ScoreMove";
        public const string STRING_REMAINING = "Last : ";
        public const string STRING_STAGEIMAGE = "StageImage";
        public const string STRING_STAGETEXT = "StageNameText";
        public const string STRING_MESSAGETEXT = "MessageText";
        public const string STRING_STAGEVIEWTEXT = "StageViewText";
        public const string STRING_SCORETEXT = "StageScoreText";
        public const string STRING_MESSAGETEXT01 = "を\nあそびますか？";
        public const string STRING_MESSAGETEXT02 = "ステージセレクトに戻りますか？";
        public const string STRING_TEXTSUCCESS = "クリア";
        public const string STRING_TEXTFAILURE = "ゲームオーバー";
        public const string STRING_SCORE = "SCORE:";
        public const string MESSAGE_UI_01 = "---↓---";
        public static readonly char[] CHAR_SCORE = { 'S', 'C', 'O', 'R', 'E', ':' };

        public const int INT_AESKEYSIZE = 128;
        public const int INT_AESBLOCKSIZE = 128;
        public const string STRING_AESVECTOR = "1234567890123456";
        public const string STRING_AESKEY = "1234567890123456";

        public static readonly Vector3[] VECTOR_VALUE = {
            new Vector3 (0.0f, 0.0f, 0.0f),
            new Vector3 (1.0f, 0.0f, 0.0f),
            new Vector3 (-1.0f, 0.0f, 0.0f),
            new Vector3 (0.0f, 1.0f, 0.0f),
            new Vector3 (0.0f, -1.0f, 0.0f),
            new Vector3 (0.0f, 0.0f, 1.0f),
            new Vector3 (0.0f, 0.0f, -1.0f),
            new Vector3 (1.0f, 1.0f, 0.0f),
            new Vector3 (-1.0f, 1.0f, 0.0f),
            new Vector3 (0.0f, 1.0f, 1.0f),
            new Vector3 (0.0f, 1.0f, -1.0f),
            new Vector3 (1.0f, -1.0f, 0.0f),
            new Vector3 (-1.0f, -1.0f, 0.0f),
            new Vector3 (0.0f, -1.0f, 1.0f),
            new Vector3 (0.0f, -1.0f, -1.0f),
            new Vector3 (1.0f, 0.0f, 1.0f),
            new Vector3 (-1.0f, 0.0f, -1.0f),
            new Vector3 (1.0f, 0.0f, -1.0f),
            new Vector3 (-1.0f, 0.0f, 1.0f),

        };

        // ベクトル種類
        public enum VECTOR_TYPE {
            ZERO,
            X_PLUS,
            X_MINUS,
            Y_PLUS,
            Y_MINUS,
            Z_PLUS,
            Z_MINUS,
            XY_PLUS,
            XY_MINUSPLUS,
            YZ_PLUS,
            YZ_PLUSMINUS,
            XY_PLUSMINUS,
            XY_MINUS,
            YZ_MINUSPLUS,
            YZ_MINUS,
            XZ_PLUS,
            XZ_MINUS,
            XZ_PLUSMINUS,
            XZ_MINUSPLUS
        }
        // シーン種類
        public enum SCENE {
            TITLE,
            STAGESELECT,
            GAME,
            MAPEDITOR,
            MAX_SCEANE
        }
    }
    /// <summary>
    /// 設定定義クラス
    /// </summary>
    public static class SettingPath {
        public const string STRING_STAGESELECTUI_FILEPATH = "/StageSelectUISave";
        public const string STRING_STAGE_FILEPATH = "/StageSave";
        public const string STRING_MAPAREA_FILEPATH = "/MapAreaSave";

    }
    /// <summary>
    /// string 型の汎用関数クラス
    /// </summary>
    public static class StringUtils {
        /// <summary>
        /// 引数値を指定書式の文字列に変換する。
        /// </summary>
        public static string Format<T> (string format, T arg) {
            return string.Format (format, arg.ToString ());
        }
        /// <summary>
        /// 引数値を指定書式の文字列に変換する。
        /// </summary>
        public static string Format<T1, T2> (string format, T1 arg1, T2 arg2) {
            return string.Format (format, arg1.ToString (), arg2.ToString ());
        }
        /// <summary>
        /// 引数値が最大値を返却する
        /// </summary>
        public static string CompareInt (string value1, string value2) {
            var parse1 = 0;
            var parse2 = 0;
            int.TryParse (value1, out parse1);
            int.TryParse (value2, out parse2);

            int result = parse1.CompareTo (parse2);
            string max = "";

            if (result > 0) {
                max = value1;
            } else {
                max = value2;
            }
            return max;
        }
        /// <summary>
        /// 引数値が最大値を返却する
        /// </summary>
        public static int CompareInt (int value1, int value2) {
            int result = value1.CompareTo (value2);
            int max = 0;

            if (result > 0) {
                max = value1;
            } else {
                max = value2;
            }
            return max;

        }
        /// <summary>
        /// 対象文字列を置換する
        /// </summary>
        public static string Replace (string value, string target, string replace) {
            return value.Replace (target, replace);

        }
    }
    public static class UnityUtils {
        /// <summary>
        /// GameObject 型の子要素の存在確認
        /// </summary>
        public static bool HasChild (this GameObject gameObject) {
            return 0 < gameObject.transform.childCount;
        }
    }
}