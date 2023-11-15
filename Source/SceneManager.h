#pragma once
#include "scnen.h"

class SceneManager
{
private:
    SceneManager() {};
    ~SceneManager() {};
public:
    static SceneManager& Instance()
    {
        static SceneManager M_Instance;
        return M_Instance;
    }
    //更新処理
    void Update(float elpsedTime);
    //描画処理
    void Render();
    //シーンクリア
    void Clear();//管理してるシーンの終了処理を行う関数
    //シーン切り替え
    void ChangeScene(Scene* scene);
private:
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;

};

