#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "camera.h"
#include"CameraController.h"
#include"enemy.h"
#include"EnemyManager.h"
#include"EnemySlaime.h"
#include"EnemyBoss.h"
#include"EffectManager.h"
#include"Input/Mouse.h"
#include"Input/Input.h"
#include"StageManager.h"
#include"StageMoveFloor.h"
#include"PlayerManager.h"
#include"parameter.h"
#include"SceneManager.h"
#include"SceneTitle.h"
// 初期化
using namespace Debugparam;
static DirectX::XMFLOAT2 Spritecenter{};

void SceneGame::Initialize()
{
	//ステージ初期化


	FB = std::make_unique<FierdBuff>();
	player* pl = new player;
	pl->Set_TPPorFPS_Flag(true);
	pl->SetPosition({});
	PlayerManager::Instance().Register(pl);

	stageMain = new StageMain();
	stagemanager.Register(stageMain);
	cameraController_ = new CameraController;
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 1; ++i)
	{

		/*EnemySlime* slime = new EnemySlime;
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);*/

	}
	EnemyBoss* boss = new EnemyBoss;
	boss->SetPosition(DirectX::XMFLOAT3(0.0f, -5.0f, 30.0f));
	boss->SetAngle(DirectX::XMFLOAT3(0.0f, 9.5f, 0.0f));
	enemyManager.Register(boss);
	//todo:カメラ初期設定

	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 0, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0));
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(90),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f);

	//ゲージスプライト
	gauge = new Sprite;

	directional_light = std::make_unique<Light>(LightType::Directional);
	ambientLightColor = { 0.2f,0.2f,0.2f,0.2f };
}

// 終了化
void SceneGame::Finalize()
{
	if (cameraController_ != nullptr)
	{
		delete cameraController_;
		cameraController_ = nullptr;
	}
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}
	EnemyManager::Instance().Clear();
	StageManager::Instance().Clear();
	PlayerManager::Instance().clear();
}
void SceneGame::changeCamera(DirectX::XMFLOAT3& target, bool Switch)
{
	PlayerManager& pl_m = PlayerManager::Instance();
	player* pl = pl_m.GetPlayer(0);
	DirectX::XMFLOAT3 p{};
	if (pl_m.GetPlayer(0)->GetPerspectiveChangeFlag().TPS)
	{
		p = pl->GetPosition();
		p.y += 1.5f;
		DirectX::XMVECTOR P = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&p), 0.5f);
		DirectX::XMStoreFloat3(&target, P);
	}
	else if (pl->GetPerspectiveChangeFlag().FPS)
	{
		p = pl->GetPosition();
		p.y += 1.0f;
		DirectX::XMVECTOR P = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&p), 0.5f);
		DirectX::XMStoreFloat3(&target, P);
	}

}
// 更新処理
void SceneGame::Update(float elapsedTime)
{
	static DirectX::XMFLOAT3 target{};
	frameRateCheck = elapsedTime;
	changeCamera(target, PlayerManager::Instance().GetPlayer(0)->GetPerspectiveChangeFlag().TPS);
	cameraController_->SetTarget(target);
	cameraController_->Update(elapsedTime);

	StageManager::Instance().Update(elapsedTime);
	PlayerManager::Instance().Update(elapsedTime, *cameraController_,*FB);
	
	FB->Update(elapsedTime);
	EnemyManager::Instance().Update(elapsedTime,*FB);
	EffectManager::Instance().Update(elapsedTime);
	RenderEnemyGaugeUpdate();

	float screenWidh = 720;//static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast <float>(graphics.GetScreenHeight());
	float textureWidth = static_cast<float>(circle->GetTextureWidth());
	float textureHeght = static_cast<float>(circle->GetTextureHeight());
	if (PlayerManager::Instance().GetPlayer(0)->GetPerspectiveChangeFlag().FPS)
	{

		DirectX::XMFLOAT2 Pos{ 640,360 };
		circle->Update(
			screenWidh, screenHeight,
			screenWidh * PlayerManager::Instance().GetPlayer(0)->GetCircleRadius(),
			screenHeight * PlayerManager::Instance().GetPlayer(0)->GetCircleRadius(),
			0, 0,
			textureWidth, textureHeght, 0,
			1, 1, 1, 1);
	}

	GamePad& gamePad = Input::Instance().GetGamePad();
	const GamePadButton anyButton = GamePad::BTN_A | GamePad::BTN_B;
	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new ScnenTitle);
	}
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	player* pl = PlayerManager::Instance().GetPlayer(0);
	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;
	rc.ambientLightColor = ambientLightColor;
	rc.directionalLightData.direction.x = directional_light->GetDirection().x;
	rc.directionalLightData.direction.y = directional_light->GetDirection().y;
	rc.directionalLightData.direction.z = directional_light->GetDirection().z;
	rc.directionalLightData.direction.w = 0;
	rc.directionalLightData.color = directional_light->GetColor();
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		shader->Begin(rc);
		//ステージ描画
		StageManager::Instance().Render(rc, shader);
		PlayerManager::Instance().Render(rc, shader);
		EnemyManager::Instance().Render(rc, shader);
		shader->End(rc);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグ表示
		PlayerManager::Instance().DarwDebugPrimitive();
		EnemyManager::Instance().DrawDebugprimitive();
		FB->primitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}
	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
	// 2Dスプライト描画
	{
		//RenderEnemyGauge(textureHeghtdc, rc.view, rc.projection);

		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
		shader->Begin(rc);
		shader->Draw(rc, gauge);
		shader->End(rc);

		RenderEnemyGauge(dc, rc.view, rc.projection);

	}

	// 2DデバッグGUI描画
	{
		stagemanager.GUI();
		PlayerManager::Instance().DrawDebugGUI();
		//cameraController_->DrawDebugGUI();
		EnemyManager::Instance().DrawDebugGUI();
		DrawDebugGui();
		
		directional_light->DrawDebugGUI();
	}
}

