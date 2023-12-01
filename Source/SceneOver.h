#pragma once
#include "Graphics/Sprite.h"
#include"scnen.h"


class SceneOver :public Scene
{
public:
    SceneOver() {};
    ~SceneOver()override {};
    //������
    void Initialize()override;
    //�I����
    void Finalize()override;
    //�X�V����
    void Update(float elpsedTime)override;
    //�`�揈��
    void Render()override;

    void DrawDebugGui();
private:
    Sprite* sprite = nullptr;
    Sprite* exit = nullptr;
    Sprite* title = nullptr;
    Sprite* retry = nullptr;

    DirectX::XMFLOAT2 mousePosition;
};