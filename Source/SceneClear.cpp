#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Input/Input.h"
#include"SceneClear.h"

void SceneClear::Initialize()
{
    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/GameClear.jpg");
}

void SceneClear::Finalize()
{

    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
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
        //タイトルスプライト描画
        shader->Begin(rc);
        shader->Draw(rc, sprite);
        shader->End(rc);
    }
}