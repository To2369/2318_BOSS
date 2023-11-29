#include"Effeck.h"
#include"Graphics/Graphics.h"
#include"EffectManager.h"

Effect::Effect(const char* filename)
{
    //別スレッド中にdevicecontextを使われていた場合に
    //同時アクセスしないように排他制御する
    std::lock_guard<std::mutex>lock(Graphics::Instance().GetMutex());
    //Effekseerのリソースを読み込む
    //EffekseerはUTF-16のファイルパス以外は対応していないため文字コード変換が必要
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);
    //effekseer::Managerを取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
    //Effeckseerエフェクト読み込み
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
