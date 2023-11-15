#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    //Effeckseer�����_���쐬
    EffekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 248);
    //Effeckseer�}�l�[�W���[�쐬
    EffekseerManager = Effekseer::Manager::Create(2048);

    //Effeckseer�����_���̊e��ݒ�i���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͒���I�Ɉȉ���ݒ�ł����j
    EffekseerManager->SetSpriteRenderer(EffekseerRenderer->CreateSpriteRenderer());
    EffekseerManager->SetRibbonRenderer(EffekseerRenderer->CreateRibbonRenderer());
    EffekseerManager->SetRingRenderer(EffekseerRenderer  ->CreateRingRenderer());
    EffekseerManager->SetTrackRenderer(EffekseerRenderer ->CreateTrackRenderer());
    EffekseerManager->SetModelRenderer(EffekseerRenderer ->CreateModelRenderer());
    //Effekseer��������W�n�Ōv�Z����
    EffekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

}
//�I����
void EffectManager::Finalize()
{
    //EffekseerMnager�Ȃǂ̓X�}�[�g�|�C���^�[�ɂ���Ĕj�������̂ŉ������Ȃ�
}
//�X�V����
void EffectManager::Update(float elapsedTime)
{
    //�G�t�F�N�g�X�V����(�����ɂ̓t���[���̌o�ߎ��Ԃ�n��)
    EffekseerManager->Update(elapsedTime * 60.0f);
}

void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //�r���[���v���W�F�N�V�����s���Effekseer�����_���ɐݒ�
    EffekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    EffekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    //Effekseer�`��J�n
    EffekseerRenderer->BeginRendering();

    //Effekseer�`����s
    //�}�l�[�W���[�P�ʂŕ`�悷��̂ŕ`�揇�𐧌䂷��ꍇ�̓}�l�[�W���[�𕡐��쐬���A
    //Draw()�֐������s���鏇�ԂŐ���ł�����
    EffekseerManager->Draw();
    //Effekseer�̕`��I��
    EffekseerRenderer->EndRendering();

}




