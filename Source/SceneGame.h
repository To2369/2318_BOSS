#pragma once
#include"Graphics/Sprite.h"
#include"CameraController.h"
#include"camera.h"
#include"scnen.h"
#include"player.h"
#include"stageMain.h"
#include"StageManager.h"
#include"FierdBuff.h"
#include "Graphics/Light.h"

// ゲームシーン
class SceneGame :public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;
	void DrawDebugGui();
public:
	void changeCamera(DirectX::XMFLOAT3& target, bool Switch);

	void RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

private:

	//エネミーHPゲージ更新処理
	void RenderEnemyGaugeUpdate();

	StageMain* stageMain = nullptr;
	Sprite* gauge = nullptr;
	Sprite* gaugeback = nullptr;
	player* player_ = nullptr;
	Sprite* sprite = nullptr;
	Sprite* exit = nullptr;
	Sprite* title = nullptr;
	Sprite* retry = nullptr;
	Sprite* Back = nullptr;

private://追加しもの
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::instance();
	CameraController* cameraController_ = nullptr;
	std::unique_ptr<Sprite>circle = std::make_unique<Sprite>("Data/Sprite/サークル.png");
	StageManager& stagemanager = StageManager::Instance();
	std::unique_ptr<FierdBuff>FB;//フィールドバフ
	//平行光源データ
	std::unique_ptr<Light> directional_light; //平行光源
	DirectX::XMFLOAT4 ambientLightColor; //環境光色

	DirectX::XMFLOAT2 mousePosition;
};
