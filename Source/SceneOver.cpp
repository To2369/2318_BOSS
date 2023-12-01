#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"SceneOver.h"
#include"SceneGame.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneTitle.h"

void SceneOver::Initialize()
{
    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/GAME_OVER.png");
    exit = new Sprite("Data/Sprite/EXIT.png");
    title = new Sprite("Data/Sprite/TITLE_.png");
    retry = new Sprite("Data/Sprite/RETRY.png");
}

void SceneOver::Finalize()
{
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (retry != nullptr)
    {
        delete retry;
        retry = nullptr;
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

void SceneOver::Update(float elpsedTime)
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
    float SpriteX = screenWidth - textureWidth;
    sprite->Update(
        SpriteX/2, 100, 800, 170,
        0, 0, 800, 170,
        0,
        1, 1, 1, 1);

    float exitWidth = static_cast<float>(exit->GetTextureWidth());
    float exitHeght = static_cast<float>(exit->GetTextureHeight());
    float positionX = screenWidth - exitWidth;
    float positionY = screenHeight - exitHeght;

    exit->Update(
        1300, 670, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);

    title->Update(
        780, 670, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);
    retry->Update(
        260, 670, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);

    Mouse& mouse = Input::Instance().GetMouse();
    mousePosition.x = static_cast<float>(mouse.GetPositionX());
    mousePosition.y = static_cast<float>(mouse.GetPositionY());

    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (mousePosition.y > 670 & mousePosition.y < 800)
        {
            if (mousePosition.x > 260 & mousePosition.x < 620)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
            }

            if (mousePosition.x > 780 & mousePosition.x < 1140)
            {
                SceneManager::Instance().ChangeScene(new ScnenTitle);
            }

            if (mousePosition.x > 1300 & mousePosition.x < 1630)
            {
                PostQuitMessage(0);
            }
        }
    }
}

void SceneOver::Render()
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
        shader->Draw(rc, title);
        shader->Draw(rc, exit);
        shader->Draw(rc, retry);
        shader->End(rc);
    }
}

void SceneOver::DrawDebugGui()
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
