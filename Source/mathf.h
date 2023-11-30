#pragma once

class Mathf
{
public:
    //線形補完
    static float Lerp(float a, float b, float t);
    //指定範囲のランダム値を計算する
    static float RandameRange(float min, float max);
    //指定範囲のランダム値を計算する
    static float RandameRange2(float max, float min);
};
