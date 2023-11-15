#pragma once

#include"Graphics/Shader.h"
#include"character.h"

class Enemy :public Character
{
public:
    Enemy() {};
    ~Enemy()override {};
    //”jŠü
    void Destroy();
    
    //XVˆ—
    virtual void Update(float elapsedTime)=0;
    //•`‰æ
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader)=0;
    //
    virtual void DrawDebugPrimitive();

};
