#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Input/Input.h"
#include"SceneClear.h"
#include"SceneLoading.h"
#include"SceneTitle.h"


void SceneClear::Initialize()
{
    StageManager& stagemanager = StageManager::Instance();
    stageMain = new StageMain();
    stagemanager.Register(stageMain);

    player* pl = new player;
    pl->Set_TPPorFPS_Flag(true);
    pl->SetPosition({});
    PlayerManager::Instance().Register(pl);

    cameraController_ = new CameraController;

    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 0, -1),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0));
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(90),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        1000.0f);

    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/VICTORY.png");
    exit = new Sprite("Data/Sprite/EXIT.png");
    title = new Sprite("Data/Sprite/TITLE_.png");
}

void SceneClear::Finalize()
{
    
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (title != nullptr)
    {
        delete title;
        title = nullptr;
    }

    if (exit != nullptr)
    {
        delete exit;
        exit = nullptr;
    }

    EnemyManager::Instance().Clear();
    StageManager::Instance().Clear();
    PlayerManager::Instance().clear();
}

void SceneClear::Update(float elapsedTime)
{
    static DirectX::XMFLOAT3 target{};
    changeCamera(target, PlayerManager::Instance().GetPlayer(0)->GetPerspectiveChangeFlag().TPS);
    cameraController_->SetTarget(target);
    cameraController_->Update(elapsedTime);

    float screenWidth = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenHeight = static_cast <float>(Graphics::Instance().GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeght = static_cast<float>(sprite->GetTextureHeight());

    float exitWidth = static_cast<float>(exit->GetTextureWidth());
    float exitHeght = static_cast<float>(exit->GetTextureHeight());
    float positionX = screenWidth - exitWidth;
    float positionY = screenHeight - exitHeght;

    StageManager::Instance().Update(elapsedTime);
    PlayerManager::Instance().Update(elapsedTime, *cameraController_, *FB);

    sprite->Update(
        500, 0, screenWidth/2, screenHeight/2,
        0, 0, textureWidth, textureHeght,
        0,
        1, 1, 1, 1);

    exit->Update(
        positionX / 1.5, positionY / 1.2, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);

    title->Update(
        positionX / 3, positionY / 1.2, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);
    Mouse& mouse = Input::Instance().GetMouse();
    mousePosition.x = static_cast<float>(mouse.GetPositionX());
    mousePosition.y = static_cast<float>(mouse.GetPositionY());

    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (mousePosition.y > positionY / 1.2 & mousePosition.y < positionY / 1.2 + 130)
        {
            if (mousePosition.x > positionX / 3 & mousePosition.x < positionX / 3 + 360)
            {
                SceneManager::Instance().ChangeScene(new ScnenTitle);
            }

            if (mousePosition.x > positionX / 1.5 & mousePosition.x < positionX / 1.5 + 360)
            {
                PostQuitMessage(0);
            }
        }
    }
}

void SceneClear::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
    player* pl = PlayerManager::Instance().GetPlayer(0);
    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    RenderContext rc;
    rc.deviceContext = dc;

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
        // ラインレンダラ描画実行
        graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

        // デバッグレンダラ描画実行
        graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
    }

    //２Dスプライト
    {
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        DrawDebugGui();
        //タイトルスプライト描画
        shader->Begin(rc);
        shader->Draw(rc, sprite);
        shader->Draw(rc, exit);
        shader->Draw(rc, title);
        shader->End(rc);
    }
}

void SceneClear::DrawDebugGui()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None))
    {

        ImGui::InputFloat2("mousePositionXY", &mousePosition.x);

    }

    ImGui::End();
}

void SceneClear::changeCamera(DirectX::XMFLOAT3& target, bool Switch)
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
