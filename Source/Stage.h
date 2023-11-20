#pragma once
#include "collision.h"
#include "Graphics/Shader.h"

class Stage
{
public:
    Stage() {};
    virtual~Stage() {};
public:
    virtual void Update(float elapsedTime)=0;
    virtual void Render(RenderContext& rc, ModelShader* shader)=0;
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)=0;

};
