#pragma once
#include"Graphics/Sprite.h"
#include"scnen.h"
#include<thread>
class SceneLoading :public Scene
{
public:
    SceneLoading(Scene* nextscene) :nextscene(nextscene) {}
    SceneLoading(){}
    ~SceneLoading()override {};
    //初期化
    void Initialize()override;
    //終了化
    void Finalize()override;
    //更新処理
    void Update(float elapsedTime)override;
    //描画処理
    void Render()override;
private:
    //ローディングスレッド
    static void LadingThread(SceneLoading* scene);

private:
    //次のシーン
    Scene* nextscene = nullptr;
    //次のシーンを作成してくれるスレッド
    std::thread* thread = nullptr;
    Sprite* sprite = nullptr;
    Sprite* ex = nullptr;
    bool exflag = false;
    float    angle = 0.0f;
    float positionX = -100;
    float positionY = 500;
};
