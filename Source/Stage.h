#pragma once
#include "collision.h"
#include "Graphics/Shader.h"

class Stage
{
public:
    Stage() {};
    virtual~Stage() {};
public:
    virtual void Update(float elapsedTime) = 0;
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit) = 0;
protected:
    void UpdateTransform();
    DirectX::XMFLOAT3   position = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    DirectX::XMFLOAT3   scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform =
    {
        1,0,0,0,//‰E•ûŒü
        0,1,0,0,//ã•ûŒü
        0,0,1,0,//‘O•ûŒü

        0,0,0,1//ˆÊ’u
    };
public:
    DirectX::XMFLOAT3 GetScale() { return scale; }
public:
    void SetScale(DirectX::XMFLOAT3 scale_) { scale = scale_; }
};
