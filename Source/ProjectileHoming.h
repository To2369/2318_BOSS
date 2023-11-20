#pragma once

#include"Graphics/Model.h"
#include"projectile.h"

//’Ç”ö’eŠÛ
class ProjectileHoming :public Projectile
{
public:
    ProjectileHoming(ProjectileManager*manajer);
    ~ProjectileHoming()override;
    //XVˆ—
    void Update(float elapsedTime)override;
    //•`‰æˆ—@
    void Render(RenderContext&rc, ModelShader* shader)override;
    //”­Ë
    void Launch(const DirectX::XMFLOAT3& Direction,
        const DirectX::XMFLOAT3& position,
        const DirectX::XMFLOAT3& target);
private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 target = { 0,0,0 };
    float moveSpeed = 10.0f;
    float turnSpeed = DirectX::XMConvertToRadians(180);
    float lifeTimer = 3.0f;
};
