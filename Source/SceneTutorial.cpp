#include"SceneTutorial.h"
#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Input/Input.h"
#include"SceneLoading.h"

void SceneTutorial::Initialize()
{
    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/SCENE_TUTORIAL.png");
}

void SceneTutorial::Finalize()
{
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

void SceneTutorial::Update(float elpsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    mousePosition.x = static_cast<float>(mouse.GetPositionX());
    mousePosition.y = static_cast<float>(mouse.GetPositionY());

    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        SceneManager::Instance().ChangeScene(new ScnenTitle);
    }

    float screenWidh = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenHeight = static_cast <float>(Graphics::Instance().GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeght = static_cast<float>(sprite->GetTextureHeight());

    sprite->Update(
        0, 0, screenWidh, screenHeight,
        0, 0, textureWidth, screenHeight,
        0,
        1, 1, 1, 1);
}

void SceneTutorial::Render()
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
        //タイトルスプライト描画
        shader->Begin(rc);
        shader->Draw(rc, sprite);
        shader->End(rc);
    }
}
