#pragma once

#include"CameraController.h"
#include"camera.h"
#include"scnen.h"
#include"Graphics/Sprite.h"
#include"player.h"
#include"stageMain.h"
#include"StageManager.h"
#include"FierdBuff.h"
#include "Graphics/Light.h"

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

	//�G�l�~�[HP�Q�[�W�X�V����
	void RenderEnemyGaugeUpdate();

	StageMain* stageMain = nullptr;
	Sprite* gauge = nullptr;
	player* player_ = nullptr;

private://�ǉ�������
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::instance();
	CameraController* cameraController_ = nullptr;
	std::unique_ptr<Sprite>circle = std::make_unique<Sprite>("Data/Sprite/�T�[�N��.png");
	StageManager& stagemanager = StageManager::Instance();
	std::unique_ptr<FierdBuff>FB;//�t�B�[���h�o�t

	//���s�����f�[�^
	std::unique_ptr<Light> directional_light; //���s����
	DirectX::XMFLOAT4 ambientLightColor; //�����F
};
