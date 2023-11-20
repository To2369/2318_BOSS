#pragma once

#include"CameraController.h"
#include"scnen.h"
#include"Graphics/Sprite.h"
#include"player.h"
#include"stageMain.h"
// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;
public:
	void changeCamera(DirectX::XMFLOAT3& target,bool Switch);


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

	CameraController* cameraController_ = nullptr;
	
};
