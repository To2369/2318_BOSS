#include"Effeck.h"
#include"Graphics/Graphics.h"
#include"EffectManager.h"

Effect::Effect(const char* filename)
{
    //�ʃX���b�h����devicecontext���g���Ă����ꍇ��
    //�����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
    std::lock_guard<std::mutex>lock(Graphics::Instance().GetMutex());
    //Effekseer�̃��\�[�X��ǂݍ���
    //Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h�ϊ����K�v
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);
    //effekseer::Manager���擾
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
    //Effeckseer�G�t�F�N�g�ǂݍ���
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}

Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale)
{

    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y,position.z);

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
    return handle;
}

void Effect::Stop(Effekseer::Handle handle)
{
    Effekseer::ManagerRef effekseermanager = EffectManager::Instance().GetEffekseerManager();
    effekseermanager->StopEffect(handle);
}

void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}
