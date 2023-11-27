#pragma once

#include"Graphics/Shader.h"
#include"character.h"
#include"FierdBuff.h"

class Enemy :public Character
{
public:
    Enemy() {};
    ~Enemy()override {};
    //�j��
    void Destroy();
    
    //�X�V����
    virtual void Update(float elapsedTime, FierdBuff& FB)=0;
    //�`��
    virtual void Render(RenderContext& rc, ModelShader* shader)=0;
    //
    virtual void DrawDebugPrimitive();

};
