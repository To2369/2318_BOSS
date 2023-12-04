#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneClear.h"
#include"SceneOver.h"
#include"SceneTutorial.h"

void ScnenTitle::Initialize()
{
    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/Title.png");
    title = new Sprite("Data/Sprite/Title_logo.png");
    start = new Sprite("Data/Sprite/START.png");
    tutorial = new Sprite("Data/Sprite/TUTORIAL.png");
    exit = new Sprite("Data/Sprite/EXIT.png");
}

void ScnenTitle::Finalize()
{

    if (sprite!=nullptr)
    {
        delete sprite; 
        sprite = nullptr;
    }
    if (start != nullptr)
    {
        delete start;
        start = nullptr;
    }
    if (tutorial != nullptr)
    {
        delete tutorial;
        tutorial = nullptr;
    }
    if (exit != nullptr)
    {
        delete exit;
        exit = nullptr;
    }
}

void ScnenTitle::Update(float elpsedTime)
{
    float screenWidth = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenHeight = static_cast <float>(Graphics::Instance().GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeght = static_cast<float>(sprite->GetTextureHeight());

    sprite->Update(
        0, 0, screenWidth, screenHeight,
        0, 0, textureWidth, textureHeght,
        0,
        1, 1, 1, 1);

    float titleWidth = static_cast<float>(title->GetTextureWidth());
    float titleHeght = static_cast<float>(title->GetTextureHeight());

    title->Update(
        50, 40, titleWidth * 2, titleHeght * 2,
        0, 0, titleWidth, titleHeght,
        0,
        1, 1, 1, 1);

    float exitWidth = static_cast<float>(exit->GetTextureWidth());
    float exitHeght = static_cast<float>(exit->GetTextureHeight());
    float positionX = screenWidth - exitWidth;
    float positionY = screenHeight - exitHeght;
    int state = 0;

    start->Update(
        positionX - 10, positionY - 200, 242, 92,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);

    tutorial->Update(
        positionX - 10, positionY -106, 242, 92,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);


    exit->Update(
        positionX-10, positionY-10, 242, 92,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);
    Mouse& mouse = Input::Instance().GetMouse();
    mousePosition.x = static_cast<float>(mouse.GetPositionX());
        mousePosition.y = static_cast<float>(mouse.GetPositionY());
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (mousePosition.x > positionX - 10)
        {
            if (  mousePosition.y < positionY - 110 & mousePosition.y > positionY - 200)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading (new SceneGame));
            }
            if (mousePosition.y > positionY - 106 & mousePosition.y < positionY - 20)
            {
                SceneManager::Instance().ChangeScene(new SceneTutorial);
            }
            if (mousePosition.y > positionY - 10)
            {
                PostQuitMessage(0);
            }
        }
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
    {
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //タイトルスプライト描画
        shader->Begin(rc);
        shader->Draw(rc, title);
        shader->Draw(rc, start);
        shader->Draw(rc, tutorial);
        shader->Draw(rc, exit);
        shader->End(rc);
    }
}