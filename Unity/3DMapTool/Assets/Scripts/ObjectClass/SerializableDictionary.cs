using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace Serialize {

    /// <summary>
    /// シリアル管理クラス
    /// </summary>
    [System.Serializable]
    public class SerialBase<Key, Value, Type> where Type : KeyAndValue<Key, Value> {
        [SerializeField]
        private List<Type> list = null;
        private Dictionary<Key, Value> dictionary = null;

        public Dictionary<Key, Value> GetDictionary () {
            if (dictionary == null) {
                dictionary = ConvertListToDictionary (list);
            }
            return dictionary;
        }

        /// <summary>
        /// Editor Only
        /// </summary>
        public List<Type> GetList () {
            return list;
        }

        static Dictionary<Key, Value> ConvertListToDictionary (List<Type> list) {
            Dictionary<Key, Value> dictionary = new Dictionary<Key, Value> ();
            foreach (KeyAndValue<Key, Value> pair in list) {
                dictionary.Add (pair.key, pair.value);
            }
            return dictionary;
        }
    }

    /// <summary>
    /// シリアル化 KeyValuePair
    /// </summary>
    [System.Serializable]
    public class KeyAndValue<Key, Value> {
        public Key key;
        public Value value;

        public KeyAndValue (Key key, Value value) {
            this.key = key;
            this.value = value;
        }
        public KeyAndValue (KeyValuePair<Key, Value> pair) {
            this.key = pair.Key;
            this.value = pair.Value;
        }
    }
}