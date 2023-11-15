#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Input/Input.h"
#include"SceneLoading.h"

void ScnenTitle::Initialize()
{
    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/Title.png");
}

void ScnenTitle::Finalize()
{

    if (sprite!=nullptr)
    {
        delete sprite; 
        sprite = nullptr;
    }
}

void ScnenTitle::Update(float elpsedTime)
{
    GamePad &gamePad= Input::Instance().GetGamePad();
    const GamePadButton anyButton = GamePad::BTN_A| GamePad::BTN_B;
    if (gamePad.GetButtonDown()&anyButton)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
}

void ScnenTitle::Render()
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
    //２Dスプライト
    {
        float screenWidh = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast <float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeght = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidh, screenHeight,
            0, 0, textureWidth, screenHeight,
            0,
            1, 1, 1, 1);
    }


}
