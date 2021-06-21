using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerContoller : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        // キー入力値 取得
        float input_x = Input.GetAxis("Horizontal");
        float input_y = Input.GetAxis("Vertical");

        // 入力値反転
        input_y *= -1;

        //プレイヤー回転制御
        rotationUpdate(input_x,input_y);
    }
    // XY軸別回転(input_x：入力水平値0～1.0f,input_y：入力垂直値0～1.0f)
    // 
    private void rotationUpdate(float input_x,float input_y){

        // X軸回転
        Vector3 localAngle = transform.localEulerAngles;
        localAngle.x += input_y;

        //可動域制御
        if(45.0f < localAngle.x && localAngle.x < 90.0f){
            localAngle.x = 45.0f;
        }
        if(270.0f < localAngle.x && localAngle.x < 270.0f + 45.0f){
            localAngle.x = 315.0f;
        }
        transform.localEulerAngles = localAngle;

        // Y軸回転
        var angle = transform.eulerAngles;
        angle.y += input_x;
        transform.eulerAngles = angle;

    }
}
