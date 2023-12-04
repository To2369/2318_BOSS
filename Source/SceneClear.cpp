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
}

void SceneClear::Update(float elpsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    const GamePadButton anyButton = GamePad::BTN_A | GamePad::BTN_B;
    if (gamePad.GetButtonDown() & anyButton)
    {
        SceneManager::Instance().ChangeScene(new ScnenTitle);
    }

    float screenWidth = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenHeight = static_cast <float>(Graphics::Instance().GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeght = static_cast<float>(sprite->GetTextureHeight());

    float exitWidth = static_cast<float>(exit->GetTextureWidth());
    float exitHeght = static_cast<float>(exit->GetTextureHeight());
    float positionX = screenWidth - exitWidth;
    float positionY = screenHeight - exitHeght;

    

    sprite->Update(
        500, 0, screenWidth/2, screenHeight/2,
        0, 0, textureWidth, textureHeght,
        0,
        1, 1, 1, 1);

    exit->Update(
        positionX / 1.5, positionY / 1.2, 242, 92,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);

    title->Update(
        positionX / 3, positionY / 1.2, 242, 92,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);
    Mouse& mouse = Input::Instance().GetMouse();
    mousePosition.x = static_cast<float>(mouse.GetPositionX());
    mousePosition.y = static_cast<float>(mouse.GetPositionY());

    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (mousePosition.y > 785 & mousePosition.y < 920)
        {
            if (mousePosition.x > 520 & mousePosition.x < 880)
            {
                SceneManager::Instance().ChangeScene(new ScnenTitle);
            }

            if (mousePosition.x > 1040 & mousePosition.x < 1400)
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

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    RenderContext rc;
    rc.deviceContext = dc;

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
