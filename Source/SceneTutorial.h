#pragma once
#include "Graphics/Sprite.h"
#include"scnen.h"

class SceneTutorial :public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial()override {};

    //������
    void Initialize()override;
    //�I����
    void Finalize()override;
    //�X�V����
    void Update(float elpsedTime)override;
    //�`�揈��
    void Render()override;
private:
    Sprite* sprite = nullptr;
};