#pragma once
#include "Graphics/Sprite.h"
#include"scnen.h"

class SceneTutorial :public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial()override {};

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

    DirectX::XMFLOAT2 mousePosition;
};