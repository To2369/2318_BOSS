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
    //�X�v���C�g�̏�����
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
        50, 40, titleWidth * 2.5, titleHeght * 2.5,
        0, 0, titleWidth, titleHeght,
        0,
        1, 1, 1, 1);

    float exitWidth = static_cast<float>(exit->GetTextureWidth());
    float exitHeght = static_cast<float>(exit->GetTextureHeight());
    float positionX = screenWidth - exitWidth;
    float positionY = screenHeight - exitHeght;
    int state = 0;

    start->Update(
        1550, 640, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);

    tutorial->Update(
        1550, 781, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);


    exit->Update(
        1550, 931, 360, 130,
        0, 0, 242, 92,
        0,
        1, 1, 1, 1);
    Mouse& mouse = Input::Instance().GetMouse();
    mousePosition.x = static_cast<float>(mouse.GetPositionX());
        mousePosition.y = static_cast<float>(mouse.GetPositionY());
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (mousePosition.x > 1550)
        {
            if (  mousePosition.y < 770 & mousePosition.y > 640)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading (new SceneGame));
            }
            if (mousePosition.y > 781 & mousePosition.y < 910)
            {
                SceneManager::Instance().ChangeScene(new SceneTutorial);
            }
            if (mousePosition.y > 921)
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

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    RenderContext rc;
    rc.deviceContext = dc;

    //�QD�X�v���C�g
    {
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //�^�C�g���X�v���C�g�`��
        shader->Begin(rc);
        shader->Draw(rc, sprite);
        shader->End(rc);
    }
    {
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //�^�C�g���X�v���C�g�`��
        shader->Begin(rc);
        shader->Draw(rc, title);
        shader->Draw(rc, start);
        shader->Draw(rc, tutorial);
        shader->Draw(rc, exit);
        shader->End(rc);
    }
}