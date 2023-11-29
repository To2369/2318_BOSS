#pragma once

#include<DirectXMath.h>
#include<Effekseer.h>

class Effect
{
public:
    Effect(const char*filename);
    ~Effect() {};
    //再生
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 scale);
    //停止
    void Stop(Effekseer::Handle handle);
    //座標設定
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);
    //スケール設定
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

private:
    Effekseer::EffectRef effekseerEffect;
};