void SceneGame::DrawDebugGui()
{
	EnemyManager::Instance().DrawDebugGUI();
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("SceneGame", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::CollapsingHeader("FrameRateCheck", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat("FrameRate", &frameRateCheck);
		}
		if (ImGui::CollapsingHeader("FierdBuff", ImGuiTreeNodeFlags_DefaultOpen))
		{
			DirectX::XMFLOAT2 fb = FB->GetNumber();

			ImGui::InputFloat("FierdPosX", &fb.x);
			ImGui::InputFloat("FierdPosY", &fb.y);
			ImGui::Text("state:%s", FB->GetStateName().c_str());
		}
	}

	ImGui::End();
	
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::TreeNode("phong"))
		{
			ImGui::ColorEdit4("Ambient Light Color", &ambientLightColor.x);
			ImGui::TreePop();
		}
	}

	ImGui::End();

}

void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全てのエネミーの頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
		//エネミーの頭上のワールド座標
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		//ワールド座標からスクリーン座標へ変換
		DirectX::XMVECTOR ScreeenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);

		//スクリーン座標
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreeenPosition);

		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

		//ゲージ描画
		gauge->Update(
			screenPosition.x - gaugeWidth * 0.5f,
			screenPosition.y - gaugeHeight,
			gaugeWidth * healthRate,
			gaugeHeight,
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f);
	}

	////マウスカーソル座標を取得
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		DirectX::XMVECTOR ScreenPosition, WorldPosition;
		//レイの始発を算出
		screenPosition.z = 0.0f;
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);
		//レイの終点を算出
		screenPosition.z = 1.0f;
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 Layend;
		DirectX::XMStoreFloat3(&Layend, WorldPosition);
		//レイキャスト
		HitResult hit;

		if (stageMain->RayCast(rayStart, Layend, hit))
		{
			//////敵を配置
			//Enemy* slime = new EnemySlime();
			//slime->SetPosition(hit.position);
			//EnemyManager::Instance().Register(slime);

			/*EnemyManager& enemy = EnemyManager::Instance();
			int count = enemy.GetEnemyCount();
			for (int i = 0; i < count; i++)
			{

				 enemy.GetEnemy(i)->Destroy();
			}*/
		}
	}
}

void SceneGame::RenderEnemyGaugeUpdate()
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	Camera& camera = Camera::instance();
	DirectX::XMFLOAT4X4 view = camera.GetView();
	DirectX::XMFLOAT4X4 projection = camera.GetProjection();

	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	//全てのエネミーの頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
		//エネミーの頭上のワールド座標
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		//ワールド座標からスクリーン座標へ変換
		DirectX::XMVECTOR ScreeenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);

		//スクリーン座標
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreeenPosition);

		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

		//ゲージ描画
		gauge->Update(
			screenPosition.x - gaugeWidth * 0.5f,
			screenPosition.y - gaugeHeight,
			gaugeWidth * healthRate,
			gaugeHeight,
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f);
	}
}
