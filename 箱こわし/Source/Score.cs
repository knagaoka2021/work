using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Score {
    public static int score = 0;
    public static int scoreBuffer = 0;
    public static int remainingNum =0;
    public static int missNum =0;

    public static void InitScore () {
        score = 0;
        scoreBuffer = 0;
    }
    public static void InitRemainingNum () {
        remainingNum = 0;
        missNum = 0;
    }
    public static void AddScore (int value) {
        score += value;
    }
    public static void AddScoreBuffer (int value) {
        scoreBuffer += value;
    }
    public static void AddRemainingNum (int value) {
        remainingNum += value;
    }
    public static void AddMissNum (int value) {
        missNum += value;
    }
    public static void SetRemainingNum (int value) {
        remainingNum = value;
    }
    public static int GetScore () {
        return score;
    }
    public static int GetScoreBuffer () {
        return scoreBuffer;
    }
    public static int GetRemainingNum () {
        return remainingNum;
    }
    public static int GetMissNum () {
        return missNum;
    }
}