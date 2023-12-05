#pragma once
#include "Graphics/Sprite.h"
#include"Graphics/Model.h"
#include"scnen.h"
#include"StageManager.h"
#include"stageMain.h"
#include"EnemyBoss.h"
#include"EnemyManager.h"
#include"camera.h"
#include"CameraController.h"
#include"player.h"
#include"PlayerManager.h"
#include"FierdBuff.h"

class SceneClear :public Scene
{
public:
	SceneClear() {};
	~SceneClear() override {};

    //������
    void Initialize()override;
    //�I����
    void Finalize()override;
    //�X�V����
    void Update(float elapsedTime)override;
    //�`�揈��
    void Render()override;

    void DrawDebugGui();

    void changeCamera(DirectX::XMFLOAT3& target, bool Switch);
private:
    StageMain* stageMain = nullptr;

    Sprite* sprite = nullptr;
    Sprite* exit = nullptr;
    Sprite* title = nullptr;
    player* player_ = nullptr;

private:
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::instance();
    CameraController* cameraController_ = nullptr;
    std::unique_ptr<FierdBuff>FB;//�t�B�[���h�o�t

    DirectX::XMFLOAT2 mousePosition;
};