#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"

#include<thread>
void SceneLoading::Initialize()
{
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/LoadingIcon.png");
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
    
}

void SceneLoading::Render()
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
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        float positionX = screenWidth - textureWidth;
        float positionY = screenHeight - textureHeight;

        sprite->Render(dc,
            positionX, positionY, textureWidth, textureHeight,
            0, 0, textureWidth, textureHeight,
            angle,
            1, 1, 1, 1);
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
