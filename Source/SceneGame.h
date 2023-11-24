#pragma once

#include"CameraController.h"
#include"camera.h"
#include"scnen.h"
#include"Graphics/Sprite.h"
#include"player.h"
#include"stageMain.h"
#include"StageManager.h"
// �Q�[���V�[��
class SceneGame :public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;
	void DrawDebugGui();
public:
	void changeCamera(DirectX::XMFLOAT3& target, bool Switch);


private:
	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	StageMain* stageMain = nullptr;
	Sprite* gauge = nullptr;
	player* player_ = nullptr;

private://�ǉ�������
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::instance();
	CameraController* cameraController_ = nullptr;
	std::unique_ptr<Sprite>circle = std::make_unique<Sprite>("Data/Sprite/�T�[�N��.png");
	StageManager& stagemanager = StageManager::Instance();
};
