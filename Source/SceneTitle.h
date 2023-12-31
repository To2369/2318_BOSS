#pragma once

#include "Graphics/Sprite.h"
#include"scnen.h"

class ScnenTitle :public Scene
{
public:
    ScnenTitle() {};
    ~ScnenTitle() override{};

    //初期化
    void Initialize()override;
    //終了化
    void Finalize()override;
    //更新処理
    void Update(float elpsedTime)override;
    //描画処理
    void Render()override;
private:
    Sprite* sprite = nullptr;
    Sprite* title = nullptr;
    Sprite* start = nullptr;
    Sprite* tutorial = nullptr;
    Sprite* exit = nullptr;
    float timer ;

    DirectX::XMFLOAT2 mousePosition;
};
