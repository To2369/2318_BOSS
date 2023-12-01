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
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit) = 0;
protected:
    void UpdateTransform();
    DirectX::XMFLOAT3   position = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    DirectX::XMFLOAT3   scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform =
    {
        1,0,0,0,//âEï˚å¸
        0,1,0,0,//è„ï˚å¸
        0,0,1,0,//ëOï˚å¸

   


        0,0,0,1//à íu
    };
public:
    DirectX::XMFLOAT3 GetScale() { return scale; }
    //DirectX::XMFLOAT3 GetPosition() { return position; }
public:
    void SetScale(DirectX::XMFLOAT3 scale_) { scale = scale_; }
};
