#pragma once

#include"Graphics/Shader.h"
#include"character.h"

class Enemy :public Character
{
public:
    Enemy() {};
    ~Enemy()override {};
    //�j��
    void Destroy();
    
    //�X�V����
    virtual void Update(float elapsedTime)=0;
    //�`��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader)=0;
    //
    virtual void DrawDebugPrimitive();

};
