﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AttachCamera : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        this.GetComponent<Canvas>().worldCamera  = GameObject.FindWithTag ("MainCamera").GetComponent<Camera> ();
    }
}
