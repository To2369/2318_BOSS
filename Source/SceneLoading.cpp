#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"

#include<thread>
void SceneLoading::Initialize()
{
    //スプライト初期化
    ex = new Sprite("Data/Sprite/ex.png");
    sprite = new Sprite("Data/Sprite/arrow.png");
    thread = new std::thread(LadingThread,this);
}

void SceneLoading::Finalize()
{
    if (thread != nullptr)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }

    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (ex != nullptr)
    {
        delete ex;
        ex = nullptr;
    }
}

void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180;
    angle += speed * elapsedTime;

    if (nextscene->IsReady())
    {
       SceneManager::Instance().ChangeScene(nextscene);
       nextscene = nullptr;
    }
    
    float screenWidth = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenHeight = static_cast<float>(Graphics::Instance().GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());
    positionX += 15;
    if (positionX > 980)
    {
        positionX = -100;
        exflag = true;
    }
    if (positionX > 100)
    {
        exflag = false;
    }
    sprite->Update(
        positionX, positionY, 224, 224,
        0, 0, textureWidth, textureHeight,
        0,
        1, 1, 1, 1);
    if (exflag)
    {
        ex->Update(
            1000, positionY, 224, 224,
            0, 0, 640, 640,
            0,
            1, 1, 1, 1);
    }
    else
    {
        ex->Update(
            1000, positionY, 224, 224,
            0, 0, 640, 640,
            0,
            1, 1, 1, 0);
    }
}

void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 1.0f,1.0f,1.0f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    RenderContext rc;
    rc.deviceContext = graphics.GetDeviceContext();

    {
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        shader->Begin(rc);
        shader->Draw(rc, sprite);
        shader->Draw(rc, ex);
        shader->End(rc);
    }
}

void SceneLoading::LadingThread(SceneLoading* scene)
{
    //com関連の初期化でスレッド毎に呼ぶ必要がある
    CoInitialize(nullptr);
    //次のシーンの初期化を行う
    
    scene->nextscene->Initialize();
    CoUninitialize();

    scene->nextscene->SetReady();
}
