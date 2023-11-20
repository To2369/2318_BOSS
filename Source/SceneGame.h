#pragma once

#include"CameraController.h"
#include"scnen.h"
#include"Graphics/Sprite.h"
#include"player.h"
#include"stageMain.h"
// ゲームシーン
class SceneGame:public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;
public:
	void changeCamera(DirectX::XMFLOAT3& target,bool Switch);


private:
	//エネミーHPゲージ描画
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	//エネミーHPゲージ更新処理
	void RenderEnemyGaugeUpdate();

	StageMain* stageMain = nullptr;
	Sprite* gauge = nullptr;
	player* player_ = nullptr;

	CameraController* cameraController_ = nullptr;
	
};
