#pragma once
#include "Graphics/Sprite.h"
#include"scnen.h"

class SceneTutorial :public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial()override {};

    //‰Šú‰»
    void Initialize()override;
    //I—¹‰»
    void Finalize()override;
    //XVˆ—
    void Update(float elpsedTime)override;
    //•`‰æˆ—
    void Render()override;
private:
    Sprite* sprite = nullptr;
};